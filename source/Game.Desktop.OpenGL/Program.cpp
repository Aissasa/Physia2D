#include "pch.h"
#include <SFML/Graphics.hpp>

#include "P2World.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "JsonParser.h"
#include "WorldRenderer.h"


#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define WINDOW_NAME "Physia2D Testbed"
#define HOLLOW_SHAPES_ENABLED false

using namespace std;
using namespace Physia2D;
using namespace sf;

/***************************************************************************************************************************/
shared_ptr<P2World> InitWorld()
{
	// setup the world
	shared_ptr<P2World> world = make_shared<P2World>();
	JsonParser& parser = JsonParser::GetInstance();
	auto body1 = parser.ParseBody("Files/Polygon2.json");
	auto body2 = parser.ParseBody("Files/Circle1.json");
	world->AddBody(body1);
	world->AddBody(body2);

	return world;
}

/***************************************************************************************************************************/
ContextSettings InitOpenGLSettings()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	return settings;
}

/***************************************************************************************************************************/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	auto world = InitWorld();

	ContextSettings settings = InitOpenGLSettings();

	// create the window
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME, Style::Default, settings);

	View view = window.getDefaultView();
	view.setSize(SCREEN_WIDTH, -SCREEN_HEIGHT);
	window.setView(view);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(Color::Black);

		// render world
		Testbed::WorldRenderer::GetInstance().RenderWorld(window, *world, HOLLOW_SHAPES_ENABLED);

		// end the current frame
		window.display();
	}

	return 0;
}