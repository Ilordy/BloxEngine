#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;


//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//TODO: This should be part of a generic include header file.
layout(std430, binding = 0) readonly buffer ssbo1
{
	mat4 model[];
};

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

void main()
{
	gl_Position = projection * view * model[gl_BaseInstance] * vec4(aPos, 1.0f);
	//TODO: Normal matrix should be set in the CPU, very expensive calculation for each vertex.
	normal = mat3(transpose(inverse(model[gl_BaseInstance]))) * aNormal;
	texCoord = aTexCoord;
	fragPos = vec3(model[gl_BaseInstance] * vec4(aPos, 1.0f));
}