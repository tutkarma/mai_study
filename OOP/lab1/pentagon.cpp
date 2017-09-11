#include <cmath>
#include <cstring>
#include <iostream>
#include "pentagon.h"

#define PI 3.14159265

Pentagon::Pentagon() : Pentagon(0)
{
}

Pentagon::Pentagon(int32_t s): side(s)
{
    std::cout << "Pentagon created: " << side << std::endl;
}

Pentagon::Pentagon(std::istream& is)
{
    is >> side;
    if(side < 0) {
        std::cerr << "Error: sides should be > 0." << std::endl;
    }
}

Pentagon::Pentagon(const Pentagon& orig)
{
    std::cout << "Pentagon copy created" << std::endl;
    side = orig.side;
}

double Pentagon::Square()
{
    return (double)(5 * side * side / (4 * (double)tan(36 * (PI / 180))));
}

void Pentagon::Print()
{
    std::cout << "Sides =  " << side << std::endl;
}

Pentagon::~Pentagon()
{
}
