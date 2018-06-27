#include "BigInt.h"

TBigInt::TBigInt(std::string &input)
{
    std::stringstream radixSS;
    if (input[0] == '0') {
        int i = 1;
        for (;input[i] == '0'; ++i);
        input = (i == (int) input.size()) ? "0" : input.substr(i);
    }
    mData.clear();
    for (int i = input.size() - 1; i >= 0; i -= BASE_POW) {
        int start = i - BASE_POW + 1;
        start = (start < 0) ? 0 : start;
        int end = i - start + 1;
        radixSS << input.substr(start, end);
        int radix = 0;
        radixSS >> radix;
        mData.push_back(radix);
        radixSS.clear();
    }
}

TBigInt::TBigInt(int n)
{
    if (n < BASE)
        mData.push_back(n);
    else {
        for(; n; n /= BASE)
            mData.push_back(n % BASE);
    }
}

TBigInt TBigInt::operator+(const TBigInt &obj)
{
    TBigInt res;
    int carry = 0;
    for (int i = 0; i < std::max(mData.size(), obj.mData.size()) || carry; ++i) {
        int aa = i < (int) mData.size() ? mData[i] : 0;
        int bb = i < (int) obj.mData.size() ? obj.mData[i] : 0;
        res.mData.push_back(aa + bb + carry);
        carry = res.mData.back() >= BASE;
        if (carry)
            res.mData.back() -= BASE;
    }
    return res;
}

TBigInt TBigInt::operator-(const TBigInt &obj)
{
    TBigInt res;
    int carry = 0;
    for (int i = 0; i < mData.size() || carry; ++i) {
        int aa = i < (int) mData.size() ? mData[i] : 0;
        int bb = i < (int) obj.mData.size() ? obj.mData[i] : 0;
        res.mData.push_back(aa - carry - bb);
        carry = res.mData.back() < 0;
        if (carry)
            res.mData.back() += BASE;
    }
    res.DeleteZeros();
    return res;
}

TBigInt TBigInt::operator*(const TBigInt &obj) const
{
    TBigInt res;
    res.mData.resize(mData.size() + obj.mData.size());
    int num1Size = (int) mData.size();
    int num2Size = (int) obj.mData.size();
    for (int i = 0; i < num1Size; ++i) {
        int carry = 0;
        for (int j = 0; j < num2Size || carry; ++j) {
            int bb = j < num2Size ? obj.mData[j] : 0;
            res.mData[i + j] += mData[i] * bb + carry;
            carry = res.mData[i + j] / BASE;
            res.mData[i + j] -= carry * BASE;
        }
    }
    res.DeleteZeros();
    return res;
}

TBigInt TBigInt::operator/(const TBigInt &obj)
{
    TBigInt res, cv = TBigInt(0);
    res.mData.resize(mData.size());
    for (int i = (int) mData.size() - 1; i >= 0; --i) {
        cv.mData.insert(cv.mData.begin(), mData[i]);
        if (!cv.mData.back())
            cv.mData.pop_back();
        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            TBigInt cur = obj * TBigInt(m);
            if (cur <= cv) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        res.mData[i] = x;
        cv = cv - obj * TBigInt(x);
    }
    res.DeleteZeros();
    return res;
}

TBigInt TBigInt::Power(int r)
{
    TBigInt res(1);
    while (r) {
        if (r % 2)
            res = res * (*this);
        (*this) = (*this) * (*this);
        r /= 2;
    }
    return res;
}

bool TBigInt::operator==(const TBigInt &obj) const
{
    return this->mData == obj.mData;
}

bool TBigInt::operator<(const TBigInt &obj) const
{
    if (mData.size() != obj.mData.size())
        return mData.size() < obj.mData.size();
    return std::lexicographical_compare(mData.rbegin(), mData.rend(), obj.mData.rbegin(), obj.mData.rend());
}

bool TBigInt::operator>(const TBigInt &obj) const
{
    if (mData.size() != obj.mData.size())
        return mData.size() > obj.mData.size();
    return std::lexicographical_compare(obj.mData.rbegin(), obj.mData.rend(), mData.rbegin(), mData.rend());
}

bool TBigInt::operator<=(const TBigInt &obj) const
{
    return (*this) < obj || (*this) == obj;
}

void TBigInt::DeleteZeros()
{
    while (mData.size() > 1 && !mData.back())
        mData.pop_back();
}

std::ostream &operator<<(std::ostream &stream, const TBigInt &num)
{
    int n = num.mData.size();
    if (!n)
        return stream;
    stream << num.mData[n - 1];
    for (int i = n - 2; i >= 0; --i)
        stream << std::setfill('0') << std::setw(BASE_POW) << num.mData[i];

    return stream;
}