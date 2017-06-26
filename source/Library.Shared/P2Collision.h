#pragma once

#include "P2Math.h"

namespace Physia2D
{
	class P2Body;
	class P2CircleShape;
	class P2PolygonShape;

	class P2Collision
	{
	public:

		static bool CheckCollision(const P2Body& body1, const P2Body& body2);

		static bool CircleCircleCollision(const P2CircleShape& circle1, const P2Transform& trans1,
										  const P2CircleShape& circle2, const P2Transform& trans2);

		static bool PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
											const P2PolygonShape& polygon2, const P2Transform& trans2);

		static bool CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
										   const P2PolygonShape& polygon, const P2Transform& polygonTrans);

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

		static ProjectionInterval ProjectPolygonOnAxis(const std::vector<glm::vec2> vertices, const glm::vec2& axis);

	};
}
