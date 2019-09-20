#pragma once

#include <fwd.hpp>
#include <vec2.hpp>

namespace Physia2D
{
	struct P2FixtureConfig;
	class P2Body;
}

namespace Testbed
{

	class Spawner final
	{

	public:

		Spawner();
		~Spawner() = default;

		std::shared_ptr<Physia2D::P2Body> CreateRandomCircle(const glm::vec2& position = glm::vec2(800.f, 800.f)) const;
		std::shared_ptr<Physia2D::P2Body> CreateRandomPolygon(const glm::vec2& position = glm::vec2(800.f, 800.f)) const;

		std::shared_ptr<Physia2D::P2Body> CreateRandomCircle(const glm::float32_t x = 800.f, const glm::float32_t y = 800.f) const;
		std::shared_ptr<Physia2D::P2Body> CreateRandomPolygon(const glm::float32_t x = 800.f, const glm::float32_t y = 800.f) const;

	private:

		void InitMaterials();
		void InitPolygonVertices();

		Physia2D::P2FixtureConfig GetRandomMaterial() const;
		std::vector<glm::vec2> GetRandomPolygon() const;

		std::vector<std::shared_ptr<Physia2D::P2FixtureConfig>> mMaterials;
		std::vector<std::vector<glm::vec2>> mPolygonSets;

	};

}
