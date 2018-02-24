#ifndef ALG_NUMERIC_MATRIX_H_
#define ALG_NUMERIC_MATRIX_H_

#include <cstddef>

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace alg {
namespace numeric {

template <class T>
class Matrix {
public:
    Matrix() = default;

    Matrix(std::size_t m, std::size_t n);
    Matrix(std::initializer_list<std::initializer_list<T>> init);

    Matrix(const Matrix<T>& rhs);
    Matrix(Matrix<T>&& rhs) noexcept;

    ~Matrix();

    std::pair<std::size_t, std::size_t> dimension() const;
    T* data_ptr() const;

    Matrix<T> transpose() const;

    Matrix<T> submatrix(std::size_t start_i, std::size_t start_j,
            std::size_t x, std::size_t y) const;

    void copy(const Matrix<T>& M, std::size_t to_i, std::size_t to_j,
            std::size_t from_i, std::size_t from_j,
            std::size_t x, std::size_t y);

    class Proxy {
    public:
        Proxy(T* a);
        T& operator[](int index) const;

    private:
        T* a;
    };

    Proxy operator[](int index) const;

    Matrix<T>& operator=(const Matrix<T>& rhs) &;
    Matrix<T>& operator=(Matrix<T>&& rhs) & noexcept;

    Matrix<T> operator+(const Matrix<T>& rhs) const;
    Matrix<T> operator-(const Matrix<T>& rhs) const;
    Matrix<T> operator*(const Matrix<T>& rhs) const;
    Matrix<T> operator*(const T& rhs) const;

private:
    std::size_t m{0};
    std::size_t n{0};
    T* buffer{nullptr};
};

template <class T>
Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs);

template <class T>
Matrix<T>::Matrix(std::size_t m, std::size_t n) :
    m{m ? m : throw std::invalid_argument("m can't be zero")},
    n{n ? n : throw std::invalid_argument("n can't be zero")},
    buffer{new T[m * n]} {
}

template <class T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init) {
    if (!(m = init.size()) || !(n = init.begin()->size())) {
        throw std::invalid_argument("Dimension can't be zero");
    }

    buffer = new T[m * n];
    int i = 0;

    for (std::initializer_list<T> row : init) {
        if (row.size() != n) {
            delete[] buffer;
            throw std::invalid_argument("Malformed initializer list");
        }

        std::copy(row.begin(), row.end(), &buffer[i]);
        i += n;
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& rhs) :
    m{rhs.m},
    n{rhs.n},
    buffer{new T[m * n]} {

    std::copy(&rhs.buffer[0], &rhs.buffer[m * n], buffer);
}

template <class T>
Matrix<T>::Matrix(Matrix<T>&& rhs) noexcept :
    m{rhs.m},
    n{rhs.n},
    buffer{rhs.buffer} {

    rhs.buffer = nullptr;
    rhs.m = 0;
    rhs.n = 0;
}

template <class T>
Matrix<T>::~Matrix() {
    delete[] buffer;
}

template <class T>
std::pair<std::size_t, std::size_t> Matrix<T>::dimension() const {
    return {m, n};
}

template <class T>
T* Matrix<T>::data_ptr() const {
    return buffer;
}

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> M(n, m);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            M[j][i] = (*this)[i][j];
        }
    }

    return M;
}

template <class T>
Matrix<T> Matrix<T>::submatrix(std::size_t start_i, std::size_t start_j,
        std::size_t x, std::size_t y) const {

    Matrix<T> M(x, y);

    for (std::size_t i = 0; i < x; ++i) {
        for (std::size_t j = 0; j < y; ++j) {
            M[i][j] = (*this)[start_i + i][start_j + j];
        }
    }

    return M;
}

template <class T>
void Matrix<T>::copy(const Matrix<T>& M, std::size_t to_i, std::size_t to_j,
        std::size_t from_i, std::size_t from_j,
        std::size_t x, std::size_t y) {

    for (std::size_t i = 0; i < x; ++i) {
        for (std::size_t j = 0; j < y; ++j) {
            (*this)[to_i + i][to_j + j] = M[from_i + i][from_j + j];
        }
    }
}

template <class T>
Matrix<T>::Proxy::Proxy(T* a) : a{a} {
}

template <class T>
T& Matrix<T>::Proxy::operator[](int index) const {
    return a[index];
}

template <class T>
typename Matrix<T>::Proxy Matrix<T>::operator[](int index) const {
    return Proxy(&buffer[index * n]);
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs) & {
    if (m != rhs.m || n != rhs.n) {
        throw std::invalid_argument("Dimension mismatch");
    }

    if (this == &rhs) {
        return *this;
    }

    std::copy(&rhs.buffer[0], &rhs.buffer[m * n], buffer);

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& rhs) & noexcept {
    delete[] buffer;

    m = rhs.m;
    n = rhs.n;
    buffer = rhs.buffer;

    rhs.buffer = nullptr;
    rhs.m = 0;
    rhs.n = 0;

    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const {
    if (m != rhs.m || n != rhs.n) {
        throw std::invalid_argument("Dimension mismatch");
    }

    Matrix<T> M(m, n);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            M[i][j] = (*this)[i][j] + rhs[i][j];
        }
    }

    return M;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const {
    if (m != rhs.m || n != rhs.n) {
        throw std::invalid_argument("Dimension mismatch");
    }

    Matrix<T> M(m, n);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            M[i][j] = (*this)[i][j] - rhs[i][j];
        }
    }

    return M;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const {
    if (n != rhs.m) {
        throw std::invalid_argument("Dimension mismatch");
    }

    Matrix<T> M(m, rhs.n);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < rhs.n; ++j) {
            T sum = T();

            for (std::size_t k = 0; k < n; ++k) {
                sum += (*this)[i][k] * rhs[k][j];
            }

            M[i][j] = sum;
        }
    }

    return M;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T& rhs) const {
    Matrix<T> M(m, n);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            M[i][j] = (*this)[i][j] * rhs;
        }
    }

    return M;
}

template <class T>
Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs) {
    return rhs * lhs;
}

}
}

#endif
