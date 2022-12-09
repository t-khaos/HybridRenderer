#pragma once

#include <chrono>

struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    float time;

    Timer() : time(0) {}

    void Begin() {
        time = 0;
        start = std::chrono::high_resolution_clock::now();
    }

    void End() {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        time = duration.count() * 1000.0f;
    }
};