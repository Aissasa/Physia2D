#include "pch.h"
#include "P2CircleShape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	RTTI_DEFINITIONS(P2CircleShape)

	/*******************************************************/
	P2CircleShape::P2CircleShape(vec2 position, float32_t radius) :
		mCenterPosition(position), mRadius(radius)
	{
		mType = EType::Circle;
		mRadiusSqr = mRadius * mRadius;
	}

	/*******************************************************/
	P2Shape* P2CircleShape::Clone()
	{
		P2CircleShape* newShape = new P2CircleShape();
		newShape->SetType(mType);
		newShape->SetCenterPosition(mCenterPosition);
		newShape->SetRadius(mRadius);

		return newShape;
	}

	/*******************************************************/
	P2AABB P2CircleShape::ComputeAABB(const P2Transform& transform) const
	{
		vec2 center = MathHelper::GetInstance().RotateAndTranslateVertex(mCenterPosition, transform);

		P2AABB aabb;
		aabb.LowerVert = center - mRadius;
		aabb.UpperVert = center + mRadius;

		return aabb;
	}

	/*******************************************************/
	P2MassData P2CircleShape::ComputeMass(const float32_t density) const
	{
		P2MassData massData;
		float32_t sqrRad = mRadius * mRadius;

		float32_t area = pi<float32_t>() * sqrRad;
		massData.Mass = area * density;
		massData.Center = mCenterPosition;
		massData.Inertia = 0.5f * massData.Mass * sqrRad;

		return massData;
	}

	/*******************************************************/
	vec2 P2CircleShape::GetCenter() const
	{
		return mCenterPosition;
	}

	/*******************************************************/
	void P2CircleShape::SetCenterPosition(const vec2& position)
	{
		mCenterPosition = position;
	}

	/*******************************************************/
	detail::float32 P2CircleShape::GetRadius() const
	{
		return mRadius;
	}

	/*******************************************************/
	void P2CircleShape::SetRadius(const float32_t radius)
	{
		mRadius = radius;
		mRadiusSqr = mRadius * mRadius;
	}

	/*******************************************************/
	float32_t P2CircleShape::GetRadiusSqr() const
	{
		return mRadiusSqr;
	}
}
