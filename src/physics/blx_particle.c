#include "internal/physicsWorld.h"
#include "blx_particle.h"

BLXAPI void blxParticle_Init(blxParticle* particle, const blxTransform* gameObject)
{
    particle->position = &gameObject->position;
    particle->velocity = BLX_VEC3_ZERO;
    particle->acceleration = BLX_VEC3_ZERO;
    particle->forceAccumulator = BLX_VEC3_ZERO;

    // Default damping value (no damping)
    particle->damping = 1.0f;

    // Default to unit mass (1.0f), which means mass = 1.0f
    particle->inverseMass = 1.0f;

    // Add the particle to the physics world.
    AddParticleToPhysicsWorld(particle);
}

void blxParticle_SetMass(blxParticle* particle, const float mass) {
    if (mass <= 0.0f) {
        // Set inverse mass to zero if the provided mass is less than or equal to zero.
        // This is essentially a way to handle "infinite mass" or "immovable" particles.
        particle->inverseMass = 0;
        return;
    }

    // Set the inverse mass to the reciprocal of the provided mass.
    particle->inverseMass = 1.0f / mass;
}

float blxParticle_GetMass(const blxParticle* particle) {
    // If the inverse mass is zero, it means the particle is immovable (infinite mass).
    if (particle->inverseMass == 0.0f) {
        return 0.0f; // or return a special value indicating immovable state
    }

    // Return the mass as the reciprocal of the inverse mass.
    return 1.0f / particle->inverseMass;
}

float blxParticle_GetKineticEnergy(const blxParticle* particle)
{
    // Kinetic energy formula: KE = 0.5 * m * ||v||^2
    // where m is the mass and v is the velocity.
    float mass = blxParticle_GetMass(particle);

    // No kinetic energy if the particle is immovable.
    if (mass <= 0.0f) {
        return 0.0f;
    }

    return 0.5f * mass * blxVec3_SquaredMagnitude(particle->velocity);
}

void blxParticle_AddForce(blxParticle* particle, const blxVec3 force)
{
    // Add the force to the force accumulator.
    particle->forceAccumulator = blxVec3_Add(particle->forceAccumulator, force);
}