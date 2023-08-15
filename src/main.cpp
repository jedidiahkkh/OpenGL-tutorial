#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// float vertices[] = {
//   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//   0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
// };
float vertices[] = { 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

// float vertices2[] = { 0.0f, 1.0f, 0.0f,
//                       1.0f, 1.0f, 0.0f,
//                       1.0f, 0.0f, 0.0f };

float mixAmt = 0.2f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
    glfwCreateWindow(800, 600, "OpenGL Tutorial", NULL, NULL);
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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shaderProgram("glsl/shader.vs", "glsl/shader.fs");

  // set up textures
  stbi_set_flip_vertically_on_load(true);
  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nChannels;
  unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

  data = stbi_load("assets/awesomeface.png", &width, &height, &nChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shaderProgram.use();
  shaderProgram.setInt("texture1", 0);
  glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture2"), 1);
  glBindTexture(GL_TEXTURE_2D, 0);

  // set up shapes
  unsigned int EBO, VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // unsigned int VBO2, VAO2;
  // glGenVertexArrays(1, &VAO2);
  // glBindVertexArray(VAO2);

  // glGenBuffers(1, &VBO2);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  // glEnableVertexAttribArray(0);
  // glBindVertexArray(0);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    shaderProgram.use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // float timeValue = glfwGetTime();
    // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shaderProgram.setFloat("mixAmt", mixAmt);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    mixAmt = fmin(1.0f, mixAmt + 0.001f);
  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    mixAmt = fmax(0.0f, mixAmt - 0.001f);
}