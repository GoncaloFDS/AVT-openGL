#include "Orange.h"

#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include <cstdlib>
#include <time.h>

#include "Timer.h"

Orange::Orange() {
	m_Speed = 15.0f;
	m_TurnSpeed = -2.0f;
	//m_Node.transform.scale = glm::vec3(5);
	glm::vec3 orig(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
	glm::vec3 dest(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
// 	LOG("Orig: " << orig.x << " " << orig.y << " " << orig.z);
// 	LOG("Dest: " << dest.x << " " << dest.y << " " << dest.z);
// 	LOG("-");
	m_Node.m_Forward = glm::normalize(dest - orig);
	m_Right = glm::cross(m_Node.m_Forward, glm::vec3(0, 1, 0));
	m_Node.transform.position = orig;	
}

Orange::~Orange() {
	LOG("Orange Destroyed");
}

SceneNode& Orange::GetSceneNode() {
	return m_Node;
}

void Orange::OnUpdate() {
	m_Node.transform.position += m_Node.m_Forward * m_Speed * Timer::deltaTime;
	m_Node.transform.rotation = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * Timer::deltaTime, m_Right) * glm::mat4_cast(m_Node.transform.rotation);
	auto dist = glm::distance(m_Node.transform.position, glm::vec3(0.0f));
	if (dist * dist > 350 * 350) {
		m_Speed *= 1.2f;	
		m_TurnSpeed *= 1.2f;
		glm::vec3 orig(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
		glm::vec3 dest(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
// 		LOG("Orig: " << orig.x << " " << orig.y << " " << orig.z);
// 		LOG("Dest: " << dest.x << " " << dest.y << " " << dest.z);
// 		LOG("-");
		m_Node.m_Forward = glm::normalize(dest - orig);
		m_Right = glm::cross(m_Node.m_Forward, glm::vec3(0, 1, 0));
		m_Node.transform.position = orig;
	}
}
