#include "rendering/blx_shader.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

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

Shader shader_create(const char* fragPath, const char* vertPath, GLboolean useShader)
{
	char* fragSource = shader_parse(fragPath);
	char* vertSource = shader_parse(vertPath);
	blxBool shaderSuccess;
	if (!vertSource)
	{
		printf("[SHADER ERROR] Vertex Shader file path not found at: %s", vertPath);
		return -1;
	}
	if (!fragSource)
	{
		printf("[SHADER ERROR] Fragment Shader file path not found at: %s", fragPath);
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);
	if (shader_checkError(vertexShader, GL_COMPILE_STATUS))
	{
		return -1;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	if (shader_checkError(fragmentShader, GL_COMPILE_STATUS))
	{
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// TODO: ERROR IS HERE ON LINE 91, PLS FIX!
	//Use glGetProgramiv instead of shaderiv.
	// if (shader_checkError(shaderProgram, GL_LINK_STATUS))
	// {
	// 	return -1;
	// }

	if (useShader)
	{
		glUseProgram(shaderProgram);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	free(fragSource);
	free(vertSource);
	return shaderProgram;
}

void shader_setBool(Shader shader, const char* name, GLboolean value)
{
	glUniform1i(shader_getUniform(shader, name), value);
}

void shader_setInt(Shader shader, const char* name, GLint value)
{
	glUniform1i(shader_getUniform(shader, name), value);
}

void shader_setFloat(Shader shader, const char* name, GLfloat value)
{
	glUniform1f(shader_getUniform(shader, name), value);
}

void shader_setFloat4f(Shader shader, const char* name, vec4 value)
{
	glUniform4f(shader_getUniform(shader, name), value[0], value[1], value[2], value[3]);
}

void shader_setMatrix4f(Shader shader, const char* name, mat4 mat)
{
	glUniformMatrix4fv(shader_getUniform(shader, name), 1, GL_FALSE, mat);
}

void shader_setVec3(Shader shader, const char* name, vec3 value) 
{
	glUniform3f(shader_getUniform(shader, name), value[0], value[1], value[2]);
}

void shader_useShader(Shader shader)
{
	glUseProgram(shader);
}

void shader_useTexture(Shader shader, Texture* texture, const char* samplerName)
{
	texture_setActive(texture);
	shader_setInt(shader, samplerName, texture->textureSlot);
}
