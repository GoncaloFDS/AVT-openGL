#include "Car.h"
#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include "Timer.h"



Car::Car(Model& model)
	: m_Model(model), m_Speed(50), m_MaxSpeed(5), m_Acceleration(1) , m_TurnSpeed(1.0f){
	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
}

Car::~Car() {
}

void Car::Move(float amount) {
	m_Model.transform.position += m_Forward * m_Speed * amount * Timer::deltaTime;
}

void Car::Turn(float amount) {
	auto rot = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * -amount * Timer::deltaTime, glm::vec3(0, 1.0f, 0));
	m_Model.transform.rotation = rot * glm::mat4_cast(m_Model.transform.rotation);
	m_Forward = rot * glm::vec4(m_Forward, 1.0f);
}
