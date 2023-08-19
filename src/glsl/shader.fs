#version 330 core
// in vec3 ourColor;
// in vec2 texCoord;
in vec3 FragPos;
in vec3 LightPos;
in vec3 normal;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{

  // ambient lighting
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse lighting
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(LightPos - FragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // specular lighting
  float specularStrength = 0.5;
  float shininess =  32;

  // viewer at 0,0,0 in view space
  vec3 viewDir = normalize(- FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}