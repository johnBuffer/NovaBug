#pragma once
#include <SFML/Graphics.hpp>
#include "engine/physics/physics.hpp"
#include "engine/common/utils.hpp"
#include "engine/common/event_manager.hpp"
#include "engine/render/viewport_handler.hpp"


struct Renderer
{
    const CIVector<PhysicObject>& objects;
    
    sf::RenderTarget& target;
    sf::VertexArray   objects_va;
    sf::Texture       circle_texture;
    sf::RenderStates  render_states;
    ViewportHandler   vp_handler;

    Renderer(sf::RenderTarget& target_, CIVector<PhysicObject>& objects_)
        : objects(objects_)
        , target(target_)
        , objects_va(sf::Quads)
        , vp_handler(toVector2f(target_.getSize()))
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
    
    void registerViewportHandlerControls(sfev::EventManager& ev_manager)
    {
        ev_manager.addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
            vp_handler.click(ev_manager.getFloatMousePosition());
        });
        ev_manager.addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
            vp_handler.unclick();
        });
        ev_manager.addEventCallback(sf::Event::MouseMoved, [&](sfev::CstEv) {
            vp_handler.setMousePosition(ev_manager.getFloatMousePosition());
        });
        ev_manager.addEventCallback(sf::Event::MouseWheelScrolled, [&](sfev::CstEv e) {
            vp_handler.wheelZoom(e.mouseWheelScroll.delta);
        });
    }

    void updateVA()
    {
        const uint64_t objects_count = objects.size();
        objects_va.resize(4 * objects_count);
        for (uint64_t i(0); i < objects_count; ++i) {
            const PhysicObject& o = objects[i];
            objects_va[4 * i + 0].position = { o.position.x - 0.5f  , o.position.y - 0.5f };
            objects_va[4 * i + 1].position = { o.position.x + 0.5f  , o.position.y - 0.5f };
            objects_va[4 * i + 2].position = { o.position.x + 0.5f  , o.position.y + 0.5f };
            objects_va[4 * i + 3].position = { o.position.x - 0.5f  , o.position.y + 0.5f };
        }
    }

    void render()
    {
        updateVA();
        render_states.transform = vp_handler.getRenderState().transform;
        target.draw(objects_va, render_states);
    }
};
