#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
template <typename T>
class Matrix
{

    T** data;
    int col;
    int row;
public:
    int getrow() { return row; }
    int getcol() { return col; }
    Matrix() : col(0), row(0), data(nullptr) {}
    Matrix(int row, int col) : data(new T* [row]), col(col), row(row)
    {
        for (int i = 0; i < row; i++)
            data[i] = new T[col]();
    }
    Matrix(const T& X) :data(new T* [1]), col(1), row(1) {
        data[0] = new T[1]();
        data[0][0] = X;
    }
    Matrix(int X) :data(new T* [1]), col(1), row(1) {
        data[0] = new T[1]();
        data[0][0] = X;
    }
    ~Matrix()
    {
        for (int i = 0; i < row; i++)
            delete[] data[i];
        delete[] data;
    }

    T& getel(int row, int col) {
        if(getrow()>row and getcol()>col){ 
            return data[row][col];
        }
        throw std::invalid_argument("out of bounds");
    }

    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    Matrix operator*(const Matrix& other);
    Matrix operator/(const Matrix& other);

    Matrix operator-();



    Matrix inv() const;
    T det();

    Matrix ElMult(const Matrix& other);
    Matrix ElDiv(const Matrix& other);
    T prod();
    T sum();

    Matrix transpose() const;

    Matrix Min(const Matrix& other);
    Matrix Max(const Matrix& other);
};
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other)
{
    if (this != &other)
    {
        Matrix<T> temp(other);
        std::swap(data, temp.data);
        std::swap(col, temp.col);
        std::swap(row, temp.row);
    }
    return *this;
}
template <typename T>
Matrix<T>::Matrix(const Matrix& other) : data(new T* [other.row]), col(other.col), row(other.row)
{
    for (int i = 0; i < row; i++){
        data[i] = new T[col];
    }    
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            data[i][j] = other.data[i][j];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& other)
{
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.data[i][j] = data[i][j] + other.data[i][j];
    return result;
}
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other)
{
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Wrong Matrix<T> dimensions");
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)

            result.data[i][j] = data[i][j] - other.data[i][j];
    return result;

}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& other)
{

    //std::cout << *this;
    //std::cout << other;

    Matrix<T> result(row, other.col);
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
        throw std::invalid_argument("Wrong Matrix<T> dimensions");
    for (int i = 0; i < row; i++)
        for (int j = 0; j < other.col; j++)
        {
            T sum = 0;
            for (int k = 0; k < col; k++)
                sum += data[i][k] * other.data[k][j];
            result.data[i][j] = sum;
        }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() {
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.getel(i, j) = -data[i][j];
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const Matrix& other)
{
    return operator*(other.inv());
}

template <typename T>
Matrix<T> Matrix<T>::inv() const {
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
            T koeff = copy.getel(j, i) / copy.getel(i, i);
            for (int k = 0; k < col; k++) {
                copy.getel(j, k) -= koeff * copy.getel(i, k);
                inv.getel(j, k) -= koeff * inv.getel(i, k);
            }
        }
    }

    for (int i = col - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            T koeff = copy.getel(j, i) / copy.getel(i, i);
            for (int k = 0; k < col; k++) {
                copy.getel(j, k) -= koeff * copy.getel(i, k);
                inv.getel(j, k) -= koeff * inv.getel(i, k);
            }
        }
    }

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < col; j++) {
            inv.getel(i, j) /= copy.getel(i, i);
        }
    }
    return inv;
}

template <typename T>
T Matrix<T>::det() {

    //Matrix inver = inv();
    //std::cout << inver;
    T deter = 1;
    Matrix copy = *this;
    bool isSwapped = true;
    for (int i = 0; i < col; i++) {
        if (copy.data[i][i] == 0) {
            isSwapped = false;
            for (int j = i + 1; j < col; j++) {
                if (copy.data[j][i] != 0) {


                    T* S = copy.data[i];
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
            T koeff = copy.data[j][i] / copy.data[i][i];

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

template <typename T>
Matrix<T> Matrix<T>::ElMult(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.data[i][j] = data[i][j] * other.data[i][j];
    return result;
}
template <typename T>
Matrix<T> Matrix<T>::ElDiv(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix<T> result(row, col);
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

template <typename T>
Matrix<T> zeros(int row, int col)
{
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.getel(i, j) = 0;
    return result;
}

template <typename T>
Matrix<T> ones(int row, int col)
{
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            result.getel(i, j) = 1;
    return result;
}

template <typename T>
Matrix<T> eye(int n)
{
    Matrix<T> result(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result.getel(i, j) = 1;
    return result;
}

template <typename T>
T Matrix<T>::prod() {
    T res = 1;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            res = res * data[i][j];
    return res;

}

template <typename T>
T Matrix<T>::sum() {
    T res = 0;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            res = res + data[i][j];
    return res;
}




template <typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> transposed(col, row);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            transposed.data[j][i] = data[i][j];
        }
    }
    return transposed;
}


template <typename T>
Matrix<T> linspace(T x0, T x1, int n) {
    Matrix<T> lineal(1, n); //row=1;
    T step = (x1 - x0) / (n - 1);
    for (int i = 0; i < n; ++i) {
        lineal.getel(0, i) = x0 + i * step;
    }
    return lineal;
}

template <typename T>
Matrix<T> Matrix<T>::Max(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result.data[i][j] = (data[i][j] < other.data[i][j]) ? other.data[i][j] : data[i][j];
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::Min(const Matrix& other) {
    if (col != other.col || row != other.row)
        throw std::invalid_argument("Matrices have different dimensions");
    Matrix<T> result(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result.data[i][j] = (data[i][j] < other.data[i][j]) ? data[i][j] : other.data[i][j];
        }
    }
    return result;
}

template <typename T>
Matrix<T> concatenateHorizontal(Matrix<T>& left, Matrix<T>& right) {
    if (left.getrow() != right.getrow()) {
        throw std::invalid_argument("Matrices have different dimensions");
    }
    int total_cols = left.getcol() + right.getcol();
    int total_rows = left.getrow();

    Matrix<T> result(total_rows, total_cols);

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

template <typename T>
Matrix<T> concatenateVertical(Matrix<T>& left, Matrix<T>& right) {
    if (left.getcol() != right.getcol()) {
        throw std::invalid_argument("Matrices have different dimensions");
    }
    int total_rows = left.getrow() + right.getrow();
    int total_cols = left.getcol();

    Matrix<T> result(total_rows, total_cols);

    for (int i = 0; i < left.getrow(); ++i) {
        for (int j = 0; j < left.getcol(); ++j) {
            result.getel(i, j) = left.getel(i, j);
        }
    }

    for (int i = 0; i < right.getrow(); ++i) {
        for (int j = 0; j < right.getcol(); ++j) {
            result.getel(left.getrow() + i, j) = right.getel(i, j);
        }
    }


    return result;
}

template <typename T>
Matrix<T> vertcat(std::vector<Matrix<T>>& cat) {
    Matrix<T> result(cat[0]);
    for (int i = 1; i < cat.size(); ++i) {
        result = concatenateVertical(result, cat[i]);
    }
    return result;
}

template <typename T>
Matrix<T> horzcat(std::vector<Matrix<T>>& cat) {
    Matrix<T> result(cat[0]);
    for (int i = 1; i < cat.size(); ++i) {
        result = concatenateHorizontal(result, cat[i]);
    }
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, Matrix<T> X) {
    // Íàõîäèì ìàêñèìàëüíóþ äëèíó ÷èñëà â êàæäîì ñòîëáöå
    if (X.getcol() != 1 or X.getrow() != 1) {
        out << "[";
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
    // Âûâîäèì ìàòðèöó ñ âûðàâíèâàíèåì ïî ïðàâîìó êðàþ

    for (int i = 0; i < X.getrow(); i++) {
        for (int j = 0; j < X.getcol(); j++) {
            out << std::setw(maxLength[j] + 1) << to_string(X.getel(i, j));
        }
        if (X.getcol() == 1 and X.getrow() == 1) {
            break;
        }
        out << std::endl;
    }
    if (X.getcol() == 1 and X.getrow() == 1) {
        return out;
    }
    out << "]";
    return out;
}
