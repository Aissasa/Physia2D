#pragma once

#include "P2Math.h"
#include "RTTI.h"
#include "P2Collision.h"

namespace Physia2D
{
	/** Struct that holds the mass data computed off of a shape.
	*/
	struct P2MassData
	{
		glm::float32_t Mass;
		glm::vec2 Center;
		glm::float32_t Inertia;
	};

	/** Class for a general shape.
	*/
	class P2Shape : public RTTI
	{
		RTTI_DECLARATIONS(P2Shape, RTTI)

	public:

		enum class EType
		{
			Circle,
			Polygon
		};

		virtual ~P2Shape() = default;

		virtual P2Shape* Clone() = 0;
		virtual P2AABB ComputeAABB(const P2Transform& transform) const = 0;
		virtual P2MassData ComputeMass(const glm::float32_t density) const = 0;

		virtual glm::vec2 GetCenter() const = 0;

		EType GetType() const;
		void SetType(const EType type);

	protected:

		EType mType;
	};
}
