#ifndef RHOMB_H
#define RHOMB_H
#include <iostream>
#include <cstdint>
#include "figure.h"

class Rhomb: public Figure
{
public:
    Rhomb();
    Rhomb(const Rhomb& orig);
    Rhomb(std::istream &is);
    Rhomb(int32_t side, int32_t smaller_angle);
    void setParams(std::istream &is);
    double getSquare() override;
    void Print() override;
    bool operator ==(const Rhomb &obj) const;
    Rhomb operator =(const Rhomb &obj);

    friend std::ostream& operator <<(std::ostream &os, const Rhomb &obj);
    friend std::istream& operator >>(std::istream &is, Rhomb &obj);

    ~Rhomb() {};
    int type() {return 2;}
private:
    int32_t side;
    int32_t smaller_angle;
};

#endif