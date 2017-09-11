#include <iostream>
#include <cmath>
#include "rhomb.h"

#define PI 3.14159265


Rhomb::Rhomb() : Rhomb(0, 0)
{
}

Rhomb::Rhomb(int32_t s, int32_t ang): side(s), smaller_angle(ang)
{
    if (smaller_angle > 180) {
        smaller_angle %= 180;
    }
    if (smaller_angle > 90) {
        smaller_angle = 180 - smaller_angle;
    }
    std::cout << "Rhomb created: " << side << ", " << smaller_angle << std::endl;
}

Rhomb::Rhomb(std::istream &is)
{
    is >> side;
    is >> smaller_angle;
    if (smaller_angle > 90 && smaller_angle < 180) {
        smaller_angle = 180 - smaller_angle;
    }
    if(side < 0) {
        std::cerr << "Error: sides should be > 0." << std::endl;
    }
    if(smaller_angle < 0 || smaller_angle > 180) {
        std::cerr << "Error: angles should be > 0 and < 180." << std::endl;
    }
}

Rhomb::Rhomb(const Rhomb& orig)
{
    std::cout << "Rhomb copy created" << std::endl;
    side = orig.side;
    smaller_angle = orig.smaller_angle;
}

double Rhomb::Square()
{
    return (double)(side * side * (double)sin(smaller_angle * (PI / 180)));
}

void Rhomb::Print()
{
    std::cout << "Side = " << side << ", smaller_angle = " << smaller_angle << std::endl;
}

Rhomb::~Rhomb()
{
}