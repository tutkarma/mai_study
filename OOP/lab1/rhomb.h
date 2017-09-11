#ifndef RHOMB_H
#define RHOMB_H

#include <iostream>
#include <cstdint>
#include "figure.h"

class Rhomb : public Figure
{
public:
    Rhomb();
    Rhomb(std::istream &is);
    Rhomb(int32_t side, int32_t smaller_angle);
    Rhomb(const Rhomb& orig);
    double Square() override;
    void Print() override;
    virtual ~Rhomb();

private:
    int32_t side;
    int32_t smaller_angle;
};

#endif /* RHOMB_H */