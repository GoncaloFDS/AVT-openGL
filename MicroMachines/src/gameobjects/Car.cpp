#include "Car.h"
#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include "Timer.h"
#include "Model.h"
#include "AABB.h"



Car::Car()
	: m_Speed(0), m_MaxSpeed(80), m_Acceleration(20) , m_Breaking(60), m_TurnSpeed(1.0f){
	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
	m_AABB = AABB(glm::vec3(-9), glm::vec3(9));
}

Car::~Car() {
}


glm::vec3 Car::GetRightVector() {
	return m_Right;
}

void Car::SetWheelsModel(Model& wheelModel) {
	
	//Right Front
	wheelRF.SetModel(wheelModel);
	wheelRF.transform.position = glm::vec3(-3.f, -1, 4);
	wheelRF.transform.scale = glm::vec3(0.8f, 0.5f, 0.5f);
	AddChildNode(&wheelRF);
	//Left Front
	wheelLF.SetModel(wheelModel);
	wheelLF.transform.position = glm::vec3(3.f, -1, 4);
	wheelLF.transform.scale = glm::vec3(0.8f, 0.5f, 0.5f);
	wheelLF.transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	AddChildNode(&wheelLF);
	//Right Back
	wheelRB.SetModel(wheelModel);
	wheelRB.transform.position = glm::vec3(-3.f, -1, -4);
	wheelRB.transform.scale = glm::vec3(0.8f, 0.5f, 0.5f);
	AddChildNode(&wheelRB);
	//LeftBack
	wheelLB.SetModel(wheelModel);
	wheelLB.transform.position = glm::vec3(3.f, -1, -4);
	wheelLB.transform.scale = glm::vec3(0.8f, 0.5f, 0.5f);
	wheelLB.transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	AddChildNode(&wheelLB);
}

void Car::SetWheelsShader(Shader& shader) {
	wheelRF.SetShader(shader);	
	wheelLF.SetShader(shader);
	wheelRB.SetShader(shader);
	wheelLB.SetShader(shader);
}

float Car::GetSpeed() {
	return m_Speed;
}

void Car::Stop() {
	m_Speed = 0;
}

void Car::Move(float amount) { // TODO Way too many ifs
	if(amount != 0) 
		m_Speed += m_Acceleration * amount * Timer::deltaTime;
	else {
		if (m_Speed > 1)
			m_Speed -= m_Breaking * Timer::deltaTime;
		else if (m_Speed < -1)
			m_Speed += m_Breaking * Timer::deltaTime;
		else
			m_Speed = 0;
	}
	
	if (m_Speed > m_MaxSpeed)
		m_Speed = m_MaxSpeed;
	if (m_Speed < -m_MaxSpeed)
		m_Speed = -m_MaxSpeed;

	transform.position += m_Forward * m_Speed * Timer::deltaTime;
}

void Car::Turn(float amount) {
	auto rot = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * -amount * Timer::deltaTime, glm::vec3(0, 1.0f, 0));
	transform.rotation = rot * glm::mat4_cast(transform.rotation);
	m_Forward = rot * glm::vec4(m_Forward, 1.0f);

	//wheelRotation
	rot = glm::rotate(glm::mat4(1.0f), 1.f * m_Speed * Timer::deltaTime, m_Right);
	wheelLB.transform.rotation = rot * glm::mat4_cast(wheelLB.transform.rotation);
	wheelRB.transform.rotation = rot * glm::mat4_cast(wheelRB.transform.rotation);
	wheelLF.transform.rotation = rot * glm::mat4_cast(wheelLF.transform.rotation);
	wheelRF.transform.rotation = rot * glm::mat4_cast(wheelRF.transform.rotation);
}

void Car::Reset() {
	transform.position = glm::vec3(200, 0, 0);
	transform.rotation = glm::quat();
	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
	m_Speed = 0;
}

void Car::OnCollision(SceneNode& node) {

}
