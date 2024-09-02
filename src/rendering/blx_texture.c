#include "rendering/blx_texture.h"
#include "vendor/stb_image/stb_image.h"


void texture_init(Texture* texture, const char* imagePath, blxBool flip, GLenum texSlot)
{
	stbi_set_flip_vertically_on_load(flip);
	texture->textureSlot = texSlot;
	texture->imageBuffer = stbi_load(imagePath, &texture->width, &texture->height, &texture->channels, 0);
	glGenTextures(1, &texture->ID);
	texture_setActive(texture);
}

//Might wanna make a float version of this.
void texture_setParameter(GLenum pName, GLint param)
{
	glTexParameteri(GL_TEXTURE_2D, pName, param);
}

void texture_setActive(Texture* texture)
{
	glActiveTexture(GL_TEXTURE0 + texture->textureSlot);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
}

void texture_generateTexture(Texture* texture)
{
	//Update this later pls
	GLint format = GL_RGBA;
	if (texture->channels == 3) {
		format = GL_RGB;
	}
	texture_setActive(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->imageBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(Texture* texture)
{
	glDeleteTextures(1, &texture->ID);
	stbi_image_free(texture->imageBuffer);
}

