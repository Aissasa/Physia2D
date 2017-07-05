#include "pch.h"
#include "JsonParser.h"
#include <istreamwrapper.h>
#include "P2World.h"
#include "P2Body.h"
#include "P2Fixture.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"

using namespace std;
using namespace glm;
using namespace rapidjson;

namespace Physia2D
{
	/*******************************************************/
	JsonParser& JsonParser::GetInstance()
	{
		static JsonParser sInstance;

		return sInstance;
	}

	/*******************************************************/
	shared_ptr<P2Body> JsonParser::ParseBody(const string& filePath) const
	{
		ifstream ifs(filePath);
		IStreamWrapper ist(ifs);

		Document jsonDoc;
		jsonDoc.ParseStream(ist);
		assert(jsonDoc.IsObject());

		return ParseBody(jsonDoc);
	}

	/*******************************************************/
	shared_ptr<P2Body> JsonParser::ParseBody(const Value& doc) const
	{
		P2BodyConfig bodyConfig;
		bodyConfig.Position.x = doc["Position"]["x"].GetFloat();
		bodyConfig.Position.y = doc["Position"]["y"].GetFloat();
		bodyConfig.Rotation = doc["Rotation"].GetFloat();
		bodyConfig.LinearVelocity.x = doc["LinearVel"]["x"].GetFloat();
		bodyConfig.LinearVelocity.y = doc["LinearVel"]["y"].GetFloat();
		bodyConfig.AngularVelocity = doc["AngularVel"].GetFloat();
		bodyConfig.GravityScale = doc["GravityScale"].GetFloat();

		shared_ptr<P2Body> body = make_shared<P2Body>(bodyConfig);

		const Value& fixtureValue = doc["Fixture"];
		const Value& shapeValue = fixtureValue["Shape"];

		P2FixtureConfig fixtureConfig;
		fixtureConfig.Density = fixtureValue["Density"].GetFloat();
		fixtureConfig.Bounciness = fixtureValue["Bounciness"].GetFloat();
		fixtureConfig.Friction = fixtureValue["Friction"].GetFloat();

		unique_ptr<P2Shape> shape;

		switch (shapeValue["Type"].GetInt())
		{
			// polygon
			case 2:
			{
				shape = make_unique<P2PolygonShape>();

				const Value& vertsValue = shapeValue["Vertices"];
				vector<vec2> vertices;

				for (auto& value : vertsValue.GetArray())
				{
					vertices.push_back(vec2(value["x"].GetFloat(), value["y"].GetFloat()));
				}

				shape->As<P2PolygonShape>()->SetVertices(vertices);
				break;
			}

			case 1:
			default:
			{
				shape = make_unique<P2CircleShape>(vec2(shapeValue["Position"]["x"].GetFloat(), shapeValue["Position"]["y"].GetFloat()), shapeValue["Radius"].GetFloat());
				break;
			}
		}

		fixtureConfig.Shape = shape.get();
		shared_ptr<P2Fixture> fixture = body->CreateFixture(fixtureConfig);

		return body;
	}
}
