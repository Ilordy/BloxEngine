
// TEMP FOR NOW
extern "C"
{
#include "game.h"
#include "BLX/maths/blx_mat3.h"
#include "BLX/maths/blx_mat4.h"
#include "BLX/maths/blx_vec3.h"
#include "BLX/rendering/blx_rendering.h"
#include "blx_imGui.h"
#include "core/blx_input.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"
#include "core/loaders/blx_objLoader.h"
#include "physics/blx_particle.h"
#include "rendering/blx_material.h"
#include "rendering/blx_model.h"
#include "utils/blx_fileManagement.h"
#include "utils/blx_hashTable.h"
#include "utils/blx_vlist.h"
}

#include <string.h>

#include "BLX/utils/Map.h"
#include "BLX/utils/list.h"

struct Vector3
{
    float x, y, z;

    Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
        BLXINFO("Parameterized Constructor Called for Vector3(%f, %f, %f)", x, y, z);
    }
    Vector3() : x(0), y(0), z(0) {}

    // copy assignment operator
    Vector3& operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        BLXINFO("Copy Assignment Operator Called");
        return *this;
    }

    // copy constructor
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z)
    {
        BLXINFO("Copy Constructor Called");
    }

    // move constructor
    Vector3(Vector3&& other) noexcept : x(other.x), y(other.y), z(other.z)
    {
        BLXINFO("Move Constructor Called for Vector3(%f, %f, %f)", x, y, z);
        other.x = 0;
        other.y = 0;
        other.z = 0;
    }

    // move assignment operator
    Vector3& operator=(Vector3&& other) noexcept
    {
        if (this != &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            BLXINFO("Move Assignment Operator Called");
            other.x = 0;
            other.y = 0;
            other.z = 0;
        }
        return *this;
    }

    bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    ~Vector3()
    {
        BLXINFO("Destructor Called for Vector3(%f, %f, %f)", x, y, z);
    }
};

namespace BlxStl
{
    template <>
    struct Hash<Vector3>
    {
        uint64 operator()(const Vector3& key)
        {
            // Simple hash combining the components
            uint64 hx = std::hash<float>()(key.x);
            uint64 hy = std::hash<float>()(key.y);
            uint64 hz = std::hash<float>()(key.z);
            return hx ^ (hy << 1) ^ (hz << 2); // Combine hashes
        }
    };
} // namespace BlxStl

static gameState* state;

static blxModel* ballObjects;

static blxModel ballModel;

void ProcessMovement(float deltaTime);
void ProcessMouseMove(float deltaTime);
void SpawnPhysicsObject();

blxBool StrCmp(void* a, void* b)
{

    return strcmp((const char*)a, (const char*)b) == 0;
}

uint64 ToHash(void* key)
{
    return blxToHash(key, strlen((const char*)(key)));
}

blxBool IntCmp(void* a, void* b)
{
    return *((int*)a) == *((int*)b);
}

blxBool InitGame(blxGameInstance* gameInstance)
{
    BLXINFO("Application Starting...");

    state = (gameState*)blxAllocate(sizeof(gameState), BLXMEMORY_TAG_GAME);

    // state->currentModel.mesh.shader =
    // blxShaderCreate("res/shaders/Standard.frag",
    //     "res/shaders/Standard.vert", BLX_TRUE);
    blxCamera_Init(&state->mainCam, 45);
    blxAddCameraToRender(&state->mainCam);

    // Temp for now.
    state->currentModel.geometries = NULL;
    state->mainCam.transform.position[2] = 10;

    ballObjects = blxInitListWithSize(blxModel, 100);

    // blxImportModelFromObj(&ballModel, "builtin/geometry/sphere.obj");

    // for (int i = 0; i < 100; i++) {

    //     memcpy(&ballObjects[i], &ballModel, sizeof(blxModel));

    //     BLXDEBUG("Ball %d: %p", i, &ballObjects[i]);

    // }

    blxMat3 mat3 = BLX_MAT3_IDENTITY;

    blxVec3 pos = {{5, 1, 6}};

    pos = blxMat3_MultVec3(mat3, pos);

    mat3.m10 = 5.5f;
    mat3.raw[2][1] = 2.0f;

    mat3 = blxMat3_Inverse(mat3);

    BLXDEBUG("Transformed Position: %f, %f, %f", pos.x, pos.y, pos.z);

    mat3s cglmmat3;
    glm_mat3_identity(cglmmat3.raw);

    cglmmat3.m10 = 5.5f;
    cglmmat3.m21 = 2.0f;

    glm_mat3_inv(cglmmat3.raw, cglmmat3.raw);

    blxVec3 pos2 = {5, 1, 6};

    glm_mat3_mulv(cglmmat3.raw, pos2.raw, pos2.raw);

    BLXDEBUG("Transformed Position: %f, %f, %f", pos2.x, pos2.y, pos2.z);

    blxMat4 mat4 = BLX_MAT4_IDENTITY;

    mat4.m10 = 5.5f;
    mat4.m21 = 2.0f;

    mat4 = blxMat4_Inverse(mat4);

    mat4s cglmmat4;
    glm_mat4_identity(cglmmat4.raw);
    cglmmat4.m10 = 5.5f;
    cglmmat4.m21 = 2.0f;

    glm_mat4_inv(cglmmat4.raw, cglmmat4.raw);

    BlxStl::Map<Vector3, int> testMap;
    Vector3 key1(1.0f, 2.0f, 3.0f);
    Vector3 key2(4.0f, 5.0f, 6.0f);
    testMap.Add(key1, 100);
    testMap.Add(key2, 200);
    if(const int* value = testMap.Find(Vector3(1.0f, 2.0f, 3.0f)); value != nullptr) {
        BLXDEBUG("Found key1 with value: %d", *value);
    } else {
        BLXDEBUG("Key1 not found!");
    }

    testMap[Vector3(1.0f, 5.0f, 3.0f)] = 350;

    BLXDEBUG("%d", testMap[key1]);

    testMap.Remove(Vector3(1.0f, 2.0f, 3.0f));

    if(!testMap.Contains(Vector3(1.0f, 2.0f, 3.0f))) {
        BLXDEBUG("Key1 successfully removed.");
    } else {
        BLXDEBUG("Key1 still exists!");
    }

    return BLX_TRUE;
}

float accumlatedTime = 0.0f;
blxBool pressed = BLX_FALSE;

blxBool UpdateGame(float deltaTime)
{
    state->deltaTime = deltaTime;
    ProcessMovement(deltaTime);
    ProcessMouseMove(deltaTime);
    mat4 rot;
    glm_quat_mat4(state->currentModel.transform.rotation, rot);
    glm_rotate(rot, glm_rad(90) * deltaTime, Vec3_Up);
    // glm_mat4_quat(rot, state->currentModel.transform.rotation);
    if (blxInputGetKeyDown(BLX_KEY_E))
    {
        char buffer[blxMaxFilePath];
        if (blxOpenFilePanel("Choose obj to load", NULL, ".obj", buffer))
        {
            BLXDEBUG("%s", buffer);
            blxImportModelFromObj(&state->currentModel, buffer);
        }
    }

    // if (blxInputGetKeyDown(BLX_KEY_F)) {
    //     shader_setVec3(state->currentModel.mesh.shader, "objectColor", (vec3) {
    //     1, 1, 1 });
    // }

    if (blxInputGetKeyDown(BLX_KEY_1))
    {
        blxSetShadingMode(BLX_SHADING_SOLID);
    }
    if (blxInputGetKeyDown(BLX_KEY_2))
    {
        blxSetShadingMode(BLX_SHADING_WIREFRAME);
    }

    if (blxGetMouseButtonDown(BLX_LMB))
    {
        // SpawnPhysicsObject();
        pressed = BLX_TRUE;
    }

    accumlatedTime += deltaTime;
    if (accumlatedTime >= 0.1f && pressed)
    {
        // TODO: FIX THIS METHOD NO WORKY!
        // SpawnPhysicsObject();
        accumlatedTime = 0.0f;
        pressed = BLX_FALSE;
    }

    return BLX_TRUE;
}

blxBool Render()
{
    // blxImGuiBtn();
    // Checking if mesh has been initialized by the renderer.
    // TODO: This should probably be handled by the engine itself.
    if (state->currentModel.geometries != NULL)
    {
        blxDrawModel(&state->currentModel);
    }

    for (int i = 0; i < blxGetListCount(ballObjects); i++)
    {
        blxDrawModel(&ballObjects[i]);
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
        glm_vec3_muladds(forward, moveSpeed, camTrans->position);
    }

    if (blxInputGetKey(BLX_KEY_S))
    {
        vec3 forward;
        blxTransform_Forward(camTrans, forward);
        glm_vec3_negate(forward);
        glm_vec3_muladds(forward, moveSpeed, camTrans->position);
    }

    if (blxInputGetKey(BLX_KEY_A))
    {
        vec3 right;
        blxTransform_Right(camTrans, right);
        glm_vec3_muladds(right, moveSpeed, camTrans->position);
    }

    if (blxInputGetKey(BLX_KEY_D))
    {
        vec3 left;
        blxTransform_Right(camTrans, left);
        glm_vec3_negate(left);
        glm_vec3_muladds(left, moveSpeed, camTrans->position);
    }
}

void ProcessMouseMove(float deltaTime)
{
    if (!blxInputGetMouseButton(BLX_RMB))
    {
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

// TODO: This should be moved to the physics module of our sandbox.
void SpawnPhysicsObject()
{
    blxModel newBall;

    blxModel_Init(&newBall);
    blxRenderableGeometry geometry = {
        .mesh = ballModel.geometries[0].mesh, .material = blxMaterial_CreateDefault(), .transform = &newBall.transform};

    vec4s randomColor = {
        .r = ((float)rand() / RAND_MAX), .g = ((float)rand() / RAND_MAX), .b = ((float)rand() / RAND_MAX), .a = 1.0f};

    float randScale = ((float)rand() / RAND_MAX) * (1.5f - 0.2f) + 0.2f; // Scale between 0.2 and 2.0

    glm_vec3_scale(newBall.transform.scale, randScale, newBall.transform.scale);

    blxMaterial_SetValue(geometry.material, "baseColor", BLX_MAT_PROP_VEC4, &randomColor);

    blxAddValueToList(newBall.geometries, geometry);
    // blxMemCpy(&newBall, &ballModel, sizeof(blxModel));
    // newBall.geometries[0].transform = &newBall.transform;

    blxAddValueToList(ballObjects, newBall);

    blxParticle* particle = static_cast<blxParticle*>(blxAllocate(sizeof(blxParticle), BLXMEMORY_TAG_PHYSICS));
    blxParticle_Init(particle, &ballObjects[blxGetListCount(ballObjects) - 1].transform);

    int count = blxGetListCount(ballObjects);

    blxModel* lastBall = &ballObjects[blxGetListCount(ballObjects) - 1];

    lastBall->geometries[0].transform = &lastBall->transform;

    float speed = 25.0f; // desired launch speed

    // Generate random components in range [-1, 1]
    float rx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    float ry = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    float rz = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    // Ensure Y is positive
    if (ry < 0.0f)
        ry = -ry;

    // Normalize vector
    float mag = sqrtf(rx * rx + ry * ry + rz * rz);
    if (mag == 0.0f)
        mag = 1.0f;

    blxVec3 dir = {.x = rx / mag, .y = ry / mag, .z = rz / mag};

    // Scale by speed
    blxVec3 velocity = blxVec3_Mult(dir, speed);

    // Set the particle's velocity
    particle->velocity = velocity;
}
