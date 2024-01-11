#ifndef __MATRIX_H__
#define __MATRIX_H__

#pragma once

#include <cmath>
#include <math.h>
#include <cassert>
#include <iostream>

#define PI 3.141592653589793115997963468544185161590576171875
// template parameter used here
template<int n> struct vec {
    double data[n] = {0};

    vec() = default;
    double & operator[] (const int i)       { assert(i >= 0 && i < n); return data[i]; }
    double   operator[] (const int i) const { assert(i >= 0 && i < n); return data[i]; }
    double norm2() const { return *this * *this; }
    double norm()  const { return std::sqrt(norm2()); }
};

// overload operator *, vector dot product
template<int n> double operator* (const vec<n>& u, const vec<n>& v) {
    double res = 0;
    for (int i = n; i--; res += u[i] * v[i]) ;
    return res;
};

// overload operator +, vector addition
template<int n> vec<n> operator+ (const vec<n>& u, const vec<n>& v) {
    vec<n> res = u;
    for (int i = n; i--; res[i] += v[i]) ;
    return res;
};

// overload operator -, vector - vector subtraction
template<int n> vec<n> operator- (const vec<n>& u, const vec<n>& v) {
    vec<n> res = u;
    for (int i = n; i--; res[i] -= v[i]) ;
    return res;
};

// overload operator -, vector negation
template<int n> vec<n> operator-(const vec<n>& v) {
    vec<n> res;
    for (int i = 0; i < n; ++i) {
        res[i] = -v[i];
    }
    return res;
}

// overload operator -, vector - scalar subtraction
template<int n> vec<n> operator- (const vec<n>& u, const double x) {
    vec<n> res = u;
    for (int i = n; i--; res[i] -= x) ;
    return res;
};

// overload operator *, vector scalar multipication
template<int n> vec<n> operator* (const vec<n>& u, const double& v) {
    vec<n> res = u;
    for (int i = n; i--; res[i] *= v) ;
    return res;
};

// overload operator *, vector scalar multipication
template<int n> vec<n> operator* (const double& v, const vec<n>& u) {
    vec<n> res = u;
    for (int i = n; i--; res[i] *= v) ;
    return res;
};

// overload operator /, vector scalar division
template<int n> vec<n> operator/ (const vec<n>& u, const double& v) {
    vec<n> res = u;
    for (int i = n; i--; res[i] /= v) ;
    return res;
};

template<int n1, int n2> vec<n1> embed(const vec<n2>& v, double fill = 0) {
    vec<n1> res;
    for (int i = n1; i--; res[i] = (i < n2) ? v[i] : fill) ;
    return res;
}

template<int n1, int n2> vec<n1> slice(const vec<n2> v, const int m=0, const int n=n1-1) {
    assert(n2 > (n - m + 1) && n >= m);
    vec<n1> res;
    for (int i = 0, j = m; j <= n; i++, j++) res[i] = v[j];
    return res;
}

// compute the projection of u onto v
template<int n> vec<n> proj(const vec<n>& u, const vec<n>& v) {
    vec<n> res = u;
    vec<n> vNorm = v / v.norm();
    return (res *  vNorm) * vNorm;
}

// overload operator <<, vector print
template<int n> std::ostream& operator<< (std::ostream& out, const vec<n>& v) {
    std::cout << "[ ";
    for (int i = 0; i < n; i++) std::cout << v[i] << " ";
    std::cout << ']';
    return out;
};

// 模板专用化
template<> struct vec<2> {
    double x{}, y{};

    vec() = default;
    vec(double x_, double y_): x(x_), y(y_) {}
    double & operator[] (const int i)       { assert(i >= 0 && i < 2); return i ? y : x; }
    double   operator[] (const int i) const { assert(i >= 0 && i < 2); return i ? y : x; }
    double norm2() const { return *this * *this; }
    double norm()  const { return sqrt(norm2()); }
    vec & normalize() { *this = (*this) / norm(); return *this; }
};

template<> struct vec<3> {
    double x{}, y{}, z{};

    vec() = default;
    vec(double x_, double y_, double z_ = 0.0): x(x_), y(y_), z(z_) {}
    double & operator[] (const int i)       { assert(i >= 0 && i < 3); return i ? (i == 2 ? z : y) : x; }
    double   operator[] (const int i) const { assert(i >= 0 && i < 3); return i ? (i == 2 ? z : y) : x; }
    double norm2() const { return *this * *this; }
    double norm()  const { return sqrt(norm2()); }
    vec & normalize() { *this = (*this) / norm(); return *this; }
};

template<> struct vec<4> {
    double x{}, y{}, z{}, w{};

    vec() = default;
    vec(double x_, double y_, double z_, double w_ = 1.0): x(x_), y(y_), z(z_), w(w_) {}
    vec(vec<3> v): x(v.x), y(v.y), z(v.z), w(0.f) {}
    vec(vec<3> v, double w): x(v.x), y(v.y), z(v.z), w(w) {}
    double & operator[] (const int i)       { assert(i >= 0 && i < 4); return i ? (i == 3 ? w : (i == 2 ? z : y)) : x; }
    double   operator[] (const int i) const { assert(i >= 0 && i < 4); return i ? (i == 3 ? w : (i == 2 ? z : y)) : x; }
    vec<3> xyz() { return vec<3>(x, y, z); }
    double norm2() const { return (*this) * (*this); }
    double norm()  const { return sqrt(norm2()); }
    vec & normalize() { *this = (*this) / norm(); return *this; }
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;

// vector cross product
vec3 cross(const vec3 &v1, const vec3 &v2);

// determinant define
template<int n> struct dt;

// matrix define
template<int nrows, int ncols> struct mat {
    vec<ncols> rows[nrows] = {{}};

    vec<ncols>& operator[] (const int idx)       { assert(idx >= 0 && idx < nrows); return rows[idx]; }
    const vec<ncols>& operator[] (const int idx) const { assert(idx >= 0 && idx < nrows); return rows[idx]; }

    vec<nrows> col(const int idx) const {
        assert(idx >= 0 && idx < ncols);
        vec<nrows> res;
        for (int i = nrows; i--; res[i] = rows[i][idx]);
        return res;
    }

    void setCol(const int idx, const vec<nrows>& v) {
        assert(idx >= 0 && idx < ncols);
        for (int i = nrows; i--; rows[i][idx] = v[i]);
    }

    // get identity matrix
    static mat<nrows, ncols> identity() {
        mat<nrows, ncols> res;
        for (int i = nrows; i--; )
            for (int j = ncols; j--; res[i][j] = (i == j));
        return res;
    }

    double det() const {
        return dt<ncols>::det(*this);
    }

    // get the complementary submatrix
    mat<nrows-1, ncols-1> getMinor(const int row, const int col) const {
        mat<nrows-1, ncols-1> res;
        for (int i = nrows - 1; i-- ;)
            for (int j = ncols - 1; j--; res[i][j] = rows[i < row ? i : i+1][j < col ? j : j+1]);
        return res;
    }

    // compute the cofactor
    double cofactor(const int row, const int col) const {
        return getMinor(row, col).det()*((row + col)%2 ? -1 : 1);
    }

    // compute the adjugate matrix
    mat<nrows, ncols> adjugate() const {
        mat<nrows, ncols> res;
        for (int i = nrows; i--; )
            for (int j = ncols; j--; res[i][j] = cofactor(i, j));
        return res;
    }

    // compute the transposed invert matrix
    mat<nrows, ncols> invertTranspose() const {
        mat<nrows, ncols> res = adjugate();
        return res/(res[0]*rows[0]);
    }

    // compute the invert matrix
    mat<nrows, ncols> invert() const {
        return invertTranspose().transpose();
    }

    // transpose the matrix
    mat<ncols, nrows> transpose() const {
        mat<ncols, nrows> res;
        for (int i = ncols; i--; res[i] = this->col(i));
        return res;
    }
};

// override operator *, matrix multiplication
template<int nrows, int ncols> vec<nrows> operator* (const mat<nrows, ncols>& m, const vec<ncols>& v) {
    vec<nrows> res;
    for (int i = nrows; i-- ; res[i] = m[i] * v) ;
    return res;
}

// override operator *, matrix multiplication
template<int M, int N, int K> mat<M, K> operator* (const mat<M, N>& lhs, const mat<N, K>& rhs) {
    mat<M, K> res;
    for (int i = M; i--; )
        for (int j = K; j--; res[i][j] = lhs[i] * rhs.col(j)) ;
    return res;
}

// override operator *, matrix multiplicat number
template<int nrows, int ncols> mat<nrows, ncols> operator* (const double& k, const mat<nrows, ncols>& m) {
    mat<nrows, ncols> res;
    for (int i = nrows; i--; res[i] = m[i] * k) ;
    return res;
}

// override operator *, matrix multiplicat number
template<int nrows, int ncols> mat<nrows, ncols> operator* (const mat<nrows, ncols>& m, const double& k) {
    mat<nrows, ncols> res;
    for (int i = nrows; i--; res[i] = m[i] * k) ;
    return res;
}

// override operator /, matrix division
template<int nrows, int ncols> mat<nrows, ncols> operator/ (const mat<nrows, ncols>& m, const double& k) {
    mat<nrows, ncols> res;
    for (int i = nrows; i--; res[i] = m[i] / k) ;
    return res;
}

// override operator +, matrix addition
template<int nrows, int ncols> mat<nrows, ncols> operator+ (const mat<nrows, ncols>& m, const mat<nrows, ncols>& n) {
    mat<nrows, ncols> res;
    for (int i = nrows; i--; res[i] = m[i] + n[i]) ;
    return res;
}

// override operator -, matrix subtraction
template<int nrows, int ncols> mat<nrows, ncols> operator- (const mat<nrows, ncols>& m, const mat<nrows, ncols>& n) {
    mat<nrows, ncols> res;
    for (int i = nrows; i--; res[i] = m[i] - n[i]);
    return res;
}

// override operator <<, print matrix
template<int nrows, int ncols> std::ostream& operator<< (std::ostream& out, const mat<nrows, ncols>& m) {
    for (int i = 0; i < nrows; i++) std::cout << m[i] << std::endl;
    return out;
}

// determant
template<int n> struct dt {
    static double det(const mat<n, n>& m) {
        double res = 0;
        for (int i = n; i--; res += m[0][i]*m.cofactor(0, i)) ;
        return res;
    }
};

template<> struct dt<1> {
    static double det(const mat<1, 1>& m) {
        return m[0][0];
    }
};

typedef mat<3,3> mat3;
typedef mat<4,4> mat4;

#endif