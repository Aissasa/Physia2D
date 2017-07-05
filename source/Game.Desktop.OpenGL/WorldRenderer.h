#pragma once

#include <SFML/Graphics/Color.hpp>
#include "P2Body.h"

// forward declarations
namespace Physia2D
{
	struct P2Transform;
	class P2PolygonShape;
	class P2CircleShape;
	class P2Body;
	class P2World;
}

namespace sf
{
	class RenderWindow;
}

namespace Testbed
{
	/** Singleton that handles rendering a world and bodies on the screen
	*/
	class WorldRenderer final
	{
	public:

		WorldRenderer(const WorldRenderer&) = delete;
		WorldRenderer(WorldRenderer&&) = delete;
		WorldRenderer& operator=(const WorldRenderer&) = delete;
		WorldRenderer& operator=(WorldRenderer&&) = delete;

		static WorldRenderer& GetInstance();

		void RenderWorld(sf::RenderWindow& window, const Physia2D::P2World& world, const bool hollowShapes = true) const;
		void RenderBody(sf::RenderWindow& window, const Physia2D::P2Body& body, 
						const bool colliding = false, const bool hollowShapes = true) const;

	private:

		WorldRenderer();
		~WorldRenderer() = default;

		void DrawCircle(sf::RenderWindow& window, const Physia2D::P2CircleShape& circleShape,
						const Physia2D::P2Transform& bodyTransform, const bool colliding = false, const bool hollowShape = true) const;
		void DrawPolygon(sf::RenderWindow& window, const Physia2D::P2PolygonShape& polygonShape,
						 const Physia2D::P2Transform& bodyTransform, const bool colliding = false, const bool hollowShape = true) const;

		static const float OUTLINE_THICKNESS;

		sf::Color mCircleColor;
		sf::Color mCircleCollisionColor;
		sf::Color mPolygonColor;
		sf::Color mPolygonCollisionColor;
	};
}
