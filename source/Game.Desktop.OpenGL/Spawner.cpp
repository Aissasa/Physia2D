#include "pch.h"
#include "Spawner.h"
#include "P2Body.h"
#include "P2Fixture.h"
#include "P2CircleShape.h"
#include "P2PolygonShape.h"

#include <glm.hpp>

using namespace std;
using namespace glm;
using namespace Physia2D;

namespace Testbed
{
	/**********************************************************/
	Spawner::Spawner()
	{
		InitMaterials();
		InitPolygonVertices();
	}

	/**********************************************************/
	shared_ptr<P2Body> Spawner::CreateRandomCircle(const vec2& position) const
	{
		auto& math = MathHelper::GetInstance();

		P2BodyConfig bodyConfig;
		bodyConfig.Type = P2BodyType::Dynamic;
		bodyConfig.Position = position;
		bodyConfig.Rotation = math.FromDegreesToRadians(static_cast<float32_t>(math.RangedRandomInt(360)));

		shared_ptr<P2Body> body = make_shared<P2Body>(bodyConfig);

		P2FixtureConfig fixtureConfig = GetRandomMaterial();

		unique_ptr<P2Shape> shape = make_unique<P2CircleShape>(vec2(0.f, 0.f), static_cast<float32_t>(math.RangedRandomInt(75, 30)));
		fixtureConfig.Shape = shape.get();
		shared_ptr<P2Fixture> fixture = body->CreateFixture(fixtureConfig);

		return body;
	}

	/**********************************************************/
	shared_ptr<P2Body> Spawner::CreateRandomCircle(const float32_t x, const float32_t y) const
	{
		vec2 position(x, y);
		return CreateRandomCircle(position);
	}

	/**********************************************************/
	shared_ptr<P2Body> Spawner::CreateRandomPolygon(const float32_t x, const float32_t y) const
	{
		vec2 position(x, y);
		return CreateRandomPolygon(position);
	}

	/**********************************************************/
	shared_ptr<P2Body> Spawner::CreateRandomPolygon(const vec2& position) const
	{
		auto& math = MathHelper::GetInstance();

		P2BodyConfig bodyConfig;
		bodyConfig.Type = P2BodyType::Dynamic;
		bodyConfig.Position = position;
		bodyConfig.Rotation = math.FromDegreesToRadians(static_cast<float32_t>(math.RangedRandomInt(360)));

		shared_ptr<P2Body> body = make_shared<P2Body>(bodyConfig);

		P2FixtureConfig fixtureConfig = GetRandomMaterial();

		unique_ptr<P2Shape> shape = make_unique<P2PolygonShape>();
		auto poly = GetRandomPolygon();
		shape->As<P2PolygonShape>()->SetVertices(poly);

		fixtureConfig.Shape = shape.get();
		shared_ptr<P2Fixture> fixture = body->CreateFixture(fixtureConfig);

		return body;
	}

	/**********************************************************/
	void Spawner::InitMaterials()
	{
		P2FixtureConfig material;

		// brick
		material.Bounciness = 0.3f;
		material.Density = 2.f;
		material.Friction = 0.6f;
		mMaterials.push_back(make_shared<P2FixtureConfig>(material));

		// glass
		material.Bounciness = 0.2f;
		material.Density = 2.6f;
		material.Friction = 0.3f;
		mMaterials.push_back(make_shared<P2FixtureConfig>(material));

		//// wood
		//material.Bounciness = 0.4f;
		//material.Density = 0.4f;
		//material.Friction = 0.6f;
		//mMaterials.push_back(make_shared<P2FixtureConfig>(material));

		//// steel
		//material.Bounciness = 0.1f;
		//material.Density = 7.8f;
		//material.Friction = 0.4f;
		//mMaterials.push_back(make_shared<P2FixtureConfig>(material));

		//// copper
		//material.Bounciness = 0.15f;
		//material.Density = 8.8f;
		//material.Friction = 0.3f;
		//mMaterials.push_back(make_shared<P2FixtureConfig>(material));

		// stone
		material.Bounciness = 0.1f;
		material.Density = 2.5f;
		material.Friction = 0.3f;
		mMaterials.push_back(make_shared<P2FixtureConfig>(material));

	}

	/**********************************************************/
	void Spawner::InitPolygonVertices()
	{
		vector<vec2> vertices;

		// hexagon
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(50.f, 0.f));
		vertices.push_back(vec2(75.f, 50.f));
		vertices.push_back(vec2(50.f, 100.f));
		vertices.push_back(vec2(0.f, 100.f));
		vertices.push_back(vec2(-25.f, 50.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// pentagon
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(100.f, 0.f));
		vertices.push_back(vec2(125.f, 50.f));
		vertices.push_back(vec2(100.f, 100.f));
		vertices.push_back(vec2(0.f, 100.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// rectangle
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(100.f, 0.f));
		vertices.push_back(vec2(100.f, 50.f));
		vertices.push_back(vec2(0.f, 50.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// square
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(50.f, 0.f));
		vertices.push_back(vec2(50.f, 50.f));
		vertices.push_back(vec2(0.f, 50.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// weird 1
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(100.f, 0.f));
		vertices.push_back(vec2(150.f, 50.f));
		vertices.push_back(vec2(100.f, 100.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// weird 2
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(120.f, 0.f));
		vertices.push_back(vec2(130.f, 50.f));
		vertices.push_back(vec2(80.f, 80.f));
		vertices.push_back(vec2(0.f, 80.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();

		// triangle
		vertices.push_back(vec2(0, 0));
		vertices.push_back(vec2(100.f, 0.f));
		vertices.push_back(vec2(0.f, 75.f));

		mPolygonSets.push_back(vertices);
		vertices.clear();
	}

	/**********************************************************/
	P2FixtureConfig Spawner::GetRandomMaterial() const
	{
		uint32_t randomIndex = MathHelper::GetInstance().RangedRandomInt(static_cast<int32_t>(mMaterials.size() - 1));
		assert(randomIndex < mMaterials.size());

		return *mMaterials[randomIndex];
	}

	/**********************************************************/
	vector<vec2> Spawner::GetRandomPolygon() const
	{
		uint32_t randomIndex = MathHelper::GetInstance().RangedRandomInt(static_cast<int32_t>(mPolygonSets.size() - 1));
		assert(randomIndex < mPolygonSets.size());

		return mPolygonSets[randomIndex];
	}
}
