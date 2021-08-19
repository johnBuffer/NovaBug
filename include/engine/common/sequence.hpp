#pragma once
#include <SFML/System.hpp>


template<typename T>
class Sequence
{
public:
    Sequence() = default;
    
    void add(const T& obj, float duration_sec)
    {
        seq_data.push_back(obj);
        durations.push_back(duration_sec);
    }
    
    void update(float dt)
    {
        if (timeOver()) {
            next();
        }
    }
    
    const T& getCurrent() const
    {
        return seq_data[current_i];
    }
    
private:
    sf::Clock clock;
    std::vector<T> seq_data;
    std::vector<float> durations;
    uint32_t current_i = 0;
    
    bool timeOver() const
    {
        return clock.getElapsedTime().asSeconds() > durations[current_i];
    }
    
    void next()
    {
        clock.restart();
        current_i = (current_i + 1) % seq_data.size();
    }
    
};
