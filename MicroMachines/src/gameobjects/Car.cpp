#include "Car.h"
#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include "Timer.h"
#include "Model.h"



Car::Car()
	:m_Node(), m_Speed(0), m_MaxSpeed(80), m_Acceleration(20) , m_Breaking(60), m_TurnSpeed(1.0f){
	m_Node.m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
}

Car::~Car() {
}

SceneNode& Car::GetSceneNode() {
	return m_Node;
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

	m_Node.transform.position += m_Node.m_Forward * m_Speed * Timer::deltaTime;
}

void Car::Turn(float amount) {
	auto rot = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * -amount * Timer::deltaTime, glm::vec3(0, 1.0f, 0));
	m_Node.transform.rotation = rot * glm::mat4_cast(m_Node.transform.rotation);
	m_Node.m_Forward = rot * glm::vec4(m_Node.m_Forward, 1.0f);
}
