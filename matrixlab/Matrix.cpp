#include "Matrix.h"
#include <vector>
#include <iomanip>

Matrix& Matrix::operator=(const Matrix& other)
{
	if (this != &other)
	{
		Matrix temp(other);
		std::swap(data, temp.data);
		std::swap(col, temp.col);
		std::swap(row, temp.row);
	}
	return *this;
}
Matrix::Matrix(const Matrix& other) : data(new BigFloat* [other.row]), col(other.col), row(other.row)
{
    for (int i = 0; i < row; i++)
        data[i] = new BigFloat[col];
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            data[i][j] = other.data[i][j];
}

Matrix Matrix::operator+(const Matrix& other)
{
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.data[i][j] = data[i][j] + other.data[i][j];
    return result;
}

Matrix Matrix::operator-(const Matrix& other)
{
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Wrong matrix dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)

            result.data[i][j] = data[i][j] - other.data[i][j];
    return result;

}

Matrix Matrix::operator*(const Matrix& other)
{

    //std::cout << *this;
    //std::cout << other;
    
    Matrix result(row, other.col);
    if (other.row == 1 and other.col == 1) {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                result.data[i][j] = data[i][j] * other.data[0][0];
        return result;
    }
    if (row == 1 and col == 1) {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                result.data[i][j] = data[0][0] * other.data[i][j];
        return result;
    }
    if (col != other.row)
        throw std::invalid_argument("Wrong matrix dimensions");
    for (int i = 0; i < row; i++)
        for (int j = 0; j < other.col; j++)
        {
            BigFloat sum = 0;
            for (int k = 0; k < col; k++)
                sum += data[i][k] * other.data[k][j];
            result.data[i][j] = sum;
        }
    return result;
}

Matrix Matrix::operator-() {
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.getel(i, j) = -data[i][j];
    return result;
}

Matrix Matrix::operator/(const Matrix& other)
{
    return operator*(other.inv());
}

Matrix Matrix::inv() const {
    if (col != row) {
        throw std::invalid_argument("Wrong matrix dimensions");
    }
    //std::cout << *this;
    Matrix inv(col, row);
    Matrix copy = *this;

    for (int i = 0; i < row; i++) {
        inv.getel(i, i) = 1;
    }

    for (int i = 0; i < col; ++i) {
        for (int j = i + 1; j < col; ++j) {
            BigFloat koeff = copy.getel(j, i) / copy.getel(i, i);
            for (int k = 0; k < col; k++) {
                copy.getel(j, k) -= koeff * copy.getel(i, k);
                inv.getel(j, k) -= koeff * inv.getel(i, k);
            }
        }
    }

    for (int i = col - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            BigFloat koeff = copy.getel(j, i) / copy.getel(i, i);
            for (int k = 0; k < col; k++) {
                copy.getel(j, k) -= koeff * copy.getel(i, k);
                inv.getel(j, k) -= koeff * inv.getel(i, k);
            }
        }
    }

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < col; j++) {
            inv.getel(i,j) /= copy.getel(i, i);
        }
    }
    return inv;
}

BigFloat Matrix::det() {

    //Matrix inver = inv();
    //std::cout << inver;
    BigFloat deter = 1;
    Matrix copy = *this;
    bool isSwapped = true;
    for (int i = 0; i < col; i++) {
        if (copy.data[i][i] == 0) {
            isSwapped = false;
            for (int j = i + 1; j < col; j++) {
                if (copy.data[j][i] != 0) {


                    BigFloat* S = copy.data[i];
                    copy.data[i] = copy.data[j];
                    copy.data[j] = S;

                    /*S = inver.data[i];
                    inver.data[i] = inver.data[j];
                    inver.data[j] = S;*/






                    deter *= -1;
                    isSwapped = true;
                    break;
                }

            }
        }
        if (!isSwapped) {
            return 0;
        }
        for (int j = i + 1; j < col; j++) {
            BigFloat koeff = copy.data[j][i] / copy.data[i][i];
            
            //std::cout << koeff<<std::endl;
            for (int k = 0; k < col; k++) {
                copy.data[j][k] = copy.data[j][k] - koeff * copy.data[i][k];

            }
        }


        deter *= copy.data[i][i];
    }
    //std::cout << inver;
    return deter;
}


Matrix Matrix::ElMult(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.data[i][j] = data[i][j] * other.data[i][j];
    return result;
}
Matrix Matrix::ElDiv(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (other.data[i][j] == 0) {
                throw std::invalid_argument("Matrices have different dimensions");
            }
            result.data[i][j] = data[i][j] / other.data[i][j];
        }
    }
    return result;
}


 Matrix zeros(int row, int col)
{
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.getel(i,j) = 0;
    return result;
}

 Matrix ones(int row, int col)
 {
     Matrix result(row, col);
     for (int i = 0; i < row; i++)
         for (int j = 0; j < col; j++)
             result.getel(i, j) = 1;
     return result;
 }

 Matrix eye(int n)
 {
     Matrix result(n,n);
     for (int i = 0; i < n; i++)
         for (int j = 0; j < n; j++)
             result.getel(i, j) = 1;
     return result;
 }

 BigFloat Matrix::prod() {
     BigFloat res=1;
     for (int i = 0; i < row; i++)
         for (int j = 0; j < col; j++)
             res = res*data[i][j];
     return res;

 }
 BigFloat Matrix::sum() {
     BigFloat res = 0;
     for (int i = 0; i < row; i++)
         for (int j = 0; j < col; j++)
             res = res + data[i][j];
     return res;
 }





 Matrix Matrix::transpose() const {
     Matrix transposed(col, row);
     for (int i = 0; i < row; ++i) {
         for (int j = 0; j < col; ++j) {
             transposed.data[j][i] = data[i][j];
         }
     }
     return transposed;
 }

Matrix linspace(BigFloat x0, BigFloat x1, int n) {
     Matrix lineal(1,n); //row=1;
     BigFloat step = (x1 - x0) / (n - 1);
     for (int i = 0; i < n; ++i) {
         lineal.getel(0, i) = x0 + i * step;
     }
     return lineal;
 }

Matrix Matrix::Max(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result.data[i][j] = (data[i][j] < other.data[i][j]) ? other.data[i][j] : data[i][j];
        }
    }
    return result;
}


Matrix Matrix::Min(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix result(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result.data[i][j] = (data[i][j] < other.data[i][j]) ? data[i][j] : other.data[i][j];
        }
    }
    return result;
}

Matrix concatenateHorizontal(Matrix& left, Matrix& right) {
    if (left.getrow() != right.getrow()) {
        throw std::invalid_argument("Matrices have different dimensions");
    }
    int total_cols = left.getcol() + right.getcol();
    int total_rows = left.getrow();

    Matrix result(total_rows, total_cols);

    for (int i = 0; i < left.getrow(); ++i) {
        for (int j = 0; j < left.getcol(); ++j) {
            result.getel(i, j) = left.getel(i, j);
        }
    }

    for (int i = 0; i < left.getrow(); ++i) {
        for (int j = 0; j < right.getcol(); ++j) {
            result.getel(i, left.getcol() + j) = right.getel(i, j);
        }
    }

    return result;
}


Matrix concatenateVertical(Matrix& left, Matrix& right) {
    if (left.getcol() != right.getcol()) {
        throw std::invalid_argument("Matrices have different dimensions");
    }
    int total_rows = left.getrow() + right.getrow();
    int total_cols = left.getcol();

    Matrix result(total_rows, total_cols);

    for (int i = 0; i < left.getrow(); ++i) {
        for (int j = 0; j < left.getcol(); ++j) {
            result.getel (i, j) = left.getel(i, j);
        }
    }

    for (int i = 0; i < right.getrow(); ++i) {
        for (int j = 0; j < right.getcol(); ++j) {
            result.getel(left.getrow() + i, j) = right.getel(i, j);
        }
    }


    return result;
}

Matrix vertcat(std::vector<Matrix>& cat) {
    Matrix result(cat[0]);
    for (int i = 1; i < cat.size(); ++i) {
        result = concatenateVertical(result, cat[i]);
    }
    return result;
}
Matrix horzcat(std::vector<Matrix>& cat) {
    Matrix result(cat[0]);
    for (int i = 1; i < cat.size(); ++i) {
        result = concatenateHorizontal(result, cat[i]);
    }
    return result;
}
std::ostream& operator<<(std::ostream& out, Matrix X) {
    // Находим максимальную длину числа в каждом столбце
    if (X.getcol() != 1 or X.getrow() != 1) {
        out<<"[";
        out << std::endl;

    }
    std::vector<int> maxLength(X.getcol(), 0);
    for (int i = 0; i < X.getrow(); i++) {
        for (int j = 0; j < X.getcol(); j++) {
            int numLength = to_string((X.getel(i, j))).length();
            if (numLength > maxLength[j]) {
                maxLength[j] = numLength;
            }
        }
    }
    maxLength[0] -= 1;
    // Выводим матрицу с выравниванием по правому краю
    
    for (int i = 0; i < X.getrow(); i++) {
        for (int j = 0; j < X.getcol(); j++) {
            out << std::setw(maxLength[j]+1) << to_string(X.getel(i, j));
        }
        out << std::endl;
    }
    if (X.getcol() == 1 and X.getrow() == 1) {
        return out;
    }
    out << "]";
    return out;
}