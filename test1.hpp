#pragma once

#include <iostream>
extern float realRand();

float externRand(){
    float a = realRand();
    std::cout << a << std::endl;
    return a;
}