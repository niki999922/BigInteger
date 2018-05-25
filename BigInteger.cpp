/**
*
* @author Nikita Kochetkov M3134
* Date: 15.05.18
*
*/

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "BigInteger.hpp"   

const int standartBase = 1000000000;

//#define OLD

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(long long val) {
    if (val < 0) {
        sign = -1;
        val *= -1;
    } else {
        sign = 1;
    }
    data.clear();
    sizeOfNumber = 0;
    while (val > 0) {
        T cur = 0;
        std::vector<T> tmp;
        for (size_t i = 0; i < numOfBlocks && val > 0; ++i) {
            tmp.push_back(val % Base);
            ++sizeOfNumber;
            val /= Base;
        }
        std::reverse(tmp.begin(), tmp.end());
        for (size_t i = 0; i < tmp.size(); ++i) {
            cur = cur << numOfBits;
            cur += tmp[i];
        }
        data.push_back(cur);
    }
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const std::vector<T>& a, const int zn) {
    data = a;
    sign = zn;
    this->trim();
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const std::string &s) {
    std::vector<int> vect;
    int pos = 0, sign = 1;
    if (s[pos] == '-' || s[pos] == '+') {
        if (s[pos] == '-') {
            sign = -1;
        }
        ++pos;
    }
    for (int i = int(s.size()) - 1; i >= pos; i -= 9) {
        int x = 0;
        for (int j = std::max(pos, i - 9 + 1); j <= i; j++)
            x = x * 10 + (s[j] - '0');
        vect.push_back(x);
    }
    BigInteger<int, standartBase> tmp(vect, sign);
    *this = static_cast<BigInteger<T, Base>>(tmp);
}

template<typename T, size_t Base>
template<typename NewT, size_t NewBase>
BigInteger<T, Base>::operator BigInteger<NewT, NewBase>() {
    BigInteger<NewT, 10> power(1), res(0), bigBase(Base), bigNewBase(NewBase);
    for (size_t i = 0; i < Size(); ++i) {
        BigInteger<NewT, 10> digit((*this)[i]);
        BigInteger<NewT, 10> tmp;
        Mul(power, digit, tmp);
        res += tmp;
        Mul(power, bigBase, power);
    }
    NewT toData = 0;
    std::vector<NewT> vect;
    int index = 0;
    size_t numOfBitsNow = ceil(log2(NewBase));
    size_t numOfBlocksNow = sizeof(NewT) * 8 / numOfBitsNow;
    NewT ost = 0;
    while (res.Size() > 0) {
        ost = res % NewBase;
        res /= NewBase;
        toData += (ost << (index % numOfBlocksNow) * numOfBitsNow);
        if (index % numOfBlocksNow == numOfBlocksNow - 1) {
            vect.push_back(toData);
            toData = 0;
        }
        ++index;
    }
    if (toData != 0) {
        vect.push_back(toData);
    }
    BigInteger<NewT, NewBase> ans(vect, sign);
    return ans;
}

template<typename T, size_t Base>
void Mul(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, BigInteger<T, Base> &result) {
    std::vector<T> construct(first.Size() + second.Size(), 0), dataConstruct;
    for (size_t i = 0; i < first.Size(); ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < second.Size() || carry; ++j) {
            unsigned long long cur = construct[i + j] + first[i] * (j < int(second.Size()) ? second[j] : 0) + carry;
            construct[i + j] = cur % Base;
            carry = cur / Base;
        }
    }
    T toData = 0;
    for (size_t i = 0; i < construct.size(); ++i) {
        toData += (construct[i] << ((i % first.numOfBlocks) * first.numOfBits));
        if (i % first.numOfBlocks == first.numOfBlocks - 1) {
            dataConstruct.push_back(toData);
            toData = 0;
        }
    }
    if (toData != 0) {
        dataConstruct.push_back(toData);
        toData = 0;
    }
    result.data = dataConstruct;
    result.sign = first.sign * second.sign;
    result.trim();
}

template<typename T, size_t Base>
void Div(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, BigInteger<T, Base> &result) {
    BigInteger<T, Base> curVal(0), bigBase(Base);
    std::vector<T> construct, dataConstruct;
    for (int i = int(first.Size()) - 1; i >= 0; --i) {
        BigInteger<T, Base> digit(first[i]);
        Mul(curVal, bigBase, curVal);
        curVal += digit;
        T x = 0, l = 0, r = Base;
        while (l <= r) {
            T m = (l + r) >> 1;
            BigInteger<T, Base> cur, bigM(m);
            Mul(second, bigM, cur);
            if (cur <= curVal) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        construct.push_back(x);
        BigInteger<T, Base> bigX(x), substract;
        Mul(second, bigX, substract);
        curVal = curVal - substract;
    }
    T toData = 0;
    int index = 0;
    for (int i = int(construct.size()) - 1; i >= 0; --i) {
        toData += (construct[i] << ((index % first.numOfBlocks) * first.numOfBits));
        if (index % first.numOfBlocks == first.numOfBlocks - 1) {
            dataConstruct.push_back(toData);
            toData = 0;
        }
        ++index;
    }
    if (toData != 0) {
        dataConstruct.push_back(toData);
        toData = 0;
    }
    result.data = dataConstruct;
    result.sign = first.sign * second.sign;
    result.trim();
}

template<typename T, size_t Base>
void Mod(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, T &result) {
    BigInteger<T, Base> curVal(0), bigBase(Base);
    std::vector<T> construct;
    for (int i = int(first.Size()) - 1; i >= 0; --i) {
        BigInteger<T, Base> digit(first[i]);
        Mul(curVal, bigBase, curVal);
        curVal += digit;
        T x = 0, l = 0, r = Base;
        while (l <= r) {
            T m = (l + r) >> 1;
            BigInteger<T, Base> cur, bigM(m);
            Mul(second, bigM, cur);
            if (cur <= curVal) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        construct.push_back(x);
        BigInteger<T, Base> bigX(x), substract;
        Mul(second, bigX, substract);
        curVal = curVal - substract;
    }
    T power = 1;
    result = 0;
    for (size_t i = 0; i < curVal.Size(); i++) {
        result += (curVal[i] * power);
        power *= Base;
    }
}

#ifndef OLD
template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::Add(BigInteger &val) const {
    T carry = 0;  
    for (size_t i = 0; i < std::max(val.data.size(), data.size()) || carry; ++i) {
        if (i == val.data.size()) {
            val.data.push_back(0);
            val.sizeOfNumber += numOfBlocks;
        }
        T toData = 0;
        for (size_t j = 0; j < numOfBlocks; ++j) {
            T first = val[i * numOfBlocks + j],
                second = (i < data.size()) ? (*this)[i * numOfBlocks + j] : 0;
            toData = toData + (((first + second + carry) % T(Base)) << j * numOfBits);
            carry = (first + second + T(carry) >= T(Base)) ? 1 : 0;
        }
        val.data[i] = toData;
    }
    val.trim();
    return val;
}


template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::Subtract(const BigInteger &val) {
    T carry = 0; 
    for (size_t i = 0; i < val.data.size() || carry; ++i) {
        T toData = 0;
        for (size_t j = 0; j < numOfBlocks; ++j) {
            T first = (*this)[i * numOfBlocks + j],
                second = (i < val.data.size()) ? val[i * numOfBlocks + j] : 0;
            toData = toData + (((first + T(Base) - second - carry) % T(Base)) << j * numOfBits);
            carry = (first < second + carry) ? 1 : 0;
        }
        (*this).data[i] = toData;
    }
    (*this).trim();
    return *this;
}
#endif

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const BigInteger &val) {
    data = val.data;
    sign = val.sign;
    sizeOfNumber = val.sizeOfNumber;
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(BigInteger &&val) {
    data = std::move(val.data);
    sign = val.sign;
    sizeOfNumber = val.sizeOfNumber;
}

template<typename T, size_t Base>
size_t BigInteger<T, Base>::Size() const {
    return sizeOfNumber;
}

template<typename T, size_t Base>
T BigInteger<T, Base>::operator[](size_t id) const {
    size_t pos = id / numOfBlocks,
        shift = id % numOfBlocks;
    T now = data[pos] >> (numOfBits * shift),
        toAnd = (T(1) <<  numOfBits) - 1;
    T res = now & toAnd;
    return res;
}

template<typename T, size_t Base>
BigInteger<T, Base>& BigInteger<T, Base>::operator=(BigInteger<T, Base> &val) {
    data = val.data;
    sign = val.sign;
    sizeOfNumber = val.sizeOfNumber;
    return *this;
}

#ifndef OLD
template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator+=(const BigInteger &val) {
    if (sign == val.sign) {
        return val.Add(*this);
    } else {
        return *this -= (-val);
    }
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator-=(const BigInteger &val) {
    if (sign == val.sign) {
        if ((*this).abs() >= val.abs()) {
            return Subtract(val);
        }
        return *this = -(val - *this);
    }
    return *this += (-val);
}
#endif


#ifdef OLD
template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator+(const BigInteger &val) const {
    if (sign == val.sign) {
        BigInteger<T, Base> ans = val;
        T carry = 0;
        for (size_t i = 0; i < std::max(val.data.size(), data.size()) || carry; ++i) {
            if (i == ans.data.size()) {
                ans.data.push_back(0);
                ans.sizeOfNumber += numOfBlocks;
            }
            T toData = 0;
                for (size_t j = 0; j < numOfBlocks; ++j) {
                    T first = ans[i * numOfBlocks + j],
                        second = (i < data.size()) ? (*this)[i * numOfBlocks + j] : 0;
                    if (first + second + T(carry) >= T(Base)) {
                        toData = toData + ((first + second + carry - T(Base)) << j * numOfBits);
                        carry = 1;
                    } else {
                        toData = toData + ((first + second + carry) << j * numOfBits);
                        carry = 0;
                    }
                }
            ans.data[i] = toData;
        }
        ans.trim();
        return ans;
    } else {
        return *this - (-val);
    }
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator-(const BigInteger &val) const {
    if (sign == val.sign) {
        if ((*this).abs() >= val.abs()) {
            BigInteger<T, Base> ans = *this;
            T carry = 0;
            for (size_t i = 0; i < val.data.size() || carry; ++i) {
                T toData = 0;
                for (size_t j = 0; j < numOfBlocks; ++j) {
                    T first = ans[i * numOfBlocks + j],
                        second = (i < val.data.size()) ? val[i * numOfBlocks + j] : 0;
                    if (first < second + carry) {
                        toData = toData + ((first + T(Base) - second - carry) << j * numOfBits);
                        carry = 1;
                    } else {
                        toData = toData + ((first - second - carry) << j * numOfBits);
                        carry = 0;
                    }
                }
                ans.data[i] = toData;
            }
            ans.trim();
            return ans;
        }
        return -(val - *this);
    }
    return *this + (-val);
}
#endif

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator*(BigInteger &val) {
    BigInteger<T, Base> ans(0);
    Mul(*this, val, ans);
    return ans;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator*(long long val) const {
    if (Size() == 0) {
        return *this;
    }
    BigInteger<T, Base> copy = *this;
    if (val < 0) {
        copy.sign *= -1;
        val *= -1;
    }
    std::vector<T> construct;
    unsigned long long carry = 0;
    T toData = 0;
    for (int i = 0; i < copy.Size(); ++i) {
        unsigned long long cur = carry + copy[i] * val;
        carry = cur / Base;
        toData += (T(cur % Base) << ((i % numOfBlocks) * numOfBits));
        if (i % numOfBlocks == numOfBlocks - 1) {
            construct.push_back(toData);
            toData = 0;
        }
    }
    if (toData != 0 || carry != 0) {
        toData += (T(carry) << ((copy.Size() % numOfBlocks) * numOfBits));
        construct.push_back(toData);
        toData = 0;
        carry = 0;
    }    
    BigInteger<T, Base> ans(construct, copy.sign);
    ans.trim();
    return ans;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator/(BigInteger &val) {
    if (val == BigInteger<T, Base>(0)) throw "division by zero";
    BigInteger<T, Base> ans(0);
    Div(*this, val, ans);
    return ans;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator/(long long val) const {
    if (val == 0) throw "division by zero";
    if (Size() == 0) {
        return *this;
    }
    BigInteger<T, Base> copy = *this;
    if (val < 0) {
        copy.sign *= -1;
        val *= -1;
    }
    unsigned long long ost = 0;
    T toData = 0;
    int index = copy.data.size() - 1;
    for (int i = int(copy.Size()) - 1; i >= 0; --i) {
        unsigned long long cur = ost * Base + copy[i];
        ost = cur % val;
        toData += (T(cur / val) << ((i % numOfBlocks) * numOfBits));
        if (i % numOfBlocks == 0) {
            copy.data[index] = toData;
            toData = 0;
            --index;
        }
    }
    if (toData != 0 || ost)
    copy.trim();
    return copy;
}

template<typename T, size_t Base>
long long BigInteger<T, Base>::operator%(long long val) const {
    if (Size() == 0) {
        return 0;
    }
    unsigned long long ost = 0;
    for (int i = int(Size()) - 1; i >= 0; i--) {
        ost = (ost * Base + (*this)[i]) % val;
    }
    return ost;
}

#ifndef OLD
template<typename T, size_t Base>
BigInteger<T, Base>  BigInteger<T, Base>::operator+(const BigInteger &val) const {
    BigInteger ans = *this;
    return ans += val;
}

template<typename T, size_t Base>
BigInteger<T, Base>  BigInteger<T, Base>::operator-(const BigInteger &val) const {
    BigInteger ans = *this;
    return ans -= val;
}
#endif

#ifdef OLD
template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator+=(const BigInteger &val) {
    *this = *this + val;
    return *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator-=(const BigInteger &val) {
    *this = *this - val;
    return *this;
}
#endif

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator*=(const BigInteger &val) {
    *this = *this * val;
    return *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> & BigInteger<T, Base>::operator/=(long long val) {
    *this = (*this) / val;
    return *this;
}

template<typename T, size_t Base>
long long BigInteger<T, Base>::operator%=(long long val) const {
    long long res = (*this) % val;
    return res;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator-() const {
    BigInteger<T, Base> res = *this;
    res.sign = -sign;
    return res;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::operator+() const {
    return *this;
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator<(const BigInteger &val) const {
    if (sign != val.sign)
        return sign < val.sign;
    if ((*this).Size() != val.Size())
        return (*this).Size() < val.Size();
    for (int i = int(val.Size()) - 1; i >= 0; --i)
        if ((*this)[i] != val[i])
            return (*this)[i] * sign < val[i] * val.sign;
    return false;
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator>(const BigInteger &val) const {
    return val < *this;
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator<=(const BigInteger &val) const {
    return !(val < *this);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator>=(const BigInteger &val) const {
    return !(*this < val);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator==(const BigInteger &val) const {
    return !(*this < val) && !(val < *this);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator!=(const BigInteger &val) const {
    return *this < val || val < *this;
}

template<typename T, size_t Base>
BigInteger<T, Base> BigInteger<T, Base>::abs() const {
    BigInteger<T, Base> res = *this;
    res.sign *= res.sign;
    return res;
}

template<typename T, size_t Base>
void BigInteger<T, Base>::trim() {
    while (!data.empty() && data.back() == 0) {
        data.pop_back();
    }
    sizeOfNumber = numOfBlocks * data.size();
    while (!data.empty() && (*this)[sizeOfNumber - 1] == 0)
        --sizeOfNumber;
    if (data.empty()) {
        sign = 1;
    }
}

template<typename T, size_t Base>
std::string BigInteger<T, Base>::toString(BigInteger<T, Base> &val) {
    auto tmp = static_cast<BigInteger<int, 10>>(val);
    std::sstream stream;
    if (tmp.sign == -1) {
        stream << '-';
    }
    if (tmp.Size() == 0) {
        stream << 0;
    } else {
        for (int i = int(tmp.Size()) - 1; i >= 0; i--) {
            stream << tmp[i];
        }
    }
    std::string result;
    stream >> result;
    return result;
}

template<typename T, size_t Base>
std::ostream & operator<<(std::ostream &stream, BigInteger<T, Base> &val) {
    auto tmp = static_cast<BigInteger<int, 10>>(val);
    if (tmp.sign == -1) {
        stream << '-';
    }
    if (tmp.Size() == 0) {
        stream << 0;
    } else {
        for (int i = int(tmp.Size()) - 1; i >= 0; i--) {
            stream << tmp[i];
        }
    }
    return stream;
}

template<typename T, size_t Base>
std::istream & operator>>(std::istream &stream, BigInteger<T, Base> &val) {
    std::string s;
    stream >> s;
    val = BigInteger<T, Base>(s);
    return stream;
}
