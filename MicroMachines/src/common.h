#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "GL\glew.h"

#define ASSERT(x) if (!x) __debugbreak()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#define LOG(x) std::cout << x << std::endl

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL ERROR] (" << error << "): " << function << " " << file
              << ": " << line << std::endl;
    return false;
  }
  return true;
}

enum class Direction {
	Front = 0, Right, Up
};

#endif 
