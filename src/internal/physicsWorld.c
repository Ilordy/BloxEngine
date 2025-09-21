#include "internal/physicsWorld.h"
#include "core/blx_memory.h"
#include "core/blx_logger.h"

#define LIST_MEM_TAG BLXMEMORY_TAG_PHYSICS
#include "utils/blx_vlist.h"

static blxParticle** particleList;
static blxRigidBody** rigidBodyList;

void InitializePhysicsWorld()
{
    // Initialize the physics world, e.g., set up gravity, collision detection, etc.
    // For now, we just initialize an empty list for particles.
    particleList = blxInitList(blxParticle*);
    rigidBodyList = blxInitList(blxRigidBody*);
    BLXINFO("Physics World Initialized");
}

// TODO: THIS IS TEMP FOR NOW.
// WE NEED THIS TO UPDATE THE QUAT BY A ROTATION SCALED BY DELTA TIME.
BLXINLINE void AddScaledVectorToQuat(Quaternion quat, const blxVec3 vectorToScale, float scale)
{
    // Add a scaled vector to a quaternion.
    Quaternion q = {
        vectorToScale.x * scale,
        vectorToScale.y * scale,
        vectorToScale.z * scale,
        0.0f // The scalar part is zero for a pure vector addition.
    };
    glm_quat_mul(q, quat, q);

    quat[0] += q[0] * 0.5f;
    quat[1] += q[1] * 0.5f;
    quat[2] += q[2] * 0.5f;
    quat[3] += q[3] * 0.5f;
}

void UpdatePhysicsWorld(float dt)
{
    // Update the physics world, e.g., integrate particle positions, handle collisions, etc.

    for (uint64 i = 0; i < blxGetListCount(rigidBodyList); i++)
    {
        blxRigidBody* body = rigidBodyList[i];

        // Calculate linear acceleration from force inputs.
        body->lastFrameAcceleration = body->acceleration;
        body->lastFrameAcceleration = blxVec3_AddScaledVector(body->lastFrameAcceleration,
            body->forceAccumulator, body->inverseMass);

        // Update linear velocity from both acceleration and impulse.
        body->velocity = blxVec3_AddScaledVector(body->velocity,
            body->lastFrameAcceleration, dt);

        // Calculate angular acceleration from torque inputs.
        vec3 angularAcceleration;
        glm_mat3_mulv(body->inverseInertiaTensorWorld, body->torqueAccumulator.raw, angularAcceleration);

        // TODO: Remove this once we remove cglm.
        blxVec3 angAcc = {
            angularAcceleration[0],
            angularAcceleration[1],
            angularAcceleration[2]
        };
        body->angularVelocity = blxVec3_AddScaledVector(body->angularVelocity,
            angAcc, dt);

        // Impose drag
        body->velocity = blxVec3_Mult(body->velocity, powf(body->linearDamping, dt));
        body->angularVelocity = blxVec3_Mult(body->angularVelocity, powf(body->angularDamping, dt));

        // Update linear position of the rigid body.
        glm_vec3_add(body->transform->position, blxVec3_Mult(body->velocity, dt).raw, body->transform->position);

        // Update angular position of the rigid body.
        AddScaledVectorToQuat(body->transform->rotation, body->angularVelocity, dt);

        // Normalise the orientation, and update the matrices with the new
        // position and orientation
        blxRigidBody_TransformInertiaTensor(body);

        // Clear force and torque accumulators.
        blxRigidBody_ClearAccumulators(body);
    }

    return;

    for (uint64 i = 0; i < blxGetListCount(particleList); i++)
    {
        blxParticle* particle = particleList[i];

        // Skip particles with zero or negative mass.
        if (particle->inverseMass <= 0.0f) {
            continue;
        }

        // Calculate the time step velocity based on the particle's current velocity and the time step.
        blxVec3 timeStepVelocity = blxVec3_Mult(particle->velocity, dt);

        // TODO: THIS IS TEMP FOR NOW TILL WE REMOVE THE GLM VEC3 FROM TRANSFORM.
        // Update linear position of the particle.
        glm_vec3_add(*particle->position, timeStepVelocity.raw, *particle->position);

        // TODO: ADDING TEMP GRAVITY FOR NOW.
        // In the future, we might want to add a global gravity vector to the physics world
        // Add gravity
        particle->acceleration = blxVec3_Add(particle->acceleration, (blxVec3) { 0.0f, -9.81f, 0.0f });

         // In the future we'll add to this vector for forces.
        blxVec3 resultingAcc = particle->acceleration;

        resultingAcc = blxVec3_Add(resultingAcc, blxVec3_Mult(particle->forceAccumulator, particle->inverseMass));

        // Update linear velocity of the particle.
        particle->velocity = blxVec3_Add(particle->velocity,
            blxVec3_Mult(resultingAcc, dt));

        // Impose damping on the particle's velocity.
        particle->velocity = blxVec3_Mult(particle->velocity,
            powf(particle->damping, dt));

        // Clear the force accumulator after applying forces.
        particle->forceAccumulator = BLX_VEC3_ZERO;
    }
}

void ShutdownPhysicsWorld()
{
    // Clean up the physics world, e.g., free resources, clear particle lists, etc.
    // For now, we just log the shutdown.
    BLXINFO("Physics World Shutdown");
}

void AddParticleToPhysicsWorld(blxParticle* particle)
{
    // Add a particle to the physics world.
    blxAddValueToList(particleList, particle);

}

void RemoveParticleFromPhysicsWorld(blxParticle* particle)
{
    // Remove a particle from the physics world.
    // For now, we just log the removal of the particle.

    // TODO: Implement actual removal logic.
    // TODO: This involves using memcmp in the blx_vList to find the particle.
}