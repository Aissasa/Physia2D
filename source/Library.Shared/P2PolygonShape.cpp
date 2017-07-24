#include "pch.h"
#include "P2PolygonShape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	RTTI_DEFINITIONS(P2PolygonShape);

	const float32_t P2PolygonShape::PolygonEdgeWidth = 0.05f;

	/*******************************************************/
	P2PolygonShape::P2PolygonShape() :
		mVertices(), mCentroid(), mArea(0)
	{
		mType = EType::Polygon;
	}

	/*******************************************************/
	P2Shape* P2PolygonShape::Clone()
	{
		P2PolygonShape* newShape = new P2PolygonShape();
		newShape->mType = mType;
		newShape->SetVertices(mVertices);

		return newShape;
	}

	/*******************************************************/
	P2AABB P2PolygonShape::ComputeAABB(const P2Transform& transform) const
	{
		P2AABB aabb;

		auto vertices = GetRotatedAndTranslatedVertices(transform);

		aabb.LowerVert = vertices[0];
		aabb.UpperVert = aabb.LowerVert;

		for (auto& vertex : vertices)
		{
			aabb.LowerVert = min(aabb.LowerVert, vertex);
			aabb.UpperVert = max(aabb.UpperVert, vertex);
		}

		aabb.LowerVert = aabb.LowerVert - PolygonEdgeWidth;
		aabb.UpperVert = aabb.UpperVert + PolygonEdgeWidth;

		return aabb;
	}

	/*******************************************************/
	P2MassData P2PolygonShape::ComputeMass(const float32_t density)
	{
		P2MassData massData;

		// Calculate centroid and moment of interia
		vec2 c(0.0f, 0.0f); // centroid
		float32_t area = 0.0f;
		float32_t I = 0.0f;
		const float32_t k_inv3 = 1.0f / 3.0f;

		for (uint32 i1 = 0; i1 < mVertices.size(); ++i1)
		{
			// Triangle vertices, third vertex implied as (0, 0)
			vec2 p1(mVertices[i1]);
			uint32 i2 = i1 + 1 < mVertices.size() ? i1 + 1 : 0;
			vec2 p2(mVertices[i2]);

			float32_t D = MathHelper::GetInstance().CrossProduct(p1, p2);
			float32_t triangleArea = 0.5f * D;

			area += triangleArea;

			// Use area to weight the centroid average, not just vertex position
			c += triangleArea * k_inv3 * (p1 + p2);

			float32_t intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
			float32_t inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
			I += (0.25f * k_inv3 * D) * (intx2 + inty2);
		}

		c *= 1.0f / area;

		// Translate vertices to centroid (make the centroid (0, 0)
		// for the polygon in model space)
		// Not really necessary, but I like doing this anyway
		for (uint32 i = 0; i < mVertices.size(); ++i)
			mVertices[i] = mVertices[i] - c;

		mArea = area;
		mCentroid = vec2(0, 0);
		massData.Mass = density * mArea;
		massData.Inertia = I * density;
		massData.Center = mCentroid;

		return massData;
	}

	/*******************************************************/
	vec2 P2PolygonShape::GetCenter() const
	{
		return mCentroid;
	}

	/*******************************************************/
	bool P2PolygonShape::SetVertices(vector<vec2>& vertices)
	{
		mVertices = vertices;

		return true;
	}

	/*******************************************************/
	bool P2PolygonShape::SetAsBox(vec2 lower, vec2 upper)
	{
		vec2 upperLeft(lower.x, upper.y);
		vec2 lowerRight(upper.x, lower.y);
		mVertices.push_back(lower);
		mVertices.push_back(lowerRight);
		mVertices.push_back(upper);
		mVertices.push_back(upperLeft);

		mArea = ComputeArea(mVertices);
		mCentroid = (upper - lower) / 2.0f;

		return true;
	}

	/*******************************************************/
	const vector<vec2>& P2PolygonShape::GetVertices() const
	{
		return mVertices;
	}

	/*******************************************************/
	vector<vec2> P2PolygonShape::GetVertices()
	{
		return mVertices;
	}

	/*******************************************************/
	vector<vec2> P2PolygonShape::GetRotatedAndTranslatedVertices(const P2Transform& transform) const
	{
		vector<vec2> resultVertices;
		for (auto& vertex : mVertices)
		{
			resultVertices.push_back(MathHelper::GetInstance().RotateAndTranslateVertex(vertex, transform));
		}

		return resultVertices;
	}

	/*******************************************************/
	uint32_t P2PolygonShape::VerticesCount() const
	{
		return mVertices.size();
	}

	/*******************************************************/
	float32_t P2PolygonShape::ComputeArea(const vector<vec2>& verts)
	{
		float32_t sum = 0;

		for (uint32_t i = 0; i < verts.size(); ++i)
		{
			vec2 currentVec = verts[i];
			vec2 nextVec = verts[(i + 1) % verts.size()];

			sum += currentVec.x * nextVec.y - currentVec.y * nextVec.x;
		}

		return glm::abs(sum / 2.0f);
	}

	/*******************************************************/
	vec2 P2PolygonShape::ComputeCentroid(const vector<vec2>& vertices)
	{
		float32_t area = ComputeArea(vertices);

		float32_t cx = 0;
		float32_t cy = 0;

		for (uint32_t i = 0; i < vertices.size(); ++i)
		{
			vec2 currentVec = vertices[i];
			vec2 nextVec = vertices[(i + 1) % vertices.size()];

			cx += (currentVec.x + nextVec.x) * (currentVec.x * nextVec.y - nextVec.x * currentVec.y);
		}
		cx /= 6 * area;

		for (uint32_t i = 0; i < vertices.size(); ++i)
		{
			vec2 currentVec = vertices[i];
			vec2 nextVec = vertices[(i + 1) % vertices.size()];

			cy += (currentVec.y + nextVec.y) * (currentVec.x * nextVec.y - nextVec.x * currentVec.y);
		}

		cy /= 6 * area;

		return vec2(cx, cy);
	}
}
