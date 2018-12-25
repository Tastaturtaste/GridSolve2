#include "INT.h"

binaryInt::binaryInt(int i)
{
	inner = i % 2;
}

binaryInt::~binaryInt()
{
}

binaryInt& binaryInt::operator+=(const binaryInt& rh)
{
	(*this).inner = ((*this).inner + rh.inner) % 2;
	return *this;
}

binaryInt& binaryInt::operator-=(const binaryInt& rh)
{
	return *this += rh;
}

binaryInt& binaryInt::operator*=(const binaryInt& rh)
{
	(*this).inner = (*this).inner * rh.inner;
	return *this;
}

binaryInt& binaryInt::operator=(const binaryInt& rh)
{
	this->inner = rh.inner;
	return *this;
}

binaryInt operator+(const binaryInt& lh, const binaryInt& rh)
{
	return binaryInt(lh.inner + rh.inner);
}

binaryInt operator-(const binaryInt& lh, const binaryInt& rh)
{
	return lh + rh;
}

binaryInt operator*(const binaryInt& lh, const binaryInt& rh)
{
	return binaryInt(lh.inner * rh.inner);
}

std::ostream& operator<<(std::ostream& ostream, const binaryInt& INT)
{
	return ostream << INT.inner;
}