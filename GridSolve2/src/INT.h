#pragma once

#include <iostream>

struct binaryInt
{
	unsigned int inner;

	binaryInt(int i = 0);
	~binaryInt();

	binaryInt& operator+=(const binaryInt& rh);
	binaryInt& operator*=(const binaryInt& rh);
	binaryInt& operator-=(const binaryInt& rh);
	binaryInt& operator=(const binaryInt& rh);
	
	inline operator int() const { return (int)inner; };
	inline operator unsigned int() const { return inner; };
};

binaryInt operator+(const binaryInt& lh, const binaryInt& rh);
binaryInt operator-(const binaryInt& lh, const binaryInt& rh);
binaryInt operator*(const binaryInt& lh, const binaryInt& rh);
std::ostream& operator<<(std::ostream& ostream, const binaryInt& INT);