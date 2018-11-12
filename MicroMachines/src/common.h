#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "GL\glew.h"
#include <glm/glm.hpp>

#define ASSERT(expr) \
	if (expr) {} \
	else { \
		ReportAssertionFailure(#expr, __FILE__, __LINE__); \
        __debugbreak(); \
	}

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

static void ReportAssertionFailure(const char* expr, const char* file, int line) {
	std::cout << "[Assertion Failure] " << expr << " " << file
		<< ": " << line << std::endl;
}

enum class Direction {
	Front = 0, Right, Up
};

#endif 
