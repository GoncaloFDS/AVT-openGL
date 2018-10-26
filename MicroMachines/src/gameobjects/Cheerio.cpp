#include "Cheerio.h"
#include "Timer.h"
#include "Car.h"

Cheerio::Cheerio() {
	m_AABB = AABB(glm::vec3(-1.5), glm::vec3(1.5));

}


Cheerio::~Cheerio() {
}

void Cheerio::OnCollision(SceneNode& other) {
	m_Direction = transform.position - other.transform.position;
	m_Direction.y = 0;
	m_Direction = glm::normalize(m_Direction);
	m_Speed = 15;
	auto& car = dynamic_cast<Car&>(other);
	car.Stop();
}

void Cheerio::OnUpdate(SceneNode& parent) {
	SceneNode::OnUpdate(parent);
	transform.position += m_Direction * m_Speed * Timer::deltaTime;
	
	m_Speed -= 1.0f * Timer::deltaTime;
	if (m_Speed < 0)
		m_Speed = 0;
}
