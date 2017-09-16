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
    //std::cout << "Pentagon created: " << side << std::endl;
}

Pentagon::Pentagon(std::istream& is)
{
    std::cout << "Enter side: ";
    is >> side;
    if(side < 0) {
        std::cerr << "Error: sides should be > 0." << std::endl;
    }
}

Pentagon::Pentagon(const Pentagon& orig)
{
    side = orig.side;
}

double Pentagon::Square()
{
    return (double)(5 * side * side / (4 * (double)tan(36 * (PI / 180))));
}

void Pentagon::Print()
{
    std::cout << "Sides =  " << side << ", type: pentagon" << std::endl;
}

Pentagon::~Pentagon()
{
}

std::ostream& operator <<(std::ostream &os, const Pentagon &obj)
{
    os << "(" << obj.side << ")," << "type: pentagon" << std::endl;
    return os;
}

std::istream& operator >>(std::istream &is, Pentagon &obj)
{
    std::cout << "Enter side: ";
    is >> obj.side;
    return is;
}

bool Pentagon::operator ==(const Pentagon &obj) const
{
    return side == obj.side;
}


Pentagon& Pentagon::operator =(const Pentagon &obj)
{
    if (&obj == this) {
        return *this;
    }

    side = obj.side;

    return *this;
}