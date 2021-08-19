#pragma once
#include "collision_cell.hpp"
#include <SFML/System/Vector2.hpp>


struct PhysicObject
{
	uint32_t index;
	sf::Vector2f position;
	sf::Vector2f last_position;
	sf::Vector2f acceleration;

	PhysicObject()
		: index(0)
	{

	}

	void setPosition(sf::Vector2f pos)
	{
		position = pos;
		last_position = pos;
	}

	void accelerate(sf::Vector2f a)
	{
		acceleration += a;
	}

	void update(float dt)
	{
		const sf::Vector2f new_position = 2.0f * position - last_position + acceleration * (dt * dt);
		last_position = position;
		position = new_position;
		// Reset acceleration
		acceleration = {};
	}

	void stop()
	{
		last_position = position;
	}

	void slowdown(float ratio)
	{
		last_position = last_position + ratio * (position - last_position);
	}
};
