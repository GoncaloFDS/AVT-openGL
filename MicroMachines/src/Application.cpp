#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
#include "gameobjects/Car.h"
#include "gameobjects/Orange.h"

bool debug_mode = false;

int main(int argc, char* argv[]) {
	Window window(1080, 720, "MicroMachines");
	srand(time(NULL));

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	Renderer renderer;
	SceneGraph sceneGraph;
	InputHandler inputHandler;
	InputBind horizontal;
	InputBind frontal;
	InputBind vertical;
	InputBind key0, key1, key2, key3;
	InputBind up, right;

	inputHandler.AddKeyControl(GLFW_KEY_A, horizontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_D, horizontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_W, frontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_S, frontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_E, vertical, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_Q, vertical, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_0, key0, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_1, key1, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_2, key2, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_3, key3, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_RIGHT, right, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_LEFT, right, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_UP, up, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_DOWN, up, -1.0f);
	
	window.SetInputHandler(&inputHandler);
	window.SetCallbacks();

	Camera followCamera(glm::vec3(0.0f, 30.0f, -60.f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	followCamera.SetAspectRatio(window.GetAspectRatio());
	Camera orthoCamera(glm::vec3(0.0f, 130.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	orthoCamera.SetAspectRatio(window.GetAspectRatio());
	orthoCamera.SetProjectionType(Projection::Orthographic);
	Camera topViewCamera(glm::vec3(0.0f, 300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	topViewCamera.SetAspectRatio(window.GetAspectRatio());
	
	sceneGraph.SetCamera(followCamera);
	Shader shader("res/shaders/modelLoader");
	
	SceneNode table;
	Model tableModel("res/models/table/diningtable.obj");
	table.SetModel(tableModel);
	table.SetShader(shader);

	Car car;
	Model carModel("res/models/car/car.obj");
	car.GetSceneNode().SetShader(shader);
	car.GetSceneNode().SetModel(carModel);

	Orange orange;
	Orange orange2;
	Model orangeModel("res/models/orange/Orange.obj");
	orange.GetSceneNode().SetShader(shader);
	orange2.GetSceneNode().SetShader(shader);
	orange.GetSceneNode().SetModel(orangeModel);
	orange2.GetSceneNode().SetModel(orangeModel);
	table.transform.scale = glm::vec3(5);

	car.GetSceneNode().AddChildNode(&followCamera);

	sceneGraph.AddNode(&table);
	sceneGraph.AddNode(&car.GetSceneNode());
	sceneGraph.AddNode(&orange.GetSceneNode());
	sceneGraph.AddNode(&orange2.GetSceneNode());
	


	//std::vector<Orange> oranges(2);
	//for (auto& orange : oranges) {
	//	orange.GetSceneNode().SetShader(shader);
	//	orange.GetSceneNode().SetModel(orangeModel);
	//	sceneGraph.AddNode(&orange.GetSceneNode());
	//}
	
	
	auto currentCamera = sceneGraph.GetCamera();

	shader.Bind();
	shader.SetUniform3fv("u_cameraPosition", currentCamera->GetPosition());
	shader.SetUniform3fv("u_lightPosition", glm::vec3(1000.0f));
	shader.SetUniformMat4f("u_viewMat", currentCamera->GetViewMatrix());
	shader.SetUniformMat4f("u_projMat", currentCamera->GetProjMatrix());

	Timer::Start();
	while (!window.ShouldClose()) {
		Timer::Tick();
		renderer.Clear();
		window.PollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		auto currentCamera = sceneGraph.GetCamera();
		//Update
		if (window.WasResized()) {
			followCamera.SetAspectRatio(window.GetAspectRatio());
			orthoCamera.SetAspectRatio(window.GetAspectRatio());
			topViewCamera.SetAspectRatio(window.GetAspectRatio());
		}

		if (key1.GetAmt() != 0)
			sceneGraph.SetCamera(followCamera);
		else if(key2.GetAmt() != 0)
			sceneGraph.SetCamera(orthoCamera);
		else if (key3.GetAmt() != 0)
			sceneGraph.SetCamera(topViewCamera);
		if (key0.GetAmt() != 0)
			debug_mode = !debug_mode;


		currentCamera->Translate(Direction::Up, vertical.GetAmt());
		currentCamera->Translate(Direction::Right, horizontal.GetAmt());
		currentCamera->Translate(Direction::Front, frontal.GetAmt());

		car.Move(up.GetAmt());
		car.Turn(right.GetAmt());
		
		orange.OnUpdate();
		orange2.OnUpdate();
		//for(auto& orange : oranges)
		//	orange.OnUpdate();

		currentCamera->ProcessMouseMovement(inputHandler.GetMouseDeltaX(), -inputHandler.GetMouseDeltaY());
		sceneGraph.OnUpdate();

		//OnRender
		shader.SetUniform3fv("u_cameraPosition", currentCamera->GetPosition());
		shader.SetUniformMat4f("u_viewMat", currentCamera->GetViewMatrix());
		shader.SetUniformMat4f("u_projMat", currentCamera->GetProjMatrix());
	
		sceneGraph.OnRender();

		if(debug_mode) {	
			ImGui::Begin("Debug Window", &debug_mode);
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		window.SwapBuffers();
	}
	
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

///////////////////////////////////////////////////////////////////////