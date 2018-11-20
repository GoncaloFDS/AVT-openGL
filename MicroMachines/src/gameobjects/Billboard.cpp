#include "Billboard.h"
#include "AABB.h"
#include "Car.h"

Billboard::Billboard(glm::vec3 position, SceneNode* target) : m_Target(target){
	m_AABB = AABB(glm::vec3(-1, -20, -1), glm::vec3(1, 20, 1));
	transform.scale = glm::vec3(25.f);
	transform.position = position;
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
}

void Billboard::OnUpdate(SceneNode& parent) {
	LookAt(*m_Target);
	SceneNode::OnUpdate(parent);
}

void Billboard::OnCollision(SceneNode& other) {
	auto& car = dynamic_cast<Car&>(other);
	car.Stop();
}

void Billboard::SetTarget(SceneNode* target) {
	m_Target = target;
}

