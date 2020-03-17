#pragma once

#include <random>
#include <functional>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);
auto realRand = std::bind(dis, gen);