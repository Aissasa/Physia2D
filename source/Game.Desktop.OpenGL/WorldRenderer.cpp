#include "pch.h"
#include "WorldRenderer.h"

#include <SFML/Graphics.hpp>

#include "P2World.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"
#include "JsonParser.h"
#include "P2Collision.h"


using namespace std;
using namespace Physia2D;
using namespace sf;
using namespace glm;

namespace Testbed
{
	const float WorldRenderer::OUTLINE_THICKNESS = 0.5f;

	/**********************************************************************************************/
	WorldRenderer::WorldRenderer() :
		mCircleColor(Color::Cyan),
		mCircleCollisionColor(Color::Blue),
		mPolygonColor(Color::Yellow),
		mPolygonCollisionColor(Color::Red)
	{
	}

	/**********************************************************************************************/
	WorldRenderer& WorldRenderer::GetInstance()
	{
		static WorldRenderer sInstance;
		return sInstance;
	}

	/**********************************************************************************************/
	void WorldRenderer::RenderWorld(RenderWindow& window, const P2World& world, bool hollowShapes) const
	{
		auto& bodies = world.GetBodies();

		for (uint32_t i = 0; i < bodies.size(); ++i)
		{
			bool colliding = CheckCollision(bodies, i);
			RenderBody(window, *bodies[i], colliding, hollowShapes);
		}
	}

	/**********************************************************************************************/
	void WorldRenderer::RenderBody(RenderWindow& window, const P2Body& body, bool colliding, bool hollowShapes) const
	{
		auto fixture = body.GetFixture();
		if (fixture != nullptr)
		{
			auto shape = fixture->GetShape();
			if (shape != nullptr)
			{
				switch (shape->GetType())
				{
					case P2Shape::EType::Circle:
					{
						DrawCircle(window, *shape->As<P2CircleShape>(), body.GetTransform(), colliding, hollowShapes);
						break;
					}

					case P2Shape::EType::Polygon:
					{
						DrawPolygon(window, *shape->As<P2PolygonShape>(), body.GetTransform(), colliding, hollowShapes);
						break;
					}

					default:;
				}
			}
		}
	}

	/**********************************************************************************************/
	void WorldRenderer::DrawCircle(RenderWindow& window, const P2CircleShape& circleShape,
								   const P2Transform& bodyTransform, bool colliding, bool hollowShape) const
	{
		CircleShape circle;
		circle.setPosition(bodyTransform.Position.x, bodyTransform.Position.y);
		circle.setRotation(MathHelper::GetInstance().FromRadiansToDegrees(bodyTransform.Rotation.GetRotation()));
		circle.setOrigin(circleShape.GetCenterPosition().x + circleShape.GetRadius(), circleShape.GetCenterPosition().y + circleShape.GetRadius());
		circle.setRadius(circleShape.GetRadius());

		Color colorToUse = colliding ? mCircleCollisionColor : mCircleColor;

		if (hollowShape)
		{
			circle.setFillColor(Color::Black);
			circle.setOutlineColor(colorToUse);
			circle.setOutlineThickness(OUTLINE_THICKNESS);
		}
		else
		{
			circle.setFillColor(colorToUse);
		}

		window.draw(circle);
	}

	/**********************************************************************************************/
	void WorldRenderer::DrawPolygon(RenderWindow& window, const P2PolygonShape& polygonShape,
									const P2Transform& bodyTransform, bool colliding, bool hollowShape) const
	{
		ConvexShape polygon;
		polygon.setPointCount(polygonShape.VerticesCount());

		polygon.setPosition(bodyTransform.Position.x, bodyTransform.Position.y);
		polygon.setRotation(MathHelper::GetInstance().FromRadiansToDegrees(bodyTransform.Rotation.GetRotation()));
		auto& vertices = polygonShape.GetVertices();
		//auto vertices = polygonShape.GetRotatedAndTranslatedVertices(bodyTransform);
		for (uint32_t i = 0; i < polygonShape.VerticesCount(); i++)
		{
			polygon.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));
		}

		Color colorToUse = colliding ? mPolygonCollisionColor : mPolygonColor;

		if (hollowShape)
		{
			polygon.setFillColor(Color::Black);
			polygon.setOutlineColor(colorToUse);
			polygon.setOutlineThickness(OUTLINE_THICKNESS);
		}
		else
		{
			polygon.setFillColor(colorToUse);
		}

		window.draw(polygon);
	}

	/**********************************************************************************************/
	bool WorldRenderer::CheckCollision(const vector<shared_ptr<P2Body>>& bodies, const uint32_t bodyIndex) const
	{
		assert(bodyIndex >= 0 && bodyIndex < bodies.size());
		auto body = bodies[bodyIndex];

		for (uint32_t i = 0; i < bodies.size(); ++i)
		{
			if (i == bodyIndex)
			{
				continue;
			}

			if (P2Collision::CheckCollision(*body, *bodies[i]))
			{
				return true;
			}
		}

		return false;
	}
}
