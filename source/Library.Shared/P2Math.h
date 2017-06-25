#pragma once

#include "glm.hpp"

namespace Physia2D
{
	/** Structure representing the rotation of a body.
	*/
	struct P2Rotation
	{
		P2Rotation(const glm::float32_t rotation = 0);

		void operator=(const glm::float32_t rotation);

		glm::float32_t GetRotation() const;
		void SetRotation(const glm::float32_t rotation);

		glm::float32_t Cos() const;
		glm::float32_t Sin() const;

	private:

		glm::float32_t mRotation; // in radians
		glm::float32_t mCos, mSin;
	};

	/** Structure representing a transform of a body in the world.
	*/
	struct P2Transform
	{
		P2Transform(const glm::vec2& position = glm::vec2(0), const glm::detail::float32 rotation = 0)
			: Position(position), Rotation(rotation)
		{
		}

		glm::vec2 Position;
		P2Rotation Rotation;
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

		glm::vec2 RotateAndTranslateVertex(const glm::vec2& vec, const P2Transform& transform) const;

		glm::vec2 LeftHandNormal(glm::vec2 v1, glm::vec2 v2) const;
		glm::vec2 LeftHandNormal(glm::vec2 v) const;
		glm::vec2 RightHandNormal(glm::vec2 v1, glm::vec2 v2) const;
		glm::vec2 RightHandNormal(glm::vec2 v) const;

		glm::float32_t LengthSquared(glm::vec2 vec) const;
		glm::float32_t MaxFloat() const;

		glm::float32_t FromRadiansToDegrees(const glm::float32_t radians) const;
		glm::float32_t FromDegreesToRadians(const glm::float32_t degrees) const;

	private:

		MathHelper();
		~MathHelper() = default;

		glm::float32_t mDegToRadMultiplier;
		glm::float32_t mRadToDegMultiplier;
	};

}