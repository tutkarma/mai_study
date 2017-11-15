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

    bool operator ==(const Rhomb &obj) const;
    Rhomb& operator =(const Rhomb &obj);
    friend std::ostream& operator <<(std::ostream &os, const Rhomb &obj);
    friend std::istream& operator >>(std::istream &is, Rhomb &obj);

    double Square() override;
    void Print() override;
    virtual ~Rhomb();

private:
    int32_t side;
    int32_t smaller_angle;
};

#endif /* RHOMB_H */