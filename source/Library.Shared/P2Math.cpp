#include "pch.h"
#include "P2Math.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/*******************************************************/
	MathHelper& MathHelper::GetInstance()
	{
		static MathHelper sInstance;
		return sInstance;
	}

	/*******************************************************/
	vec2 MathHelper::Vec2CrossProduct(vec2 v1, vec2 v2) const
	{
		vec2 temp = v2 - v1;
		return vec2(-temp.y, temp.x); // we can go with the negative of that
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
}
