#include "core/blx_defines.h"
#include "physics/blx_particle.h"
#include "physics/blx_rigidBody.h"

void InitializePhysicsWorld();

void UpdatePhysicsWorld(float dt);

void ShutdownPhysicsWorld();

void AddParticleToPhysicsWorld(blxParticle* particle);
void RemoveParticleFromPhysicsWorld(blxParticle* particle);

// TODO: Add RigidBody functions later.
// TODO: also we need to add a broadphase collision detection system later. example: blx_broadphase.h using a BVH