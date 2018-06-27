#ifndef _TBIGINT_H_
#define _TBIGINT_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

const int BASE = 10000;
const int BASE_POW = 4;

class TBigInt
{
public:
    TBigInt() {};
    TBigInt(std::string&);
    TBigInt(int n);
    ~TBigInt() {};

    TBigInt operator+(const TBigInt&);
    TBigInt operator-(const TBigInt&);
    TBigInt operator*(const TBigInt&) const;
    TBigInt operator/(const TBigInt&);
    TBigInt Power(int r);
    bool operator==(const TBigInt&) const;
    bool operator<(const TBigInt&) const;
    bool operator>(const TBigInt&) const;
    bool operator<=(const TBigInt&) const;
    friend std::ostream& operator<<(std::ostream&, const TBigInt&);

private:
    void DeleteZeros();
    std::vector<int> mData;
};

#endif