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
	m_AABB = AABB(glm::vec3(-7), glm::vec3(7));
	m_HP = m_MaxHP = 3;
	transform.position = glm::vec3(200.0f, -4.44f, 0.0f);
	transform.scale = glm::vec3(0.04f);
}

Car::~Car() {
}

void Car::SetWheelsModel(Model& wheel) {
	
	//Right Front
	wheelRF.SetModel(wheel);
	wheelRF.transform.position = glm::vec3(-85.429, 34.599, 128.173);
	wheelRF.transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	AddChildNode(&wheelRF);
	//Left Front
	wheelLF.SetModel(wheel);
	wheelLF.transform.position = glm::vec3(85.429, 34.599, 128.173);
	AddChildNode(&wheelLF);
	//Right Back
	wheelRB.SetModel(wheel);
	wheelRB.transform.position = glm::vec3(-85.429, 36.416, -146.734);
	wheelRB.transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	AddChildNode(&wheelRB);
	//LeftBack
	wheelLB.SetModel(wheel);
	wheelLB.transform.position = glm::vec3(85.429, 36.416, -146.734);
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

int Car::GetHP() {
	return m_HP;
}

void Car::LoseHP() {
	m_HP--;
	if (m_HP < 0)
		m_HP = 0;
}

void Car::Stop() {
	m_Speed = 0;
}

void Car::Move(float amount) { // TODO Way too many ifs
	if(amount != 0 && amount*m_Speed > 0 ) 
		m_Speed += m_Acceleration * amount * Timer::deltaTime;
	else if(amount != 0)
		m_Speed += (m_Acceleration + m_Breaking) * amount * Timer::deltaTime;
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
	transform.position = glm::vec3(200.0f, -4.44f, 0.0f);
	transform.rotation = glm::quat();
	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
	m_Speed = 0;
}

void Car::Restart() {
	Reset();
	m_HP = m_MaxHP;
}

void Car::Teleport() {
	transform.position = glm::vec3(-190, -4.44f, 0);
	transform.rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0)) * glm::mat4_cast(transform.rotation);
	m_Forward = -m_Forward;
	m_Right = -m_Right;
}

