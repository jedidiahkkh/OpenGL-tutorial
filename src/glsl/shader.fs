#version 330 core
// in vec3 ourColor;
in vec2 texCoord;
in vec3 FragPos;
// in vec3 LightPos;
// in vec3 LightDir;
in vec3 normal;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

uniform Material material;

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

void main()
{
  vec3 norm = normalize(normal);

  vec3 result = calcDirLight(dirLight, norm, FragPos);
  for (int i = 0 ; i < NR_POINT_LIGHTS ; i++) {
    result += calcPointLight(pointLights[i], norm, FragPos);
  }
  result = max(result,calcSpotLight(spotLight, norm, FragPos));
  FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = normalize(light.direction);
  vec3 viewDir = normalize(-fragPos); // viewer at (0,0,0)

  // ambient
  vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

  //diffuse
  float diff = max(dot(normal, -lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoord));

  //specular
  vec3 reflectDir = reflect(lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoord));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = normalize(fragPos - light.position);
  vec3 viewDir = normalize(-fragPos); // viewer at (0,0,0)

  // attenuation
  float distance = length(fragPos - light.position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  // ambient
  vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

  //diffuse
  float diff = max(dot(normal, -lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoord));

  //specular
  vec3 reflectDir = reflect(lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoord));

  return (ambient + diffuse + specular) * attenuation;
}
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = normalize(fragPos - light.position);
  vec3 viewDir = normalize(-fragPos); // viewer at (0,0,0)

  // attenuation
  // float distance = length(fragPos - light.position);
  // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  // intensity
  float theta = dot(lightDir, normalize(light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // ambient
  vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

  //diffuse
  float diff = max(dot(normal, -lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.emission, texCoord));

  //specular
  vec3 reflectDir = reflect(lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoord));

  return ambient /* * attenuation */ + (diffuse + specular) * intensity;
}