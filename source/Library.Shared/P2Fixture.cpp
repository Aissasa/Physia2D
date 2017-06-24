#include "pch.h"
#include "P2Fixture.h"
#include "P2Shape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/*******************************************************/
	P2Fixture::P2Fixture(const float32_t density, const float32_t bounciness, const float32_t friction, shared_ptr<P2Shape> shape) :
		mBody(nullptr), mShape(shape), mDensity(density), mBounciness(bounciness), mFriction(friction)
	{
	}

	/*******************************************************/
	void P2Fixture::Create(P2Body& body, const P2FixtureConfig& fixtureConfig)
	{
		mBody = &body;
		mShape = shared_ptr<P2Shape>(fixtureConfig.Shape->Clone());
		mDensity = fixtureConfig.Density;
		mBounciness = fixtureConfig.Bounciness;
		mFriction = fixtureConfig.Friction;
	}

	/*******************************************************/
	P2Body* P2Fixture::GetBody() const
	{
		return mBody;
	}

	/*******************************************************/
	shared_ptr<P2Shape> P2Fixture::GetShape() const
	{
		return mShape;
	}

	/*******************************************************/
	float32_t P2Fixture::GetDensity() const
	{
		return mDensity;
	}

	/*******************************************************/
	void P2Fixture::SetDensity(const float32_t density)
	{
		mDensity = density;
	}

	/*******************************************************/
	float32_t P2Fixture::GetBounciness() const
	{
		return mBounciness;
	}

	/*******************************************************/
	void P2Fixture::SetBounciness(const float32_t bounciness)
	{
		mBounciness = bounciness;
	}

	/*******************************************************/
	float32_t P2Fixture::GetFriction() const
	{
		return mFriction;
	}

	/*******************************************************/
	void P2Fixture::SetFriction(const float32_t friction)
	{
		mFriction = friction;
	}
}