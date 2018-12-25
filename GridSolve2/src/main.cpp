#include "INT.h"
#include "binaryMatrix.h"
#include <string>
#include <chrono>

#include <iostream>

using uint = unsigned int;

binaryMatrix getVector(uint size)
{
	binaryMatrix b(size*size, 1);
	std::cout << "Please enter each row of the field and confirm with Enter.\n" 
		<< "Enter a \'1\' for a lit field and a \'0\' for an unlit field without spaces"  << std::endl;
	std::cout << "Enter \"void\" to initialize field with no lights on" << std::endl;
	std::string input;
	for (uint i = 0; i < size; i++)
	{
		std::cout << "Row " << i + 1 << ": ";
		std::cin >> input;
		if (input == "void")
			return MATRIX::ONES(size*size, 1);
		for (uint j = 0; j < input.size(); j++)
			b(i*size + j, 0) = input.at(j) == '1' ? binaryInt(0) : binaryInt(1);
		input = "";
	}
	return b;
}

binaryMatrix getA(uint size)
{
	binaryMatrix A(size*size, size*size);
	binaryMatrix TRI = MATRIX::TRIDIAGONAL(size);
	binaryMatrix ID = MATRIX::IDENTITY(size);

	if (size == 1)
		return TRI;

	A.block(0, 0, TRI);
	A.block(0, size, ID);
	A.block(size*(size - 1), size*(size - 2), ID);
	A.block(size*(size - 1), size*(size - 1), TRI);

	for (uint i = 1; i < size - 1; i++)
	{
		A.block(i*size, (i - 1)*size, ID);
		A.block(i*size, i*size, TRI);
		A.block(i*size, (i + 1)*size, ID);
	}
	return A;
}

int main()
{
	uint size;
	std::cout << "Please enter the size of your field: ";
	std::cin >> size;
	binaryMatrix A = getA(size);
	binaryMatrix b = getVector(size);
	auto t0 = std::chrono::high_resolution_clock::now();
	binaryMatrix RES = MATRIX::SOLVE(A, b);
	auto t1 = std::chrono::high_resolution_clock::now();

	std::cout << "Solving this puzzle took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
		<< " ms" << std::endl;

	for (uint i = 0; i < size; i++)
	{
		for (uint j = 0; j < size; j++)
			std::cout << RES(i*size + j, 0) << " ";
		std::cout << "\n";
	}


	system("PAUSE");
	return 0;
}