#pragma once
#include "physics/physics.hpp"


struct Engine
{
    CIVector<uint64_t> entities;
    PhysicSolver solver;

    Engine(uint32_t world_width, uint32_t world_height)
        : solver(world_width, world_height)
    {
    }

    void update(float dt)
    {
        solver.update(dt);
    }

    void render(sf::RenderTarget& target, const sf::RenderStates& vp_states)
    {
        sf::CircleShape c(0.5f);
        c.setPointCount(32);
        c.setOrigin(0.5f, 0.5f);
        for (const uint64_t id : entities) {
			sf::RenderStates states = vp_states;
            const PhysicObject& obj = solver.objects[id];
            c.setPosition(obj.position);
            target.draw(c, vp_states);
		}
    }

    uint64_t addEntity(const PhysicObject &entity)
    {
        const uint64_t id = solver.addObject(entity);
        entities.push_back(id);
        return id;
    }
    
    void removeEntity(uint64_t id)
    {
        entities.erase(id);
        solver.objects.erase(id);
    }
};
