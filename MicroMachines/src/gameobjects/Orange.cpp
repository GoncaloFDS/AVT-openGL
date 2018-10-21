#include "Orange.h"

#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include <cstdlib>
#include <time.h>

#include "Timer.h"
#include "AABB.h"

Orange::Orange() {
	m_Speed = 15.0f;
	m_TurnSpeed = -2.0f;
	transform.scale = glm::vec3(1);
	glm::vec3 orig(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
	glm::vec3 dest(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
// 	LOG("Orig: " << orig.x << " " << orig.y << " " << orig.z);
// 	LOG("Dest: " << dest.x << " " << dest.y << " " << dest.z);
// 	LOG("-");
	 m_Forward = glm::normalize(dest - orig);
	 m_Right = glm::cross( m_Forward, glm::vec3(0, 1, 0));
	 transform.position = orig;	
	 m_AABB = AABB(glm::vec3(-3.5, -3.5, -3.5), glm::vec3(3.5, 3.5, 3.5));
}

Orange::~Orange() {
	LOG("Orange Destroyed");
}

void Orange::OnUpdate() {
	 transform.position +=  m_Forward * m_Speed * Timer::deltaTime;
	 transform.rotation = glm::rotate(glm::mat4(1.0f), m_TurnSpeed * Timer::deltaTime, m_Right) * glm::mat4_cast( transform.rotation);
	 transform.scale = glm::vec3(1);
	auto dist = glm::distance( transform.position, glm::vec3(0.0f));
	if (dist * dist > 350 * 350) {
		m_Speed += 1.5f;	
		m_TurnSpeed -= 1.5f;
		glm::vec3 orig(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
		glm::vec3 dest(rand() % 700 - 350, 0.0f, rand() % 700 - 350);
// 		LOG("Orig: " << orig.x << " " << orig.y << " " << orig.z);
// 		LOG("Dest: " << dest.x << " " << dest.y << " " << dest.z);
// 		LOG("-");
		m_Forward = glm::normalize(dest - orig);
		m_Right = glm::cross( m_Forward, glm::vec3(0, 1, 0));
		transform.position = orig;
	}
}
