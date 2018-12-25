#include "binaryMatrix.h"
#include <cassert>
#include <exception>
#include <queue>

void binaryMatrix::rows_switch(uint row1, uint row2)
{	
	binaryInt temp;
	for (uint i = 0; i < this->cols; i++)
	{
		temp =(*this)(row1, i);
		(*this)(row1, i) = (*this)(row2, i);
		(*this)(row2, i) = temp;
	}

}

void binaryMatrix::rows_add(uint targetRow, uint row, binaryMatrix& source)
{
	if (targetRow >= this->rows)
		throw std::out_of_range("This Matrix does not have as much rows");	
	if (source.cols != this->cols)
		throw std::out_of_range("Rows don't have the same lenght");
	if (row >= source.rows)
		throw std::out_of_range("Sourcematrix does not have as much rows");
	
	for (uint i = 0; i < this->cols; i++)
		(*this)(targetRow, i) += source(row, i);
}

void binaryMatrix::rows_add(uint targetRow, uint row)
{
	if (targetRow >= this->rows)
		throw std::out_of_range("This Matrix does not have as much rows");

	for (uint i = 0; i < this->cols; i++)
		(*this)(targetRow, i) += (*this)(row, i);
}


binaryMatrix::binaryMatrix(uint nRows, uint nCols)
	: rows(nRows), cols(nCols)
{
	m_data.resize(rows*cols, binaryInt(0));
}

binaryMatrix::binaryMatrix()
	: rows(0), cols(0)
{
}

std::pair<binaryMatrix, binaryMatrix> binaryMatrix::triangularize() 
{
	if (this->rows != this->cols)
		throw std::out_of_range("Matrix has to be square to get triangularized");
	
	binaryMatrix R = *this;
	std::queue<binaryMatrix> Ps;

	binaryMatrix L = MATRIX::IDENTITY(R.rows);

	for (uint i = 0; i < rows; i++)
	{
		if ((int)R(i, i) == 0)
			for (uint index = i + 1; index < R.rows; index++)
				if ((int)R(index, i) != 0)
				{
					R.rows_switch(i, index);
					Ps.push(MATRIX::IDENTITY(R.rows));
					binaryMatrix temp(2, 2);
					temp(0, 0) = 0;
					temp(0, 1) = 1;
					temp(1, 0) = 1;
					temp(1, 1) = 0;
					Ps.front().block(i, i, temp);
				}
		for (uint row = i + 1; row < R.rows; row++)
		{
			if ((int)R(row, i) != 0)
			{
				R.rows_add(row, i);
				L(row, i) = 1;
			}
		}
	}
	binaryMatrix P = MATRIX::IDENTITY(R.rows);
	while (!Ps.empty())
	{
		P = Ps.front() * P;
		Ps.pop();
	}
	return { P*L, R };
}

binaryMatrix binaryMatrix::inverse()
{
	if (this->rows != this->cols)
		throw std::out_of_range("Only square matrices have an inverse");

	binaryMatrix Inverse = MATRIX::IDENTITY(this->rows);
	binaryMatrix LH = *this;

	for (uint i = 0; i < LH.rows; i++)
	{
		if ((int)LH(i, i) == 0)
			for (uint index = i + 1; index < LH.rows; index++)
				if ((int)LH(index, i) != 0)
				{
					LH.rows_switch(i, index);
					Inverse.rows_switch(i, index);
				}
		for (uint row = i + 1; row < LH.rows; row++)
		{
			if ((int)LH(row, i) != 0)
			{
				LH.rows_add(row, i);
				Inverse.rows_add(row, i);
			}
		}
	}

	for (uint i = LH.rows - 1; i >= 1; i--)
	{
		for (int row = (int)i - 1; row >= 0; row--)
		{
			if ((int)LH(row, i) != 0)
			{
				Inverse.rows_add(row, i);
				LH.rows_add(row, i);
			}
		}
	}
	return Inverse;
}

binaryMatrix& binaryMatrix::block(uint row, uint col, binaryMatrix& insert)
{
	if (row + insert.rows > this->rows || col + insert.cols > this->cols)
		throw std::out_of_range("Blockmatrix is too big");
	for (uint i = 0; i < insert.rows; i++)
		for (uint j = 0; j < insert.cols; j++)
			(*this)(row + i, col + j) = insert(i, j);
	return *this;
}

std::ostream& operator<<(std::ostream& ostream, const binaryMatrix& matrix)
{
	for (uint i = 0; i < matrix.rows; i++)
	{
		for (uint j = 0; j < matrix.cols; j++)
		{
			ostream << matrix.m_data[matrix.cols*i + j] << " ";
		}
		ostream << "\n";
	}
	return ostream;
}

binaryInt& binaryMatrix::operator()(uint row, uint col)
{
	if (row > this->rows - 1)
		throw std::out_of_range("Rowindex out of range");
	if (col > this->cols - 1)
		throw std::out_of_range("Columnindex out of range");
	return this->m_data[row*this->cols + col];
}

const binaryInt& binaryMatrix::operator()(uint row, uint col) const
{
	if (row > this->rows - 1)
		throw std::out_of_range("Rowindex out of range");
	if (col > this->cols - 1)
		throw std::out_of_range("Columnindex out of range");
	return this->m_data[row*this->cols + col];
}

binaryMatrix& binaryMatrix::operator=(const binaryMatrix& rh)
{
	this->cols = rh.cols;
	this->rows = rh.rows;
	this->m_data = rh.m_data;
	return *this;
}

binaryMatrix binaryMatrix::operator*(const binaryMatrix& rh) const
{
	if (this->cols != rh.rows)
		throw std::range_error("LH number of columns has to be the same as RH number of rows");
	binaryMatrix result(this->rows, rh.cols);
	binaryInt temp(0);
	for (uint i = 0; i < result.rows; i++)
	{
		for (uint j = 0; j < result.cols; j++)
		{
			for (uint k = 0; k < this->cols; k++)
			{
				temp += (*this)(i, k) * rh(k, j);
			}
			result(i, j) = temp;
			temp = 0;
		}
	}
	return result;
}

binaryMatrix& binaryMatrix::operator+=(const binaryMatrix& rh)
{
	if (this->rows != rh.rows || this->cols != rh.cols)
		throw std::out_of_range("Matrices have to be of same size");
	for (uint i = 0; i < this->rows; i++)
		for (uint j = 0; j < this->cols; j++)
			(*this)(i, j) + rh(i, j);
	return *this;
}

binaryMatrix binaryMatrix::operator+(const binaryMatrix& rh) const
{
	binaryMatrix result = *this;
	result += rh;
	return result;
}

binaryMatrix MATRIX::ONES(uint nRows, uint nCols)
{
	binaryMatrix temp(nRows, nCols);
	temp.m_data.assign(nRows*nCols, binaryInt(1));
	return temp;
}

binaryMatrix MATRIX::ZEROS(uint nRows, uint nCols)
{
	binaryMatrix temp(nRows, nCols);
	temp.m_data.assign(nRows*nCols, binaryInt(0));
	return temp;
}

binaryMatrix MATRIX::IDENTITY(uint size)
{
	binaryMatrix temp(size, size);
	for (uint i = 0; i < temp.rows; i++)
	{
		for (uint j = 0; j < temp.cols; j++)
		{
			if (i == j)
				temp.m_data[i*temp.cols + j] = binaryInt(1);
			else
				temp.m_data[i*temp.cols + j] = binaryInt(0);
		}
	}
	return temp;
}

binaryMatrix MATRIX::TRIDIAGONAL(uint size)
{
	if (size == 0)
		return MATRIX::ONES(0, 0);
	if (size == 1)
		return MATRIX::ONES(1, 1);

	binaryMatrix tri(size, size);

	tri(0, 0) = 1;
	tri(0, 1) = 1;
	tri(tri.rows - 1, tri.cols - 2) = 1;
	tri(tri.rows - 1, tri.cols - 1) = 1;

	for (uint i = 1; i < size - 1; i++)
	{
		tri(i, i - 1) = 1;
		tri(i, i) = 1;
		tri(i, i + 1) = 1;
	}
	return tri;
}

binaryMatrix MATRIX::SOLVE(binaryMatrix& A, binaryMatrix& b)
{
	if (A.rows != A.cols)
		throw std::out_of_range("A has to be square");
	if (A.rows != b.rows)
		throw std::out_of_range("A has to have the same Nr. of rows as b");
	return A.inverse() * b;
}