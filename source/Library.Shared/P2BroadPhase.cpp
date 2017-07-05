#include "pch.h"
#include "P2BroadPhase.h"
#include "P2Collision.h"
#include "P2Body.h"
#include "P2Shape.h"

using namespace std;
using namespace glm;

namespace Physia2D
{
	/*******************************************************/
	P2BroadPhase& P2BroadPhase::GetInstance()
	{
		static P2BroadPhase sInstance;

		return sInstance;
	}

	/*******************************************************/
	vector<P2BroadPhase::PotentiallyCollidingPair> P2BroadPhase::GetPotentiallyCollidingPairs(const vector<shared_ptr<P2Body>>& bodies)
	{
		vector<PotentiallyCollidingPair> pairs;
		PotentiallyCollidingPair pair;
		auto& collisionManager = P2Collision::GetInstance();
		P2AABB aabb1, aabb2;

		for (uint32_t i = 0; i < bodies.size(); ++i)
		{
			aabb1 = bodies[i]->GetFixture()->GetShape()->ComputeAABB(bodies[i]->GetTransform());
			for (uint32_t j = i; j < bodies.size(); ++j)
			{
				if (i == j)
				{
					continue;
				}

				aabb2 = bodies[j]->GetFixture()->GetShape()->ComputeAABB(bodies[j]->GetTransform());
				if (collisionManager.AABBvsAABBOverlap(aabb1, aabb2))
				{
					pair.first = bodies[i];
					pair.second = bodies[j];

					pairs.push_back(pair);
				}
			}
		}

		return move(pairs);
	}
}
