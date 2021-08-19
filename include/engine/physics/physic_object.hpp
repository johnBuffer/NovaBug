#pragma once
#include "collision_cell.hpp"
#include <SFML/System/Vector2.hpp>
#include "engine/common/utils.hpp"


struct PhysicObject
{
	uint32_t index;
	// Verlet
	sf::Vector2f position;
	sf::Vector2f last_position;
	sf::Vector2f acceleration;
	// Debug
	float pressure = 1.0f;

	PhysicObject()
		: index(0)
	{}

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
		pressure = pressure * 0.99f;
	}

	void stop()
	{
		last_position = position;
	}

	void slowdown(float ratio)
	{
		last_position = last_position + ratio * (position - last_position);
	}

	float getSpeed() const
	{
		return getLength(position - last_position);
	}

	sf::Vector2f getVelocity() const
	{
		return position - last_position;
	}

	void addVelocity(sf::Vector2f v)
	{
		last_position -= v;
	}
};
