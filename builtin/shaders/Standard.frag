#version 460 core
out vec4 FragColor;

in vec3 normal; 
in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D textureImage;
uniform sampler2D textureImage2;
uniform vec3 camPos = vec3(0.0f, 0.0f, 0.0f);;
uniform vec3 baseColor = vec3(1.0f, 0.5f, 0.2f);
uniform float specular = 0.5;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform float ambientStrength = 0.7;
//hard coded test values for now.

vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
vec3 lightColor = vec3(1f, 1f, 1f);

void main()
{
   
   vec3 ambient = ambientStrength * ambientColor;
   vec3 norm = normalize(normal);
 
   vec3 lightDir = normalize(lightPos - fragPos);
   float diff = max(dot(norm, lightDir), 0.0f);
   vec3 diffuse = diff * lightColor;
   vec3 viewDir = normalize(camPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   //spec value should be 2, 4, 8, 16, 32, 64, 128, or 256.   
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
   vec3 specular = specular * spec * lightColor;

   vec3 result = (ambient + diffuse + specular) * baseColor;
   FragColor = vec4(result, 1.0f);
}