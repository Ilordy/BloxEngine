#pragma once
#include "core/blx_defines.h"
#include "core/blx_transform.h"
#include "maths/blx_vec3.h"
#include "core/blx_object.h"

// TODO: Change to blxvec3 in the future.
// TODO: These variables in this particle struct should probably all be private.

/// @brief Represents a particle in the physics system.
typedef struct blxParticle {
    // TODO: vec3* position should probably just be a pointer to a transform..

    /// @brief world transform of the particle.
    /// @note For now we are using the cglm version of vec3, but we might change this to our own blxVec3 in the future.
    vec3* position;

    /// @brief Velocity of the particle.
    blxVec3 velocity;

    /// @brief Acceleration of the particle.
    blxVec3 acceleration;

    /// @brief Damping factor for the particle's velocity. (used to fake drag).
    /// @note A value of 1.0f means no damping, 0.0f means full damping (velocity becomes zero).
    float damping;

    /// @brief This is the reciprocal of the mass, used to avoid division by zero in calculations. (1 / mass).
    /// @note DO NOT set this value directly, use the `blxParticle_SetMass` function instead.
    float inverseMass;

    /// @brief Holds the accumlated force to be applied to the particle. This is used to accumulate forces like gravity, collisions, etc.
    /// @note This vector is cleared after each physics update.
    blxVec3 forceAccumulator;

} blxParticle;

// TODO: This should either be moved to a add component function, or we should have a blxParticleComponent that can be added to a blxObject.
BLXAPI void blxParticle_Init(blxParticle* particle, const blxTransform* gameObject);

/// @brief Sets the mass of a particle.
/// @note If the mass is set to zero or a negative value, the particle will be considered immovable (infinite mass).
/// @param particle The particle to set the mass for.
/// @param mass The value to set as the mass of the particle.
BLXAPI void blxParticle_SetMass(blxParticle* particle, const float mass);

/// @brief Gets the mass of a particle.
BLXAPI float blxParticle_GetMass(const blxParticle* particle);

BLXAPI float blxParticle_GetKineticEnergy(const blxParticle* particle);

BLXAPI void blxParticle_AddForce(blxParticle* particle, const blxVec3 force);


