#include "pch.h"
#include "P2Body.h"
#include "P2Fixture.h"
#include "P2Shape.h"

using namespace glm;

/*******************************************************/
P2Body::P2Body(): 
	mAngularVelocity(0), mTorque(0), mMass(1), mInvMass(1), mInertia(1), mInvInertia(1), mGravityScale(0)
{
}

/*******************************************************/
void P2Body::ResetMassData()
{
	if (mFixture && mFixture->GetShape())
	{
		P2MassData massData = mFixture->GetShape()->ComputeMass(mFixture->GetDensity());
		mMass = massData.Mass;
		mInvMass = 1 / mMass;
		mInertia = massData.Inertia;
		mInvInertia = 1 / mInertia;
	}
}

/*******************************************************/
P2Transform P2Body::GetTransform() const
{
	return mTransform;
}

/*******************************************************/
void P2Body::SetTransform(const P2Transform& transform)
{
	mTransform = transform;
}

/*******************************************************/
vec2 P2Body::GetLinearVelocity() const
{
	return mLinearVelocity;
}

/*******************************************************/
void P2Body::SetLinearVelocity(const vec2& linearVelocity)
{
	mLinearVelocity = linearVelocity;
}

/*******************************************************/
float32_t P2Body::GetAngularVelocity() const
{
	return mAngularVelocity;
}

/*******************************************************/
void P2Body::SetAngularVelocity(const float32_t angularVelocity)
{
	mAngularVelocity = angularVelocity;
}

/*******************************************************/
vec2 P2Body::GetForce() const
{
	return mForce;
}

/*******************************************************/
void P2Body::SetForce(const vec2& force)
{
	mForce = force;
}

/*******************************************************/
float32_t P2Body::GetTorque() const
{
	return mTorque;
}

/*******************************************************/
void P2Body::SetTorque(const float32_t torque)
{
	mTorque = torque;
}

/*******************************************************/
std::shared_ptr<P2Fixture> P2Body::GetFixture() const
{
	return mFixture;
}

/*******************************************************/
void P2Body::SetFixture(const std::shared_ptr<P2Fixture>& p2Fixture)
{
	mFixture = p2Fixture;
}

/*******************************************************/
float32_t P2Body::GetGravityScale() const
{
	return mGravityScale;
}

/*******************************************************/
void P2Body::SetGravityScale(const float32_t gravityScale)
{
	mGravityScale = gravityScale;
}

/*******************************************************/
float32_t P2Body::GetMass() const
{
	return mMass;
}

/*******************************************************/
float32_t P2Body::GetInvMass() const
{
	return mInvMass;
}

/*******************************************************/
float32_t P2Body::GetInertia() const
{
	return mInertia;
}

/*******************************************************/
float32_t P2Body::GetInvInertia() const
{
	return mInvInertia;
}
