#pragma once
#include <SFML/Graphics.hpp>
#include "engine/physics/physics.hpp"
#include "engine/common/utils.hpp"


struct Renderer
{
    const CIVector<PhysicObject>& objects;
    
    sf::RenderTarget&       target;
    sf::VertexArray         objects_va;
    sf::Texture             circle_texture;
    sf::RenderStates        render_states;

    Renderer(sf::RenderTarget& target_, CIVector<PhysicObject>& objects_)
        : objects(objects_)
        , target(target_)
        , objects_va(sf::Quads)
    {
        // Load texture
        circle_texture.loadFromFile("res/circle.png");
        render_states.texture = &circle_texture;
        // Initialize vertex array
        const uint64_t objects_count = objects.size();
        objects_va.resize(4 * objects_count);
        for (uint64_t i(0); i < objects_count; ++i) {
            objects_va[4 * i + 0].texCoords = { 0.0f  , 0.0f };
            objects_va[4 * i + 1].texCoords = { 512.0f, 0.0f };
            objects_va[4 * i + 2].texCoords = { 512.0f, 512.0f };
            objects_va[4 * i + 3].texCoords = { 0.0f  , 512.0f };
        }
    }

    void updateVA()
    {
        const uint64_t objects_count = objects.size();
        objects_va.resize(4 * objects_count);
        for (uint64_t i(0); i < objects_count; ++i) {
            const PhysicObject& o = objects[i];
            const sf::Vector2f vel = o.getVelocity();
            const float speed = 1.0f * getLength(vel);
            const float pressure = 1.0f * o.pressure;

            const float base = 5.0f;
            const float intensity = base + pressure + speed;
            const float r = 100.0f * (pressure + speed);
            const float g = 10.0f * pressure;
            const float b = 1500.0f * speed;

            const sf::Color color(
                std::max(std::min(255.0f, r), 0.0f),
                std::max(std::min(255.0f, g), 0.0f),
                std::max(std::min(255.0f, b), 0.0f),
                100
            );

            //sf::Color color = sf::Color::White;
            const sf::Vector2f vel_v = vel / speed;
            const sf::Vector2f vel_n = { -vel_v.y, vel_v.x };
            const float width = 1.0f + 3.0f * speed;
            objects_va[4 * i + 0].position = o.position + width * vel_v + 0.35f * width * vel_n;
            objects_va[4 * i + 1].position = o.position + width * vel_v - 0.35f * width * vel_n;
            objects_va[4 * i + 2].position = o.position - width * vel_v - 0.35f * width * vel_n;
            objects_va[4 * i + 3].position = o.position - width * vel_v + 0.35f * width * vel_n;

            objects_va[4 * i + 0].color = color;
            objects_va[4 * i + 1].color = color;
            objects_va[4 * i + 2].color = color;
            objects_va[4 * i + 3].color = color;
        }
    }

    void render(const sf::RenderStates& states)
    {
        updateVA();
        render_states.transform = states.transform;
        target.draw(objects_va, render_states);
    }
};
