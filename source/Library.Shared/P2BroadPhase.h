#pragma once

namespace Physia2D
{
	class P2Body;

	/** Singleton handling the broad phase of collision.
	*/
	class P2BroadPhase final
	{
	public:

		typedef std::pair<std::shared_ptr<P2Body>, std::shared_ptr<P2Body>> PotentiallyCollidingPair;

		P2BroadPhase(const P2BroadPhase&) = delete;
		P2BroadPhase(const P2BroadPhase&&) = delete;
		P2BroadPhase& operator=(const P2BroadPhase&) = delete;
		P2BroadPhase& operator=(const P2BroadPhase&&) = delete;

		static P2BroadPhase& GetInstance();

		std::vector<PotentiallyCollidingPair> GetPotentiallyCollidingPairs(const std::vector<std::shared_ptr<P2Body>>& bodies);

	private:

		P2BroadPhase() = default;
		~P2BroadPhase() = default;
	};
}
