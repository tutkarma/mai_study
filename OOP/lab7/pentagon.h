#ifndef PENTAGON_H
#define PENTAGON_H
#include "figure.h"

class Pentagon: public Figure
{
public:
    Pentagon();
    Pentagon(std::istream &is);
    Pentagon(int32_t side);
    Pentagon(const Pentagon &orig);
    void Print() override;
    double getSquare() override;
    void setParams(std::istream &is);
    bool operator ==(const Pentagon &obj) const;
    Pentagon operator =(const Pentagon &obj);
    friend std::ostream& operator <<(std::ostream &os, const Pentagon &obj);
    friend std::istream& operator >>(std::istream &is, Pentagon &obj);
    virtual ~Pentagon() {};
    int type() {return 3;}
private:
    int32_t side;
};

#endif