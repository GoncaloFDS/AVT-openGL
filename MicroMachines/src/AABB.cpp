#include "AABB.h"
#include "glm/common.hpp"

AABB::~AABB() {
}

CollisionData AABB::CheckCollision(AABB& other) {
	glm::vec3 distances1 = other.GetMinExtents() - m_MaxExtents;
	glm::vec3 distances2 = m_MinExtents - other.GetMinExtents();
	glm::vec3 distances = glm::max(distances1, distances2);
	
	float maxDistance = glm::max(glm::max(distances.x, distances.y), distances.z);

	return CollisionData(maxDistance < 0, maxDistance);
}

glm::vec3 AABB::GetMinExtents() {
	return m_MinExtents;
}

glm::vec3 AABB::GetMaxExtents() {
	return m_MaxExtents;
}

void AABB::OnUpdate(glm::vec3 position) {
	m_MinExtents = position + m_MinSize;
	m_MaxExtents = position + m_MaxSize;
}
