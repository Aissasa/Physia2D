#include "pch.h"
#include "P2Collision.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"
#include "P2Body.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	// todo add radii of shapes to the equations

	const float32_t P2Collision::kSlop = 0.02f;
	const float32_t P2Collision::kPercent = 0.2f;

	const float32_t P2Collision::kDynamicFrictionMultiplier = 0.7f;

	/***********************************************************************************************/
	P2Collision& P2Collision::GetInstance()
	{
		static P2Collision sInstance;

		return sInstance;
	}

	/***********************************************************************************************/
	vector<P2Collision::CollidingPair> P2Collision::ResolveCollisions(vector<PotentiallyCollidingPair>& pairs, const float32_t elapsedTime) const
	{
		vector<CollidingPair> collidingPairs;

		for (auto& potColPair : pairs)
		{
			P2Manifold manifold;
			if (TestOverlap(*potColPair.first, *potColPair.second, manifold, elapsedTime))
			{
				// for rendering
				potColPair.first->SetIsColliding();
				potColPair.second->SetIsColliding();

				bool changedVel = ResolveCollision(*potColPair.first, *potColPair.second, manifold, elapsedTime);

				if (changedVel)
				{
					collidingPairs.push_back(CollidingPair(potColPair, manifold));
				}
			}
		}

		return move(collidingPairs);
	}

	/***********************************************************************************************/
	bool P2Collision::ResolveCollision(P2Body& body1, P2Body& body2, P2Manifold& manifold, const float32_t elapsedTime) const
	{
		elapsedTime;
		auto& math = MathHelper::GetInstance();
		bool collided = true;

		for (auto& contactPoint : manifold.ContactPoints)
		{
			// get the radii of the collision
			vec2 body1Center = math.RotateAndTranslateVertex(body1.GetFixture()->GetShape()->GetCenter(), body1.GetTransform());
			vec2 body2Center = math.RotateAndTranslateVertex(body2.GetFixture()->GetShape()->GetCenter(), body2.GetTransform());
			vec2 body1Rad = contactPoint - body1Center;
			vec2 body2Rad = contactPoint - body2Center;

			// correct the collision normal
			vec2 fromBody1ToBody2 = body2Center - body1Center;

			// todo check if this fixes the collision with big objects
			if (dot(fromBody1ToBody2, manifold.Normal) < 0)
			{
				manifold.Normal = -manifold.Normal;
			}

			// Calculate relative velocity
			vec2 relativeVel = body2.GetLinearVelocity() + math.CrossProduct(body2.GetAngularVelocity(), body2Rad) -
				body1.GetLinearVelocity() - math.CrossProduct(body1.GetAngularVelocity(), body1Rad);

			// Calculate relative velocity in terms of the normal direction
			float32_t velAlongNormal = dot(manifold.Normal, relativeVel);

			// Calculate restitution
			float32_t e = glm::min(body1.GetFixture()->GetBounciness(), body2.GetFixture()->GetBounciness());

			float32_t body1RadCrossN = math.CrossProduct(body1Rad, manifold.Normal);
			float32_t body2RadCrossN = math.CrossProduct(body2Rad, manifold.Normal);

			float32_t body1InertiaComponent = body1RadCrossN * body1RadCrossN * body1.GetInvInertia();
			float32_t body2InertiaComponent = body2RadCrossN * body2RadCrossN * body2.GetInvInertia();

			// Calculate impulse scalar along the normal
			float jNormal = -(1 + e) * velAlongNormal;

			jNormal /= body1.GetInvMass() + body2.GetInvMass() + body1InertiaComponent + body2InertiaComponent;
			jNormal /= static_cast<float32_t>(manifold.ContactPoints.size());

			// Apply impulse
			vec2 nImpulse = jNormal * manifold.Normal;
			vec2 vel1 = body1.GetLinearVelocity() - body1.GetInvMass() * nImpulse;
			vec2 vel2 = body2.GetLinearVelocity() + body2.GetInvMass() * nImpulse;
			float32_t angVel1 = body1.GetAngularVelocity() - body1.GetInvInertia() * math.CrossProduct(body1Rad, nImpulse);
			float32_t angVel2 = body2.GetAngularVelocity() + body2.GetInvInertia() * math.CrossProduct(body2Rad, nImpulse);

			//// compute the potential position with the new velocity
			P2Transform body1PotentialTransform = body1.GetTransform();
			P2Transform body2PotentialTransform = body2.GetTransform();
			body1PotentialTransform.Position += vel1 * elapsedTime;
			body1PotentialTransform.Rotation = body1PotentialTransform.Rotation.GetRotation() + angVel1 * elapsedTime;
			body2PotentialTransform.Position += vel2 * elapsedTime;
			body2PotentialTransform.Rotation = body2PotentialTransform.Rotation.GetRotation() + angVel2 * elapsedTime;

			vec2 body1PotentialNewCenter = math.RotateAndTranslateVertex(body1.GetFixture()->GetShape()->GetCenter(), body1PotentialTransform);
			vec2 body2PotentialNewCenter = math.RotateAndTranslateVertex(body2.GetFixture()->GetShape()->GetCenter(), body2PotentialTransform);
			vec2 fromPotCenter2ToPotCenter1 = body1PotentialNewCenter - body2PotentialNewCenter;

			// compute the potential position with the old velocity
			P2Transform body1trans = body1.GetTransform();
			P2Transform body2trans = body2.GetTransform();
			body1trans.Position += body1.GetLinearVelocity() * elapsedTime;
			body1trans.Rotation = body1PotentialTransform.Rotation.GetRotation() + body1.GetAngularVelocity() * elapsedTime;
			body2trans.Position += body2.GetLinearVelocity() * elapsedTime;
			body2trans.Rotation = body2PotentialTransform.Rotation.GetRotation() + body2.GetAngularVelocity() * elapsedTime;

			vec2 body1PotentialOldCenter = math.RotateAndTranslateVertex(body1.GetFixture()->GetShape()->GetCenter(), body1trans);
			vec2 body2PotentialOldCenter = math.RotateAndTranslateVertex(body2.GetFixture()->GetShape()->GetCenter(), body2trans);
			vec2 fromCenter2ToCenter1 = body1PotentialOldCenter - body2PotentialOldCenter;

			// if the bodies are not moving towards each other, than do nothing
			if (/*dot(fromPotCenter2ToPotCenter1, relativeVel) < 0 &&*/
				math.LengthSquared(fromPotCenter2ToPotCenter1) < math.LengthSquared(fromCenter2ToCenter1))
			{
				collided = false;
			}

			// compute the friction impulse
			relativeVel = vel2 + math.CrossProduct(angVel2, body2Rad) - vel1 - math.CrossProduct(angVel1, body1Rad);
			vec2 tangent = relativeVel - dot(relativeVel, manifold.Normal) * manifold.Normal;

			// the body is not stationary
			if (tangent.x != 0 || tangent.y != 0)
			{
				tangent /= length(tangent);

				float32_t jTangent = -dot(relativeVel, tangent);
				jTangent /= body1.GetInvMass() + body2.GetInvMass() + body1InertiaComponent + body2InertiaComponent;
				jTangent /= static_cast<float32_t>(manifold.ContactPoints.size());

				// Coulomb's Law : i chose mu as the avg of both frictions, and 0.7f the dynamic friction multiplier
				float32_t mu = (body1.GetFixture()->GetFriction() + body2.GetFixture()->GetFriction()) / 2.f;
				vec2 frictionImpulse;

				if (abs(jTangent) < abs(jNormal * mu))
				{
					frictionImpulse = jTangent * tangent;
				}
				else
				{
					float32_t dynamicMu = mu * kDynamicFrictionMultiplier;
					frictionImpulse = -jNormal * tangent * dynamicMu;
				}

				// apply friction to the velocity
				if (collided)
				{
					vel1 -= body1.GetInvMass() * frictionImpulse;
					vel2 += body2.GetInvMass() * frictionImpulse;
					angVel1 -= body1.GetInvInertia() * math.CrossProduct(body1Rad, frictionImpulse);
					angVel2 += body2.GetInvInertia() * math.CrossProduct(body2Rad, frictionImpulse);

				}
				else
				{
					// override the normal impulse if they are not going in different directions
					vel1 = body1.GetLinearVelocity() - body1.GetInvMass() * frictionImpulse;
					vel2 = body2.GetLinearVelocity() + body2.GetInvMass() * frictionImpulse;
					angVel1 = body1.GetAngularVelocity() - body1.GetInvInertia() * math.CrossProduct(body1Rad, frictionImpulse);
					angVel2 = body2.GetAngularVelocity() + body2.GetInvInertia() * math.CrossProduct(body2Rad, frictionImpulse);
				}
			}

			body1.SetLinearVelocity(vel1);
			body2.SetLinearVelocity(vel2);
			body1.SetAngularVelocity(angVel1);
			body2.SetAngularVelocity(angVel2);
		}

		return collided;
	}

	/***********************************************************************************************/
	void P2Collision::PositionalCorrection(P2Body& body1, P2Body& body2, const P2Manifold& manifold) const
	{
		auto& math = MathHelper::GetInstance();
		vec2 fromB1ToB2 = math.RotateAndTranslateVertex(body1.GetFixture()->GetShape()->GetCenter(), body1.GetTransform())
			- math.RotateAndTranslateVertex(body2.GetFixture()->GetShape()->GetCenter(), body2.GetTransform());

		// reverse the normal if necessary
		vec2 normal = dot(manifold.Normal, fromB1ToB2) < 0 ? manifold.Normal : -manifold.Normal;

		vec2 correction = (glm::max(manifold.Penetration - kSlop, 0.0f) / (body1.GetInvMass() + body2.GetInvMass())) * kPercent * normal;
		auto body1Trans = body1.GetTransform();
		auto body2Trans = body2.GetTransform();
		body1Trans.Position -= correction * body1.GetInvMass();
		body2Trans.Position += correction * body2.GetInvMass();

		body1.SetTransform(body1Trans);
		body2.SetTransform(body2Trans);
	}

	/***********************************************************************************************/
	bool P2Collision::TestOverlap(const P2Body& body1, const P2Body& body2, P2Manifold& manifold, const float32_t elapsedTime) const
	{
		if (body1.GetFixture() == nullptr || body1.GetFixture()->GetShape() == nullptr ||
			body2.GetFixture() == nullptr || body2.GetFixture()->GetShape() == nullptr)
		{
			return false;
		}

		// check collision on the next position
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
											const P2CircleShape& circle2, const P2Transform& trans2, P2Manifold& manifold) const
	{
		auto& math = MathHelper::GetInstance();

		// rotate and translate the shapes
		vec2 center1 = math.RotateAndTranslateVertex(circle1.GetCenter(), trans1);
		vec2 center2 = math.RotateAndTranslateVertex(circle2.GetCenter(), trans2);

		float32_t distanceSqr = math.LengthSquared(center2 - center1);
		float32_t radiiSum = circle1.GetRadius() + circle2.GetRadius();
		float32_t radiiSumSqr = radiiSum * radiiSum;

		if (distanceSqr > radiiSumSqr)
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
			manifold.ContactPoints.push_back(center1 + manifold.Normal * circle1.GetRadius());
		}
		else
		{
			// Circles are on same position
			manifold = P2Manifold(circle1.GetRadius());
			manifold.ContactPoints.push_back(center1);
		}
		return true;
	}

	/***********************************************************************************************/
	bool P2Collision::PolygonPolygonCollision(const P2PolygonShape& polygon1, const P2Transform& trans1,
											  const P2PolygonShape& polygon2, const P2Transform& trans2, P2Manifold& manifold) const
	{
		auto& math = MathHelper::GetInstance();

		// rotate and translate the shapes
		vector<vec2> vertices1 = polygon1.GetRotatedAndTranslatedVertices(trans1);
		vector<vec2> vertices2 = polygon2.GetRotatedAndTranslatedVertices(trans2);

		vec2 edge;
		float32_t minIntervalDistance = math.MaxFloat();
		bool isIntersecting = true;
		vec2 normalToUse = {};
		bool needToFlipNormal = false;

		//vec2 fromPoly2ToPoly1 = math.RotateAndTranslateVertex(polygon1.GetCenter(), trans1)
		//	- math.RotateAndTranslateVertex(polygon1.GetCenter(), trans2);

		// Loop through all the vertices of both polygons and create the current edge off of them
		for (uint32_t vertexIndex = 0; vertexIndex < polygon1.VerticesCount() + polygon2.VerticesCount(); ++vertexIndex)
		{
			vec2 currentVertex;
			if (vertexIndex < polygon1.VerticesCount())
			{
				uint32_t nextVertexIndex = (vertexIndex + 1) % polygon1.VerticesCount();
				edge = vertices1[nextVertexIndex] - vertices1[vertexIndex];
				currentVertex = vertices1[vertexIndex];
			}
			else
			{
				uint32_t actualIndex = vertexIndex - polygon1.VerticesCount();
				uint32_t nextVertexIndex = (actualIndex + 1) % polygon2.VerticesCount();
				edge = vertices2[nextVertexIndex] - vertices2[actualIndex];
				currentVertex = vertices2[actualIndex];
			}

			// ===== Find if the polygons are currently intersecting =====

			// Find the axis perpendicular to the current edge, i went right because the polygons are CCW
			vec2 axis = math.RightHandNormal(edge);
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
				//if (dot(normalToUse, fromPoly2ToPoly1) < 0)
				//{
				//	needToFlipNormal = true;
				//}
			}
		}

		if (isIntersecting)
		{
			manifold.Penetration = minIntervalDistance;
			manifold.Normal = needToFlipNormal ? -normalToUse : normalToUse;

			//// generate contact points //////

			// get the best edges for both polygons
			float32_t poly1Penetration;
			float32_t poly2Penetration;
			P2Edge poly1BestEdge;
			P2Edge poly2BestEdge;
			GetBestEdge(vertices1, normalToUse, poly1BestEdge, poly1Penetration);
			GetBestEdge(vertices2, -normalToUse, poly2BestEdge, poly2Penetration);

			// clipping
			P2Edge referenceEdge; 
			P2Edge incidentEdge;

			bool flip = false;
			if (abs(dot(poly1BestEdge.ResultEdge(), normalToUse)) <= abs(dot(poly2BestEdge.ResultEdge(), normalToUse)))
			{
				referenceEdge = poly1BestEdge;
				incidentEdge = poly2BestEdge;
			}
			else
			{
				referenceEdge = poly2BestEdge;
				incidentEdge = poly1BestEdge;
				// we need to set a flag indicating that the reference
				// and incident edge were flipped so that when we do the final
				// clip operation, we use the right edge normal
				flip = true;
			}

			vec2 refVect = referenceEdge.NormalizedResultEdge();

			float32_t overlap1 = dot(refVect, referenceEdge.CurrentVertex);

			// clip the incident edge by the first
			// vertex of the reference edge
			manifold.ContactPoints = Clip(incidentEdge.CurrentVertex, incidentEdge.NextVertex, refVect, overlap1);

			if (manifold.ContactPoints.size() < 2)
			{
				return isIntersecting;
			}

			// clip whats left of the incident edge by the
			// second vertex of the reference edge
			// but we need to clip in the opposite direction
			// so we flip the direction and offset
			float32_t overlap2 = dot(refVect, referenceEdge.NextVertex);
			manifold.ContactPoints = Clip(manifold.ContactPoints.front(), manifold.ContactPoints.back(), -refVect, -overlap2);

			if (manifold.ContactPoints.size() < 2)
			{
				return isIntersecting;
			}

			auto refVectNormal = math.RightHandNormal(refVect);
			// if we had to flip the incident and reference edges
			// then we need to flip the reference edge normal to
			// clip properly
			if (flip)
			{
				refVectNormal = -refVectNormal;
			}

			// get the largest depth
			float32_t max = dot(refVectNormal, referenceEdge.MaxVertex);
			// make sure the final points are not past this maximum
			if (dot(refVectNormal, manifold.ContactPoints.front()) - max < 0)
			{
				manifold.ContactPoints.erase(manifold.ContactPoints.begin());
			}

			if (dot(refVectNormal, manifold.ContactPoints.back()) - max < 0)
			{
				manifold.ContactPoints.pop_back();
			}
		}

		return isIntersecting;
	}

	/***********************************************************************************************/
	bool P2Collision::CirclePolygonCollision(const P2CircleShape& circle, const P2Transform& circleTrans,
											 const P2PolygonShape& polygon, const P2Transform& polygonTrans, P2Manifold& manifold) const
	{
		auto& math = MathHelper::GetInstance();

		// rotate and translate the shapes
		vec2 circleCenter = math.RotateAndTranslateVertex(circle.GetCenter(), circleTrans);
		vector<vec2> polygonVertices = polygon.GetRotatedAndTranslatedVertices(polygonTrans);

		vec2 fromCircleToPolygon = math.RotateAndTranslateVertex(polygon.GetCenter(), polygonTrans) - circleCenter;

		// init variables
		vec2 vertex = polygonVertices.back();

		float32_t nearestDistanceSqr = math.MaxFloat();
		int32_t nearestVertex = -1;

		// detect the nearest vertex
		for (uint32_t i = 0; i < polygon.VerticesCount(); i++)
		{
			vec2 axis = circleCenter - polygonVertices[i];
			float32_t distanceSqr = math.LengthSquared(axis) - circle.GetRadiusSqr();

			// collides with the vertex
			if (distanceSqr <= 0)
			{
				float32_t distance = length(axis);
				if (distance != 0)
				{
					manifold.Penetration = circle.GetRadius() - distance;
					manifold.Normal = axis / distance;
					//if (dot(manifold.Normal, fromCircleToPolygon) < 0)
					//{
					//	manifold.Normal = -manifold.Normal;
					//}
					manifold.ContactPoints.push_back(polygonVertices[i]);
				}
				else
				{
					manifold = P2Manifold(circle.GetRadius());
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
			float32_t edgeLengthSquared = math.LengthSquared(edge);

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
				if (math.LengthSquared(axis) <= circle.GetRadiusSqr())
				{
					float32_t distance = length(axis);
					if (distance != 0)
					{
						manifold.Penetration = circle.GetRadius() - distance;
						manifold.Normal = axis / distance;
						//if (dot(manifold.Normal, fromCircleToPolygon) < 0)
						//{
						//	manifold.Normal = -manifold.Normal;
						//}
						manifold.ContactPoints.push_back(projection);
					}
					else
					{
						manifold = P2Manifold(circle.GetRadius());
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
	P2Collision::ProjectionInterval P2Collision::ProjectPolygonOnAxis(const vector<vec2>& vertices, const vec2& axis)
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

	/***********************************************************************************************/
	void P2Collision::GetBestEdge(const vector<vec2>& vertices, const vec2& normal, P2Edge& edge, float32_t& penetration)
	{
		//auto& math = MathHelper::GetInstance();
		float32_t max = dot(normal, vertices[0]);
		uint32_t index = 0;

		// find the farthest vertex in
		// the polygon along the separation normal
		for (uint32_t i = 1; i < vertices.size(); ++i)
		{
			float32_t projection = dot(normal, vertices[i]);
			if (projection > max)
			{
				max = projection;
				index = i;
			}
		}

		penetration = max;

		// now we need to use the edge that
		// is most perpendicular, either the
		// right or the left
		vec2 v = vertices[index];
		uint32_t nextIndex = index + 1 < vertices.size() ? index + 1 : 0;
		uint32_t prevIndex = index == 0 ? vertices.size() - 1 : index - 1;

		vec2 next = vertices[nextIndex];
		vec2 prev = vertices[prevIndex];

		vec2 left = normalize(v - next);
		vec2 right = normalize(v - prev);

		// the edge that is most perpendicular
		// to normal will have a dot product closer to zero
		if ( abs(dot(right, normal)) <= abs(dot(left, normal)))
		{
			// the right edge is better
			// make sure to retain the winding direction : max vert
			edge = P2Edge(prev, v, v);
		}
		else
		{
			// the left edge is better
			// make sure to retain the winding direction : max vert
			edge = P2Edge(v, next, v);
		}
	}

	/***********************************************************************************************/
	vector<vec2> P2Collision::Clip(const vec2& v1, const vec2& v2, const vec2& normal, const float32_t overlap)
	{
		// clips the line segment points v1, v2
		// if they are past overlap along normal
		vector<vec2> clippingPoints;

		float32_t d1 = dot(v1, normal) - overlap;
		float32_t d2 = dot(v2, normal) - overlap;

		// if either point is past overlap along normal
		// then we can keep the point

		if (d1 >= 0)
		{
			clippingPoints.push_back(v1);
		}

		if (d2 >= 0)
		{
			clippingPoints.push_back(v2);
		}

		// finally we need to check if they
		// are on opposing sides so that we can
		// compute the correct point
		if (d1 * d2 < 0)
		{
			// get the vector for the edge we are clipping
			vec2 edge = v2 - v1;

			// compute the location along edge: projection
			float32_t u = d1 / (d1 - d2);
			vec2 projection = (edge * u) + v1;

			// add the point
			clippingPoints.push_back(projection);
		}

		assert(clippingPoints.size() < 3);

		return move(clippingPoints);
	}
}
