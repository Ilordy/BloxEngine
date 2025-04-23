#include "game.h"
#include "core/blx_logger.h"
#include "core/blx_input.h"
#include "rendering/blx_model.h"
#include "utils/blx_fileManagement.h"
#include "core/blx_memory.h"
#include "blx_imGui.h"
#include "core/loaders/blx_objLoader.h"
#include "utils/blx_hashTable.h"
#include "rendering/blx_material.h"

#include <string.h>

static gameState* state;

void ProcessMovement(float deltaTime);
void ProcessMouseMove(float deltaTime);

blxBool StrCmp(void* a, void* b) {

    return strcmp((const char*)a, (const char*)b) == 0;
}

uint64 ToHash(void* key) {
    return blxToHash(key, strlen(key));
}

blxBool IntCmp(void* a, void* b) {
    return *((int*)a) == *((int*)b);
}


blxBool InitGame(blxGameInstance* gameInstance)
{
    BLXINFO("Application Starting...");

    state = (gameState*)blxAllocate(sizeof(gameState), BLXMEMORY_TAG_GAME);

    // state->currentModel.mesh.shader = blxShaderCreate("res/shaders/Standard.frag",
    //     "res/shaders/Standard.vert", BLX_TRUE);
    blxCamera_Init(&state->mainCam, 45);
    blxAddCameraToRender(&state->mainCam);

    //Temp for now.
    state->currentModel.geometries = NULL;
    state->mainCam.transform.position[2] = 10;

    char** strList = blxInitList(char*);
    char* str1 = "Hello";
    char* str2 = "World";
    char* str3 = "Test";
    blxAddBufferToList(strList, str1);

    char* f = strList[0];

    vec3 quadP1 = { -1.0f, 1.0f, 0.0f };
    vec3 quadP2 = { 1.0f, 1.0f, 0.0f };
    vec3 quadP3 = { 1.0f, -1.0f, 0.0f };
    vec3 quadP4 = { -1.0f, -1.0f, 0.0f };

    vec3* testList = blxInitList(vec3);

    blxAddBufferToList(testList, quadP1);

    vec3 quadTest;
    glm_vec3_copy(testList[0], quadTest);


    int** intPtrList = blxInitList(int*);
    int a = 1;
    int b = 2;
    int c = 3;

    int* intPtr = &a;
    int* intPtr2 = &b;
    int* intPtr3 = &c;
    int intArray[3] = { 5, 2, 3 };
    int** arrPtr = &intArray;

    blxAddValueToList(intPtrList, intPtr);
    blxAddValueToList(intPtrList, &intArray);
    //blxAddBufferToList(intPtrList, &arrPtr);
    //blxAddValueToList(intPtrList, intPtr3);
    //intPtrList[1][0] = 5;
    //intPtrList[0] = &a;

    BLXDEBUG("Int 1: %d", *(intPtrList[0]));
  
    BLXDEBUG("Int 4: %d", intPtrList[1][0]);
    BLXDEBUG("Int 5: %d", intPtrList[1][1]);
    BLXDEBUG("Int 6: %d", intPtrList[1][2]);

    return BLX_TRUE;
}

blxBool UpdateGame(float deltaTime)
{
    state->deltaTime = deltaTime;
    ProcessMovement(deltaTime);
    ProcessMouseMove(deltaTime);
    mat4 rot;
    glm_quat_mat4(state->currentModel.transform.rotation, rot);
    glm_rotate(rot, glm_rad(90) * deltaTime, Vec3_Up);
    //glm_mat4_quat(rot, state->currentModel.transform.rotation);
    if (blxInputGetKeyDown(BLX_KEY_E)) {
        char buffer[blxMaxFilePath];
        if (blxOpenFilePanel("Choose obj to load", NULL, ".obj", buffer)) {
            BLXDEBUG("%s", buffer);
            blxImportModelFromObj(&state->currentModel, buffer);
        }
    }

    // if (blxInputGetKeyDown(BLX_KEY_F)) {
    //     shader_setVec3(state->currentModel.mesh.shader, "objectColor", (vec3) { 1, 1, 1 });
    // }

    if (blxInputGetKeyDown(BLX_KEY_1)) {
        blxSetShadingMode(BLX_SHADING_SOLID);
    }
    if (blxInputGetKeyDown(BLX_KEY_2)) {
        blxSetShadingMode(BLX_SHADING_WIREFRAME);
    }



    return BLX_TRUE;
}


blxBool Render()
{
    // blxImGuiBtn();
    // Checking if mesh has been initialized by the renderer.
    // TODO: This should probably be handled by the engine itself.
    if (state->currentModel.geometries != NULL) {
        blxDrawModel(&state->currentModel);
    }
    return BLX_TRUE;
}

void ProcessMovement(float deltaTime)
{
    const float moveSpeed = 10 * deltaTime;
    blxTransform* camTrans = &state->mainCam.transform;

    if (blxInputGetKey(BLX_KEY_W))
    {
        vec3 forward;
        blxTransform_Forward(camTrans, forward);
        glm_vec3_muladds(forward, moveSpeed, camTrans);
    }

    if (blxInputGetKey(BLX_KEY_S))
    {
        vec3 forward;
        blxTransform_Forward(camTrans, forward);
        glm_vec3_negate(forward);
        glm_vec3_muladds(forward, moveSpeed, camTrans);
    }

    if (blxInputGetKey(BLX_KEY_A))
    {
        vec3 right;
        blxTransform_Right(camTrans, right);
        glm_vec3_muladds(right, moveSpeed, camTrans);
    }

    if (blxInputGetKey(BLX_KEY_D))
    {
        vec3 left;
        blxTransform_Right(camTrans, left);
        glm_vec3_negate(left);
        glm_vec3_muladds(left, moveSpeed, camTrans);
    }
}

void ProcessMouseMove(float deltaTime)
{
    if (!blxInputGetMouseButton(BLX_RMB)) {
        return;
    }

    static float xAngle, yAngle;
    const float sensitivity = 30 * deltaTime;

    int xPos, yPos;
    blxInputGetMousePos(&xPos, &yPos);

    int lastX, lastY;
    blxInputGetPrevMousePos(&lastX, &lastY);

    float xDelta = lastX - xPos;
    float yDelta = yPos - lastY; // reversed since y-coordinates range from bottom to top
    xDelta *= sensitivity;
    yDelta *= sensitivity;
    yAngle += glm_rad(yDelta);
    xAngle += glm_rad(xDelta);

    Quaternion xRot, yRot;
    glm_quatv(xRot, yAngle, Vec3_Right);
    glm_quatv(yRot, xAngle, Vec3_Up);
    glm_quat_mul(yRot, xRot, state->mainCam.transform.rotation);
}
