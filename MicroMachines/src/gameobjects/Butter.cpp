#include "Butter.h"
#include "Timer.h"
#include "Car.h"

Butter::Butter() {
	m_AABB = AABB(glm::vec3(-10), glm::vec3(10));
}


Butter::~Butter() {
}

void Butter::OnCollision(SceneNode& other) {
	m_Direction = transform.position - other.transform.position;
	m_Direction.y = 0;
	m_Direction = glm::normalize(m_Direction);
	m_Speed = 5;
	auto& car = dynamic_cast<Car&>(other);
	car.Stop();
}

void Butter::OnUpdate(SceneNode& parent) {
	SceneNode::OnUpdate(parent);
	transform.position += m_Direction * m_Speed * Timer::deltaTime;

	m_Speed -= 1.0f * Timer::deltaTime;
	if (m_Speed < 0)
		m_Speed = 0;
}
