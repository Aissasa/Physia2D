#include "pch.h"
#include "P2Shape.h"

namespace Physia2D
{
	RTTI_DEFINITIONS(P2Shape)

	/*******************************************************/
	P2Shape::EType P2Shape::GetType() const
	{
		return mType;
	}

	/*******************************************************/
	void P2Shape::SetType(const EType type)
	{
		mType = type;
	}
}