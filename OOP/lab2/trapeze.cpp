#include <iostream>
#include <cmath>
#include <algorithm>
#include "trapeze.h"


Trapeze::Trapeze() : Trapeze(0, 0, 0, 0)
{
}

Trapeze::Trapeze(int32_t sb, int32_t bb, int32_t ls, int32_t rs): small_base(sb), big_base(bb), l_side(ls), r_side(rs)
{
    if (small_base > big_base) {
        std::swap(small_base, big_base);
    }
    //std::cout << "Trapeze created: " << small_base << ", " << big_base << ", " << l_side << ", " << r_side << std::endl;
}

Trapeze::Trapeze(std::istream &is)
{
    is >> small_base;
    is >> big_base;
    is >> l_side;
    is >> r_side;
    if (small_base > big_base) {
        std::swap(small_base, big_base);
    }
    if(small_base < 0 || big_base < 0 || l_side < 0 || r_side < 0) {
        std::cerr << "Error: sides should be > 0." << std::endl;
    }
}

Trapeze::Trapeze(const Trapeze &orig)
{
    small_base = orig.small_base;
    big_base = orig.big_base;
    l_side = orig.l_side;
    r_side = orig.r_side;
}

double Trapeze::Square()
{
    double h = std::sqrt(l_side * l_side - 0.25 * std::pow((l_side * l_side - r_side * r_side)
        / (big_base - small_base) + big_base - small_base, 2.0));
    return (big_base + small_base) / 2 + h;
}

void Trapeze::Print()
{
    std::cout << "Smaller base = " << small_base << ", bigger base = " << big_base << ", left side = " <<  l_side << ", right side = " << r_side << std::endl;
}

Trapeze::~Trapeze()
{
}

std::ostream& operator <<(std::ostream &os, const Trapeze &obj)
{
    os << obj.small_base << " " << obj.big_base << " " << obj.l_side << " " << obj.r_side;
    return os;
}

std::istream& operator >>(std::istream &is, Trapeze &obj)
{
    std::cout << "Enter bigger base: ";
    is >> obj.big_base;
    std::cout << "Enter smaller base: ";
    is >> obj.small_base;
    std::cout << "Enter left side: ";
    is >> obj.l_side;
    std::cout << "Enter right side: ";
    is >> obj.r_side;
    return is;
}

bool Trapeze::operator ==(const Trapeze &obj) const
{
    return small_base == obj.small_base && big_base == obj.big_base && l_side == obj.l_side && r_side == obj.r_side;
}


Trapeze& Trapeze::operator =(const Trapeze &obj)
{
    if (&obj == this) {
        return *this;
    }

    small_base = obj.small_base;
    big_base = obj.big_base;
    l_side = obj.l_side;
    r_side = obj.r_side;

    return *this;
}


