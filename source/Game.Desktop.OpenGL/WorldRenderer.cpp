#include "pch.h"
#include "WorldRenderer.h"

#include <SFML/Graphics.hpp>

#include "P2World.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"
#include "JsonParser.h"


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
		for (auto& body : bodies)
		{
			RenderBody(window, *body, hollowShapes);
		}
	}

	/**********************************************************************************************/
	void WorldRenderer::RenderBody(RenderWindow& window, const P2Body& body, bool hollowShapes) const
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
						DrawCircle(window, *shape->As<P2CircleShape>(), body.GetTransform(), hollowShapes);
						break;
					}

					case P2Shape::EType::Polygon:
					{
						DrawPolygon(window, *shape->As<P2PolygonShape>(), body.GetTransform(), hollowShapes);
						break;
					}

					default:;
				}
			}
		}
	}

	/**********************************************************************************************/
	void WorldRenderer::DrawCircle(RenderWindow& window, const P2CircleShape& circleShape,
								   const P2Transform& bodyTransform, bool hollowShape) const
	{
		CircleShape circle;
		circle.setPosition(bodyTransform.Position.x, bodyTransform.Position.y);
		circle.setRotation(MathHelper::GetInstance().FromRadiansToDegrees(bodyTransform.Rotation.GetRotation()));
		circle.setOrigin(circleShape.GetCenterPosition().x, circleShape.GetCenterPosition().y);
		circle.setRadius(circleShape.GetRadius());

		if (hollowShape)
		{
			circle.setFillColor(Color::Black);
			circle.setOutlineColor(mCircleColor);
			circle.setOutlineThickness(OUTLINE_THICKNESS);
		}
		else
		{
			circle.setFillColor(mCircleColor);
		}

		window.draw(circle);
	}

	/**********************************************************************************************/
	void WorldRenderer::DrawPolygon(RenderWindow& window, const P2PolygonShape& polygonShape,
									const P2Transform& bodyTransform, bool hollowShape) const
	{
		ConvexShape polygon;
		polygon.setPosition(bodyTransform.Position.x, bodyTransform.Position.y);
		//polygon.setRotation(MathHelper::GetInstance().FromRadiansToDegrees(bodyTransform.Rotation.GetRotation()));
		polygon.setPointCount(polygonShape.VerticesCount());
		//auto& vertices = polygonShape.GetVertices();
		auto vertices = polygonShape.GetRotatedAndTranslatedVertices(bodyTransform);
		for (uint32_t i = 0; i < polygonShape.VerticesCount(); i++)
		{
			polygon.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));
		}

		if (hollowShape)
		{
			polygon.setFillColor(Color::Black);
			polygon.setOutlineColor(mPolygonColor);
			polygon.setOutlineThickness(OUTLINE_THICKNESS);
		}
		else
		{
			polygon.setFillColor(mPolygonColor);
		}

		window.draw(polygon);
	}
}
