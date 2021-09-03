

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 lightSpaceMatrix2;

out vec3 fragPos;
out vec4 fragPosLightSpace;
out vec4 fragPosLightSpace2;
out vec3 normalFrag;
out vec3 camPosFrag;
out vec2 texCoordFrag;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   // https://learnopengl.com/Lighting/Basic-Lighting
   normalFrag = mat3(transpose(inverse(model))) * normal;
   fragPos = vec3(model * vec4(aPos, 1.0));
   fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
   fragPosLightSpace2 = lightSpaceMatrix2 * vec4(fragPos, 1.0);
   camPosFrag = camPos;
   texCoordFrag = texCoord;
}


