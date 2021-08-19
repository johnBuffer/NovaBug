#include <SFML/Graphics.hpp>
#include "engine/engine.hpp"
#include "engine/render/viewport_handler.hpp"
#include "renderer.hpp"
#include "engine/common/event_manager.hpp"
#include "engine/common/number_generator.hpp"
#include "world.hpp"


int main()
{
    // Create context
    const int32_t window_width = 1920;
    const int32_t window_height = 1080;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "VPE Object Builder", sf::Style::Fullscreen, settings);
    window.setFramerateLimit(0);
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(false);

    bool slow_mo = false;
    
    Engine engine(500, 500);
    sfev::EventManager ev_manager(window, true);
    ev_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) { window.close(); });
    ev_manager.addKeyPressedCallback(sf::Keyboard::Escape, [&](sfev::CstEv) { window.close(); });
    ev_manager.addKeyPressedCallback(sf::Keyboard::S, [&](sfev::CstEv) { slow_mo = !slow_mo; window.setFramerateLimit(144 * slow_mo); });
    ev_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) {
        ev_manager.getWindow().close();
    });
    ev_manager.addKeyPressedCallback(sf::Keyboard::Escape, [&](sfev::CstEv) {
        ev_manager.getWindow().close();
    });
        
    const float dt = 0.016f;

    const uint32_t objects_count = 10000;
    for (uint32_t i(objects_count); i--;) {
        PhysicObject o;
        o.setPosition({ RNGf::getUnder(engine.solver.world_size.x), RNGf::getUnder(engine.solver.world_size.y) });
        engine.addEntity(o);
    }
    engine.solver.gravity = sf::Vector2f(0.0f, 0.0f);

    Renderer renderer(window, engine.solver.objects);
    renderer.registerViewportHandlerControls(ev_manager);

    while (window.isOpen()) {
        ev_manager.processEvents();
        
        const float force = 100.0f;
        const sf::Vector2f center = engine.solver.world_size * 0.5f;
        for (PhysicObject& obj : engine.solver.objects) {
            const sf::Vector2f to_center = center - obj.position;
            const sf::Vector2f to_center_v = to_center / getLength(to_center);
            obj.accelerate(to_center_v * force);
        }

        engine.update(dt);

        for (PhysicObject& obj : engine.solver.objects) {
            obj.slowdown(0.01f);
        }

        //window.clear();
        sf::RectangleShape fade({ window_width, window_height });
        fade.setFillColor({ 0, 0, 0, 10 });
        window.draw(fade);

        renderer.render();
        
        window.display();
    }
    
	return 0;
}

#if defined(_WIN32)
#include <windows.h>
int APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
  return main();
}
#endif
