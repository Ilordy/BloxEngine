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

typedef struct {
    const char* path;
    unsigned int referenceCount;
    unsigned int index;
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

blxBool MaterialExists(blxMaterial* mat)
{
    MaterialData* matData = (MaterialData*)mat->_internalData;
    for (unsigned int i = 0; i < materialCount; i++)
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

blxBool LoadAndParseBmtFile(const char* path)
{
    // TODO: Move to a seperate function.
    blxFile* matFile;
    if (!blxOpenFile(path, BLX_FILE_MODE_READ, &matFile)) {
        return BLX_FALSE;
    }

    char buffer[512];
    char* p = &buffer;
    uint64 lineLength;
    Shader matShader = 0;
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
                    int equalsIndex = blxStrIndexOf(buffer, '=');
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
                    int equalsIndex = blxStrIndexOf(buffer, '=');
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

                int equalsIndex = blxStrIndexOf(buffer, '=');
                if (equalsIndex == -1) {
                    BLXWARNING("Invalid Variable Uniform format given should be [VarType] varName = varValue");
                    continue;
                }

                //TODO: Refactor later.
                //Get Uniform name.
                char uniformName[128];
                int startIndex = blxStrIndexOf(buffer, ']') + 1;
                blxStrnCpy(uniformName, buffer + startIndex, equalsIndex - startIndex);

                switch (variableCase)
                {
                    case FLOAT_CASE: {
                    //mat set float yada
                        float value = atof(buffer + equalsIndex + 1);
                        blxShaderSetFloat(matShader, uniformName, value);
                    }break;

                    case INT_CASE: {
                        int value = atoi(buffer + equalsIndex + 1);
                        blxShaderSetInt(matShader, uniformName, value);
                    }break;

                    case VEC3_CASE: {
                        //TODO: Replace sscanf with blx function.
                        vec3 value = GLM_VEC3_ONE_INIT;
                        sscanf(buffer + equalsIndex + 1, "%f,%f,%f", &value[0], &value[1], &value[2]);
                        blxShaderSetVec3(matShader, uniformName, value);
                    }break;

                    case VEC4_CASE:{
                        vec4 value = GLM_VEC4_ONE_INIT;
                        float val;
                        sscanf(buffer + equalsIndex + 1, "%f,%f,%f,%f", &value[0], &value[1], &value[2], &value[3]);
                        blxShaderSetVec4(matShader, uniformName, value);
                    }break;
                }
            }break;
        }

        if (!matShader && !blxStrNullOrEmpty(fragPath) && !blxStrNullOrEmpty(vertPath))
        {
            matShader = blxShaderCreate(fragPath, vertPath, BLX_FALSE);
            blxShaderUseShader(matShader);

        }
    }

    return BLX_TRUE;
}

blxBool blxLoadMaterial(const char* path, blxMaterial** outMat)
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


        *outMat = mat;
        return LoadAndParseBmtFile(path);
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



