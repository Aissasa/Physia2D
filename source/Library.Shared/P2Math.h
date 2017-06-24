#pragma once

#include "glm.hpp"

namespace Physia2D
{
	/** Structure representing a transform of an object in the world.
	*/
	struct P2Transform
	{
		P2Transform(const glm::vec2& position = glm::vec2(0), const glm::detail::float32 rotation = 0)
			: Position(position), Rotation(rotation)
		{
		}

		glm::vec2 Position;
		glm::float32_t Rotation; // in radians
	};

	/** Singleton math helper that encapsulates some math helper methods.
	*/
	class MathHelper final
	{
	public:

		MathHelper(const MathHelper&) = delete;
		MathHelper(MathHelper&&) = delete;
		MathHelper& operator=(const MathHelper&) = delete;
		MathHelper& operator=(MathHelper&&) = delete;

		static MathHelper& GetInstance();

		glm::vec2 Vec2CrossProduct(glm::vec2 v1, glm::vec2 v2) const;
		glm::float32_t LengthSquared(glm::vec2 vec) const;

		glm::float32_t MaxFloat() const;

	private:

		MathHelper() = default;
		~MathHelper() = default;
	};

	// todo add rotation struct that has cos and sin, better than having the angle
	
}
