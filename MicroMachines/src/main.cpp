#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>  
#include <glm/vec3.hpp>    
#include <glm/vec4.hpp>    

#include "Common.h"
#include "Shader.h"
#include "InputEvent.h"
#include "inputControl.h"
#include "Mesh.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "Model.h"

int WinX = 1080, WinY = 720;

void OnUpdate(float deltaTime) {

}

void OnRender() {

}

int main(int argc, char* argv[]) {
	Window window(1080, 720, "MicroMachines");
	
	Renderer renderer;
	renderer.SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	window.SetCallbacks();

	Camera mainCamera(Projection::Perspective, window.GetAspectRatio(), glm::vec3(0.0f, 20.0f, 20.f), glm::vec3(0.0f, 10.0f, 0.0f));

	Model model("res/models/nanosuit/nanosuit.obj");
	
	//Model model("res/models/texturedcube.obj");
	Shader shader("res/shaders/modelLoader");

	
	glm::mat4 mvp = mainCamera.GetViewProjMatrix() * glm::mat4(1);
	shader.Bind();
	shader.SetUniformMat4f("Matrix", mvp);

	while (!window.ShouldClose()) {
		renderer.Clear();

		if (window.WasResized()) {
			mainCamera.SetAspectRatio(window.GetAspectRatio());
			glm::mat4 mvp = mainCamera.GetViewProjMatrix() * glm::mat4(1);
			shader.Bind();
			shader.SetUniformMat4f("Matrix", mvp);
		}
		
		model.Draw(shader);
		
		
		//LOG(Timer::elapsedTime);
		//OnUpdate(clock.DeltaTime());
		//OnRender();

		window.SwapBuffers();
		window.PollEvents();
		Timer::Tick();
	}
	

}

///////////////////////////////////////////////////////////////////////