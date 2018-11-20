#include "Orange.h"

#include <glm/vec3.hpp> 
#include <glm/mat4x4.hpp>  
#include <cstdlib>
#include <time.h>

#include "Timer.h"
#include "AABB.h"
#include "Car.h"

Orange::Orange() {
	m_Speed = 15.0f;
	m_TurnSpeed = -2.0f;
	transform.scale = glm::vec3(1);
	glm::vec3 orig(rand() % 700 - 350, 3.0f, rand() % 700 - 350);
	glm::vec3 dest(rand() % 700 - 350, 3.0f, rand() % 700 - 350);
	if (dest != orig)
		m_Forward = glm::normalize(dest - orig);
	else
		m_Forward = glm::vec3(1, 0, 0);
	m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0, 1, 0)));
	transform.position = orig;	
	m_AABB = AABB(glm::vec3(-3.5, -3.5, -3.5), glm::vec3(3.5, 3.5, 3.5));
}

Orange::~Orange() {
	LOG("Orange Destroyed");
}

void Orange::OnUpdate(SceneNode& parent) {
	transform.position += m_Forward * m_Speed * Timer::deltaTime;
	ASSERT((m_Right != glm::vec3(0.0f)));
	turningAngle += m_TurnSpeed * Timer::deltaTime;
	transform.rotation = glm::rotate(glm::mat4(1.0f), turningAngle, m_Right);
	//transform.scale = glm::vec3(1);
	auto dist = glm::distance(transform.position, glm::vec3(0.0f));
	if (dist * dist > 122500 /*350 * 350*/) {
		Reset();
	}
	SceneNode::OnUpdate(parent);
}

void Orange::Reset() {
	m_Speed += 1.5f;
	m_TurnSpeed -= 1.5f;
	glm::vec3 orig(rand() % 700 - 350, 3.0f, rand() % 700 - 350);
	glm::vec3 dest(rand() % 700 - 350, 3.0f, rand() % 700 - 350);
	if (dest != orig)
		m_Forward = glm::normalize(dest - orig);
	else
		m_Forward = glm::vec3(1, 0, 0);
	m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0, 1, 0)));
	transform.position = orig;
}

void Orange::OnCollision(SceneNode& other) {
	auto& car = dynamic_cast<Car&>(other);
	car.Reset();
	car.LoseHP();
	Reset();
}
