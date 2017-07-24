#include "pch.h"
#include <SFML/Graphics.hpp>

#include "P2World.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "JsonParser.h"
#include "WorldRenderer.h"
#include "Stopwatch.h"
#include "Spawner.h"


#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define WINDOW_NAME "Physia2D Testbed"
#define HOLLOW_SHAPES_ENABLED true
#define FPS 60

#define CIRCLE_1_FILE "Files/Circle1.json"
#define CIRCLE_2_FILE "Files/Circle2.json"
#define CIRCLE_VS_FILE "Files/CircleVS.json"
#define POLYGON_1_FILE "Files/Polygon1.json"
#define POLYGON_2_FILE "Files/Polygon2.json"
#define POLYGON_VS_FILE "Files/PolygonVS.json"
#define POLYGON_STATIC_FILE "Files/PolygonStatic.json"
#define CIRCLE_STATIC_FILE "Files/CircleStatic.json"

#define CIRCLE_BRAWL_1_FILE "Files/CircleBrawl1.json"
#define CIRCLE_BRAWL_2_FILE "Files/CircleBrawl2.json"
#define CIRCLE_BRAWL_3_FILE "Files/CircleBrawl3.json"
#define CIRCLE_BRAWL_4_FILE "Files/CircleBrawl4.json"
#define POLYGON_BRAWL_1_FILE "Files/PolygonBrawl1.json"
#define POLYGON_BRAWL_2_FILE "Files/PolygonBrawl2.json"
#define POLYGON_BRAWL_3_FILE "Files/PolygonBrawl3.json"
#define POLYGON_BRAWL_4_FILE "Files/PolygonBrawl4.json"

using namespace std;
using namespace Physia2D;
using namespace sf;
using namespace glm;
using namespace Testbed;

/***************************************************************************************************************************/
enum class TestCase
{
	CircleVsCircle,
	PolygonVsPolygon,
	CircleVsPolygon,
	Brawl,
	Empty
};

/***************************************************************************************************************************/
shared_ptr<P2World> InitWorld(const TestCase testCase = TestCase::CircleVsCircle)
{
	// setup the world
	shared_ptr<P2World> world = make_shared<P2World>();
	JsonParser& parser = JsonParser::GetInstance();

	string polyStaticFile = POLYGON_STATIC_FILE;
	auto polygonStatic = parser.ParseBody(polyStaticFile);
	world->AddBody(polygonStatic);

	string circleStaticFile = CIRCLE_STATIC_FILE;
	auto circleStatic = parser.ParseBody(circleStaticFile);
	world->AddBody(circleStatic);

	string body1File;
	string body2File;

	switch (testCase)
	{
		case TestCase::CircleVsCircle:
		{
			body1File = CIRCLE_1_FILE;
			body2File = CIRCLE_2_FILE;
			auto body1 = parser.ParseBody(body1File);
			auto body2 = parser.ParseBody(body2File);
			world->AddBody(body1);
			world->AddBody(body2);

			break;
		}
		case TestCase::PolygonVsPolygon:
		{
			body1File = POLYGON_1_FILE;
			body2File = POLYGON_2_FILE;
			auto body1 = parser.ParseBody(body1File);
			auto body2 = parser.ParseBody(body2File);
			world->AddBody(body1);
			world->AddBody(body2);

			break;
		}
		case TestCase::CircleVsPolygon:
		{
			body1File = POLYGON_VS_FILE;
			body2File = CIRCLE_VS_FILE;
			auto body1 = parser.ParseBody(body1File);
			auto body2 = parser.ParseBody(body2File);
			world->AddBody(body1);
			world->AddBody(body2);

			break;
		}
		case TestCase::Brawl:
		{
			body1File = CIRCLE_BRAWL_1_FILE;
			body2File = CIRCLE_BRAWL_2_FILE;
			string body3File = CIRCLE_BRAWL_3_FILE;
			string body7File = CIRCLE_BRAWL_4_FILE;
			string body4File = POLYGON_BRAWL_1_FILE;
			string body5File = POLYGON_BRAWL_2_FILE;
			string body6File = POLYGON_BRAWL_3_FILE;
			string body8File = POLYGON_BRAWL_4_FILE;

			auto body1 = parser.ParseBody(body1File);
			auto body2 = parser.ParseBody(body2File);
			auto body3 = parser.ParseBody(body3File);
			auto body4 = parser.ParseBody(body4File);
			auto body5 = parser.ParseBody(body5File);
			auto body6 = parser.ParseBody(body6File);
			auto body7 = parser.ParseBody(body7File);
			auto body8 = parser.ParseBody(body8File);
			world->AddBody(body1);
			world->AddBody(body2);
			world->AddBody(body3);
			world->AddBody(body4);
			world->AddBody(body5);
			world->AddBody(body6);
			world->AddBody(body7);
			world->AddBody(body8);

			break;
		}
		default:;
	}

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

	auto world = InitWorld(TestCase::Brawl);

	ContextSettings settings = InitOpenGLSettings();

	// create the window
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME, Style::Default, settings);

	// show with inverted y
	View view = window.getDefaultView();
	view.setSize(SCREEN_WIDTH, -SCREEN_HEIGHT);
	window.setView(view);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);

	Spawner sp;
	Stopwatch sw;
	const float32_t dt = 1.0f / FPS;
	float32_t timer = 0;

	bool leftButtonReleased = true;
	bool rightButtonReleased = true;
	bool middleButtonReleased = true;

	while (window.isOpen())                    // run the program as long as the window is open
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			// check if the mouse buttons are released to avoid multiple spawns
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left)
				leftButtonReleased = true;

			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right)
				rightButtonReleased = true;

			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Middle)
				middleButtonReleased = true;

		}

		// handle input
		if (middleButtonReleased && Mouse::isButtonPressed(Mouse::Middle))
		{
			middleButtonReleased = false;
			world->Clear();
			world = InitWorld(TestCase::Empty);
		}

		if (leftButtonReleased && Mouse::isButtonPressed(Mouse::Left))
		{
			leftButtonReleased = false;
			auto position = Mouse::getPosition(window);
			world->AddBody(sp.CreateRandomPolygon(static_cast<float32_t>(position.x), SCREEN_HEIGHT - static_cast<float32_t>(position.y)));
		}

		if (rightButtonReleased && Mouse::isButtonPressed(Mouse::Right))
		{
			rightButtonReleased = false;
			auto position = Mouse::getPosition(window);
			world->AddBody(sp.CreateRandomCircle(static_cast<float32_t>(position.x), SCREEN_HEIGHT - static_cast<float32_t>(position.y)));
		}

		timer += sw.DelayFromLastFrameInSeconds();

		if (timer > dt)
		{
			world->Update(dt);
			timer -= dt;
		}

		window.clear(Color::Black);
		WorldRenderer::GetInstance().RenderWorld(window, *world, HOLLOW_SHAPES_ENABLED);
		window.display();
	}

	return 0;
}