#include "rendering/blx_shader.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "utils/blx_fileManagement.h"
#include "internal/opengl.h"
#include "core/blx_memory.h"

typedef struct {
	void (*SetFloat) (Shader shader, const char* name, float value);
	void (*SetInt) (Shader shader, const char* name, int value);
	void (*SetBool) (Shader shader, const char* name, blxBool value);
	void (*SetVec3f) (Shader shader, const char* name, vec3 value);
	void (*SetVec4f) (Shader shader, const char* name, vec4 value);
	void (*SetMatrix4f) (Shader shader, const char* name, mat4 value);
	Shader(*CreateShader) (const char* fragSource, const char* vertSource);
}ShaderSystem;

ShaderSystem shaderSystem;

void _blxShaderSystemInitialize(GraphicsAPI graphicApi)
{
	switch (graphicApi)
	{
		case OPENGL:
			shaderSystem.SetFloat = blxGLSetFloat;
			shaderSystem.SetInt = blxGLSetInt;
			shaderSystem.SetVec4f = blxGLSetVec4f;
			shaderSystem.SetBool = blxGLSetBool;
			shaderSystem.SetVec3f = blxGLSetVec3f;
			shaderSystem.SetMatrix4f = blxGLSetMatrix4f;
			shaderSystem.CreateShader = blxGLCreateShader;
			break;
	}
}

blxBool shader_checkError(GLuint shader, GLenum errorType)
{
	int success;
	char log[512];
	glGetShaderiv(shader, errorType, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("[SHADER ERROR] %s", log);
		glGetShaderSource(shader, 512, NULL, log);
		printf("%s", log);
	}
	return !success;
}

GLint shader_getUniform(Shader shader, const char* name)
{
	GLint loc = glGetUniformLocation(shader, name);
	if (loc == -1)
	{
		printf("[SHADER ERROR] Shader uniform %s not found!", name);
	}
	return loc;
}

char* shader_parse(const char* path)
{

	FILE* shaderFile = fopen(path, "r");
	struct _stat st;

	if (stat(path, &st) != 0)
	{
		return 0;
	}

	const long size = st.st_size;
	char* buffer = malloc(size);
	size_t bytesRead = fread(buffer, sizeof(char), size, shaderFile);
	buffer[bytesRead] = '\0';

	fclose(shaderFile);
	return buffer;
}

Shader blxShaderCreate(const char* fragPath, const char* vertPath, GLboolean useShader)
{
	blxFile* fragFile;
	blxFile* vertFile;

	blxOpenFile(fragPath, BLX_FILE_MODE_READ, &fragFile);
	blxOpenFile(vertPath, BLX_FILE_MODE_READ, &vertFile);
	uint64 fragSize, vertSize;
	char* fragSource = blxFileReadAllText(fragFile, &fragSize);
	char* vertSource = blxFileReadAllText(vertFile, &vertSize);

	Shader shader = shaderSystem.CreateShader(fragSource, vertSource);

	blxFree(fragSource, fragSize, BLXMEMORY_TAG_STRING);
	blxFree(vertSource, vertSize, BLXMEMORY_TAG_STRING);

	return shader;

	// char* fragSource = shader_parse(fragPath);
	// char* vertSource = shader_parse(vertPath);



	// blxBool shaderSuccess;
	// if (!vertSource)
	// {
	// 	printf("[SHADER ERROR] Vertex Shader file path not found at: %s", vertPath);
	// 	return -1;
	// }

	// if (!fragSource)
	// {
	// 	printf("[SHADER ERROR] Fragment Shader file path not found at: %s", fragPath);
	// 	return -1;
	// }

	// unsigned int vertexShader;
	// vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vertexShader, 1, &vertSource, NULL);
	// glCompileShader(vertexShader);
	// if (shader_checkError(vertexShader, GL_COMPILE_STATUS))
	// {
	// 	return -1;
	// }

	// unsigned int fragmentShader;
	// fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(fragmentShader, 1, &fragSource, NULL);
	// glCompileShader(fragmentShader);
	// if (shader_checkError(fragmentShader, GL_COMPILE_STATUS))
	// {
	// 	return -1;
	// }

	// unsigned int shaderProgram;
	// shaderProgram = glCreateProgram();
	// glAttachShader(shaderProgram, vertexShader);
	// glAttachShader(shaderProgram, fragmentShader);
	// glLinkProgram(shaderProgram);
	// // TODO: ERROR IS HERE ON LINE 91, PLS FIX!
	// //Use glGetProgramiv instead of shaderiv.
	// // if (shader_checkError(shaderProgram, GL_LINK_STATUS))
	// // {
	// // 	return -1;
	// // }

	// if (useShader)
	// {
	// 	glUseProgram(shaderProgram);
	// }

	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);

	// free(fragSource);
	// free(vertSource);
	// return shaderProgram;
}

void blxShaderSetBool(Shader shader, const char* name, GLboolean value)
{
	shaderSystem.SetBool(shader, name, value);
}

void blxShaderSetInt(Shader shader, const char* name, GLint value)
{
	shaderSystem.SetInt(shader, name, value);
}

void blxShaderSetFloat(Shader shader, const char* name, GLfloat value)
{
	shaderSystem.SetFloat(shader, name, value);
}

void blxShaderSetVec4(Shader shader, const char* name, vec4 value)
{
	shaderSystem.SetVec4f(shader, name, value);
}

void blxShaderSetMatrix4f(Shader shader, const char* name, mat4 mat)
{
	shaderSystem.SetMatrix4f(shader, name, mat);
}

void blxShaderSetVec3(Shader shader, const char* name, vec3 value)
{
	shaderSystem.SetVec3f(shader, name, value);
}

void blxShaderUseShader(Shader shader)
{
	glUseProgram(shader);
}

void blxShaderUseTexture(Shader shader, Texture* texture, const char* samplerName)
{
	texture_setActive(texture);
	blxShaderSetInt(shader, samplerName, texture->textureSlot);
}
