#pragma once

#include <glm/detail/type_vec2.hpp>
#include "P2Math.h"

/** Struct that holds the mass data computed off of a shape.
*/
struct P2MassData
{
	glm::float32_t Mass;
	glm::vec2 Center;
	glm::float32_t Inertia;
};

/** Struct that holds the shape bounds.
*/
struct P2AABB
{
	P2AABB(const glm::vec2& lowerVert = glm::vec2(0), const glm::vec2& upperVert = glm::vec2(0))
		: LowerVert(lowerVert), UpperVert(upperVert)
	{
	}

	glm::vec2 LowerVert;
	glm::vec2 UpperVert;
};

/** Class for a general shape.
*/
class P2Shape
{
public:

	// todo add radius here

	enum class EType
	{
		Circle,
		Polygon
	};

	virtual ~P2Shape() = default;

	virtual P2Shape* Clone() = 0;
	virtual P2AABB ComputeAABB(const P2Transform& transform) const = 0;
	virtual P2MassData ComputeMass(const glm::float32_t density) const = 0;

	EType GetType() const;
	void SetType(const EType type);

protected:

	EType mType;
};

inline P2Shape::EType P2Shape::GetType() const
{
	return mType;
}

inline void P2Shape::SetType(const EType type)
{
	mType = type;
}

