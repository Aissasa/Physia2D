#pragma once

#include "P2Math.h"
#include "P2Fixture.h"

namespace Physia2D
{
	class P2World;

	/** Enum expressing the type of a body.
	 * Static: zero mass, zero velocity, may be manually moved
	 * Kinematic: zero mass, non-zero velocity set by user, moved by update
	 * Dynamic: positive mass, non-zero velocity determined by forces, moved by
	*/
	enum class P2BodyType
	{
		Static,
		Dynamic,
		Kinematic,
		Max
	};

	/** Struct that encapsulates the configuration of a body.
	*/
	struct P2BodyConfig
	{
		P2BodyConfig() :
			Type(P2BodyType::Dynamic), Position(0.f), Rotation(0.f), LinearVelocity(0.f), AngularVelocity(0.f), GravityScale(1.f)
		{
		}

		P2BodyType Type;
		glm::vec2 Position;
		glm::float32_t Rotation;
		glm::vec2 LinearVelocity;
		glm::float32_t AngularVelocity;
		glm::float32_t GravityScale;
	};

	/** Class representing a Rigidbody.
	*/
	class P2Body final
	{
	public:

		P2Body(const P2BodyConfig& bodyConfig);
		~P2Body();

		std::shared_ptr<P2Fixture> CreateFixture(const P2FixtureConfig& fixtureConfig, bool replaceIfExists = true);

		void Update(const glm::float32_t elapsedTime);

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
		const P2World* GetWorld() const;
		void SetWorld(const P2World& world);

		glm::float32_t GetMass() const;
		glm::float32_t GetInvMass() const;
		glm::float32_t GetInertia() const;
		glm::float32_t GetInvInertia() const;

		bool IsColliding() const;
		void SetIsColliding();

		P2BodyType GetBodyType() const;
		void SetBodyType(const P2BodyType bodyType);

	private:

		friend class P2World;

		void UpdateForces();
		void UpdateVelocities(const glm::float32_t elapsedTime);
		void UpdatePosition(const glm::float32_t elapsedTime);

		P2Transform mTransform;
		glm::vec2 mLinearVelocity;
		glm::float32_t mAngularVelocity;

		glm::vec2 mForce;
		glm::float32_t mTorque;

		// todo replace this with waek_ptr
		const P2World* mWorld;
		std::shared_ptr<P2Fixture> mFixture;

		glm::float32_t mMass;
		glm::float32_t mInvMass;
		glm::float32_t mInertia;
		glm::float32_t mInvInertia;

		glm::float32_t mGravityScale;

		P2BodyType mBodyType;

		// todo for rendering purposes : remove it later
		bool mIsColliding;
		glm::float32_t mCollisionTimer;

		static const glm::float32_t kCollisionColorChangeTime;
	};
}