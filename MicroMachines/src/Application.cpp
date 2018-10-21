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
#include "glm/ext/scalar_constants.hpp"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "gameobjects/Cheerio.h"

bool debug_mode = false;
bool isPaused = false;

int main(int argc, char* argv[]) {
	Window window(1080, 720, "MicroMachines");
	srand(time(NULL));

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	Renderer renderer;
	SceneGraph sceneGraph;
	InputHandler inputHandler;
	InputBind horizontal, frontal, vertical;
	InputBind up, right;
	InputBind key0, key1, key2, key3, keyP;

	//Axis
	inputHandler.AddKeyControl(GLFW_KEY_A, horizontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_D, horizontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_W, frontal, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_S, frontal, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_E, vertical, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_Q, vertical, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_RIGHT, right, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_LEFT, right, -1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_UP, up, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_DOWN, up, -1.0f);
	//Keys
	inputHandler.AddKeyControl(GLFW_KEY_0, key0, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_1, key1, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_2, key2, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_3, key3, 1.0f);
	inputHandler.AddKeyControl(GLFW_KEY_P, keyP, 1.0f);

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
	Shader shader("res/shaders/multipleLights");
	
	SceneNode table;
	Model tableModel("res/models/table/diningtable.obj");
	table.SetModel(tableModel);
	table.SetShader(shader);
	table.transform.scale = glm::vec3(5);

	Car car;
	Model carModel("res/models/car/car.obj");
	car.SetShader(shader);
	car.SetModel(carModel);
	car.AddChildNode(&followCamera);
	car.transform.position = glm::vec3(200, 0, 0);
	car.SetWheelsShader(shader);
	Model wheelModel("res/models/wheel/wheel.obj");
	car.SetWheelsModel(wheelModel);
	
	Cheerio cheerio;
	Model cheerioModel("res/models/cheerio/cheerio.obj");
	cheerio.SetModel(cheerioModel);
	cheerio.SetShader(shader);

	SceneNode butter;
	Model butterModel("res/models/butter/butter.obj");
	butter.SetModel(butterModel);
	butter.SetShader(shader);

	sceneGraph.AddNode(&cheerio);
	sceneGraph.AddNode(&butter);
	sceneGraph.AddNode(&table);
	sceneGraph.AddNode(&car);
	
	Model orangeModel("res/models/goodorange/orange.obj");
	std::vector<Orange*> oranges;
	for (int i = 0; i < 5; i++) {
		oranges.push_back(new Orange());
		oranges[i]->SetShader(shader);
		oranges[i]->SetModel(orangeModel);
		sceneGraph.AddNode(oranges[i]);
	}

	std::vector<Cheerio*> innerCheerios;
	int cheerioCount = 30;
	float increment = 360 / cheerioCount;
	for (int i = 0; i < cheerioCount; i++) {
		float angle = increment * i;
		float x = 150 * cos(glm::radians(angle));
		float z = 150 * sin(glm::radians(angle));

		innerCheerios.push_back(new Cheerio());
		innerCheerios[i]->SetShader(shader);
		innerCheerios[i]->SetModel(cheerioModel);
		sceneGraph.AddNode(innerCheerios[i]);
		innerCheerios[i]->transform.position = glm::vec3(x, -3, z);
	}

	std::vector<Cheerio*> outterCheerios;
	cheerioCount = 50;
	increment = 360 / cheerioCount;
	for (int i = 0; i < cheerioCount; i++) {
		float angle = increment * i;
		float x = 300 * cos(glm::radians(angle));
		float z = 300 * sin(glm::radians(angle));

		outterCheerios.push_back(new Cheerio());
		outterCheerios[i]->SetShader(shader);
		outterCheerios[i]->SetModel(cheerioModel);
		sceneGraph.AddNode(outterCheerios[i]);
		outterCheerios[i]->transform.position = glm::vec3(x, -3, z);
	}

	
	
	auto currentCamera = sceneGraph.GetCamera();

	PointLight pointLight;
	pointLight.transform.position = glm::vec3(0, 30, 0);
	DirectionalLight sunLight(glm::vec3(-1, -1, 0));
	SpotLight spotLightL, spotLightR;
	spotLightL.transform.position = glm::vec3(-3.f, -1, 10);
	spotLightR.transform.position = glm::vec3(3.f, -1, 10);
	car.AddChildNode(&spotLightL);
	car.AddChildNode(&spotLightR);

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

		if (key1.isPressed())
			sceneGraph.SetCamera(followCamera);
		else if(key2.isPressed())
			sceneGraph.SetCamera(orthoCamera);
		else if (key3.isPressed())
			sceneGraph.SetCamera(topViewCamera);
		if (key0.isPressed())
			debug_mode = !debug_mode;
		if (keyP.isPressed()) 
			Timer::Pause();				
		
		

		

		currentCamera->Translate(Direction::Up, vertical.GetAmt());
		currentCamera->Translate(Direction::Right, horizontal.GetAmt());
		currentCamera->Translate(Direction::Front, frontal.GetAmt());

		pointLight.transform.position += glm::vec3(5, 0, 0) * Timer::deltaTime;
		//LOG("Light position: " << pointLight.transform.position.x << " " << pointLight.transform.position.y << " " << pointLight.transform.position.z);
		pointLight.UpdateShader(shader);
		sunLight.UpdateShader(shader);
		spotLightL.UpdateShader(shader);
		spotLightR.UpdateShader(shader);

		car.Move(up.GetAmt());
		car.Turn(right.GetAmt());
		
		for(auto orange : oranges)
			orange->OnUpdate();

		currentCamera->ProcessMouseMovement(inputHandler.GetMouseDeltaX(), -inputHandler.GetMouseDeltaY());
		sceneGraph.OnUpdate();

		for (auto orange : oranges) {
			CollisionData cdata = car.CheckCollision(*orange);
			if (cdata.isColliding) {
				car.Reset();
			}
				
		}

		for (auto cheerio : innerCheerios) {
			CollisionData cdata = car.CheckCollision(*cheerio);
			if (cdata.isColliding) {
				car.Stop();
				cheerio->OnCollision(car);
			}

		}
		for (auto cheerio : outterCheerios) {
			CollisionData cdata = car.CheckCollision(*cheerio);
			if (cdata.isColliding) {
				car.Stop();
				cheerio->OnCollision(car);
			}
		}

		//OnRender
	
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