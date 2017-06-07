#include "pch.h"
#include "P2Fixture.h"
#include "P2Shape.h"

/*******************************************************/
P2Fixture::P2Fixture(std::shared_ptr<P2Shape> shape, const float density, const float bounciness, const float friction):
	mShape(shape), mDensity(density), mBounciness(bounciness), mFriction(friction)
{
}

/*******************************************************/
std::shared_ptr<P2Shape> P2Fixture::GetShape() const
{
	return mShape;
}

/*******************************************************/
void P2Fixture::SetShape(const std::shared_ptr<P2Shape>& p2Shape)
{
	mShape = p2Shape;
}

/*******************************************************/
glm::float32_t P2Fixture::GetDensity() const
{
	return mDensity;
}

/*******************************************************/
void P2Fixture::SetDensity(const glm::float32_t density)
{
	mDensity = density;
}

/*******************************************************/
glm::float32_t P2Fixture::GetBounciness() const
{
	return mBounciness;
}

/*******************************************************/
void P2Fixture::SetBounciness(const glm::float32_t bounciness)
{
	mBounciness = bounciness;
}

/*******************************************************/
glm::float32_t P2Fixture::GetFriction() const
{
	return mFriction;
}

/*******************************************************/
void P2Fixture::SetFriction(const glm::float32_t friction)
{
	mFriction = friction;
}
