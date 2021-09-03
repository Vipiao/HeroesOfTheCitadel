#pragma once

#include <string>

class ShaderProgram {
private:
   unsigned int m_vertexShader{ 0 };
   unsigned int m_fragmentShader{ 0 };
   unsigned int m_shaderProgram{ 0 };

   bool m_vertexShaderIsLoaded = false;
   bool m_fragmentShaderIsLoaded = false;
   bool m_programIsLinked = false;
public:
   void loadVertexShaderFromPath (std::string vertexCodePath);
   void loadFragmentShaderFromPath (std::string fragmentCodePath);
   void loadVertexShader (std::string vertexCode);
   void loadFragmentShader (std::string fragmentCode);
   void linkShaders ();
   unsigned int getID ();
   void use ();
   ~ShaderProgram ();
};

