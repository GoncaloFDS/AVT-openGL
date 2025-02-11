#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "glm/ext/scalar_constants.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>  
#include <glm/vec3.hpp>    
#include <glm/vec4.hpp>

#include "Common.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "input/InputBind.h"
#include "input/InputHandler.h"
#include "Model.h"
#include "Renderer.h"
#include "SceneGraph.h"
#include "Shader.h"
#include "TextRenderer.h"
#include "Timer.h"
#include "Window.h"
#include "cameras/Camera.h"
#include "cameras/FollowCamera.h"
#include "cameras/DebugCamera.h"
#include "gameobjects/Butter.h"
#include "gameobjects/Car.h"
#include "gameobjects/Cheerio.h"
#include "gameobjects/Orange.h"
#include "gameobjects/Billboard.h"
#include "gameobjects/Sun.h"
#include "particles/ParticleEmitter.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "gameobjects/Portal.h"
#include "LensFlareManager.h"

extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

bool debug_mode = false;
bool gameover = false;
bool fogIsEnabled = true;
bool particleEnabled = false;
float points = 0;
bool inverted;

float debugFloat = 1.0f;
glm::vec3 debugVec3a(1);
glm::vec3 debugVec3b(1);


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
	InputBind key0, key1, key2, key3, keyESC, keyP, keyC, keyN, keyH, keyR, keyF, keyL, keyZ;

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
	inputHandler.AddKeyControl(GLFW_KEY_ESCAPE, keyESC);
	inputHandler.AddKeyControl(GLFW_KEY_C, keyC);
	inputHandler.AddKeyControl(GLFW_KEY_H, keyH);
	inputHandler.AddKeyControl(GLFW_KEY_N, keyN);
	inputHandler.AddKeyControl(GLFW_KEY_P, keyP);
	inputHandler.AddKeyControl(GLFW_KEY_R, keyR);
	inputHandler.AddKeyControl(GLFW_KEY_F, keyF);
	inputHandler.AddKeyControl(GLFW_KEY_L, keyL);
	inputHandler.AddKeyControl(GLFW_KEY_Z, keyZ);

	window.SetInputHandler(&inputHandler);
	window.SetCallbacks();

	FollowCamera followCamera(glm::vec3(0.0f, 30.0f, -60.f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	followCamera.SetAspectRatio(window.GetAspectRatio());
	Camera portalCamera(glm::vec3(-190, 14, 0), glm::vec3(-190, 13, -10) , glm::vec3(0.0f, 1.0f, 0.0f));
	portalCamera.SetAspectRatio(window.GetAspectRatio());
	Camera orthoCamera(glm::vec3(0.0f, 130.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	orthoCamera.SetAspectRatio(window.GetAspectRatio());
	orthoCamera.SetProjectionType(Projection::Orthographic);
	Camera topViewCamera(glm::vec3(0.0f, 800.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	topViewCamera.SetAspectRatio(window.GetAspectRatio());
	Camera HUDCamera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	HUDCamera.SetAspectRatio(window.GetAspectRatio());
	HUDCamera.SetProjectionType(Projection::Orthographic);
	DebugCamera debugCamera(glm::vec3(0.0f, 30.0f, -60.f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	sceneGraph.SetCamera(followCamera);
	Shader basicShader("res/shaders/multipleLights");
	//Shader tableShader("res/shaders/multiTexture");
	Shader singleColorShader("res/shaders/singleColor");
	Shader hudShader("res/shaders/hud");
	Shader particleShader("res/shaders/particle");

	SceneNode table;
	Model tableModel("res/models/Table/table.obj");
	table.SetModel(tableModel);
	//table.SetShader(tableShader);
	table.SetShader(basicShader);
	table.transform.scale = glm::vec3(5);

	Car car;
	Model carModel("res/models/Lamborginhi/Body.obj");
	car.SetShader(basicShader);
	car.SetModel(carModel);
	car.AddChildNode(&followCamera);
	car.SetWheelsShader(basicShader);
	Model wheel("res/models/Lamborginhi/Wheel.obj");
	car.SetWheelsModel(wheel);

	auto butter = new Butter();
	Model butterModel("res/models/Butter/butter.obj");
	butter->SetModel(butterModel);
	butter->SetShader(basicShader);
	butter->transform.position = glm::vec3(200, 2, 50);
	butter->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0));
	colliders.push_back(butter);

	auto butter2 = new Butter();
	butter2->SetModel(butterModel);
	butter2->SetShader(basicShader);
	butter2->transform.position = glm::vec3(-200, 2, 50);
	butter2->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0));
	colliders.push_back(butter2);

	auto butter3 = new Butter();
	butter3->SetModel(butterModel);
	butter3->SetShader(basicShader);
	butter3->transform.position = glm::vec3(0, 2, -200);
	butter3->transform.rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>() / 2.f, glm::vec3(0, 1, 0));
	colliders.push_back(butter3);
	
	SceneNode portalFrame;
	Model portalFrameModel("res/models/Portal/frame.obj");
	portalFrame.SetModel(portalFrameModel);
	portalFrame.SetShader(basicShader);
	portalFrame.transform.position = glm::vec3(200, 0, 150);
	portalFrame.transform.scale = glm::vec3(20);
	//portalFrame.transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	SceneNode innerPortal;
	Model innerPortalModel("res/models/Portal/innerPortal.obj");
	innerPortal.SetModel(innerPortalModel);
	innerPortal.SetShader(basicShader);
	innerPortal.SetEnabled(false);
	portalFrame.AddChildNode(&innerPortal);
	Portal portalImage(glm::vec2(window.GetWidth(), window.GetHeight()));
	Model portalModel("res/models/portal.obj");
	Shader portalShader("res/shaders/portal");
	portalImage.SetModel(portalModel);
	portalImage.SetShader(portalShader);
	portalImage.SetEnabled(false);
	portalImage.transform.scale = glm::vec3(1, -1, 1);
	portalFrame.AddChildNode(&portalImage);
	colliders.push_back(&portalImage);

	sceneGraph.AddNode(butter);
	sceneGraph.AddNode(butter2);
	sceneGraph.AddNode(butter3);
	sceneGraph.AddNode(&table);
	sceneGraph.AddNode(&car);
	sceneGraph.AddNode(&portalFrame);

	//sceneGraph.AddNode(&portal);
	sceneGraph.AddNode(&orthoCamera);
	sceneGraph.AddNode(&topViewCamera);
	sceneGraph.AddNode(&debugCamera);

	Model sprite("res/models/Sprite/Sprite.obj");
	ParticleEmitter particleEmitter(&followCamera, sprite, particleShader);
	particleEmitter.transform.position = glm::vec3(0, 20, 0);
	sceneGraph.AddNode(&particleEmitter);

	Model orangeModel("res/models/Orange/orange.obj");
	for (int i = 0; i < 10; i++) {
		auto orange = new Orange();
		orange->SetShader(basicShader);
		orange->SetModel(orangeModel);
		sceneGraph.AddNode(orange);
		colliders.push_back(orange);
	}

	Model hpHUD("res/models/HUD/heart.obj");
	Model gameoverHUD("res/models/HUD/gameover.obj");

	SceneNode mirror;
	Model mirrorModel("res/models/quad.obj");
	mirror.SetModel(mirrorModel);
	mirror.SetShader(basicShader);
	mirror.transform.scale = glm::vec3(50);
	mirror.transform.position = glm::vec3(0, 0.1, 0);
	sceneGraph.AddNode(&mirror);
	Model cheerioModel("res/models/Cheerio/cheerio.obj");
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
		cheerio->transform.position = glm::vec3(x, 1, z);
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
		cheerio->transform.position = glm::vec3(x, 1, z);
		colliders.push_back(cheerio);
	}

	std::vector<Light*> lights;
	std::vector<Billboard*> lamps;
	Model lampModel("res/models/Lamp/lamp.obj");
	increment = 360.f / 6.f;
	for (int i = 0; i < 6; i++) {
		float angle = increment * i;
		float x = 225 * cos(glm::radians(angle));
		float z = 225 * sin(glm::radians(angle));

		auto pointLight = new PointLight();
		pointLight->transform.position = glm::vec3(x, 30, z);
		lights.push_back(pointLight);

		auto* lamp = new Billboard(glm::vec3(x*1.1f, 23, z*1.1f), &followCamera);
		lamp->SetModel(lampModel);
		lamp->SetShader(basicShader);
		colliders.push_back(lamp);
		lamps.push_back(lamp);
		sceneGraph.AddNode(lamp);
	}

	Model sun_model("res/models/lens flare/sun.obj");
	Sun sun(5.0f, glm::vec3(2, 0.4, 2), &followCamera);
	sun.transform.scale = glm::vec3(150.0f);
	sun.SetModel(sun_model);
	sun.SetShader(singleColorShader);
	sceneGraph.AddNode(&sun);

	lights.push_back(sun.GetLight());
	SpotLight spotLightL, spotLightR;
	spotLightL.transform.position = glm::vec3(-35.0f, 30.0f, 85.0f);
	spotLightR.transform.position = glm::vec3(35.0f, 30.0f, 85.0f);
	car.AddChildNode(&spotLightL);
	car.AddChildNode(&spotLightR);
	lights.push_back(&spotLightL);
	lights.push_back(&spotLightR);

	for (auto& light : lights) {
		light->SetupShader(basicShader);
		//light->SetupShader(tableShader);
	}

	LensFlareManager flareManager(0.07f, &hudShader);

	Timer::Start();
	auto currentCamera = sceneGraph.GetCamera();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// GameLoop
	//////////////////////////////////////////////////////////////////////////////////////////////////
	while (!window.ShouldClose()) {
		Timer::Tick();
		renderer.Clear();
		window.PollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		//portalFrame.transform.position = debugVec3a;
		//portalImage.transform.position = debugVec3b;
		portalImage.transform.scale = glm::vec3(window.GetAspectRatio(), 1, 1);
		// 

		points += Timer::deltaTime;
		if (points > 50)
			particleEnabled = true;
		//Input Control
		if (window.WasResized()) {
			followCamera.SetAspectRatio(window.GetAspectRatio());
			orthoCamera.SetAspectRatio(window.GetAspectRatio());
			topViewCamera.SetAspectRatio(window.GetAspectRatio());
			debugCamera.SetAspectRatio(window.GetAspectRatio());
			HUDCamera.SetAspectRatio(window.GetAspectRatio());
			Text->Resize(window.GetWidth(), window.GetHeight());
		}
		if (key1.isPressed()) {
			sceneGraph.SetCamera(followCamera);
			currentCamera = sceneGraph.GetCamera();
			sun.SetTarget(currentCamera);
			for (auto lamp : lamps)
				lamp->SetTarget(currentCamera);
			particleEmitter.SetTarget(currentCamera);

		}
		if (key2.isPressed()) {
			sceneGraph.SetCamera(orthoCamera);
			currentCamera = sceneGraph.GetCamera();
			sun.SetTarget(currentCamera);
			for (auto lamp : lamps)
				lamp->SetTarget(currentCamera);
			particleEmitter.SetTarget(currentCamera);
		}
		if (key3.isPressed()) {
			sceneGraph.SetCamera(topViewCamera);
			currentCamera = sceneGraph.GetCamera();
			sun.SetTarget(currentCamera);
			for (auto lamp : lamps)
				lamp->SetTarget(currentCamera);
			particleEmitter.SetTarget(currentCamera);
		}		
		if (keyESC.isPressed()) {
			debugCamera.DetachFrom(*currentCamera);
			sceneGraph.SetCamera(debugCamera);
			currentCamera = sceneGraph.GetCamera();
			sun.SetTarget(currentCamera);
			for (auto lamp : lamps)
				lamp->SetTarget(currentCamera);
			particleEmitter.SetTarget(currentCamera);

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
		if (keyF.isPressed()) {
			fogIsEnabled = !fogIsEnabled;
			basicShader.Bind();
			basicShader.SetUniform1i("fogIsEnabled", fogIsEnabled);
			basicShader.Unbind();
			portalShader.Bind();
			portalShader.SetUniform1i("fogIsEnabled", fogIsEnabled);
			portalShader.Unbind();
		}
		if (keyN.isPressed()) {
			sun.ToogleLight();
		}
		if (keyH.isPressed()) {
			inverted = !inverted;
			currentCamera->InvertPitch();
			spotLightL.ToogleLight();
			spotLightR.ToogleLight();
		}
		if (keyR.isPressed() && gameover) {
			gameover = false;
			car.Restart();
			Timer::Unpause();
			points = 0;
		}
		if (keyL.isPressed()) {
			flareManager.Toogle();
		}
		if (keyZ.isPressed()) {
			particleEnabled = !particleEnabled;
			particleEmitter.SetEnabled(particleEnabled);
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
			//light->UpdateShader(tableShader);
		}
		sceneGraph.OnUpdate();

		for (auto node : colliders) {
			CollisionData cdata = car.CheckCollision(*node);
			if (cdata.isColliding) {
				node->OnCollision(car);
			}
		}
		//Temp Update TODO clean me
		sun.transform.position = sun.GetWorldPosition(currentCamera->GetPosition());
		
		//Render Scene

		//Mirror Stencil
 		renderer.EnableStencilTest();
 		renderer.ClearStencil();
  		renderer.SetStencilFunc(GL_NEVER, 1, 1);
  		renderer.SetStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
		mirror.OnRender(*currentCamera);
  		glEnable(GL_DEPTH_TEST);
		renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		renderer.SetStencilFunc(GL_EQUAL, 1, 1);
  		renderer.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  		// Render reflections
  		currentCamera->InvertPitch();
  		glCullFace(GL_FRONT);
 		table.SetEnabled(false);
		mirror.SetEnabled(false);
 		sceneGraph.OnRender();
		mirror.SetEnabled(true);
 		table.SetEnabled(true);
  		currentCamera->InvertPitch();
 		glCullFace(GL_BACK);

		//Render Shadows
		renderer.SetBlendFunc(GL_ZERO, GL_SRC_COLOR);
		renderer.SetStencilFunc(GL_EQUAL, 1, 1);
		renderer.SetStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
		renderer.DisableStencilTest();
		currentCamera->SetIsRenderingShadows(true);
		currentCamera->SetShadowMatrix(sun.GetShadowMatrix());
		//render
		table.SetEnabled(false);
		mirror.SetEnabled(false);
		sun.SetEnabled(false);
		glDisable(GL_DEPTH_TEST);
		sceneGraph.OnRender();
		sun.SetEnabled(true);
		mirror.SetEnabled(true);
		table.SetEnabled(true);
		currentCamera->SetIsRenderingShadows(false);
		//
		glEnable(GL_DEPTH_TEST);
		//Scene clipping y > 0
		renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		renderer.DisableStencilTest();
		table.SetEnabled(false);
		mirror.SetEnabled(false);
		sceneGraph.OnRender();
		mirror.SetEnabled(true);
		table.SetEnabled(true);
		//
		//
		////Render Table
		renderer.EnableStencilTest();
		renderer.SetStencilFunc(GL_EQUAL, 0, 1);
		renderer.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		table.OnRender(*currentCamera);
		renderer.ClearStencil();
		renderer.SetStencilFunc(GL_ALWAYS, 0, 1);
		//

 		//// Render Portal
 		renderer.EnableStencilTest();
 		sceneGraph.SetCamera(portalCamera);
 		portalImage.RenderToTexture(sceneGraph, renderer);
 		sceneGraph.SetCamera(*currentCamera);
 		renderer.ClearStencil();
 		renderer.SetStencilFunc(GL_NEVER, 1, 1);
 		renderer.SetStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
 		innerPortal.SetEnabled(true);
 		innerPortal.OnRender(*currentCamera);
 		innerPortal.SetEnabled(false);
 		renderer.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
 		renderer.SetStencilFunc(GL_EQUAL, 1, 1);
 		portalImage.BindTexture();
 		portalImage.SetEnabled(true);
 		portalImage.OnRender(*currentCamera);
 		portalImage.SetEnabled(false);
 		renderer.SetStencilFunc(GL_ALWAYS, 1, 1);
 		renderer.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		//Render Particles
		renderer.DisableStencilTest();
		glEnable(GL_DEPTH_TEST);
		renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		particleEmitter.SetEnabled(particleEnabled);
		particleEmitter.OnRender(*currentCamera);
		particleEmitter.SetEnabled(false);

		//update HUD
		hudShader.Bind();
		glm::vec3 p(-200 * window.GetAspectRatio(), 0, -180);
		for (int i = 0; i < car.GetHP(); i++) {
			p.x += 50;
			auto mvp = HUDCamera.GetProjMatrix() * HUDCamera.GetViewMatrix() * glm::translate(glm::mat4(1), p) * glm::scale(glm::mat4(1), glm::vec3(20));
			hudShader.SetUniformMat4f("MVPMat", mvp);
			hudShader.SetUniform1f("brightness", 1.0f);
			hpHUD.Draw(hudShader);
		}

		if (flareManager.IsEnabled()) {
			renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
			flareManager.OnRender(currentCamera, &HUDCamera, sun.GetWorldPosition(currentCamera->GetPosition()), &window);
			renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		Text->RenderText("Points: " + std::to_string(static_cast<int>(points)), window.GetWidth() * 0.88f, 20, 1.0f);

		if (car.GetHP() == 0) {
			hudShader.Bind();
			auto mvp = HUDCamera.GetProjMatrix() * HUDCamera.GetViewMatrix() * glm::scale(glm::mat4(1), glm::vec3(210));
			hudShader.SetUniformMat4f("MVPMat", mvp);
			hudShader.SetUniform1f("brightness", 1.0f);
			gameoverHUD.Draw(hudShader);
			Timer::Pause();
			gameover = true;

		}

		if (debug_mode) {
			ImGui::Begin("Debug Window", &debug_mode);
			ImGui::DragFloat3("debugVec3a", &debugVec3a[0], 1, -1000, 10000);
			ImGui::DragFloat3("debugVec3b", &debugVec3b[0], 1, -1000, 10000);
			ImGui::DragFloat("debugFloat", &debugFloat, 0.01f, -100, 100);
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