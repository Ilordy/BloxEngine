#pragma once
#include "core/blx_defines.h"
#include "core/blx_transform.h"
#include "maths/blx_vec3.h"
#include "core/blx_object.h"

// TODO: Change to blxvec3 in the future.
// TODO: These variables in this particle struct should probably all be private.

typedef struct blxRigidBody {
    /// @brief Transform this rigid body is using.
    blxTransform* transform;

    /// @brief Linear Velocity of the rigid body.
    blxVec3 velocity;

    /// @brief Angular velocity of the rigid body.
    blxVec3 angularVelocity;

    /// @brief Acceleration of the particle.
    blxVec3 acceleration;

    /// @brief Acceleration of the rigid body in the last frame.
    blxVec3 lastFrameAcceleration;

    /// @brief Damping factor for the body's velocity. (used to fake drag).
    /// @note A value of 1.0f means no damping, 0.0f means full damping (velocity becomes zero).
    float linearDamping;

    /// @brief Angular damping factor for the body's angular velocity. (used to fake rotational drag).
    /// @note A value of 1.0f means no damping, 0.0f means full damping (angular velocity becomes zero).
    float angularDamping;

    /// @brief This is the reciprocal of the mass, used to avoid division by zero in calculations. (1 / mass).
    /// @note DO NOT set this value directly, use the `blxParticle_SetMass` function instead.
    float inverseMass;

    /// @brief Holds the accumlated force to be applied to the particle. This is used to accumulate forces like gravity, collisions, etc.
    /// @note This vector is cleared after each physics update.
    blxVec3 forceAccumulator;

    /// @brief Holds the accumulated torque to be applied to the rigid body. This is used to accumulate torques from forces, collisions, etc.
    /// @note This vector is cleared after each physics update.
    blxVec3 torqueAccumulator;

    // TODO: USE BLXMAT3 INSTEAD OF CGLM MAT3. 
    // TODO: Verify why exactly we need the inverse of these matrices...

    /// @brief Inverse inertia tensor for rotational motion.
    /// @note This is a 3x3 matrix that represents the inertia of the body around its center of mass in local space.
    mat3 inverseInertiaTensor;

    /// @brief Inverse inertia tensor in world space.
    mat3 inverseInertiaTensorWorld;

    /// @brief Rigidbodies that are awake will be updated in the physics world.
    blxBool isAwake;

}blxRigidBody;

// TODO: This should either be moved to a add component function, or we should have a blxParticleComponent that can be added to a blxObject.

BLXAPI void blxRigidBody_Init(blxRigidBody* body, const blxTransform* gameObject);

BLXAPI void blxRigidBody_ClearAccumulators(blxRigidBody* body);

/// @brief Adds a force to the center of the rigid body. (No torque is applied).
/// @param body The rigid body to which the force will be applied.
/// @param force The force vector to apply (Given in world space).
BLXAPI void blxRigidBody_AddForce(blxRigidBody* body, const blxVec3 force);

/// @brief Adds a given force to the rigid body at specified point in world space.
/// @param body The rigid body to which the force will be applied.
/// @param force The force vector to apply (In world coordinates).
/// @param point The point in world space where the force is applied.
BLXAPI void blxRigidBody_AddForceAtPoint(blxRigidBody* body, const blxVec3 force, const blxVec3 point);

/// @brief Adds a given force to the rigid body at specified point in local space.
/// @param body The rigid body to which the force will be applied.
/// @param force The force vector to apply (In world coordinates).
/// @param localPoint The point in local space where the force is applied.
BLXAPI void blxRigidBody_AddForceAtPointLocal(blxRigidBody* body, const blxVec3 force, const blxVec3 localPoint);

/// @brief Adds a torque to the rigid body.
/// @param body The rigid body to which the torque will be applied.
/// @param torque The torque vector to apply (Given in world space).
BLXAPI void blxRigidBody_AddTorque(blxRigidBody* body, const blxVec3 torque);

/// @brief Returns true if the rigid body has a finite mass (i.e., inverse mass is not zero).
BLXAPI blxBool blxRigidBody_HasFiniteMass(const blxRigidBody* body);

// TODO: Verify is this is only needed by the physics world and not every rigid body.
BLXAPI void blxRigidBody_TransformInertiaTensor(blxRigidBody* body);