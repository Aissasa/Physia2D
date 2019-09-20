#pragma once

#include <fwd.hpp>
#include <vec2.hpp>

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
		glm::float32_t mCos{}, mSin{};
	};

	/** Structure representing a transform of a body in the world.
	*/
	struct P2Transform
	{
		P2Transform(const glm::vec2& position = glm::vec2(0), const glm::float32_t rotation = 0)
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
		glm::vec2 RotateVertex(const glm::vec2& vec, const P2Rotation& rotation) const;

		glm::vec2 LeftHandNormal(const glm::vec2& v1, const glm::vec2& v2) const;
		glm::vec2 LeftHandNormal(const glm::vec2& v) const;
		glm::vec2 RightHandNormal(const glm::vec2& v1, const glm::vec2& v2) const;
		glm::vec2 RightHandNormal(const glm::vec2& v) const;

		glm::float32_t CrossProduct(const glm::vec2& v1, const glm::vec2& v2) const;
		glm::vec2 CrossProduct(const glm::vec2& v, const glm::float32_t s) const;
		glm::vec2 CrossProduct(const glm::float32_t s, const glm::vec2& v) const;

		glm::float32_t LengthSquared(glm::vec2 vec) const;
		glm::float32_t MaxFloat() const;

		glm::int32_t RangedRandomInt(const glm::int32_t max = RAND_MAX, const glm::int32_t min = 0) const;
		glm::uint8_t RangedRandomUByte(const glm::uint8_t max = UINT8_MAX, const glm::uint8_t min = 0) const;

		glm::float32_t FromRadiansToDegrees(const glm::float32_t radians) const;
		glm::float32_t FromDegreesToRadians(const glm::float32_t degrees) const;

	private:

		MathHelper();
		~MathHelper() = default;

		glm::float32_t mDegToRadMultiplier;
		glm::float32_t mRadToDegMultiplier;
	};

}