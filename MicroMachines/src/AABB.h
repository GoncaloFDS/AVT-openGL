#pragma once
#include <glm/vec3.hpp>

struct CollisionData {
	bool isColliding;
	float distance;
	CollisionData(bool b, float d) : isColliding(b), distance(d) {}
};

class AABB {
private:
	glm::vec3 m_MinExtents;	//min centered at tranform.position
	glm::vec3 m_MaxExtents; //max centered at tranform.position
	glm::vec3 m_MinSize;	//min with center (0, 0, 0)
	glm::vec3 m_MaxSize;	//max with center (0, 0, 0)
public:
	AABB(const glm::vec3& mins, const glm::vec3& maxs) : m_MinSize(mins), m_MaxSize(maxs) {}
	AABB() = default;
	~AABB();

	CollisionData CheckCollision(AABB& other);

	glm::vec3 GetMinExtents();
	glm::vec3 GetMaxExtents();

	void OnUpdate(glm::vec3 position);
};

