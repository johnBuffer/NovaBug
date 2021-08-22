#pragma once
#include "collision_cell.hpp"
#include "physic_object.hpp"
#include "../common/utils.hpp"
#include "../common/index_vector.hpp"


struct PhysicSolver
{
	CIVector<PhysicObject> objects;
	CollisionGrid grid;
	sf::Vector2f world_size;

	// Simulation solving pass count
	uint32_t solver_iterations;
	uint32_t sub_steps;
	sf::Vector2f gravity = { 0.0f, 50.0f };
    float frame_dt;

	// Parameters
	float response_coef = 0.8f;
	float vel_coef = 0.0025f;

	PhysicSolver(uint32_t width, uint32_t height)
		: grid(width, height)
		, solver_iterations(1)
		, sub_steps(2)
		, world_size(to<float>(width), to<float>(height))
	{}

	// Checks if two atoms are colliding and if so create a new contact
	void checkContact(uint32_t atom_1, uint32_t atom_2, float dt)
	{
		if (atom_1 == atom_2) {
			return;
		}
		const sf::Vector2f o2_o1 = objects[atom_1].position - objects[atom_2].position;
		const float dist2 = o2_o1.x * o2_o1.x + o2_o1.y * o2_o1.y;
		if (dist2 < 1.0f) {
			const float dist = sqrt(dist2);
			const float delta = response_coef * 0.5f * (1.0f - dist);
			const sf::Vector2f col_vec = o2_o1 / dist;

			const float influence_sum = 1.0f / (1.0f + objects[atom_1].mass + objects[atom_2].mass);
			const float influence_1 = objects[atom_1].mass * influence_sum;
			const float influence_2 = objects[atom_2].mass * influence_sum;

			objects[atom_1].pressure += (delta * objects[atom_2].mass);
			objects[atom_2].pressure += (delta * objects[atom_1].mass);

			// Fusion
			float fusion_factor_1 = 1.0f;
			float fusion_factor_2 = 1.0f;
			const float pressure_threshold = 100.0f;
			if (objects[atom_1].pressure > objects[atom_1].mass * pressure_threshold) {
			    objects[atom_1].mass += 1.0f;
			    fusion_factor_1 = 100.0f;
			}
			if (objects[atom_2].pressure > objects[atom_2].mass * pressure_threshold) {
			    objects[atom_2].mass += 1.0f;
			    fusion_factor_2 = 100.0f;
			}

			objects[atom_1].position += col_vec * (delta * influence_2 * fusion_factor_1);
			objects[atom_2].position -= col_vec * (delta * influence_1 * fusion_factor_2);

			const sf::Vector2f delta_v = objects[atom_1].getVelocity() - objects[atom_2].getVelocity();
			objects[atom_1].addVelocity(-delta_v * (vel_coef * influence_2));
			objects[atom_2].addVelocity( delta_v * (vel_coef * influence_1));
		}
	}

	// Checks an atom collision against a cell
	void checkAtomsCollisions(uint32_t a, CollisionCell& cell)
	{
		for (uint8_t i(cell.objects_count); i--;) {
			checkContact(a, cell.objects[i], frame_dt);
		}
	}

	void checkAtomCellCollisions(uint32_t id, CollisionCell& cell, uint32_t i)
	{
		checkAtomsCollisions(cell.objects[i], cell);
		checkAtomsCollisions(cell.objects[i], grid.data[id + 1]);
		checkAtomsCollisions(cell.objects[i], grid.data[id + 1 + grid.width]);
		checkAtomsCollisions(cell.objects[i], grid.data[id + grid.width]);
		checkAtomsCollisions(cell.objects[i], grid.data[id + 1 - grid.width]);
	}
	
	// Given an atom checks its collisions with close area
	void checkCellCollisions(uint32_t id)
	{
		for (uint8_t i(grid.data[id].objects_count); i--;) {
			checkAtomCellCollisions(id, grid.data[id], i);
		}
	}
	
	// Finds colliding atoms
	void findContacts()
    {
		for (uint32_t i(grid.used_count); i--;) {
			checkCellCollisions(grid.used[i]);
		}
	}

	// Applies impulses to solve collisions
	void solveCollisions()
	{
		for (uint32_t i(solver_iterations); i--;) {
			findContacts();
		}
	}

	// Add a new object to the solver
	uint64_t addObject(const PhysicObject& object)
	{
		const uint64_t object_id = objects.push_back(object);
		PhysicObject& new_object = objects[object_id];
		new_object.index = to<uint32_t>(object_id);
		return object_id;
	}
    
	void update(float dt)
	{
        frame_dt = dt / float(sub_steps);
        for (uint32_t i(sub_steps); i--;) {
            addObjectsToGrid();
            solveCollisions();
            updateObjects();
        }
	}

	void addObjectsToGrid()
	{
		grid.clear();
		for (PhysicObject& obj : objects) {
			if (obj.position.x > 1.0f && obj.position.x < world_size.x - 1.0f &&
				obj.position.y > 1.0f && obj.position.y < world_size.y - 1.0f) {
				grid.addAtom(to<uint32_t>(obj.position.x), to<uint32_t>(obj.position.y), obj.index);
			}
		}
	}

	void updateObjects()
	{	
		for (PhysicObject& obj : objects) {
			obj.accelerate(gravity);
			obj.update(frame_dt);
		}
	}
};
