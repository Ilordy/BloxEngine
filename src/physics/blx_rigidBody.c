#include "blx_rigidBody.h"

//TODO: Fix this file...

void blxRigidBody_Init(blxRigidBody* body, const blxTransform* gameObject)
{
    //body->transform = *gameObject;
    body->velocity = BLX_VEC3_ZERO;
    body->acceleration = BLX_VEC3_ZERO;
    body->forceAccumulator = BLX_VEC3_ZERO;

    // Default damping value (no damping)
    //body->damping = 1.0f;

    // Default to unit mass (1.0f), which means mass = 1.0f
    body->inverseMass = 1.0f;

    // Add the rigid body to the physics world.
    //AddRigidBodyToPhysicsWorld(body);
}

 // Transform the inverse inertia tensor from local space to world space.
 
void blxRigidBody_TransformInertiaTensor(blxRigidBody* body)
{
    glm_quat_normalize(body->transform->rotation);

    mat3 rotMat;
    glm_quat_mat3(body->transform->rotation, rotMat);

    mat3 intertiaTensor;
    glm_mat3_inv(body->inverseInertiaTensor, intertiaTensor);

    mat3 rotMatTranspose;
    glm_mat3_transpose_to(rotMat, rotMatTranspose);

    mat3 m0;
    glm_mat3_mul(rotMat, intertiaTensor, m0);
    glm_mat3_mul(m0, rotMatTranspose, body->inverseInertiaTensorWorld);
    glm_mat3_inv(body->inverseInertiaTensorWorld, body->inverseInertiaTensorWorld);
}

void blxRigidBody_SetMass(blxRigidBody* body, const float mass)
{
    if (mass <= 0.0f) {
        // Set inverse mass to zero if the provided mass is less than or equal to zero.
        // This is essentially a way to handle "infinite mass" or "immovable" bodies.
        body->inverseMass = 0;
        return;
    }

    // Set the inverse mass to the reciprocal of the provided mass.
    body->inverseMass = 1.0f / mass;
}

void blxRigidBody_AddForce(blxRigidBody* body, const blxVec3 force)
{
    // Add the force to the force accumulator.
    body->forceAccumulator = blxVec3_Add(body->forceAccumulator, force);
    // Wake the body up if a force is applied.
    body->isAwake = BLX_TRUE;
}

void blxRigidBody_ClearAccumulators(blxRigidBody* body)
{
   // Clear the force and torque accumulators.
    body->forceAccumulator = BLX_VEC3_ZERO;
    body->torqueAccumulator = BLX_VEC3_ZERO;
}

void blxRigidBody_AddForceAtPoint(blxRigidBody* body, const blxVec3 force, const blxVec3 point)
{

    blxVec3 pt = point;

    // TODO: Rework to use blxVec3Sub instead.
    glm_vec3_sub(pt.raw, body->transform->position, pt.raw);

     // Add the force to the force accumulator.
    body->forceAccumulator = blxVec3_Add(body->forceAccumulator, force);

    body->torqueAccumulator = blxVec3_Add(body->torqueAccumulator,
        blxVec3_Cross(pt, force));

    // Wake the body up if a force is applied.
    body->isAwake = BLX_TRUE;

    // 
}

void blxRigidBody_AddForceAtPointLocal(blxRigidBody* body, const blxVec3 force, const blxVec3 localPoint)
{
    // TODO: Rework to use blxVec3 instead once we change the transform file.
    vec3s localPt = glms_vec3_make(localPt.raw);

    vec3s pt = blxTrasform_TransformPoint(body->transform, localPt);

    blxVec3 forceWorld;
    glm_vec3_copy(force.raw, forceWorld.raw);

    blxRigidBody_AddForceAtPoint(body, force, forceWorld);
}

void blxRigidBody_AddTorque(blxRigidBody* body, const blxVec3 torque)
{
    // Add the torque to the torque accumulator.
    body->torqueAccumulator = blxVec3_Add(body->torqueAccumulator, torque);

    // Wake the body up if a torque is applied.
    body->isAwake = BLX_TRUE;
}

blxBool blxRigidBody_HasFiniteMass(const blxRigidBody* body)
{
    return (body->inverseMass > 0.0f);
}

