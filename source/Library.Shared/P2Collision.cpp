#include "pch.h"
#include "P2Collision.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"
#include "P2Body.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/***********************************************************************************************/
	P2Collision& P2Collision::GetInstance()
	{
		static P2Collision sInstance;

		return sInstance;
	}

	/***********************************************************************************************/
	void P2Collision::ResolveCollisions(vector<PotentiallyCollidingPair>& pairs, const float32_t elapsedTime) const
	{
		for (auto& pair : pairs)
		{
			Manifold manifold;
			if (TestOverlap(*pair.first, *pair.second, manifold, elapsedTime))
			{
				// for rendering
				pair.first->SetIsColliding();
				pair.second->SetIsColliding();
				ResolveCollision(*pair.first, *pair.second, manifold, elapsedTime);
			}
		}
	}

	/***********************************************************************************************/
	void P2Collision::ResolveCollision(P2Body& body1, P2Body& body2, const Manifold& manifold, const float32_t elapsedTime) const
	{
		// Calculate relative velocity
		vec2 relativeVel = body2.GetLinearVelocity() - body1.GetLinearVelocity();

		P2Transform body1PotentialTransform = body1.GetTransform();
		P2Transform body2PotentialTransform = body2.GetTransform();
		body1PotentialTransform.Position += body1.GetLinearVelocity() * elapsedTime;
		body1PotentialTransform.Rotation = body1PotentialTransform.Rotation.GetRotation() + body1.GetAngularVelocity() * elapsedTime;
		body2PotentialTransform.Position += body2.GetLinearVelocity() * elapsedTime;
		body2PotentialTransform.Rotation = body2PotentialTransform.Rotation.GetRotation() + body2.GetAngularVelocity() * elapsedTime;

		vec2 body1Center = body1.GetFixture()->GetShape()->ComputeAABB(body1PotentialTransform).GetCenter();
		vec2 body2Center = body2.GetFixture()->GetShape()->ComputeAABB(body2PotentialTransform).GetCenter();

		// if the bodies are not moving towards each other, than do nothing
		if (dot(body1Center - body2Center, relativeVel) <= 0)
		{
			return;
		}

		// Calculate relative velocity in terms of the normal direction
		float32_t velAlongNormal = dot(manifold.Normal, relativeVel);

		if (body1.GetFixture()->GetShape()->GetType() == P2Shape::EType::Polygon &&
			body2.GetFixture()->GetShape()->GetType() == P2Shape::EType::Polygon &&
			velAlongNormal > 0)
		{
			return;
		}

		// Calculate restitution
		float32_t e = glm::min(body1.GetFixture()->GetBounciness(), body2.GetFixture()->GetBounciness());

		// Calculate impulse scalar
		float j = -(1 + e) * velAlongNormal;
		j /= body1.GetInvMass() + body2.GetInvMass();

		// Apply impulse
		vec2 impulse = j * manifold.Normal;
		body1.SetLinearVelocity(body1.GetLinearVelocity() - body1.GetInvMass() * impulse);
		body2.SetLinearVelocity(body2.GetLinearVelocity() + body2.GetInvMass() * impulse);
	}

	/***********************************************************************************************/
	bool P2Collision::TestOverlap(const P2Body& body1, const P2Body& body2, Manifold& manifold, const float32_t elapsedTime) const
	{
		if (body1.GetFixture() == nullptr || body1.GetFixture()->GetShape() == nullptr ||
			body2.GetFixture() == nullptr || body2.GetFixture()->GetShape() == nullptr)
		{
			return false;
		}

		P2Transform body1PotentialTransform = body1.GetTransform();
		P2Transform body2PotentialTransform = body2.GetTransform();
		body1PotentialTransform.Position += body1.GetLinearVelocity() * elapsedTime;
		body1PotentialTransform.Rotation = body1PotentialTransform.Rotation.GetRotation() + body1.GetAngularVelocity() * elapsedTime;
		body2PotentialTransform.Position += body2.GetLinearVelocity() * elapsedTime;
		body2PotentialTransform.Rotation = body2PotentialTransform.Rotation.GetRotation() + body2.GetAngularVelocity() * elapsedTime;

		switch (body1.GetFixture()->GetShape()->GetType())
		{
			case P2Shape::EType::Circle:
			{
				switch (body2.GetFixture()->GetShape()->GetType())
				{
					case P2Shape::EType::Circle:
					{
						return CircleCircleCollision(*body1.GetFixture()->GetShape()->As<P2CircleShape>(), body1PotentialTransform,
													 *body2.GetFixture()->GetShape()->As<P2CircleShape>(), body2PotentialTransform, manifold);
					}
					case P2Shape::EType::Polygon:
					{
						return CirclePolygonCollision(*body1.GetFixture()->GetShape()->As<P2CircleShape>(), body1PotentialTransform,
													  *body2.GetFixture()->GetShape()->As<P2PolygonShape>(), body2PotentialTransform, manifold);
					}
					default:;
				}
				break;
			}
			case P2Shape::EType::Polygon:
			{
				switch (body2.GetFixture()->GetShape()->GetType())
				{
					case P2Shape::EType::Circle:
					{
						return CirclePolygonCollision(*body2.GetFixture()->GetShape()->As<P2CircleShape>(), body2PotentialTransform,
													  *body1.GetFixture()->GetShape()->As<P2PolygonShape>(), body1PotentialTransform, manifold);
					}
					case P2Shape::EType::Polygon:
					{
						return PolygonPolygonCollision(*body2.GetFixture()->GetShape()->As<P2PolygonShape>(), body2PotentialTransform,
													   *body1.GetFixture()->GetShape()->As<P2PolygonShape>(), body1PotentialTransform, manifold);
					}
					default:;
				}
				break;
			}
			default:;
		}

		return false;
	}

	/***********************************************************************************************/
	bool P2Collision::CircleCircleCollision(const P2CircleShape& circle1, const P2Transform& trans1,
											const P2CircleShape& circle2, const P2Transform& trans2, Manifold& manifold) const
	{
		// rotate and translate the shapes
		vec2 center1 = MathHelper::GetInstance().RotateAndTranslateVertex(circle1.GetCenter(), trans1);
		vec2 center2 = MathHelper::GetInstance().RotateAndTranslateVertex(circle2.GetCenter(), trans2);

		float32_t distanceSqr = MathHelper::GetInstance().LengthSquared(center2 - center1);
		float32_t radiusSqrSum = (circle1.GetRadius() + circle2.GetRadius()) * (circle1.GetRadius() + circle2.GetRadius());

		if (distanceSqr > radiusSqrSum)
		{
			return false;
		}

		// circles are colliding, so we will build the manifold
		vec2 vec = center2 - center1;		
		float32_t distance = length(vec);

		if (distance != 0)
		{
			manifold.Penetration = circle1.GetRadius() + circle2.GetRadius() - distance;

			// Utilize our distance since we performed sqrt on it already
			manifold.Normal = vec / distance;
		}
		else
		{
			// Circles are on same position
			manifold = Manifold(circle1.GetRadius());
		}
		return true;
	}

	/***********************************************************************************************/
	bool P2Collision::PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
											  const P2PolygonShape& polygon2, const P2Transform& trans2, Manifold& manifold) const
	{
		// rotate and translate the shapes
		vector<vec2> vertices1 = polygon1.GetRotatedAndTranslatedVertices(trans1);
		vector<vec2> vertices2 = polygon2.GetRotatedAndTranslatedVertices(trans2);

		vec2 edge;
		float32_t minIntervalDistance = MathHelper::GetInstance().MaxFloat();
		bool isIntersecting = true;
		vec2 normalToUse;

		// todo change this with the translated and rotated center of the polygon !!!
		vec2 fromPoly2ToPoly1 = MathHelper::GetInstance().RotateAndTranslateVertex(polygon1.GetCenter(), trans1)
			- MathHelper::GetInstance().RotateAndTranslateVertex(polygon1.GetCenter(), trans2);

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

			// Find the axis perpendicular to the current edge, i went right because the polygons are CCW
			vec2 axis = MathHelper::GetInstance().RightHandNormal(edge);
			axis = normalize(axis);

			// Find the projection of the polygons on the current axis
			auto interval1 = ProjectPolygonOnAxis(vertices1, axis);
			auto interval2 = ProjectPolygonOnAxis(vertices2, axis);

			// Check if the polygon projections are currentlty intersecting
			float32_t currentIntervalDistance = interval1.IntervalDistance(interval2);
			if (currentIntervalDistance > 0)
			{
				isIntersecting = false;
			}

			// update the min intersection and normal : to be used in case of collision to populate the manifold
			currentIntervalDistance = abs(currentIntervalDistance);
			if (minIntervalDistance > currentIntervalDistance)
			{
				minIntervalDistance = currentIntervalDistance;
				normalToUse = axis;
				if (dot(normalToUse, fromPoly2ToPoly1) < 0)
				{
					normalToUse = -normalToUse;
				}
			}
		}

		if (isIntersecting)
		{
			manifold.Penetration = minIntervalDistance;
			manifold.Normal = normalToUse;
		}

		return isIntersecting;
	}

	/***********************************************************************************************/
	bool P2Collision::CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
											 const P2PolygonShape& polygon, const P2Transform& polygonTrans, Manifold& manifold) const
	{
		// rotate and translate the shapes
		vec2 circleCenter = MathHelper::GetInstance().RotateAndTranslateVertex(circle.GetCenter(), circleTrans);
		vector<vec2> polygonVertices = polygon.GetRotatedAndTranslatedVertices(polygonTrans);

		// todo change this with the translated and rotated center of the polygon !!!
		vec2 fromCircleToPolygon = MathHelper::GetInstance().RotateAndTranslateVertex(polygon.GetCenter(), polygonTrans) - circleCenter;

		// init variables
		vec2 vertex = polygonVertices.back();

		float32_t nearestDistanceSqr = MathHelper::GetInstance().MaxFloat();
		int32_t nearestVertex = -1;

		// detect the nearest vertex
		for (uint32_t i = 0; i < polygon.VerticesCount(); i++)
		{
			vec2 axis = circleCenter - polygonVertices[i];
			float32_t distanceSqr = MathHelper::GetInstance().LengthSquared(axis) - circle.GetRadiusSqr();

			// collides with the vertex
			if (distanceSqr <= 0)
			{
				float32_t distance = length(axis);
				if (distance != 0)
				{
					manifold.Penetration = circle.GetRadius() - distance;
					manifold.Normal = axis / distance;
					if (dot(manifold.Normal, fromCircleToPolygon) < 0)
					{
						manifold.Normal = -manifold.Normal;
					}
				}
				else
				{
					manifold = Manifold(circle.GetRadius());
				}
				return true;
			}

			if (distanceSqr < nearestDistanceSqr)
			{
				nearestVertex = i;
				nearestDistanceSqr = distanceSqr;
			}
		}

		int32_t previousIndex = (nearestVertex + polygonVertices.size() - 1) % polygonVertices.size();
		vertex = polygonVertices[previousIndex];

		for (uint32_t i = 0; i < 2; i++)
		{
			vec2 nextVertex = polygonVertices[(nearestVertex + i) % polygonVertices.size()];
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
					float32_t distance = length(axis);
					if (distance != 0)
					{
						manifold.Penetration = circle.GetRadius() - distance;
						manifold.Normal = axis / distance;
						if (dot(manifold.Normal, fromCircleToPolygon) < 0)
						{
							manifold.Normal = -manifold.Normal;
						}
					}
					else
					{
						manifold = Manifold(circle.GetRadius());
					}
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

		return false;
	}

	/***********************************************************************************************/
	bool P2Collision::AABBvsAABBOverlap(const P2AABB& aabb1, const P2AABB& aabb2) const
	{
		return aabb1.LowerVert.x < aabb2.UpperVert.x && // left of R1 is not on the right of R2
			aabb2.LowerVert.x < aabb1.UpperVert.x && // left of R2 is not on the right of R1
			aabb1.UpperVert.y > aabb2.LowerVert.y && // top of R1 is not below bottom of R2
			aabb2.UpperVert.y > aabb1.LowerVert.y;   // top of R2 is not below bottom of R1
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
			if (dotProduct > interval.Max)
			{
				interval.Max = dotProduct;
			}
		}

		return interval;
	}
}
