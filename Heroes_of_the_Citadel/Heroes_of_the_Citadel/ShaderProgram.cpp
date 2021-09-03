
#include "ShaderProgram.h"


#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void ShaderProgram::loadVertexShaderFromPath (std::string vertexCodePath) {
   std::ifstream ifs (vertexCodePath);
   std::string vertexCode ((std::istreambuf_iterator<char> (ifs)),
      (std::istreambuf_iterator<char> ()));
   if (vertexCode == "") {
      throw std::runtime_error ("ERROR: Could not find file at: " + vertexCodePath);
   }
   loadVertexShader (vertexCode);
}

void ShaderProgram::loadFragmentShaderFromPath (std::string fragmentCodePath) {
   std::ifstream ifs (fragmentCodePath);
   std::string fragmentCode ((std::istreambuf_iterator<char> (ifs)),
      (std::istreambuf_iterator<char> ()));
   if (fragmentCode == "") {
      throw std::runtime_error ("ERROR: Could not find file at: " + fragmentCode);
   }
   loadFragmentShader (fragmentCode);
}

void ShaderProgram::loadVertexShader (std::string vertexCode) {
   // vertex shader
   m_vertexShader = glCreateShader (GL_VERTEX_SHADER);
   const char* vertCode = vertexCode.c_str ();
   glShaderSource (m_vertexShader, 1, &vertCode, NULL);
   glCompileShader (m_vertexShader);
   // Check for shader compile errors.
   int success;
   char infoLog[512];
   glGetShaderiv (m_vertexShader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog (m_vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      throw std::runtime_error ("ERROR: SHADER::VERTEX::COMPILATION_FAILED.");
   }
   m_vertexShaderIsLoaded = true;
}

void ShaderProgram::loadFragmentShader (std::string fragmentCode) {
   // fragment shader
   m_fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
   const char* fragCode = fragmentCode.c_str ();
   glShaderSource (m_fragmentShader, 1, &fragCode, NULL);
   glCompileShader (m_fragmentShader);
   // Check for shader compile errors.
   int success;
   char infoLog[512];
   glGetShaderiv (m_fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog (m_fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      throw std::runtime_error ("ERROR: SHADER::FRAGMENT::COMPILATION_FAILED.");
   }
   m_fragmentShaderIsLoaded = true;
}

void ShaderProgram::linkShaders () {
   if (!m_vertexShaderIsLoaded) {
      throw std::runtime_error ("ERROR: Cannot link shader program before loading vertex shader.");
   }
   if (!m_fragmentShaderIsLoaded) {
      throw std::runtime_error ("ERROR: Cannot link shader program before loading fragment shader.");
   }
   // link shaders
   m_shaderProgram = glCreateProgram ();
   glAttachShader (m_shaderProgram, m_vertexShader);
   glAttachShader (m_shaderProgram, m_fragmentShader);
   glLinkProgram (m_shaderProgram);
   // check for linking errors
   int success;
   char infoLog[512];
   glGetProgramiv (m_shaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog (m_shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      throw std::runtime_error ("ERROR: SHADER::PROGRAM::LINKING_FAILED.");
   }
   glDeleteShader (m_vertexShader);
   glDeleteShader (m_fragmentShader);
   m_programIsLinked = true;
}

unsigned int ShaderProgram::getID () {
   if (m_programIsLinked) {
      return m_shaderProgram;
   }
   throw std::runtime_error ("ERROR: Shader program is not linked.");
}

void ShaderProgram::use () {
   if (!m_programIsLinked) {
      throw std::runtime_error ("ERROR: Cannot use a program before it is linked.");
   }
   glUseProgram (m_shaderProgram);
}

ShaderProgram::~ShaderProgram () {
   if (m_programIsLinked) {
      glDeleteProgram (m_shaderProgram);
   } else {
      if (m_vertexShader != 0) {
         // 0 means it is not created yet.
         glDeleteShader (m_vertexShader);
      }
      if (m_fragmentShader != 0) {
         // 0 means it is not created yet.
         glDeleteShader (m_fragmentShader);
      }
   }
}
