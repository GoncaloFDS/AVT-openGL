#include "Cheerio.h"
#include "Timer.h"

Cheerio::Cheerio() {
	m_AABB = AABB(glm::vec3(-2), glm::vec3(2));

}


Cheerio::~Cheerio() {
}

void Cheerio::OnCollision(SceneNode& other) {
	direction =transform.position - other.transform.position;
	direction.y = 0;
	direction = glm::normalize(direction);
	m_Speed = 15;
}

void Cheerio::OnUpdate(SceneNode& parent) {
	SceneNode::OnUpdate(parent);
	transform.position += direction * m_Speed * Timer::deltaTime;
	
	m_Speed -= 1.0f * Timer::deltaTime;
	if (m_Speed < 0)
		m_Speed = 0;
}
