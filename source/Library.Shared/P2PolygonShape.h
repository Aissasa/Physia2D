#pragma once

#include "P2Shape.h"

namespace Physia2D
{
	/** Class that represents the polygon shape.
	*/
	class P2PolygonShape : public P2Shape
	{
		RTTI_DECLARATIONS(P2PolygonShape, P2Shape);

	public:

		P2PolygonShape();
		~P2PolygonShape() = default;

		virtual P2Shape* Clone() override;
		virtual P2AABB ComputeAABB(const P2Transform& transform) const override;
		virtual P2MassData ComputeMass(const glm::float32_t density) const override;

		// todo use http://en.wikipedia.org/wiki/Gift_wrapping_algorithm to make sure it's convex
		bool SetVertices(std::vector<glm::vec2>& vertices);
		bool SetAsBox(glm::vec2 lower, glm::vec2 upper); // todo think about the angle of the box

		const std::vector<glm::vec2>& GetVertices() const;
		std::vector<glm::vec2> GetVertices();
		std::vector<glm::vec2> GetRotatedAndTranslatedVertices(const P2Transform& transform) const;

		glm::uint32_t VerticesCount() const;

		static const glm::float32_t PolygonEdgeWidth;

	private:

		glm::float32_t CalculateArea() const;

		std::vector<glm::vec2> mVertices;
		glm::vec2 mCentroid;

	};
}
