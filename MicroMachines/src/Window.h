#pragma once
#include <string>


struct GLFWwindow;

class Window {
private:
	GLFWwindow* m_Window;
	static int m_Width;
	static int m_Height;
	static bool m_WasResized;

public:
	Window(int width, int height, const char* name);
	~Window();
	void OnKey(int key, int scancode, int actions, int mods);
	void SetCallbacks();
	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();
	void Close();
	bool WasResized();
	float GetAspectRatio();

	GLFWwindow* GetWindow();

private:
	static void OnResize(GLFWwindow* window, int width, int height);
	static void OnKey(GLFWwindow* window, int key, int scancode, int actions, int mods);
};

