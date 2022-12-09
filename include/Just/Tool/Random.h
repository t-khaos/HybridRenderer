#pragma once

#include <random>

class RNG
{
public:
    explicit RNG() : rngEngine(rd()) {}
    float UniformFloat()
    {
        return uniform(rngEngine);
    }
    float UniformFloat(float min, float max)
    {
        return min + (max - min) * uniform(rngEngine);
    }
private:
    std::random_device rd;
    std::mt19937_64 rngEngine;
    std::uniform_real_distribution<float> uniform{0.0f, 1.0f};
};