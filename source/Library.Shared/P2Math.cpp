﻿#include "pch.h"
#include "P2Math.h"

using namespace std;
using namespace glm;

namespace Physia2D
{

#pragma region P2Rotation

	/*******************************************************/
	P2Rotation::P2Rotation(const float32_t rotation)
		: mRotation(rotation)
	{
		SetRotation(rotation);
	}

	/*******************************************************/
	void P2Rotation::operator=(const float32_t rotation)
	{
		SetRotation(rotation);
	}

	/*******************************************************/
	float32_t P2Rotation::GetRotation() const
	{
		return mRotation;
	}

	/*******************************************************/
	void P2Rotation::SetRotation(const float32_t rotation)
	{
		mRotation = rotation;
		mCos = glm::cos(mRotation);
		mSin = glm::sin(mRotation);
	}

	/*******************************************************/
	float32_t P2Rotation::Cos() const
	{
		return mCos;
	}

	/*******************************************************/
	float32_t P2Rotation::Sin() const
	{
		return mSin;
	}

#pragma endregion

#pragma region MathHelper

	/*******************************************************/
	MathHelper& MathHelper::GetInstance()
	{
		static MathHelper sInstance;
		return sInstance;
	}

	/*******************************************************/
	vec2 MathHelper::RotateAndTranslateVertex(const vec2& vec, const P2Transform& transform) const
	{
		vec2 resultVec;

		// Rotation
		// x' = x cos θ − y sin θ : rotate along x
		// y' = x sin θ + y cos θ : rotate along y
		resultVec.x = (vec.x * transform.Rotation.Cos() - vec.y * transform.Rotation.Sin());
		resultVec.y = (vec.x * transform.Rotation.Sin() + vec.y * transform.Rotation.Cos());

		// Translation
		resultVec += transform.Position;

		return resultVec;
	}

	/*******************************************************/
	vec2 MathHelper::LeftHandNormal(vec2 v1, vec2 v2) const
	{
		vec2 temp = v2 - v1;
		return vec2(-temp.y, temp.x);
	}

	/*******************************************************/
	vec2 MathHelper::LeftHandNormal(vec2 v) const
	{
		return vec2(-v.y, v.x);
	}

	/*******************************************************/
	vec2 MathHelper::RightHandNormal(vec2 v1, vec2 v2) const
	{
		vec2 temp = v2 - v1;
		return vec2(temp.y, -temp.x);
	}

	/*******************************************************/
	vec2 MathHelper::RightHandNormal(vec2 v) const
	{
		return vec2(v.y, -v.x);
	}

	/*******************************************************/
	float32_t MathHelper::LengthSquared(vec2 vec) const
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	/*******************************************************/
	float32_t MathHelper::MaxFloat() const
	{
		return FLT_MAX;
	}

	/*******************************************************/
	float32_t MathHelper::FromRadiansToDegrees(const float32_t radians) const
	{
		return radians * mRadToDegMultiplier;
	}

	/*******************************************************/
	float32_t MathHelper::FromDegreesToRadians(const float32_t degrees) const
	{
		return degrees * mDegToRadMultiplier;
	}

	/*******************************************************/
	MathHelper::MathHelper() : 
		mDegToRadMultiplier(pi<float32_t>() / 180.0f), 
		mRadToDegMultiplier(180.0f / pi<float32_t>())
	{
	}

#pragma endregion

}
