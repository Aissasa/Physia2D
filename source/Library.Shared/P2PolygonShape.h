#pragma once

#include "P2Shape.h"

namespace Physia2D
{
	/** Struct that holds information about an edge.
	*/
	struct P2Edge
	{
		P2Edge(const glm::vec2& currentVertex = glm::vec2(0), const glm::vec2& nextVertex = glm::vec2(0), const glm::vec2& maxVertex = glm::vec2(0))
			: CurrentVertex(currentVertex),
			  NextVertex(nextVertex),
			  MaxVertex(maxVertex)
		{
		}

		glm::vec2 CurrentVertex;
		glm::vec2 NextVertex;
		glm::vec2 MaxVertex;

		glm::vec2 ResultEdge() const { return NextVertex - CurrentVertex; }
		glm::vec2 NormalizedResultEdge() const { return glm::normalize(ResultEdge()); }
		glm::vec2 Normal() const { return MathHelper::GetInstance().RightHandNormal(ResultEdge()); }
		glm::vec2 NormalizedNormal() const { return MathHelper::GetInstance().RightHandNormal(NormalizedResultEdge()); }
	};

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
		virtual P2MassData ComputeMass(const glm::float32_t density) override;
		virtual glm::vec2 GetCenter() const override;

		// todo use http://en.wikipedia.org/wiki/Gift_wrapping_algorithm to make sure it's convex
		bool SetVertices(std::vector<glm::vec2>& vertices);
		bool SetAsBox(glm::vec2 lower, glm::vec2 upper);

		const std::vector<glm::vec2>& GetVertices() const;
		std::vector<glm::vec2> GetVertices();
		std::vector<glm::vec2> GetRotatedAndTranslatedVertices(const P2Transform& transform) const;

		glm::uint32_t VerticesCount() const;

		static glm::float32_t ComputeArea(const std::vector<glm::vec2>& verts);
		static glm::vec2 ComputeCentroid(const std::vector<glm::vec2>& vertices);

		static const glm::float32_t PolygonEdgeWidth;

	private:

		//glm::float32_t ComputeInertia(const glm::float32_t density) const;

		std::vector<glm::vec2> mVertices;
		glm::vec2 mCentroid;
		glm::float32_t mArea;

	};
}
