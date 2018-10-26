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

#include "Camera.h"
#include "Common.h"
#include "DirectionalLight.h"
#include "FollowCamera.h"
#include "InputBind.h"
#include "InputHandler.h"
#include "Model.h"
#include "PointLight.h"
#include "Renderer.h"
#include "SceneGraph.h"
#include "Shader.h"
#include "SpotLight.h"
#include "TextRenderer.h"
#include "Timer.h"
#include "Window.h"
#include "gameobjects/Butter.h"
#include "gameobjects/Car.h"
#include "gameobjects/Cheerio.h"
#include "gameobjects/Orange.h"
#include "glm/ext/scalar_constants.hpp"

extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

bool debug_mode = false;
bool gameover = false;
float points = 0;

int main(int argc, char* argv[]) {
	Window window(1080, 720, "MicroMachines");
	srand(static_cast<unsigned int>(time(nullptr)));

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	Renderer renderer;
	auto Text = new TextRenderer(window.GetWidth(), window.GetHeight());
	Text->Load("res/fonts/Roboto-Medium.ttf", 24);
	SceneGraph sceneGraph;
	std::vector<SceneNode*> colliders;
	InputHandler inputHandler;
	InputBind horizontal, frontal, vertical;
	InputBind up, right;
	InputBind key0, key1, key2, key3, keyP, keyC, keyN, keyH, keyR;

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
	inputHandler.AddKeyControl(GLFW_KEY_0, key0);
	inputHandler.AddKeyControl(GLFW_KEY_1, key1);
	inputHandler.AddKeyControl(GLFW_KEY_2, key2);
	inputHandler.AddKeyControl(GLFW_KEY_3, key3);
	inputHandler.AddKeyControl(GLFW_KEY_C, keyC);
	inputHandler.AddKeyControl(GLFW_KEY_H, keyH);
	inputHandler.AddKeyControl(GLFW_KEY_N, keyN);
	inputHandler.AddKeyControl(GLFW_KEY_P, keyP);
	inputHandler.AddKeyControl(GLFW_KEY_R, keyR);

	window.SetInputHandler(&inputHandler);
	window.SetCallbacks();

	FollowCamera followCamera(glm::vec3(0.0f, 30.0f, -60.f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	followCamera.SetAspectRatio(window.GetAspectRatio());
	Camera orthoCamera(glm::vec3(0.0f, 130.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	orthoCamera.SetAspectRatio(window.GetAspectRatio());
	orthoCamera.SetProjectionType(Projection::Orthographic);
	Camera topViewCamera(glm::vec3(0.0f, 800.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	topViewCamera.SetAspectRatio(window.GetAspectRatio());
	Camera HUDCamera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	HUDCamera.SetAspectRatio(window.GetAspectRatio());
	HUDCamera.SetProjectionType(Projection::Orthographic);

	sceneGraph.SetCamera(followCamera);
	Shader basicShader("res/shaders/multipleLights");
	Shader tableShader("res/shaders/multiTexture");
	Shader hudShader("res/shaders/hud");

	SceneNode table;
	Model tableModel("res/models/mTable/table.obj");
	table.SetModel(tableModel);
	table.SetShader(tableShader);
	table.transform.scale = glm::vec3(5);

	Car car;
	Model carModel("res/models/car/car.obj");
	car.SetShader(basicShader);
	car.SetModel(carModel);
	car.AddChildNode(&followCamera);
	car.transform.position = glm::vec3(200, 0, 0);
	car.SetWheelsShader(basicShader);
	Model wheelModel("res/models/wheel/wheel.obj");
	car.SetWheelsModel(wheelModel);

	auto butter = new Butter();
	Model butterModel("res/models/butter/butter.obj");
	butter->SetModel(butterModel);
	butter->SetShader(basicShader);
	butter->transform.position = glm::vec3(200, 0, 50);
	butter->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0));
	colliders.push_back(butter);

	auto butter2 = new Butter();
	butter2->SetModel(butterModel);
	butter2->SetShader(basicShader);
	butter2->transform.position = glm::vec3(-200, 0, 50);
	butter2->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0));
	colliders.push_back(butter2);

	auto butter3 = new Butter();
	butter3->SetModel(butterModel);
	butter3->SetShader(basicShader);
	butter3->transform.position = glm::vec3(0, 0, -200);
	butter3->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>() / 2.f, glm::vec3(0, 1, 0));
	colliders.push_back(butter3);


	sceneGraph.AddNode(butter);
	sceneGraph.AddNode(butter2);
	sceneGraph.AddNode(butter3);
	sceneGraph.AddNode(&table);
	sceneGraph.AddNode(&car);
	sceneGraph.AddNode(&orthoCamera);
	sceneGraph.AddNode(&topViewCamera);


	Model orangeModel("res/models/goodorange/orange.obj");
	for (int i = 0; i < 10; i++) {
		auto orange = new Orange();
		orange->SetShader(basicShader);
		orange->SetModel(orangeModel);
		sceneGraph.AddNode(orange);
		colliders.push_back(orange);
	}


	Model hpHUD("res/models/hud/heart.obj");
	Model gameoverHUD("res/models/hud/gameover.obj");


	Model cheerioModel("res/models/goodcheerio/cheerio.obj");
	int cheerioCount = 30;
	float increment = 360.f / cheerioCount;
	for (int i = 0; i < cheerioCount; i++) {
		float angle = increment * i;
		float x = 150 * cos(glm::radians(angle));
		float z = 150 * sin(glm::radians(angle));

		auto cheerio = new Cheerio();
		cheerio->SetShader(basicShader);
		cheerio->SetModel(cheerioModel);
		sceneGraph.AddNode(cheerio);
		cheerio->transform.position = glm::vec3(x, -3, z);
		colliders.push_back(cheerio);
	}

	cheerioCount = 60;
	increment = 360.f / cheerioCount;
	for (int i = 0; i < cheerioCount; i++) {
		float angle = increment * i;
		float x = 300 * cos(glm::radians(angle));
		float z = 300 * sin(glm::radians(angle));

		auto cheerio = new Cheerio();
		cheerio->SetShader(basicShader);
		cheerio->SetModel(cheerioModel);
		sceneGraph.AddNode(cheerio);
		cheerio->transform.position = glm::vec3(x, -3, z);
		colliders.push_back(cheerio);
	}

	std::vector<Light*> lights;
	increment = 360.f / 6.f;
	for (int i = 0; i < 6; i++) {
		float angle = increment * i;
		float x = 225 * cos(glm::radians(angle));
		float z = 225 * sin(glm::radians(angle));

		auto pointLight = new PointLight();
		pointLight->transform.position = glm::vec3(x, 30, z);
		lights.push_back(pointLight);
	}

	DirectionalLight sunLight(glm::vec3(-1, -1, 0));
	lights.push_back(&sunLight);
	SpotLight spotLightL, spotLightR;
	spotLightL.transform.position = glm::vec3(-2.f, 0, 10);
	spotLightR.transform.position = glm::vec3(2.f, 0, 10);
	car.AddChildNode(&spotLightL);
	car.AddChildNode(&spotLightR);
	lights.push_back(&spotLightL);
	lights.push_back(&spotLightR);

	for (auto& light : lights) {
		light->SetupShader(basicShader);
		light->SetupShader(tableShader);
	}

	Timer::Start();
	auto currentCamera = sceneGraph.GetCamera();
	glm::vec2 pos(0, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// GameLoop
	//////////////////////////////////////////////////////////////////////////////////////////////////
	while (!window.ShouldClose()) {
		Timer::Tick();
		points += Timer::deltaTime;
		renderer.Clear();
		window.PollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		//Input Control
		if (window.WasResized()) {
			followCamera.SetAspectRatio(window.GetAspectRatio());
			orthoCamera.SetAspectRatio(window.GetAspectRatio());
			topViewCamera.SetAspectRatio(window.GetAspectRatio());
			HUDCamera.SetAspectRatio(window.GetAspectRatio());
			Text->Resize(window.GetWidth(), window.GetHeight());
		}
		if (key1.isPressed()) {
			sceneGraph.SetCamera(followCamera);
			currentCamera = sceneGraph.GetCamera();
		}
		if (key2.isPressed()) {
			sceneGraph.SetCamera(orthoCamera);
			currentCamera = sceneGraph.GetCamera();
		}
		if (key3.isPressed()) {
			sceneGraph.SetCamera(topViewCamera);
			currentCamera = sceneGraph.GetCamera();
		}
		if (key0.isPressed()) {
			debug_mode = !debug_mode;
		}
		if (keyP.isPressed() & !gameover) {
			Timer::TooglePause();
		}
		if (keyC.isPressed()) {
			for (int i = 0; i < 6; i++)
				lights[i]->ToogleLight();
		}
		if (keyN.isPressed()) {
			sunLight.ToogleLight();
		}
		if (keyH.isPressed()) {
			spotLightL.ToogleLight();
			spotLightR.ToogleLight();
		}
		if (keyR.isPressed() && gameover) {
			gameover = false;
			car.Restart();
			Timer::Unpause();
			points = 0;
		}

		car.Move(up.GetAmt());
		car.Turn(right.GetAmt());

		currentCamera->Translate(Direction::Up, vertical.GetAmt());
		currentCamera->Translate(Direction::Right, horizontal.GetAmt());
		currentCamera->Translate(Direction::Front, frontal.GetAmt());
		currentCamera->ProcessMouseMovement(inputHandler.GetMouseDeltaX(), -inputHandler.GetMouseDeltaY());

		//Update Scene
		for (auto& light : lights) {
			light->UpdateShader(basicShader);
			light->UpdateShader(tableShader);
		}
		sceneGraph.OnUpdate();

		for (auto node : colliders) {
			CollisionData cdata = car.CheckCollision(*node);
			if (cdata.isColliding) {
				node->OnCollision(car);
			}
		}


		//Render Scene
		sceneGraph.OnRender();

		//update HUD
		hudShader.Bind();
		glm::vec3 p(-200 * window.GetAspectRatio(), 0, -180);
		for (int i = 0; i < car.GetHP(); i++) {
			p.x += 50;
			auto mvp = HUDCamera.GetProjMatrix() * HUDCamera.GetViewMatrix() * glm::translate(glm::mat4(1), p) * glm::scale(glm::mat4(1), glm::vec3(20));
			hudShader.SetUniformMat4f("MVPMat", mvp);
			hpHUD.Draw(hudShader);
		}

		if (car.GetHP() == 0) {
			auto mvp = HUDCamera.GetProjMatrix() * HUDCamera.GetViewMatrix() * glm::scale(glm::mat4(1), glm::vec3(210));
			hudShader.SetUniformMat4f("MVPMat", mvp);
			gameoverHUD.Draw(hudShader);
			Timer::Pause();
			gameover = true;

		}
		Text->RenderText("Points: " + std::to_string(static_cast<int>(points)), window.GetWidth() * 0.8f + pos.x, 20 + pos.y, 1.0f);

		if (debug_mode) {
			ImGui::Begin("Debug Window", &debug_mode);
			ImGui::DragFloat2("Position", &pos[0], 1, -1000, 10000);
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