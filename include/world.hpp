#pragma once
#include "engine/common/grid.hpp"


struct WorldCell
{
    float dist;
    bool wall;
};


template<typename T>
struct FixedQueue
{
    std::vector<T> data;
    int32_t front_i;
    int32_t back_i;
    
    FixedQueue(uint32_t size = 0)
        : data(size)
        , front_i(0)
        , back_i(0)
    {
        
    }
    
    void pushBack(const T& v)
    {
        data[back_i] = v;
        ++back_i;
    }
    
    void pushFront(const T& v)
    {
        
    }
    
    uint32_t getSize() const
    {
        // To be check with tricky cases
        return back_i - front_i;
    }
    
    T pop_back()
    {
        return data[back_i--];
    }
    
    void clear()
    {
        front_i = 0;
        back_i = 0;
    }
};


struct World
{
    Grid<WorldCell> grid;
    FixedQueue<uint32_t> to_check;
    
    World(uint32_t width, uint32_t height)
        : grid(width, height)
        , to_check(width * height)
    {
        
    }
    
    void updateDistances(sf::Vector2i exit)
    {
        to_check.pushBack(grid.getID(exit));
        while (to_check.getSize()) {
            
        }
    }
    
    void addWall(int32_t x, int32_t y)
    {
        grid.get(x, y).wall = true;
    }
};
