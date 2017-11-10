#ifndef TRAPEZE_H
#define TRAPEZE_H
#include <iostream>
#include <cstdint>
#include "figure.h"

class Trapeze: public Figure
{
public:
    Trapeze();
    Trapeze(const Trapeze &orig);
    Trapeze(int32_t small_base, int32_t big_base, int32_t l_side, int32_t r_side);
    Trapeze(std::istream &is);

    friend std::ostream& operator <<(std::ostream &os, const Trapeze &obj);
    friend std::istream& operator >>(std::istream &is, Trapeze &obj);

    bool operator ==(const Trapeze &obj) const;
    Trapeze operator =(const Trapeze &obj);

    double getSquare() override;
    void Print() override;
    void setParams(std::istream &is);
    virtual ~Trapeze() {};
    int type() {return 1;}
private:
    int32_t small_base;
    int32_t big_base;
    int32_t l_side;
    int32_t r_side;
};

#endif