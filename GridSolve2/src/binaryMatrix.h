#pragma once
#include <vector>
#include <iostream>
#include <string>

#include "INT.h"

using uint = unsigned int;

class binaryMatrix;

namespace MATRIX {
	binaryMatrix ONES(uint nRows, uint nCols);
	binaryMatrix ZEROS(uint nRows, uint nCols);
	binaryMatrix IDENTITY(uint size);
	binaryMatrix TRIDIAGONAL(uint size);
	binaryMatrix SOLVE(binaryMatrix& A, binaryMatrix& b);
}

class binaryMatrix // row-major
{
private:
	std::vector<binaryInt> m_data;
	uint rows;
	uint cols;

	void rows_switch(uint row1, uint row2);
	void rows_add(uint targetRow, uint row, binaryMatrix& source);
	void rows_add(uint targetRow, uint row);

public:
	binaryMatrix(uint nRows, uint nCols);
	binaryMatrix();

	std::pair<binaryMatrix, binaryMatrix> triangularize();
	binaryMatrix inverse();
	binaryMatrix& block(uint row, uint col, binaryMatrix& insert);

	binaryInt& operator()(uint row, uint col);
	const binaryInt& operator()(uint row, uint col) const;
	binaryMatrix& operator=(const binaryMatrix& rh);
	binaryMatrix operator*(const binaryMatrix& rh) const;
	binaryMatrix& operator+=(const binaryMatrix& rh);
	binaryMatrix operator+(const binaryMatrix& rh) const;

	friend std::ostream& operator<<(std::ostream& ostream, const binaryMatrix& matrix);
	friend binaryMatrix MATRIX::ONES(uint nRows, uint nCols);
	friend binaryMatrix MATRIX::ZEROS(uint nRows, uint nCols);
	friend binaryMatrix MATRIX::IDENTITY(uint size);
	friend binaryMatrix MATRIX::TRIDIAGONAL(uint size);
	friend binaryMatrix MATRIX::SOLVE(binaryMatrix& A, binaryMatrix& b);
};


std::ostream& operator<<(std::ostream& ostream, const binaryMatrix& matrix);

