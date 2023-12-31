#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iostream>
#include "shader.h"
#include <fly-camera.h>
#include <flight-camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

inline glm::vec3 transformVec3(glm::mat4 matrix, glm::vec3 vector, bool translate = true);

int gwidth = 800, gheight = 600;

// float vertices[] = {
//   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//   0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
// };
// vertices with color and texel data
// float vertices[] = { 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//                     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//                     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//                     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

// unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

// float vertices2[] = { 0.0f, 1.0f, 0.0f,
//                       1.0f, 1.0f, 0.0f,
//                       1.0f, 0.0f, 0.0f };

float cube[] = {
  // positions          // normals           // texture coords
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// float mixAmt = 0.2f;

// camera
Camera* camera;

// light
glm::vec3 dirLightDirection = glm::vec3(-0.3f, -1.0f, -0.2f);
const int NR_POINT_LIGHTS = 4;
glm::vec3 pointLightPositions[] = {
  glm::vec3(0.7f,  0.2f,  2.0f),
  glm::vec3(2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -12.0f),
  glm::vec3(0.0f,  0.0f, -3.0f)
};

// timing
float deltaTime = 0.0f; // time between frames
float lastFrame = 0.0f; // time of last frame

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
    glfwCreateWindow(gwidth, gheight, "OpenGL Tutorial", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initalise GLAD" << std::endl;
    return -1;
  }

  // glViewport(0, 0, 800, 600);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);

  Shader shaderProgram("glsl/shader.vs", "glsl/shader.fs");
  Shader lightShader("glsl/light.vs", "glsl/light.fs");

  FlyCamera cam(glm::vec3(1.0f, 1.0f, 3.0f));
  camera = &cam;

  // set up textures
  stbi_set_flip_vertically_on_load(true);
  unsigned int texture1, texture2, texture3;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nChannels;
  unsigned char* data = stbi_load("assets/container2.png", &width, &height, &nChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("assets/container2_specular.png", &width, &height, &nChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  data = stbi_load("assets/hidden.jpg", &width, &height, &nChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shaderProgram.use();
  // shaderProgram.setInt("texture1", 0);
  // glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture2"), 1);
  // glBindTexture(GL_TEXTURE_2D, 0);

  // set up shapes
  unsigned int EBO, VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //   GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // light data
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // already has the vertex data from before when we loaded it
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);


    glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // shaderProgram.setFloat("mixAmt", mixAmt);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float(gwidth)) / (float(gheight)), 0.1f, 100.0f);


    glm::vec3 lightColor = glm::vec3(1.0f);
    // lightPos = glm::vec3(2.0f * cos(glfwGetTime() * glm::radians(20.0f)), 1.0f, 2.0f * sin(glfwGetTime() * glm::radians(20.0f)));

    shaderProgram.use();

    // set light variables
    // directional light
    shaderProgram.setVec3("dirLight.ambient", lightColor * glm::vec3(0.1f));
    shaderProgram.setVec3("dirLight.diffuse", lightColor * glm::vec3(0.5f));
    shaderProgram.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setVec3("dirLight.direction", transformVec3(view, dirLightDirection, false));
    // point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
      shaderProgram.setVec3("pointLights[" + std::to_string(i) + "].ambient", lightColor * glm::vec3(0.1f));
      shaderProgram.setVec3("pointLights[" + std::to_string(i) + "].diffuse", lightColor * glm::vec3(0.5f));
      shaderProgram.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
      shaderProgram.setVec3("pointLights[" + std::to_string(i) + "].position", transformVec3(view, pointLightPositions[i]));
      shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
      shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.14f);
      shaderProgram.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.07f);
    }
    // spot light
    shaderProgram.setVec3("spotLight.ambient", lightColor * glm::vec3(0.0f));
    shaderProgram.setVec3("spotLight.diffuse", lightColor * glm::vec3(1.0f));
    shaderProgram.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setVec3("spotLight.position", glm::vec3(0, 0, 0)/* camera->position */);
    shaderProgram.setVec3("spotLight.direction", glm::vec3(0, 0, -1)/* camera->front */);
    shaderProgram.setFloat("spotLight.cutOff", glm::cos(glm::radians(8.0f)));
    shaderProgram.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));

    for (unsigned int i = 0; i < 10; i++) {

      // set up matrices
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.5f, 1.0f, 0.0f));

      // glm::mat4 model = glm::mat4(1.0f);

      glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
      shaderProgram.setMat4("model", model);
      shaderProgram.setMat3("normalTrans", glm::transpose(glm::inverse(view * model)));

      shaderProgram.setInt("material.diffuse", 0);
      shaderProgram.setInt("material.specular", 1);
      shaderProgram.setInt("material.emission", 2);
      shaderProgram.setFloat("material.shininess", 32);

      // lightColor.x = sin(glfwGetTime() * 2.4f);
      // lightColor.y = sin(glfwGetTime() * 0.7f);
      // lightColor.z = sin(glfwGetTime() * 1.3f);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lightShader.use();

    for (int i = 0;i < NR_POINT_LIGHTS;i++) {
      lightShader.setMat4("view", view);
      lightShader.setMat4("projection", projection);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f));
      lightShader.setMat4("model", model);

      lightShader.setVec3("lightColor", lightColor);
      glBindVertexArray(lightVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

inline glm::vec3 transformVec3(glm::mat4 matrix, glm::vec3 vector, bool translate) {
  return glm::vec3(matrix * glm::vec4(vector.x, vector.y, vector.z, (float)translate));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  gwidth = width;
  gheight = height;
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // // mixing adjustments
  // const float mixStep = 1.0f * deltaTime;
  // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  //   mixAmt = fmin(1.0f, mixAmt + mixStep);
  // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  //   mixAmt = fmax(0.0f, mixAmt - mixStep);

  // camera adjustments
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->processKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->processKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->processKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->processKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  static float lastX = 400, lastY = 300;
  static bool firstUpdate = true;
  float xoffset = xpos - lastX;
  float yoffset = ypos - lastY;
  lastX = xpos;
  lastY = ypos;
  if (firstUpdate) {
    firstUpdate = false;
    return;
  }

  camera->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera->processMouseScroll(yoffset);
}
