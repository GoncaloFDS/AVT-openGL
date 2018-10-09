#include "Orange.h"

#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include "Timer.h"

Orange::Orange(Model& model) 
	:m_Model(model), m_Speed(15), m_TurnSpeed(1.0f) {
	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
	model.transform.scale = glm::vec3(5);
	model.transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
}

Orange::~Orange() {
}

void Orange::OnUpdate() {
	m_Model.transform.position += m_Forward * m_Speed * Timer::deltaTime;
	m_Model.transform.rotation = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * Timer::deltaTime, m_Right) * glm::mat4_cast(m_Model.transform.rotation);
}
