#pragma once
#include "BIgFloat.h"
#include <vector>
#include <iostream>
class Matrix
{
	BigFloat** data;
	int col;
	int row;
public:
	int getrow() { return row; }
	int getcol() { return col; }
	Matrix(): col(0), row(0), data(nullptr){}
	Matrix(int row, int col) : data(new BigFloat* [row]), col(col), row(row)
	{
		for (int i = 0; i < row; i++)
			data[i] = new BigFloat[col]();
	}
	Matrix(const BigFloat& const X):data(new BigFloat* [1]), col(1), row(1) {
		data[0] = new BigFloat[1]();
		data[0][0] = X;
	}
	Matrix(int X) :data(new BigFloat* [1]), col(1), row(1) {
		data[0] = new BigFloat[1]();
		data[0][0] = X;
	}
	~Matrix()
	{
		for (int i = 0; i < row; i++)
			delete[] data[i];
		delete[] data;
	}

	BigFloat& getel(int row, int col) {
		return data[row][col];
	}

	Matrix(const Matrix& other);
	Matrix& operator=(const Matrix& other);
	Matrix operator+(const Matrix& other);
	Matrix operator-(const Matrix& other);
	Matrix operator*(const Matrix& other);
	Matrix operator/(const Matrix& other);

	Matrix operator-();

	

	Matrix inv() const;
	BigFloat det();

	Matrix ElMult(const Matrix& other);
	Matrix ElDiv(const Matrix& other);
	BigFloat prod();
	BigFloat sum();

	Matrix transpose() const;

	Matrix Min(const Matrix& other);
	Matrix Max(const Matrix& other);
};

Matrix zeros(int, int);
Matrix ones(int row, int col);
Matrix eye(int n);

Matrix linspace(BigFloat x0, BigFloat x1, int n);
std::ostream& operator<<(std::ostream& out, Matrix X);
Matrix concatenateHorizontal(Matrix& left, Matrix& right);\
Matrix concatenateVertical(Matrix& left, Matrix& right);
Matrix vertcat(std::vector<Matrix>& cat);
Matrix horzcat(std::vector<Matrix>& cat);