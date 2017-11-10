#include "rhomb.h"
#include <cmath>

#define PI 3.14159265

Rhomb::Rhomb() : Rhomb(0, 0) {}

Rhomb::Rhomb(const Rhomb& orig)
{
    side = orig.side;
    smaller_angle = orig.smaller_angle;
}

Rhomb::Rhomb(int32_t s, int32_t ang): side(s), smaller_angle(ang)
{
    if (smaller_angle > 180) {
        smaller_angle %= 180;
    }
    if (smaller_angle > 90) {
        smaller_angle = 180 - smaller_angle;
    }
}

void Rhomb::setParams(std::istream &is)
{
    std::cout << "Enter side: ";
    is >> side;
    std::cout << "Enter smaller angle: ";
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

double Rhomb::getSquare()
{
    return (double)(side * side * (double)sin(smaller_angle * (PI / 180)));
}

void Rhomb::Print()
{
    std::cout << "Side = " << side << ", smaller_angle = " << smaller_angle << ", square = " << this->getSquare() << ", type: rhomb" << std::endl;
}

Rhomb::Rhomb(std::istream &is)
{
    std::cout << "Enter side: ";
    is >> side;
    std::cout << "Enter smaller angle: ";
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

std::ostream& operator <<(std::ostream &os, const Rhomb &obj)
{
     os << "(" << obj.side << " " << obj.smaller_angle << ")," << "type: rhomb" << std::endl;
    return os;
}

std::istream& operator >>(std::istream &is, Rhomb &obj)
{
    std::cout << "Enter side: ";
    is >> obj.side;
    std::cout << "Enter smaller angle: ";
    is >> obj.smaller_angle;
    return is;
}

bool Rhomb::operator ==(const Rhomb &obj) const
{
    return smaller_angle == obj.smaller_angle && side == obj.side;
}


Rhomb Rhomb::operator =(const Rhomb &obj)
{
    if (&obj == this) {
        return *this;
    }

    smaller_angle = obj.smaller_angle;
    side = obj.side;

    return *this;
}