#include "pch.h"
#include <SFML/Graphics.hpp>

sf::ContextSettings InitOpenGLSettings()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	return settings;
}

/***************************************************************************************************************************/
void DrawWorld(sf::RenderWindow& window)
{
	// create an empty shape
	sf::ConvexShape convex;

	convex.setPosition(50, 50);
	convex.setFillColor(sf::Color::Red);
	// resize it to 5 points
	convex.setPointCount(5);

	// define the points
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(150, 10));
	convex.setPoint(2, sf::Vector2f(120, 90));
	convex.setPoint(3, sf::Vector2f(30, 100));
	convex.setPoint(4, sf::Vector2f(0, 50));

	window.draw(convex);

	// define a triangle
	sf::CircleShape triangle(80, 3);
	triangle.setPosition(150, 150);
	triangle.setFillColor(sf::Color::Green);

	// define a square
	sf::CircleShape square(80, 4);
	square.setPosition(500, 500);
	square.setFillColor(sf::Color::Blue);

	// define an octagon
	sf::CircleShape octagon(80, 8);
	octagon.setPosition(250, 250);
	octagon.setFillColor(sf::Color::Cyan);

	window.draw(triangle);
	window.draw(square);
	window.draw(octagon);
}

/***************************************************************************************************************************/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);


	sf::ContextSettings settings = InitOpenGLSettings();

	// create the window
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Physia2D testbed", sf::Style::Default, settings);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		DrawWorld(window);

		// end the current frame
		window.display();
	}

	return 0;
}