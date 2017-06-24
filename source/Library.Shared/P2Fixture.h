#pragma once

#include <memory>
#include "glm.hpp"

namespace Physia2D
{
	class P2Shape;
	class P2Body;

	/** Struct that encapsulates the configuration of a fixture.
	*/
	struct P2FixtureConfig
	{
		P2FixtureConfig() :
			Shape(nullptr), Density(0), Bounciness(0), Friction(0.3f)
		{
		}

		P2Shape* Shape;
		glm::float32_t Density;
		glm::float32_t Bounciness;
		glm::float32_t Friction;
	};

	/** Class to encapsulate the shape and non geometrical attributes of a body.
	* It can only created from a P2Body
	*/
	class P2Fixture
	{
	public:

		P2Fixture(const glm::float32_t density = 0.0f, const glm::float32_t bounciness = 0.0f, const glm::float32_t friction = 0.3f, std::shared_ptr<P2Shape> shape = nullptr);
		virtual ~P2Fixture() = default;

		P2Body* GetBody() const;
		std::shared_ptr<P2Shape> GetShape() const;

		glm::float32_t GetDensity() const;
		void SetDensity(const glm::float32_t density);

		glm::float32_t GetBounciness() const;
		void SetBounciness(const glm::float32_t bounciness);

		glm::float32_t GetFriction() const;
		void SetFriction(const glm::float32_t friction);

	private:

		//friend class P2World;
		friend class P2Body;

		void Create(P2Body& body, const P2FixtureConfig& fixtureConfig);

		P2Body* mBody;

		std::shared_ptr<P2Shape> mShape;
		glm::float32_t mDensity;
		glm::float32_t mBounciness;
		glm::float32_t mFriction;
	};
}