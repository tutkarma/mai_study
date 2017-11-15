#ifndef PENTAGON_H
#define PENTAGON_H

#include <iostream>
#include <cstdint>
#include "figure.h"

class Pentagon : public Figure
{
public:
    Pentagon();
    Pentagon(std::istream &is);
    Pentagon(int32_t side);
    Pentagon(const Pentagon &orig);

    bool operator ==(const Pentagon &obj) const;
    Pentagon& operator =(const Pentagon &obj);
    friend std::ostream& operator <<(std::ostream &os, const Pentagon &obj);
    friend std::istream& operator >>(std::istream &is, Pentagon &obj);

    double Square() override;
    void Print() override;
    virtual ~Pentagon();

private:
    int32_t side;
};


#endif /*PENTAGON_H */