#pragma once

#include <random>
#include <chrono>
#include <functional>

auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
static std::mt19937 gen(seed);
static std::uniform_real_distribution<> dis(0.0, 1.0);
static auto realRand = std::bind(dis, gen);