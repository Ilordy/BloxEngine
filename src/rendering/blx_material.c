#include "blx_material.h"
#include "core/blx_string.h"
#include "internal/platform/platform.h"
#include "utils/blx_fileManagement.h"
#include "utils/blx_hashTable.h"
#include "core/blx_memory.h"

#define MAT_KEY_VERT "vert"
#define MAT_KEY_FRAG "frag"
#define MAT_KEY_FLOAT "float"
#define MAT_KEY_VEC3 "vec3"
#define MAT_KEY_VEC4 "vec4"
#define MAT_KEY_INT "int"

#define FLOAT_CASE 0
#define INT_CASE 1
#define VEC3_CASE 2 
#define VEC4_CASE 3

static blxMaterial* loadedMaterials[BLX_MAX_MATERIALS];
static unsigned int materialCount = 0;
static blxHashTable* table;
static blxHashTable* matKeyTypes;


// TODO: Use and make a string tokenizer!!!

#define MAT_KEY(nameStr) #nameStr
#define MAT_KEY_INDEX(name, index) MAT_KEY_##name = index

static const char* const materialKeyWords[] = {
    MAT_KEY(vert),
    MAT_KEY(frag),
    MAT_KEY(float),
    MAT_KEY(int),
    MAT_KEY(vec3)
};

typedef struct
{
    const char propertyName[128];
    blxMaterialPropertyType propertyType;
    union
    {
        int intValue;
        float floatValue;
        vec3s vec3Value;
        vec4s vec4Value;
    };
}blxMaterialProperty;

typedef blxMaterialProperty* list_blxMaterialProperty;

typedef struct {
    const char* path;
    unsigned int referenceCount;
    unsigned int index;
    /// @brief properties list of type blxMaterialProperty.
    list_blxMaterialProperty properties;
}MaterialData;

blxBool StringCompare(void* a, void* b)
{
    return blxStrCmp((const char*)a, (const char*)b);
}

// TODO: This should be in the blxString header file.
uint64 StrToHash(void* key) {
    return blxToHash(key, blxStrLen(key));
}

void _blxInitMaterialSystem()
{
    blxZeroMemory(loadedMaterials, sizeof(blxMaterial*) * BLX_MAX_MATERIALS);
    //TODO: Use no heap alloc on hash table once that function is done!
    table = blxCreateHashTableWithHash(const char*, blxMaterial*, StringCompare, StrToHash);
    matKeyTypes = blxCreateHashTableWithHash(const char*, int, StringCompare, StrToHash);
    blxAddToHashTableAlloc(matKeyTypes, MAT_KEY_FLOAT, FLOAT_CASE);
    blxAddToHashTableAlloc(matKeyTypes, MAT_KEY_INT, INT_CASE);
    blxAddToHashTableAlloc(matKeyTypes, MAT_KEY_VEC3, VEC3_CASE);
    blxAddToHashTableAlloc(matKeyTypes, MAT_KEY_VEC4, VEC4_CASE);
}

void _blxMaterialSetValues(blxMaterial* mat)
{
    MaterialData* matData = (MaterialData*)mat->_internalData;

    for (uint64 i = 0; i < blxGetListCount(matData->properties); i++)
    {
        blxMaterialProperty prop = matData->properties[i];
        switch (prop.propertyType)
        {
            case BLX_MAT_PROP_INT:
                blxShader_SetInt(mat->shader, prop.propertyName, prop.intValue);
                break;

            case BLX_MAT_PROP_FLOAT:
                blxShader_SetFloat(mat->shader, prop.propertyName, prop.floatValue);
                break;

            case BLX_MAT_PROP_VEC3:
                blxShader_SetVec3(mat->shader, prop.propertyName, prop.vec3Value.raw);
                break;

            case BLX_MAT_PROP_VEC4:
                blxShader_SetVec4(mat->shader, prop.propertyName, prop.vec4Value.raw);
                break;
        }
    }
}


//TODO: use a list or memory arena and also make a stack.h utility for finding an open index.
blxBool MaterialExists(blxMaterial* mat)
{
    MaterialData* matData = (MaterialData*)mat->_internalData;
    for (unsigned int i = 0; i < BLX_MAX_MATERIALS; i++)
    {
        if (loadedMaterials[i]->id == mat->id)
        {
            return BLX_TRUE;
        }

    }

    return blxFileExists(matData->path);

    return BLX_FALSE;
}

int FindFreeSlot()
{
    for (unsigned int i = 0; i < materialCount; i++)
    {
        if (!loadedMaterials[i])
        {
            return i;
        }
    }

    return -1;
}

blxBool LoadAndParseBmtFile(const char* path, blxMaterial* mat)
{
    blxFile* matFile;
    system("echo %cd%");
    if (!blxOpenFile(path, BLX_FILE_MODE_READ, &matFile)) {
        return BLX_FALSE;
    }

    MaterialData* matData = (MaterialData*)mat->_internalData;
    matData->properties = blxInitList(blxMaterialProperty);

    char buffer[512];
    char* p = &buffer;
    uint64 lineLength;
    Shader matShader = -1;
    char vertPath[blxMaxFilePath] = BLX_EMPTY_STRING;
    char fragPath[blxMaxFilePath] = BLX_EMPTY_STRING;

    //TODO: Use asserts instead of if statements for error checking.
    while (blxFileReadLine(matFile, 512, &p, &lineLength))
    {
        blxStrTrim(buffer);
        BLXDEBUG("%s", buffer);
        //char* buf = buffer;
        char firstChar = buffer[0];
        switch (firstChar)
        {
            case '#': {
                continue;
            }break;
            case MAT_KEY_VERT[0]: {
                char vertBuffer[8];
                blxStrnCpy(vertBuffer, buffer, blxStrLen(MAT_KEY_VERT));
                if (blxStrCmp(vertBuffer, MAT_KEY_VERT))
                {
                    int equalsIndex = blxStrIndexOfChar(buffer, '=');
                    if (equalsIndex == -1) {
                        BLXERROR("Invalid format for vert path given, should be %s = \"path/to/vertfile.vert\"", MAT_KEY_VERT);
                        return BLX_FALSE;
                    }

                    int vPathStartIndex = equalsIndex + 1;
                    if (buffer[vPathStartIndex] != '"') {
                        BLXERROR("Invalid format for vert path given, should be %s = \"path/to/vertfile.vert\"", MAT_KEY_VERT);
                        return BLX_FALSE;
                    }

                    //Skips the first " and starts at the first letter of the string.
                    char* vertSubString = buffer + vPathStartIndex + 1;
                    //Loop and copy the file path until we reach the second ".
                    for (unsigned int i = 0; vertSubString[i] != '"'; i++)
                    {
                        vertPath[i] = vertSubString[i];
                    }
                }

            }break;

            case MAT_KEY_FRAG[0]: {
                char fragBuffer[8];
                blxStrnCpy(fragBuffer, buffer, blxStrLen(MAT_KEY_FRAG));
                if (blxStrCmp(fragBuffer, MAT_KEY_FRAG))
                {
                    int equalsIndex = blxStrIndexOfChar(buffer, '=');
                    if (equalsIndex == -1) {
                        // TODO: Reformat this to have the vert be directily from materialKeyWords array printed.
                        BLXERROR("Invalid format for frag path given, should be frag = \"path/to/fragfile.frag\"");
                        return BLX_FALSE;
                    }

                    int fPathStartIndex = equalsIndex + 1;
                    if (buffer[fPathStartIndex] != '"') {
                        BLXERROR("Invalid format for frag path given, should be frag = \"path/to/fragfile.frag\"");
                        return BLX_FALSE;
                    }

                    //Skips the first " and starts at the first letter of the string.
                    char* fragSubString = buffer + fPathStartIndex + 1;
                    //Loop and copy the file path until we reach the second ".
                    for (unsigned int i = 0; fragSubString[i] != '"'; i++)
                    {
                        fragPath[i] = fragSubString[i];
                    }
                }

            }break;
            case '[': {

                //TODO Temp for now till we have tokenizer.
                if (!matShader) {
                    BLXERROR("Vert and Frag paths must be written first before setting variables!");
                    return BLX_FALSE;
                }

                char variableBuffer[32];
                for (uint64 i = 0; i < 32; i++)
                {
                    char nextChar = buffer[i + 1];

                    if (nextChar == ']') {
                        variableBuffer[i] = '\0';
                        break;
                    }
                    variableBuffer[i] = nextChar;
                }

                int variableCase;
                if (!blxHashTableKeyExist(matKeyTypes, variableBuffer, &variableCase)) {
                    BLXWARNING("Variable of type %s is not supported.", variableBuffer);
                    continue;
                }

                int equalsIndex = blxStrIndexOfChar(buffer, '=');
                if (equalsIndex == -1) {
                    BLXWARNING("Invalid Variable Uniform format given should be [VarType] varName = varValue");
                    continue;
                }

                //TODO: Refactor later.
                //Get Uniform name.
                char uniformName[128];
                int startIndex = blxStrIndexOfChar(buffer, ']') + 1;
                blxStrnCpy(uniformName, buffer + startIndex, equalsIndex - startIndex);

                switch (variableCase)
                {
                    //TODO: Setting shader values directly should not be done here!!
                    case FLOAT_CASE: {
                        float value = atof(buffer + equalsIndex + 1);

                        blxMaterialProperty property;
                        property.propertyType = BLX_MAT_PROP_FLOAT;
                        property.floatValue = value;
                        blxStrCpy(property.propertyName, uniformName);
                        blxAddValueToList(matData->properties, property);
                    }break;

                    case INT_CASE: {
                        int value = atoi(buffer + equalsIndex + 1);

                        blxMaterialProperty property;
                        property.propertyType = BLX_MAT_PROP_INT;
                        property.intValue = value;
                        blxStrCpy(property.propertyName, uniformName);
                        blxAddValueToList(matData->properties, property);
                    }break;

                    case VEC3_CASE: {
                        //TODO: Replace sscanf with blx function.
                        vec3 value = GLM_VEC3_ONE_INIT;
                        sscanf(buffer + equalsIndex + 1, "%f,%f,%f", &value[0], &value[1], &value[2]);

                        blxMaterialProperty property;
                        property.propertyType = BLX_MAT_PROP_VEC3;
                        glm_vec3_copy(value, property.vec3Value.raw);
                        blxStrCpy(property.propertyName, uniformName);
                        blxAddValueToList(matData->properties, property);
                    }break;

                    case VEC4_CASE: {
                        vec4 value = GLM_VEC4_ONE_INIT;
                        sscanf(buffer + equalsIndex + 1, "%f,%f,%f,%f", &value[0], &value[1], &value[2], &value[3]);

                        blxMaterialProperty property;
                        property.propertyType = BLX_MAT_PROP_VEC4;
                        glm_vec4_copy(value, property.vec4Value.raw);
                        blxStrCpy(property.propertyName, uniformName);
                        blxAddValueToList(matData->properties, property);
                    }break;
                }
            }break;
        }

        //TODO: Our shader system/Resource System should keep track of what shaders are loaded.
        if (matShader == -1 && !blxStrNullOrEmpty(fragPath) && !blxStrNullOrEmpty(vertPath))
        {
            matShader = blxShader_Create(fragPath, vertPath, BLX_FALSE);
            blxShader_UseShader(matShader);
            BLXDEBUG("Shader has been created! for shader value is: %i", matShader);
            mat->shader = matShader;
        }
    }

    blxCloseFile(matFile);
    return BLX_TRUE;
}


blxMaterial* blxMaterial_CreateDefault()
{
    blxMaterial* mat = blxAllocate(sizeof(blxMaterial) + sizeof(MaterialData) +
        (sizeof(char) * blxMaxFilePath), BLXMEMORY_TAG_MATERIAL);

    mat->_internalData = (char*)mat + sizeof(blxMaterial);
    MaterialData* matData = (MaterialData*)mat->_internalData;

    matData->path = (char*)mat + sizeof(blxMaterial) + sizeof(MaterialData);

    //TODO: This is not neccessrary, refactor is needed!
    
    matData->path = "builtin/materials/Standard.bmt";
    materialCount++;
    matData->index = FindFreeSlot();
    loadedMaterials[matData->index] = mat;
    matData->referenceCount = 1;
    _blxRendererRegisterMaterial(mat);
    LoadAndParseBmtFile(matData->path, mat);
    return mat;
}

void blxMaterial_SetValue(blxMaterial* mat, const char* propName, blxMaterialPropertyType propType, void* propValue)
{
    MaterialData* matData = (MaterialData*)mat->_internalData;
    
    blxMaterialProperty* prop;
    //TODO: This should be done with a hashtable, swap the properties list with a hashtable..
    for (uint64 i = 0; i < blxGetListCount(matData->properties); i++)
    {
        if (blxStrCmp(propName, matData->properties[i].propertyName))
        {
            prop = (blxMaterialProperty*)(&matData->properties[i]);
        }
    }
    
    switch (propType)
    {
        case BLX_MAT_PROP_INT:
            prop->intValue = *((int*)propValue);
            break;

        case BLX_MAT_PROP_FLOAT:
            prop->floatValue = *((float*)propValue);
            break;

        case BLX_MAT_PROP_VEC3:
            prop->vec3Value = *((vec3s*)propValue);
            break;

        case BLX_MAT_PROP_VEC4:
            prop->vec4Value = *((vec4s*)propValue);
            break;
    }
}

//TODO: Change to just create material, keep an arena ready.
//TODO: No need to use hashtables.
blxBool blxMaterial_Load(const char* path, blxMaterial** outMat)
{
    // TODO: Memory arena for materials.

    if (blxHashTableKeyExist(table, path, outMat)) {
        MaterialData* matData = (MaterialData*)((*outMat)->_internalData);
        ((MaterialData*)(*outMat)->_internalData)->referenceCount++;
        BLXDEBUG(matData->path);
        return BLX_TRUE;
    }
    else if (materialCount == BLX_MAX_MATERIALS) {
        BLXERROR("Cannot load another material as the max amount of loaded materials has been reached!");
        return BLX_FALSE;
    }
    else if (blxStrLen(path) > blxMaxFilePath) {
        BLXERROR("File path: %s \nis greater than the max allowed file path!", path);
        return BLX_FALSE;
    }
    else if (!blxFileExists(path)) {
        BLXERROR("File path %s was unable to be found!", path);
        return BLX_FALSE;
    }
    else { // Generate a new material
        blxMaterial* mat = blxAllocate(sizeof(blxMaterial) + sizeof(MaterialData) +
            (sizeof(char) * blxMaxFilePath), BLXMEMORY_TAG_MATERIAL);

        mat->_internalData = (char*)mat + sizeof(blxMaterial);
        MaterialData* matData = (MaterialData*)mat->_internalData;

        matData->path = (char*)mat + sizeof(blxMaterial) + sizeof(MaterialData);
        blxStrCpy(matData->path, path);
        blxAddToHashTable(table, matData->path, &mat);

        materialCount++;
        matData->index = FindFreeSlot();
        loadedMaterials[matData->index] = mat;
        matData->referenceCount = 1;

        _blxRendererRegisterMaterial(mat);
        *outMat = mat;
        return LoadAndParseBmtFile(path, mat);
    }

    return BLX_FALSE;
}

void blxUnloadMaterial(blxMaterial** mat) {
    MaterialData* matData = (MaterialData*)((*mat)->_internalData);
    matData->referenceCount--;
    if (matData->referenceCount == 0)
    {
        loadedMaterials[matData->index] = NULL;
        materialCount--;
    }
}



