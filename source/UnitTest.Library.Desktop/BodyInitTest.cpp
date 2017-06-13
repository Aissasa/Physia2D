#include "pch.h"
#include "CppUnitTest.h"
#include "document.h"
#include "istreamwrapper.h"
#include "P2World.h"
#include "P2CircleShape.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace glm;
using namespace rapidjson;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(BodyInitTest)
	{
	public:

#pragma region Memory leak check
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#pragma endregion

#pragma region Test methods

		/*******************************************************/
		TEST_METHOD(InitWithoutJson)
		{
			vec2 gravity(0, -9.8f);
			P2World world(gravity);

			P2BodyConfig bodyConfig;
			bodyConfig.Position = vec2(0, 5.0f);
			bodyConfig.Rotation = 0.6f;
			bodyConfig.LinearVelocity = vec2(5, 9);
			bodyConfig.AngularVelocity = 0.6f;
			bodyConfig.GravityScale = 1;

			shared_ptr<P2Body> body = world.CreateBody(bodyConfig);

			Assert::AreEqual(1U, world.GetBodies().size());
			Assert::IsTrue(body->GetFixture() == nullptr);
			Assert::IsTrue(body->GetWorld() == &world);
			Assert::IsTrue(bodyConfig.Position == body->GetTransform().Position);
			Assert::AreEqual(bodyConfig.Rotation, body->GetTransform().Rotation);
			Assert::IsTrue(bodyConfig.LinearVelocity == body->GetLinearVelocity());
			Assert::AreEqual(bodyConfig.AngularVelocity, body->GetAngularVelocity());
			Assert::AreEqual(bodyConfig.GravityScale, body->GetGravityScale());

			P2CircleShape circleShape(body->GetTransform().Position, 2);
			Assert::IsTrue(P2Shape::EType::Circle == circleShape.GetType());

			P2FixtureConfig fixtureConfig;
			fixtureConfig.Bounciness = 0.5f;
			fixtureConfig.Density = 0.3f;
			fixtureConfig.Friction = 2;
			fixtureConfig.Shape = &circleShape;

			shared_ptr<P2Fixture> fixture = body->CreateFixture(fixtureConfig);
			Assert::IsTrue(fixture == body->GetFixture());
			Assert::AreEqual(fixtureConfig.Bounciness, body->GetFixture()->GetBounciness());
			Assert::AreEqual(fixtureConfig.Density, body->GetFixture()->GetDensity());
			Assert::AreEqual(fixtureConfig.Friction, body->GetFixture()->GetFriction());
			Assert::IsTrue(body->GetFixture()->GetShape()->GetType() == P2Shape::EType::Circle);
		}

		/*******************************************************/
		TEST_METHOD(InitWithJson)
		{
			vec2 gravity(0, -9.8f);
			P2World world(gravity);

			ifstream ifs("Files/Test.json");
			IStreamWrapper ist(ifs);

			Document jsonDoc;
			jsonDoc.ParseStream(ist);
			Assert::IsTrue(jsonDoc.IsObject());

			P2BodyConfig bodyConfig;
			bodyConfig.Position.x = jsonDoc["Position"]["x"].GetFloat();
			bodyConfig.Position.y = jsonDoc["Position"]["y"].GetFloat();
			bodyConfig.Rotation = jsonDoc["Rotation"].GetFloat();
			bodyConfig.LinearVelocity.x = jsonDoc["LinearVel"]["x"].GetFloat();
			bodyConfig.LinearVelocity.y = jsonDoc["LinearVel"]["y"].GetFloat();
			bodyConfig.AngularVelocity = jsonDoc["AngularVel"].GetFloat();
			bodyConfig.GravityScale = jsonDoc["GravityScale"].GetFloat();

			Assert::IsTrue(bodyConfig.Position == vec2(0, 0.8f));
			Assert::AreEqual(0.2f, bodyConfig.Rotation);
			Assert::IsTrue(bodyConfig.LinearVelocity == vec2(2, 8));
			Assert::AreEqual(0.5f, bodyConfig.AngularVelocity);
			Assert::AreEqual(1.0f, bodyConfig.GravityScale);

			shared_ptr<P2Body> body = world.CreateBody(bodyConfig);

			Assert::AreEqual(1U, world.GetBodies().size());
			Assert::IsTrue(body->GetFixture() == nullptr);
			Assert::IsTrue(body->GetWorld() == &world);
			Assert::IsTrue(bodyConfig.Position == body->GetTransform().Position);
			Assert::AreEqual(bodyConfig.Rotation, body->GetTransform().Rotation);
			Assert::IsTrue(bodyConfig.LinearVelocity == body->GetLinearVelocity());
			Assert::AreEqual(bodyConfig.AngularVelocity, body->GetAngularVelocity());
			Assert::AreEqual(bodyConfig.GravityScale, body->GetGravityScale());

			const Value& fixtureValue = jsonDoc["Fixture"];
			const Value& shapeValue = fixtureValue["Shape"];

			P2CircleShape circleShape(vec2(shapeValue["Position"]["x"].GetFloat(), shapeValue["Position"]["y"].GetFloat()), shapeValue["Radius"].GetFloat());
			Assert::AreEqual(3.0f, circleShape.GetRadius());
			Assert::IsTrue(vec2(0.2f, 0.3f) == circleShape.GetPosition());

			P2FixtureConfig fixtureConfig;
			fixtureConfig.Density = fixtureValue["Density"].GetFloat();
			fixtureConfig.Bounciness = fixtureValue["Bounciness"].GetFloat();
			fixtureConfig.Friction = fixtureValue["Friction"].GetFloat();
			fixtureConfig.Shape = &circleShape;

			Assert::AreEqual(0.9f, fixtureConfig.Density);
			Assert::AreEqual(0.2f, fixtureConfig.Bounciness);
			Assert::AreEqual(1.0f, fixtureConfig.Friction);

			shared_ptr<P2Fixture> fixture = body->CreateFixture(fixtureConfig);
			Assert::IsTrue(fixture == body->GetFixture());
			Assert::AreEqual(fixtureConfig.Bounciness, body->GetFixture()->GetBounciness());
			Assert::AreEqual(fixtureConfig.Density, body->GetFixture()->GetDensity());
			Assert::AreEqual(fixtureConfig.Friction, body->GetFixture()->GetFriction());
			Assert::IsTrue(body->GetFixture()->GetShape()->GetType() == P2Shape::EType::Circle);
		}

		/*******************************************************/
		TEST_METHOD(InitWithJsonParser)
		{
			JsonParser& parser = JsonParser::GetInstance();
			auto body = parser.ParseBody("Files/Test.json");

			Assert::IsTrue(body->GetWorld() == nullptr);
			Assert::IsTrue(vec2(0, 0.8f) == body->GetTransform().Position);
			Assert::AreEqual(0.2f, body->GetTransform().Rotation);
			Assert::IsTrue(vec2(2, 8) == body->GetLinearVelocity());
			Assert::AreEqual(0.5f, body->GetAngularVelocity());
			Assert::AreEqual(1.0f, body->GetGravityScale());

			Assert::IsTrue(body->GetFixture() != nullptr);
			Assert::AreEqual(0.9f, body->GetFixture()->GetDensity());
			Assert::AreEqual(0.2f, body->GetFixture()->GetBounciness());
			Assert::AreEqual(1.0f, body->GetFixture()->GetFriction());
			Assert::IsTrue(body->GetFixture()->GetShape()->GetType() == P2Shape::EType::Circle);

			JsonParser::GetInstance().DeleteParser();
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState BodyInitTest::sStartMemState;
}