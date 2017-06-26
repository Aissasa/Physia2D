#include "pch.h"
#include <SFML/Graphics.hpp>

#include "P2World.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "JsonParser.h"
#include "WorldRenderer.h"
#include "Stopwatch.h"


#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define WINDOW_NAME "Physia2D Testbed"
#define HOLLOW_SHAPES_ENABLED true
#define FPS 100.0f

#define CIRCLE_1_FILE "Files/Circle1.json"
#define CIRCLE_2_FILE "Files/Circle2.json"
#define CIRCLE_VS_FILE "Files/CircleVS.json"
#define POLYGON_1_FILE "Files/Polygon1.json"
#define POLYGON_2_FILE "Files/Polygon2.json"
#define POLYGON_VS_FILE "Files/PolygonVS.json"

using namespace std;
using namespace Physia2D;
using namespace sf;
using namespace glm;

/***************************************************************************************************************************/
enum class TestCase
{
	CircleVsCircle,
	PolygonVsPolygon,
	CircleVsPolygon
};

/***************************************************************************************************************************/
shared_ptr<P2World> InitWorld(const TestCase testCase = TestCase::CircleVsCircle)
{
	// setup the world
	shared_ptr<P2World> world = make_shared<P2World>();
	JsonParser& parser = JsonParser::GetInstance();
	string body1File;
	string body2File;

	switch (testCase)
	{
		case TestCase::CircleVsCircle:
		{
			body1File = CIRCLE_1_FILE;
			body2File = CIRCLE_2_FILE;
			break;
		}
		case TestCase::PolygonVsPolygon:
		{
			body1File = POLYGON_1_FILE;
			body2File = POLYGON_2_FILE;
			break;
		}
		case TestCase::CircleVsPolygon:
		{		
			body1File = POLYGON_VS_FILE;
			body2File = CIRCLE_VS_FILE;
			break;
		}
		default:;
	}

	auto body1 = parser.ParseBody(body1File);
	auto body2 = parser.ParseBody(body2File);
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

	auto world = InitWorld(TestCase::CircleVsCircle);

	ContextSettings settings = InitOpenGLSettings();

	// create the window
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME, Style::Default, settings);

	// show with inverted y
	View view = window.getDefaultView();
	view.setSize(SCREEN_WIDTH, -SCREEN_HEIGHT);
	window.setView(view);

	Stopwatch sw;
	const float32_t dt = 1.0f / FPS;
	float32_t timer = 0;

	while (window.isOpen())                    // run the program as long as the window is open
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		timer += sw.DelayFromLastFrameInSeconds();

		if (timer > dt)
		{
			world->Update(dt);
			timer -= dt;
		}

		window.clear(Color::Black);
		Testbed::WorldRenderer::GetInstance().RenderWorld(window, *world, HOLLOW_SHAPES_ENABLED);
		window.display();
	}

	return 0;
}