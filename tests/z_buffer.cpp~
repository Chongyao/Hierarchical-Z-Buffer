#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <cmath>
using namespace std;

//vertexshader
const GLchar* vertexSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

//fragmentshader
const GLchar* fragmentSource = R"glsl(
#version 150 core
out vec4 outColor;
uniform vec3 triangleColor;

void main()
{
    outColor = vec4(triangleColor, 1.0);
}
)glsl";


int main(){

  auto t_start = std::chrono::high_resolution_clock::now();
  glfwInit();
  // Set paras
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  //Create winodw
  GLFWwindow* window = glfwCreateWindow(800, 600, "my_test_opengl", nullptr, nullptr); // Windowed      //create opengl context
  glfwMakeContextCurrent(window);

  //This should be called after window and context
  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);




  
  GLfloat vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
  };

  GLuint vbo;
  glGenBuffers(1, &vbo); // Generate 1 buffer

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE)
    cout << "vertex shader successfullly" << endl;

  //compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE)
    cout << "fragmentshader successfullly" << endl;

  //build program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);


  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");















  
  while(!glfwWindowShouldClose(window)){

    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);

        // Set the color of the triangle
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        
    glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
    
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);

  }

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  
  glfwTerminate();
}
