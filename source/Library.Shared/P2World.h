#pragma once

#include "P2Body.h"

namespace Physia2D
{
	class P2World
	{
	public:

		P2World(const glm::vec2& gravity = glm::vec2(0, -9.8f));
		~P2World() = default;

		std::shared_ptr<P2Body> CreateBody(const P2BodyConfig& bodyConfig);
		void DeleteBody(std::shared_ptr<P2Body> body);

		std::vector<std::shared_ptr<P2Body>> GetBodies() const;

		glm::vec2 GetGravity() const;
		void SetGravity(const glm::vec2& gravity);

	private:

		std::vector<std::shared_ptr<P2Body>> mBodies;
		glm::vec2 mGravity;
	};
}