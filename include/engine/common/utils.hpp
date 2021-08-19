#pragma once
#include "index_vector.hpp"
#include <sstream>


template<typename U, typename T>
U to(const T& v)
{
    return static_cast<U>(v);
}


template<typename T>
using CIVector = civ::Vector<T>;


template<typename T>
T sign(T v)
{
    return v < 0.0f ? -1.0f : 1.0f;
}


template<typename T>
static std::string toString(T value)
{
    std::stringstream sx;
    sx << value;
    return sx.str();
}


template<typename T>
float getLength(sf::Vector2<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}


template<typename T>
float dot(sf::Vector2<T> v1, sf::Vector2<T> v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}


template<typename T>
sf::Vector2f toVector2f(sf::Vector2<T> v)
{
    return {to<float>(v.x), to<float>(v.y)};
}
