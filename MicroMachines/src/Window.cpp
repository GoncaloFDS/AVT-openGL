#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Common.h"

int Window::m_Width = 500;
int Window::m_Height = 500;
bool Window::m_WasResized = false;

Window::Window(int width, int height, const char* name) {

	if (!glfwInit()) ASSERT(false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if (!m_Window) {
		glfwTerminate();
		ASSERT(false);
	}
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); //V-Sync enabled

	m_Width = width;
	m_Height = height;
	m_WasResized = false;
}


Window::~Window() {
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(m_Window);
}

void Window::PollEvents() {
	m_WasResized = false;
	glfwPollEvents();
}

void Window::Close() {
	glfwTerminate();
}

bool Window::WasResized() {
	return m_WasResized;
}

float Window::GetAspectRatio() {
	return (float) m_Width / (float) m_Height;
}

GLFWwindow* Window::GetWindow() {
	return m_Window;
}

void Window::OnResize(GLFWwindow* window, int width, int height) {
	m_Width = width;
	m_Height = height;
	m_WasResized = true;
	GLCall(glViewport(0, 0, width, height));
	
}

void Window::OnKey(GLFWwindow* window, int key, int scancode, int actions, int mods) {
	//Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	//windowPtr->OnKey(key, scancode, actions, mods);
}

void Window::OnKey(int key, int scancode, int actions, int mods) {
	//input.OnKey(key);
}

void Window::SetCallbacks() {
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, OnKey);
	glfwSetWindowSizeCallback(m_Window, OnResize);
}
