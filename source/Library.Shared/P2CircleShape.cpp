#include "pch.h"
#include "P2CircleShape.h"

using namespace std;
using namespace glm;

/*******************************************************/
P2CircleShape::P2CircleShape(vec2 position, float32_t radius): 
	mPosition(position), mRadius(radius)
{
	mType = EType::Circle;
}

/*******************************************************/
P2Shape* P2CircleShape::Clone()
{
	P2CircleShape* newShape = new P2CircleShape();
	newShape->SetPosition(mPosition);
	newShape->SetRadius(mRadius);
	newShape->SetType(mType);

	return newShape;
}

/*******************************************************/
P2AABB P2CircleShape::ComputeAABB(const P2Transform& transform) const
{
	UNREFERENCED_PARAMETER(transform);
	// todo implement aabb
	return P2AABB();
}

/*******************************************************/
P2MassData P2CircleShape::ComputeMass(const float32_t density) const
{
	P2MassData massData;
	float32_t sqrRad = mRadius * mRadius;

	float32_t area = pi<float32_t>() * sqrRad;
	massData.Mass = area * density;
	massData.Center = mPosition;
	massData.Inertia = 0.5f * massData.Mass * sqrRad;

	return massData;
}

/*******************************************************/
vec2 P2CircleShape::GetPosition() const
{
	return mPosition;
}

/*******************************************************/
void P2CircleShape::SetPosition(const vec2& position)
{
	mPosition = position;
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
}
