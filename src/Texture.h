#pragma once
#include "Defines.h"
#include "GL/glew.h"

// Might want to have an active slot that texture uses and can be switched.
// In the future we can have a dictionary array for setting up gl settings
// params.
typedef struct {
  int width, height, channels;
  unsigned char* imageBuffer;
  unsigned int ID, textureSlot;
} Texture;

BLXAPI void texture_init(Texture* texture, const char* imagePath, blxBool flip,
  GLenum texSlot);

/// <summary>
/// Frees the given texture's image memory,
/// and calls glDeleteTextures().
/// Call before exiting program.
/// </summary>
BLXAPI void texture_delete(Texture* texture);

BLXAPI void texture_setParameter(GLenum pName, GLint param);

BLXAPI void texture_setActive(Texture* texture);

/// <summary>
/// Generates the given texture.
/// Use after updating texture parameters to
/// apply changes.
/// </summary>
BLXAPI void texture_generateTexture(Texture* texture);