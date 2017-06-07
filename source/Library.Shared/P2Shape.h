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
	P2AABB(const glm::vec2& lowwerVert = glm::vec2(0), const glm::vec2& upperVert = glm::vec2(0))
		: LowwerVert(lowwerVert), UpperVert(upperVert)
	{
	}

	glm::vec2 LowwerVert;
	glm::vec2 UpperVert;
};

/** Class for a general shape.
*/
class P2Shape
{
public:

	// todo add shape types enum and type field
	// todo think about adding a struct to init a shape

	P2Shape() = default;
	virtual ~P2Shape() = default;

	virtual P2AABB ComputeAABB(const P2Transform& transform) const = 0;
	virtual P2MassData ComputeMass(const glm::float32_t density) const = 0;
};

