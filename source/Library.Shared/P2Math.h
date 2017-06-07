#pragma once

#include <glm/detail/type_vec2.hpp>

struct P2Transform
{
	P2Transform(const glm::vec2& position = glm::vec2(0), const glm::detail::float32 rotation = 0)
		: Position(position), Rotation(rotation)
	{
	}

	glm::vec2 Position;
	glm::float32_t Rotation; // in radians
};

// todo add transform: add a squared length func as an opt to length

// todo add rotation struct that has cos and sin, better than having the angle