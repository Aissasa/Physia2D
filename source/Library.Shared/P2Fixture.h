#pragma once

#include "glm/glm.hpp"

class P2Shape;

/** Class to encapsulate the shape and non geometrical attributes of a body.
*/
class P2Fixture final
{
public:

	// todo think about adding create, clone and destroy methods
	// todo also think about adding a struct to init a fixture

	P2Fixture(std::shared_ptr<P2Shape> shape = nullptr, const float density = 0.0f, const float bounciness = 0.0f, const float friction = 0.3f);
	~P2Fixture() = default;

	std::shared_ptr<P2Shape> GetShape() const;
	void SetShape(const std::shared_ptr<P2Shape>& p2Shape);

	glm::float32_t GetDensity() const;
	void SetDensity(const glm::float32_t density);

	glm::float32_t GetBounciness() const;
	void SetBounciness(const glm::float32_t bounciness);

	glm::float32_t GetFriction() const;
	void SetFriction(const glm::float32_t friction);

private:

	std::shared_ptr<P2Shape> mShape;
	glm::float32_t mDensity;
	glm::float32_t mBounciness;
	glm::float32_t mFriction;
};
