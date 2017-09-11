#ifndef TRAPEZE_H
#define TRAPEZE_H

#include <iostream>
#include <cstdint>
#include "figure.h"

class Trapeze : public Figure
{
public:
    Trapeze();
    Trapeze(std::istream &is);
    Trapeze(int32_t small_base, int32_t big_base, int32_t l_side, int32_t r_side);
    Trapeze(const Trapeze& orig);
    double Square() override;
    void Print() override;
    virtual ~Trapeze();

private:
    int32_t small_base;
    int32_t big_base;
    int32_t l_side;
    int32_t r_side;
};

#endif /* TRAPEZE_H */
