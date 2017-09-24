#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "trapeze.h"


Trapeze::Trapeze() : Trapeze(0, 0, 0, 0)
{
}

Trapeze::Trapeze(int32_t sb, int32_t bb, int32_t ls, int32_t rs): small_base(sb), big_base(bb), l_side(ls), r_side(rs)
{
    if (small_base > big_base) {
        std::swap(small_base, big_base);
    }
    std::cout << "Trapeze created: " << small_base << ", " << big_base << ", " << l_side << "," << r_side << std::endl;
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

Trapeze::Trapeze(const Trapeze& orig)
{
    std::cout << "Trapeze copy created" << std::endl;
    small_base = orig.small_base;
    big_base = orig.big_base;
    l_side = orig.l_side;
    r_side = orig.r_side;
}

double Trapeze::Square()
{
    if (big_base > small_base && small_base > 0) {
        return double(((small_base + big_base) / 2) * sqrt(l_side * l_side - 1 / 4 * 
            pow((l_side * l_side - r_side * r_side) / (big_base - small_base) + big_base - small_base, 2)));
    }
    else if (big_base == small_base) {
        if (l_side == r_side) {
            return l_side * small_base;
        }
        else {
            std::cerr << "Such trapeze can't exist" << std::endl;
        }
    } else {
        std::cerr << "Error: bigger base < smaller base" << std::endl;
    }
    return 0.0;
}

void Trapeze::Print()
{
    std::cout << "Smaller base = " << small_base << ", bigger base = " << big_base << ", left side = " <<  l_side << ", right side = " << r_side << std::endl;
}

Trapeze::~Trapeze()
{
}