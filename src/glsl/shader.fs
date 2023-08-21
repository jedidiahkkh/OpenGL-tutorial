#version 330 core
// in vec3 ourColor;
in vec2 texCoord;
in vec3 FragPos;
in vec3 LightPos;
in vec3 LightDir;
in vec3 normal;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  // sampler2D emission;
  float shininess;
};

uniform Material material;

struct Light {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Light light;

void main()
{

  // ambient lighting
  float ambientStrength = 0.1;
  vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(LightPos - FragPos);//normalize(-LightDir);
  float distance = length(LightPos - FragPos);
  float attenuation = 1.0/ (light.constant - light.linear * distance + light.quadratic * (distance * distance));
  
  float theta = dot(lightDir, normalize(-LightDir));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // diffuse lighting
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoord));

  // specular lighting
  float specularStrength = 0.5;
  float shininess =  32;

  // viewer at 0,0,0 in view space
  vec3 viewDir = normalize(- FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoord));

  vec3 result = ambient * attenuation + (specular + diffuse) * intensity;// + vec3(texture(material.emission, texCoord));
  FragColor = vec4(result, 1.0);
}