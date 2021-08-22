#include "engine/common/grid.hpp"


struct GravityCell
{
    sf::Vector2f position;
    float mass = 0.0f;
    
    sf::Vector2f getForce(sf::Vector2f pos) const
    {
        const sf::Vector2f to_cell = position - pos;
        const float dist2 = to_cell.x * to_cell.x + to_cell.y * to_cell.y;
        const float dist = sqrt(dist2);
        return (mass / (1.0f + dist2 * dist)) * to_cell;
    }
};


struct GravityGrid
{
    const uint32_t width_size = 16;
    const float cell_size = 500.0f / to<float>(width_size);
    const float inv_cell_size = 1.0f / cell_size;
    Grid<GravityCell> grid;
    
    GravityGrid()
        : grid(width_size, width_size)
    {
        for (uint32_t x(0); x<grid.width; ++x) {
            for (uint32_t y(0); y<grid.height; ++y) {
                grid.get(x, y).position = sf::Vector2f(cell_size * (0.5f + x), cell_size * (0.5f + y));
            }
        }
    }
    
    void clear()
    {
        for (GravityCell& c : grid.data) {
            c.mass = 0.0f;
        }
    }
    
    void addMass(sf::Vector2f p)
    {
        const int32_t cell_x = p.x * inv_cell_size;
        const int32_t cell_y = p.y * inv_cell_size;
        if (cell_x > 0 && cell_x < grid.width && cell_y > 0 && cell_y < grid.height) {
            grid.get(cell_x, cell_y).mass += 0.1f;
        }
    }
    
    void applyGravity(PhysicObject& obj)
    {
        for (const GravityCell& c : grid.data) {
            obj.accelerate(c.getForce(obj.position));
        }
    }
};
