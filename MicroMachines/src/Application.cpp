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
#include "InputHandler.h"
#include "InputBind.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "Model.h"
#include "SceneGraph.h"

int WinX = 1080, WinY = 720;

void OnUpdate(float deltaTime) {

}

void OnRender() {

}


int main(int argc, char* argv[]) {
	Window window(1080, 720, "MicroMachines");
	
	Renderer renderer;
	renderer.SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	
	InputHandler inputHandler;
	InputBind horizontal;
	InputBind frontal;
	InputBind vertical;

	inputHandler.AddKeyControl(GLFW_KEY_A, horizontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_D, horizontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_W, frontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_S, frontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_E, vertical, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_Q, vertical, -1.0f);

	window.SetInputHandler(&inputHandler);
	window.SetCallbacks();

	Camera mainCamera(Projection::Perspective, window.GetAspectRatio(), 
		glm::vec3(0.0f, 10.0f, 10.f), glm::vec3(0.0f, 10.0f, 0.0f));

	SceneGraph* sceneGraph = new SceneGraph();

	//Model model("res/models/Volkswagen/Volkswagen.obj");
	Model model("nanosuit", "res/models/nanosuit/nanosuit.obj");
	Shader shader("res/shaders/modelLoader");
	model.SetShader(shader);
	sceneGraph->AddNode(&model);
	
	glm::mat4 modelMatrix = glm::mat4(1);
	shader.Bind();
	shader.SetUniform3fv("viewPosition", mainCamera.GetPosition());
	shader.SetUniform3fv("lightPos", glm::vec3(30.0f));
	shader.SetUniformMat4f("model", modelMatrix);
	shader.SetUniformMat4f("view", mainCamera.GetViewMatrix());
	shader.SetUniformMat4f("projection", mainCamera.GetProjMatrix());


	while (!window.ShouldClose()) {
		renderer.Clear();

		//Update
		if (window.WasResized())
			mainCamera.SetAspectRatio(window.GetAspectRatio());
		mainCamera.Translate(Direction::Up, vertical.GetAmt());
		mainCamera.Translate(Direction::Right, horizontal.GetAmt());
		mainCamera.Translate(Direction::Front, frontal.GetAmt());

		mainCamera.ProcessMouseMovement(inputHandler.GetMouseDeltaX(), -inputHandler.GetMouseDeltaY());
		sceneGraph->OnUpdate();

		//OnRender
		//shader.SetUniform3fv("viewPosition", mainCamera.GetPosition());
		//shader.SetUniform3fv("lightPos", glm::vec3(10.0f));
		//shader.SetUniformMat4f("model", modelMatrix);
		shader.SetUniformMat4f("view", mainCamera.GetViewMatrix());
		//shader.SetUniformMat4f("projection", mainCamera.GetProjMatrix());
		sceneGraph->OnRender();
		
		
		window.SwapBuffers();
		window.PollEvents();
		Timer::Tick();
	}
	

}

///////////////////////////////////////////////////////////////////////