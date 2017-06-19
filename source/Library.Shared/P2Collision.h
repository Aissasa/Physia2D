#pragma once

#include "P2Math.h"

namespace Physia2D
{
	class P2CircleShape;
	class P2PolygonShape;

	class P2Collision
	{
	public:

		static bool CircleCircleCollision(const P2CircleShape& circle1, const P2Transform& trans1,
										  const P2CircleShape& circle2, const P2Transform& trans2);

		static bool PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
											const P2PolygonShape& polygon2, const P2Transform& trans2);

		static bool CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
										   const P2PolygonShape& polygon, const P2Transform& polygonTrans);

	};
}
