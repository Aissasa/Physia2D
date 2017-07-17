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
	P2MassData P2PolygonShape::ComputeMass(const float32_t density) const
	{
		P2MassData massData;

		float32_t area = ComputeArea(mVertices);
		massData.Mass = area * density;
		massData.Center = mCentroid;
		massData.Inertia = ComputeInertia(density);

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

		mArea = ComputeArea(mVertices);
		mCentroid = ComputeCentroid(mArea);

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
	float32_t P2PolygonShape::ComputeArea(const vector<vec2>& verts) const
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
	vec2 P2PolygonShape::ComputeCentroid(const float32_t area) const
	{
		float32_t cx = 0;
		float32_t cy = 0;

		for (uint32_t i = 0; i < mVertices.size(); ++i)
		{
			vec2 currentVec = mVertices[i];
			vec2 nextVec = mVertices[(i + 1) % mVertices.size()];

			cx += (currentVec.x + nextVec.x) * (currentVec.x * nextVec.y - nextVec.x * currentVec.y);
		}
		cx /= 6 * area;

		for (uint32_t i = 0; i < mVertices.size(); ++i)
		{
			vec2 currentVec = mVertices[i];
			vec2 nextVec = mVertices[(i + 1) % mVertices.size()];

			cy += (currentVec.y + nextVec.y) * (currentVec.x * nextVec.y - nextVec.x * currentVec.y);
		}

		cy /= 6 * area;

		return vec2(cx, cy);
	}

	/*******************************************************/
	float32_t P2PolygonShape::ComputeInertia(const float32_t density) const
	{
		float32_t total = 0;

		for (uint32_t i = 0; i < mVertices.size(); ++i)
		{
			vec2 currentVec = mVertices[i];
			vec2 nextVec = mVertices[(i + 1) % mVertices.size()];

			vector<vec2> triangle;
			triangle.push_back(mCentroid);
			triangle.push_back(currentVec);
			triangle.push_back(nextVec);

			float32_t area = ComputeArea(triangle);
			float32_t mass = area * density;

			//I_{ triangle } = m / 36 * (x1*(y3 - y2) + x2*(y1 - y3) + x3*(y2 - y1))*(x1 ^ 2 + x2 ^ 2 + x3 ^ 2 – x1*x2 – x1*x3 – x2*x3 + y1 ^ 2 + y2 ^ 2 + y3 ^ 2 – y1*y2 – y1*y3 – y2*y3)
			float32_t triangleInertia = (mass / 36) * ((mCentroid.x * (nextVec.y - currentVec.y) + currentVec.x * (mCentroid.y - nextVec.y)) *
				(mCentroid.x * mCentroid.x + currentVec.x * currentVec.x + nextVec.x * nextVec.x -
				 mCentroid.x * currentVec.x - mCentroid.x * nextVec.x - currentVec.x +
				 mCentroid.y * mCentroid.y + currentVec.y * currentVec.y + nextVec.y * nextVec.y -
				 mCentroid.y * currentVec.y - mCentroid.y * nextVec.y - currentVec.y));

			total += triangleInertia;
		}

		return total;
	}
}
