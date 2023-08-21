#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
// layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

in vec4 gl_Position;

// out vec3 ourColor;
out vec2 texCoord;
out vec3 normal;
out vec3 FragPos;
out vec3 LightDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalTrans;

struct Light {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  FragPos = vec3(view * model * vec4(aPos, 1.0));
  LightDir = vec3(view * vec4(light.direction, 0.0f));
  normal = normalTrans * aNormal;
  // ourColor = aColor;
  texCoord = aTex;
}