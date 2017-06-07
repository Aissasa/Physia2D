#pragma once

#include <glm/detail/type_vec2.hpp>
#include "P2Math.h"
#include <memory>

class P2Fixture;

// todo add init struct for body

/** Class representing a Rigidbody.
*/
class P2Body final
{
public:

	// todo add body types enum : static, dynamic, kinamatic

	P2Body();
	~P2Body() = default;

	// in case the mass data of the shape changed
	void ResetMassData();


	P2Transform GetTransform() const;
	void SetTransform(const P2Transform& transform);

	glm::vec2 GetLinearVelocity() const;
	void SetLinearVelocity(const glm::vec2& linearVelocity);

	glm::float32_t GetAngularVelocity() const;
	void SetAngularVelocity(const glm::float32_t angularVelocity);

	glm::vec2 GetForce() const;
	void SetForce(const glm::vec2& force);

	glm::float32_t GetTorque() const;
	void SetTorque(const glm::float32_t torque);

	std::shared_ptr<P2Fixture> GetFixture() const;
	void SetFixture(const std::shared_ptr<P2Fixture>& p2Fixture);

	glm::float32_t GetGravityScale() const;
	void SetGravityScale(const glm::float32_t gravityScale);

	glm::float32_t GetMass() const;
	glm::float32_t GetInvMass() const;
	glm::float32_t GetInertia() const;
	glm::float32_t GetInvInertia() const;

private:

	P2Transform mTransform;
	glm::vec2 mLinearVelocity;
	glm::float32_t mAngularVelocity;

	glm::vec2 mForce;
	glm::float32_t mTorque;

	// todo add world

	std::shared_ptr<P2Fixture> mFixture;

	glm::float32_t mMass; 
	glm::float32_t mInvMass;
	glm::float32_t mInertia; 
	glm::float32_t mInvInertia;

	glm::float32_t mGravityScale;
};
