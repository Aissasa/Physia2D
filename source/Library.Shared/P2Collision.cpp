#include "pch.h"
#include "P2Collision.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/***********************************************************************************************/
	bool P2Collision::CircleCircleCollision(const P2CircleShape& circle1, const P2Transform& trans1,
											const P2CircleShape& circle2, const P2Transform& trans2)
	{
		UNREFERENCED_PARAMETER(trans1); // todo multiply the center by the transform to get the world position
		UNREFERENCED_PARAMETER(trans2);

		float32_t distanceSqr = MathHelper::GetInstance().LengthSquared(circle2.GetCenterPosition() - circle1.GetCenterPosition());
		float32_t radiusSqrSum = (circle1.GetRadius() + circle2.GetRadius()) * (circle1.GetRadius() + circle2.GetRadius());

		return distanceSqr <= radiusSqrSum;
	}

	/***********************************************************************************************/
	bool P2Collision::CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
											 const P2PolygonShape& polygon, const P2Transform& polygonTrans)
	{
		UNREFERENCED_PARAMETER(circleTrans); // todo multiply the center by the transform to get the world position
		UNREFERENCED_PARAMETER(polygonTrans);

		// init variables
		auto& polygonVertices = polygon.GetVertices();
		vec2 vertex = polygonVertices.back();

		float32_t nearestDistance = MathHelper::GetInstance().MaxFloat();
		int32_t nearestVertex = -1;

		// detect the nearest vertex
		for (uint32_t i = 0; i < polygon.VerticesCount(); i++)
		{
			vec2 axis = circle.GetCenterPosition() - polygonVertices[i];
			float32_t distance = MathHelper::GetInstance().LengthSquared(axis) - circle.GetRadiusSqr();

			// collides with the vertex
			if (distance <= 0)
			{
				return true;
			}

			if (distance < nearestDistance)
			{
				nearestVertex = i;
				nearestDistance = distance;
			}
		}

		vertex = polygonVertices[nearestVertex - 1];

		for (uint32_t i = 0; i < 2; i++)
		{
			vec2 nextVertex = polygonVertices[nearestVertex + i];
			vec2 edge = nextVertex - vertex;
			float32_t edgeLengthSquared = MathHelper::GetInstance().LengthSquared(edge);

			assert(edgeLengthSquared != 0);
			vec2 axis = circle.GetCenterPosition() - vertex;

			float32_t edgeAxisDot = dot(edge, axis);

			// see if the circle is in the edge region
			// first one means that the angle between the edge and the cicleCenter and the vertex axis is less than +-pi/2
			// second one means the projection scalor is less than 1
			if (edgeAxisDot >= 0 && edgeAxisDot <= edgeLengthSquared) 
			{
				vec2 projection = vertex + (edgeAxisDot / edgeLengthSquared) * edge;
				axis = projection - circle.GetCenterPosition();

				// check edge and circle collision
				if (MathHelper::GetInstance().LengthSquared(axis) <= circle.GetRadiusSqr())
				{
					return true;
				}

				// test if the circle is inside of the polygon
				// the circle will be to the left of at least one of the two edges of 
				// the nearest vertex if it is inside the polygon
				if (edge.x > 0)
				{
					if (axis.y > 0)
					{
						return false;
					}
				}
				else if (edge.x < 0)
				{
					if (axis.y < 0)
					{
						return false;
					}
				}
				else if (edge.y > 0)
				{
					if (axis.x < 0)
					{
						return false;
					}
				}
				else
				{
					if (axis.x > 0)
					{
						return false;
					}
				}
			}

			vertex = nextVertex;
		}

		return true;
	}
}
