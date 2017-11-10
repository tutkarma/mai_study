#include "pentagon.h"
#include <cmath>
#include <cstring>

#define PI 3.14159265

Pentagon::Pentagon() : Pentagon(0) {}

Pentagon::Pentagon(int32_t s): side(s)
{
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

void Pentagon::setParams(std::istream &is)
{
    std::cout << "Enter side: ";
    is >> side;
    if(side < 0) {
        std::cerr << "Error: sides should be > 0." << std::endl;
    }
}

void Pentagon::Print()
{
    std::cout << "Sides =  " << side << ", square = " << this->getSquare() << ", type: pentagon" << std::endl;
}

bool Pentagon::operator ==(const Pentagon &obj) const
{
    return side == obj.side;
}


Pentagon Pentagon::operator =(const Pentagon &obj)
{
    if (&obj == this) {
        return *this;
    }

    side = obj.side;

    return *this;
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


double Pentagon::getSquare()
{
    return (double)(5 * side * side / (4 * (double)tan(36 * (PI / 180))));
}