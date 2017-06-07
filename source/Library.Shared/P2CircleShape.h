#pragma once

#include "P2Shape.h"

/** Class that represents the circle shape.
*/
class P2CircleShape final : P2Shape
{
public:

	P2CircleShape();
	~P2CircleShape() = default;

	virtual P2AABB ComputeAABB(const P2Transform& transform) const override;
	virtual P2MassData ComputeMass(const glm::float32_t density) const override;

	glm::vec2 GetPosition() const;
	void SetPosition(const glm::vec2& position);
	glm::float32_t GetRadius() const;
	void SetRadius(const glm::float32_t radius);

private:

	glm::vec2 mPosition;
	glm::float32_t mRadius;

};
