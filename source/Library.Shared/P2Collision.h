#pragma once

#include "P2Math.h"

namespace Physia2D
{
	class P2Body;
	class P2CircleShape;
	class P2PolygonShape;

	/** Struct that holds a shape bounds.
	*/
	struct P2AABB
	{
		P2AABB(const glm::vec2& lowerVert = glm::vec2(0), const glm::vec2& upperVert = glm::vec2(0))
			: LowerVert(lowerVert), UpperVert(upperVert)
		{
		}

		glm::vec2 LowerVert;
		glm::vec2 UpperVert;

		glm::vec2 GetExtents() const
		{
			return UpperVert - LowerVert;
		}

		glm::vec2 GetCenter() const
		{
			return LowerVert + GetExtents() / 2.f;
		}
	};

	/** Struct that holds information about a collision point.
	*/
	struct Manifold
	{
		Manifold(const glm::float32_t penetration = 0.f, const glm::vec2 normal = glm::vec2(1, 0))
			: Penetration(penetration),
			Normal(normal)
		{
		}

		glm::float32_t Penetration;
		glm::vec2 Normal;
	};

	/** Singleton that handles collisions detection and resolution.
	*/
	class P2Collision final
	{
	public:

		typedef std::pair<std::shared_ptr<P2Body>, std::shared_ptr<P2Body>> PotentiallyCollidingPair;
		typedef std::pair<PotentiallyCollidingPair, Manifold> CollidingPair;

		P2Collision(const P2Collision &) = delete;
		P2Collision(const P2Collision &&) = delete;
		P2Collision& operator=(const P2Collision &) = delete;
		P2Collision& operator=(const P2Collision &&) = delete;

		static P2Collision& GetInstance();

		std::vector<CollidingPair> ResolveCollisions(std::vector<PotentiallyCollidingPair>& pairs, const glm::float32_t elapsedTime) const;

		bool ResolveCollision(P2Body& body1, P2Body& body2, const Manifold& manifold, const glm::float32_t elapsedTime) const;

		void PositionalCorrection(P2Body& body1, P2Body& body2, const Manifold& manifold) const;

		bool TestOverlap(const P2Body& body1, const P2Body& body2, Manifold& manifold, const glm::float32_t elapsedTime) const;

		bool CircleCircleCollision(const P2CircleShape& circle1, const P2Transform& trans1,
								   const P2CircleShape& circle2, const P2Transform& trans2, Manifold& manifold) const;

		bool PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
									 const P2PolygonShape& polygon2, const P2Transform& trans2, Manifold& manifold) const;

		bool CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
									const P2PolygonShape& polygon, const P2Transform& polygonTrans, Manifold& manifold) const;

		bool AABBvsAABBOverlap(const P2AABB& aabb1, const P2AABB& aabb2) const;

	private:

		struct ProjectionInterval
		{
			ProjectionInterval(const glm::float32 min = MathHelper::GetInstance().MaxFloat(), const glm::float32 max = -MathHelper::GetInstance().MaxFloat())
				: Min(min),
				Max(max)
			{
			}

			glm::float32 Min;
			glm::float32 Max;

			glm::float32_t IntervalDistance(const ProjectionInterval& otherInterval) const;
		};

		P2Collision() = default;
		~P2Collision() = default;

		static ProjectionInterval ProjectPolygonOnAxis(const std::vector<glm::vec2> vertices, const glm::vec2& axis);

		static const glm::float32_t kSlop; // Penetration allowance
		static const glm::float32_t kPercent; // Penetration percentage to correct
	};
}
