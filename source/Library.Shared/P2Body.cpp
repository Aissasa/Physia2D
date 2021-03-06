#include "pch.h"
#include "P2Body.h"
#include "P2Shape.h"
#include "P2World.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	const float32_t P2Body::kCollisionColorChangeTime = 0.2f;

	/*******************************************************/
	P2Body::P2Body(const P2BodyConfig& bodyConfig) :
		mTransform(bodyConfig.Position, bodyConfig.Rotation), mLinearVelocity(bodyConfig.LinearVelocity), mAngularVelocity(bodyConfig.AngularVelocity),
		mForce(0), mTorque(0), mWorld(nullptr), mFixture(nullptr), mMass(0), mInvMass(0), mInertia(0), mInvInertia(0), mGravityScale(bodyConfig.GravityScale),
		mBodyType(bodyConfig.Type), mIsColliding(false), mCollisionTimer(kCollisionColorChangeTime)
	{
		if (mBodyType == P2BodyType::Static)
		{
			mLinearVelocity = vec2(0);
			mAngularVelocity = 0;
		}
	}

	/*******************************************************/
	P2Body::~P2Body()
	{
		mFixture.reset();
	}

	/*******************************************************/
	shared_ptr<P2Fixture> P2Body::CreateFixture(const P2FixtureConfig& fixtureConfig, bool replaceIfExists)
	{
		if (mFixture == nullptr || mFixture != nullptr && replaceIfExists)
		{
			if (mFixture == nullptr)
			{
				mFixture = make_shared<P2Fixture>();
			}
			mFixture->Create(*this, fixtureConfig);

			ResetMassData();
		}

		return mFixture;
	}

	/*******************************************************/
	void P2Body::Update(const float32_t elapsedTime)
	{
		UpdateForces();
		UpdateVelocities(elapsedTime);
		UpdatePosition(elapsedTime);

		if (mIsColliding)
		{
			mCollisionTimer -= elapsedTime;
			if (mCollisionTimer <= 0)
			{
				mCollisionTimer = kCollisionColorChangeTime;
				mIsColliding = false;
			}
		}
	}

	/*******************************************************/
	void P2Body::ResetMassData()
	{
		// we need mass data only when the body is dynamic
		if (mBodyType != P2BodyType::Static && mFixture && mFixture->GetShape())
		{
			P2MassData massData = mFixture->GetShape()->ComputeMass(mFixture->GetDensity());
			if (mBodyType == P2BodyType::Dynamic)
			{
				mMass = massData.Mass;
				mInvMass = mMass == 0 ? mInvMass = 0 : mInvMass = 1 / mMass;
				mInertia = massData.Inertia;
				mInvInertia = mInertia == 0 ? mInvInertia = 0 : mInvInertia = 1 / mInertia;
			}
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
		if (mBodyType != P2BodyType::Static)
		{
			mLinearVelocity = linearVelocity;
		}
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
	shared_ptr<P2Fixture> P2Body::GetFixture() const
	{
		return mFixture;
	}

	/*******************************************************/
	const P2World* P2Body::GetWorld() const
	{
		return mWorld;
	}

	/*******************************************************/
	void P2Body::SetWorld(const P2World& world)
	{
		mWorld = &world;
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

	/*******************************************************/
	bool P2Body::IsColliding() const
	{
		return mIsColliding;
	}

	/*******************************************************/
	void P2Body::SetIsColliding()
	{
		mCollisionTimer = kCollisionColorChangeTime;
		mIsColliding = true;
	}

	/*******************************************************/
	P2BodyType P2Body::GetBodyType() const
	{
		return mBodyType;
	}

	/*******************************************************/
	void P2Body::SetBodyType(const P2BodyType bodyType)
	{
		mBodyType = bodyType;
	}

	/*******************************************************/
	void P2Body::UpdateForces()
	{
		if (mBodyType == P2BodyType::Dynamic)
		{
			mForce = mWorld->GetGravity() * mGravityScale * mMass;
		}
	}

	/*******************************************************/
	void P2Body::UpdateVelocities(const float32_t elapsedTime)
	{
		if (mBodyType != P2BodyType::Static)
		{
			mLinearVelocity += mForce * mInvMass * elapsedTime;
			mAngularVelocity += mTorque * mInvInertia * elapsedTime;
		}
	}

	/*******************************************************/
	void P2Body::UpdatePosition(const float32_t elapsedTime)
	{
		if (mBodyType != P2BodyType::Static)
		{
			mTransform.Position += mLinearVelocity * elapsedTime;
			mTransform.Rotation = mTransform.Rotation.GetRotation() + mAngularVelocity * elapsedTime;
		}
	}
}
