#include "pch.h"
#include "P2World.h"
#include "P2Shape.h"

using namespace std;
using namespace glm;

/*******************************************************/
P2World::P2World(const vec2& gravity): 
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
void P2World::DeleteBody(shared_ptr<P2Body> body)
{
	vector<shared_ptr<P2Body>>::iterator it;
	for (it = mBodies.begin(); it != mBodies.end(); ++it)
	{
		if (*it == body)
		{
			mBodies.erase(it);
			break;
		}
	}
}

/*******************************************************/
vector<shared_ptr<P2Body>> P2World::GetBodies() const
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
