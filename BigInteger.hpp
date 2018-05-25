#pragma once

#include <fstream>
#include <vector>

template<typename T, size_t Base = std::numeric_limits<T>::max()>
class BigInteger;

template<typename T, size_t Base>
class BigInteger {
public:
	explicit BigInteger(long long val = 0);
	explicit BigInteger(const std::string & s);
	explicit BigInteger(const std::vector<T> & a, const int zn);
	template<typename NewT, size_t NewBase>
	explicit operator BigInteger<NewT, NewBase>();

	BigInteger(const BigInteger &);
	BigInteger(BigInteger &&);

	size_t Size() const; 
    std::string toString(BigInteger &val);
	T operator[](size_t id) const;

	BigInteger& operator=(BigInteger &val);
	bool operator<(const BigInteger &val) const;
	bool operator>(const BigInteger &val) const;
	bool operator<=(const BigInteger &val) const;
	bool operator>=(const BigInteger &val) const;
	bool operator==(const BigInteger &val) const;
	bool operator!=(const BigInteger &val) const;
	BigInteger abs() const;

	BigInteger & operator +=(const BigInteger &val);
	BigInteger operator +(const BigInteger &val) const;
	BigInteger & operator -=(const BigInteger &val);
	BigInteger operator -(const BigInteger &val) const;
	BigInteger & operator *=(const BigInteger &val);
	BigInteger operator *(BigInteger &val); 
	BigInteger & operator /=(long long val);
    BigInteger operator /(BigInteger &val);

    BigInteger operator *(long long val) const;
	BigInteger operator /(long long val) const;
	long long operator %=(long long val) const;
	long long operator %(long long val) const;
	BigInteger operator-() const;
	BigInteger operator+() const;
	
private:
	void trim();
	template<typename T, size_t Base>
	friend std::ostream & operator <<(std::ostream &stream, BigInteger<T, Base> &num);
	template<typename T, size_t Base>
	friend std::istream & operator >>(std::istream &stream, BigInteger<T, Base> &num);	
	template<typename T, size_t Base>
	friend void Mul(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, BigInteger<T, Base> &result);
	template<typename T, size_t Base>
	friend void Div(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, BigInteger<T, Base> &result);
	template<typename T, size_t Base>
	friend void Mod(const BigInteger<T, Base> &first, const BigInteger<T, Base> &second, T &result);
    
    BigInteger & Add(BigInteger &val) const;
    BigInteger & Subtract(const BigInteger &val);
    

	std::vector<T> data;
	int sign;
	size_t sizeOfNumber;
	const size_t numOfBits = std::ceil(log2(Base));
	const size_t numOfBlocks = sizeof(T) * 8 / numOfBits;
};
