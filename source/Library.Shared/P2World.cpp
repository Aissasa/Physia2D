#include "pch.h"
#include "P2World.h"
#include "P2Shape.h"
#include "P2BroadPhase.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/*******************************************************/
	P2World::P2World(const vec2& gravity) :
		mBodies(), mGravity(gravity)
	{
	}

	/*******************************************************/
	shared_ptr<P2Body> P2World::CreateBody(const P2BodyConfig& bodyConfig)
	{
		shared_ptr<P2Body> body = make_shared<P2Body>(bodyConfig);
		body->mWorld = this;
		mBodies.push_back(body);

		return body;
	}

	/*******************************************************/
	bool P2World::AddBody(const shared_ptr<P2Body>& body)
	{
		if (body == nullptr)
		{
			return false;
		}

		body->SetWorld(*this);
		mBodies.push_back(body);
		return true;
	}

	/*******************************************************/
	bool P2World::DeleteBody(const shared_ptr<P2Body>& body)
	{
		for (vector<shared_ptr<P2Body>>::iterator it = mBodies.begin(); it != mBodies.end(); ++it)
		{
			if (*it == body)
			{
				mBodies.erase(it);
				return true;
			}
		}

		return false;
	}

	/*******************************************************/
	void P2World::Update(const float32_t elapsedTime)
	{
		assert(elapsedTime > 0);

		auto pairs = P2BroadPhase::GetInstance().GetPotentiallyCollidingPairs(mBodies);
		P2Collision::GetInstance().ResolveCollisions(pairs, elapsedTime);

		for (auto& body : mBodies)
		{
			body->Update(elapsedTime);
		}
	}

	/*******************************************************/
	const vector<shared_ptr<P2Body>>& P2World::GetBodies() const
	{
		return mBodies;
	}

	/*******************************************************/
	vec2 P2World::GetGravity() const
	{
		return mGravity;
	}

	/*******************************************************/
	void P2World::SetGravity(const vec2& gravity)
	{
		mGravity = gravity;
	}
}