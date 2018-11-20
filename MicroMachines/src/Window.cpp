#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Common.h"

int Window::m_Width = 500;
int Window::m_Height = 500;
bool Window::m_WasResized = false;
InputHandler* Window::m_InputHandler;

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

void Window::SetInputHandler(InputHandler* inputHandler) {
	m_InputHandler = inputHandler;
}

bool Window::WasResized() {
	return m_WasResized;
}

float Window::GetAspectRatio() {
	return (float)m_Width / (float)m_Height;
}

GLFWwindow* Window::GetWindow() {
	return m_Window;
}

int Window::GetWidth() {
	return m_Width;
}

int Window::GetHeight() {
	return m_Height;
}

void Window::OnResize(GLFWwindow* window, int width, int height) {
	m_Width = width;
	m_Height = height;
	m_WasResized = true;
	GLCall(glViewport(0, 0, width, height));

}

void Window::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		m_InputHandler->OnKeyDown(key, false);
	if (action == GLFW_RELEASE)
		m_InputHandler->OnKeyUp(key, false);
	if (action == GLFW_REPEAT)
		m_InputHandler->OnKeyDown(key, true);
}

void Window::OnMouseMove(GLFWwindow* window, double xpos, double ypos) {
	m_InputHandler->OnMouseMove((float)xpos, (float)ypos);
}

void Window::OnMouseClick(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
		m_InputHandler->pressed = true;
	//m_InputHandler->OnKeyDown(button, false);
	if (action == GLFW_RELEASE)
		m_InputHandler->pressed = false;
	//m_InputHandler->OnKeyUp(button, false);
}

void Window::SetCallbacks() {
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, OnKey);
	glfwSetCursorPosCallback(m_Window, OnMouseMove);
	glfwSetMouseButtonCallback(m_Window, OnMouseClick);
	glfwSetWindowSizeCallback(m_Window, OnResize);
}
