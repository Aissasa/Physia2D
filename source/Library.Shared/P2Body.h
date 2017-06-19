#pragma once

#include "P2Math.h"
#include "P2Fixture.h"

namespace Physia2D
{
	class P2World;

	/** Struct that encapsulates the configuration of a body.
	*/
	struct P2BodyConfig
	{
		P2BodyConfig() :
			Position(0), Rotation(0), LinearVelocity(0), AngularVelocity(0), GravityScale(0)
		{
		}

		glm::vec2 Position;
		glm::float32_t Rotation;
		glm::vec2 LinearVelocity;
		glm::float32_t AngularVelocity;
		glm::float32_t GravityScale;
	};

	/** Class representing a Rigidbody.
	*/
	class P2Body
	{
	public:

		// todo add body types enum : static, dynamic, kinamatic

		P2Body(const P2BodyConfig& bodyConfig);
		virtual ~P2Body();

		std::shared_ptr<P2Fixture> CreateFixture(const P2FixtureConfig& fixtureConfig, bool replaceIfExists = true);

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

		glm::float32_t GetGravityScale() const;
		void SetGravityScale(const glm::float32_t gravityScale);

		std::shared_ptr<P2Fixture> GetFixture() const;
		P2World* GetWorld() const;

		glm::float32_t GetMass() const;
		glm::float32_t GetInvMass() const;
		glm::float32_t GetInertia() const;
		glm::float32_t GetInvInertia() const;

	private:

		friend class P2World;

		P2Transform mTransform;
		glm::vec2 mLinearVelocity;
		glm::float32_t mAngularVelocity;

		glm::vec2 mForce;
		glm::float32_t mTorque;

		P2World* mWorld;
		std::shared_ptr<P2Fixture> mFixture;

		glm::float32_t mMass;
		glm::float32_t mInvMass;
		glm::float32_t mInertia;
		glm::float32_t mInvInertia;

		glm::float32_t mGravityScale;
	};
}