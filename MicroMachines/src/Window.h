#pragma once
#include <string>
#include "InputHandler.h"


struct GLFWwindow;

class Window {
private:
	GLFWwindow* m_Window;
	static int m_Width;
	static int m_Height;
	static bool m_WasResized;
	static InputHandler* m_InputHandler;

public:
	Window(int width, int height, const char* name);
	~Window();

	void SetCallbacks();
	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();
	void Close();
	
	void SetInputHandler(InputHandler* inputHandler);

	bool WasResized();
	float GetAspectRatio();
	GLFWwindow* GetWindow();

private:
	static void OnResize(GLFWwindow* window, int width, int height);
	static void OnKey(GLFWwindow* window, int key, int scancode, int actions, int mods);
	static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
	static void OnMouseClick(GLFWwindow* window, int button, int action, int mods);
};

