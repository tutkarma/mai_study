#ifndef PENTAGON_H
#define PENTAGON_H

#include <iostream>
#include <cstdint>
#include "figure.h"

class Pentagon : public Figure
{
public:
    Pentagon();
    Pentagon(std::istream& is);
    Pentagon(int32_t side);
    Pentagon(const Pentagon& orig);
    double Square() override;
    void Print() override;
    virtual ~Pentagon();

private:
    int32_t side;
};


#endif /*PENTAGON_H */