#include "pch.h"
#include "P2PolygonShape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	RTTI_DEFINITIONS(P2PolygonShape);

	const float32_t P2PolygonShape::PolygonEdgeWidth = 0.05f;

	/*******************************************************/
	P2PolygonShape::P2PolygonShape():
		mVertices(), mCentroid()
	{
		mType = EType::Polygon;
	}

	/*******************************************************/
	P2Shape* P2PolygonShape::Clone()
	{
		P2PolygonShape* newShape = new P2PolygonShape();
		newShape->mType = mType;
		newShape->SetVertices(mVertices);
		newShape->mCentroid = mCentroid;

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
		// todo compute center and intertia using trianglation : https://www.experts-exchange.com/questions/20986874/Calculating-Polygon-Inertia.html pointyears comment
		P2MassData massData;

		float32_t area = CalculateArea();
		massData.Mass = area * density;

		return massData;
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
	float32_t P2PolygonShape::CalculateArea() const
	{
		float32_t sum = 0;

		for (uint32_t i = 0; i < mVertices.size(); ++i)
		{
			vec2 currentVec = mVertices[i];
			vec2 nextVec = mVertices[i % mVertices.size()];

			sum += currentVec.x * nextVec.y - currentVec.y * nextVec.x;
		}

		return glm::abs(sum / 2.0f);
	}
}
