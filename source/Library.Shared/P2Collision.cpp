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
		// rotate the shapes
		vec2 center1 = MathHelper::GetInstance().RotateAndTranslateVertex(circle1.GetCenterPosition(), trans1);
		vec2 center2 = MathHelper::GetInstance().RotateAndTranslateVertex(circle2.GetCenterPosition(), trans2);

		float32_t distanceSqr = MathHelper::GetInstance().LengthSquared(center2 - center1);
		float32_t radiusSqrSum = (circle1.GetRadius() + circle2.GetRadius()) * (circle1.GetRadius() + circle2.GetRadius());

		return distanceSqr <= radiusSqrSum;
	}

	/***********************************************************************************************/
	bool P2Collision::PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
											  const P2PolygonShape& polygon2, const P2Transform& trans2)
	{
		// rotate the shapes
		vector<vec2> vertices1 = polygon1.GetRotatedAndTranslatedVertices(trans1);
		vector<vec2> vertices2 = polygon2.GetRotatedAndTranslatedVertices(trans2);

		vec2 edge;

		// Loop through all the vertices of both polygons and create the current edge off of them
		for (uint32_t vertexIndex = 0; vertexIndex < polygon1.VerticesCount() + polygon2.VerticesCount(); ++vertexIndex)
		{
			if (vertexIndex < polygon1.VerticesCount())
			{
				uint32_t nextVertexIndex = (vertexIndex + 1) % polygon1.VerticesCount();
				edge = vertices1[nextVertexIndex] - vertices1[vertexIndex];
			}
			else
			{
				uint32_t actualIndex = vertexIndex - polygon1.VerticesCount();
				uint32_t nextVertexIndex = (actualIndex + 1) % polygon2.VerticesCount();
				edge = vertices2[nextVertexIndex] - vertices2[actualIndex];
			}

			// ===== Find if the polygons are currently intersecting =====

			// Find the axis perpendicular to the current edge
			// We go right because the polygons are CCW
			vec2 axis = MathHelper::GetInstance().RightHandNormal(edge);
			axis = normalize(axis);;

			// Find the projection of the polygons on the current axis
			auto interval1 = ProjectPolygonOnAxis(vertices1, axis);
			auto interval2 = ProjectPolygonOnAxis(vertices2, axis);

			// Check if the polygon projections are currentlty intersecting
			if (interval1.IntervalDistance(interval2) > 0)
			{
				return true;;
			}
		}

		return false;
	}

	/***********************************************************************************************/
	bool P2Collision::CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
											 const P2PolygonShape& polygon, const P2Transform& polygonTrans)
	{
		// rotate the shapes
		vec2 circleCenter = MathHelper::GetInstance().RotateAndTranslateVertex(circle.GetCenterPosition(), circleTrans);
		vector<vec2> polygonVertices = polygon.GetRotatedAndTranslatedVertices(polygonTrans);

		// init variables
		vec2 vertex = polygonVertices.back();

		float32_t nearestDistance = MathHelper::GetInstance().MaxFloat();
		int32_t nearestVertex = -1;

		// detect the nearest vertex
		for (uint32_t i = 0; i < polygon.VerticesCount(); i++)
		{
			vec2 axis = circleCenter - polygonVertices[i];
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
			vec2 axis = circleCenter - vertex;

			float32_t edgeAxisDot = dot(edge, axis);

			// see if the circle is in the edge region
			// first one means that the angle between the edge and the cicleCenter and the vertex axis is less than +-pi/2
			// second one means the projection scalor is less than 1
			if (edgeAxisDot >= 0 && edgeAxisDot <= edgeLengthSquared)
			{
				vec2 projection = vertex + (edgeAxisDot / edgeLengthSquared) * edge;
				axis = projection - circleCenter;

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

	/***********************************************************************************************/
	float32_t P2Collision::ProjectionInterval::IntervalDistance(const ProjectionInterval& otherInterval) const
	{
		if (Min < otherInterval.Min)
		{
			return otherInterval.Min - Max;
		}
		return Min - otherInterval.Max;
	}

	/***********************************************************************************************/
	P2Collision::ProjectionInterval P2Collision::ProjectPolygonOnAxis(const vector<vec2> vertices, const vec2& axis)
	{
		ProjectionInterval interval;

		for (auto& vertex : vertices)
		{
			// To project a point on an axis use the dot product
			float32_t dotProduct = dot(axis, vertex);
			if (dotProduct < interval.Min)
			{
				interval.Min = dotProduct;
			}
			else
			{
				if (dotProduct > interval.Max)
				{
					interval.Max = dotProduct;
				}
			}
		}

		return interval;
	}

}
