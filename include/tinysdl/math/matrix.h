#pragma once 

#include <cmath>
#include <array>

// M : rows
// N : columns

namespace TinySDL {

    template <typename T, size_t M, size_t N>
    struct Matrix {            
        Matrix() { data = new T[M * N]; shape = {M, N};};
        Matrix(const Matrix & other); 
        ~Matrix() { delete[] data; };
        
        Matrix & operator= (const Matrix & other);
        T operator[](size_t i) const {return data[i];}
        T & operator[](size_t i) {return data[i];}
        Matrix operator+ (const Matrix & other);
        Matrix operator+ (T val);
        Matrix operator* (const Matrix & other);
        Matrix operator* (T val);


        std::array<size_t, 2> shape = {0, 0};
        T * data = nullptr;

        static Matrix zeros();
        static Matrix ones();
    };
    
    template <typename T, size_t M, size_t N>
    Matrix<T, M, N>::Matrix(const Matrix<T, M, N> & other) {
        shape = other.shape;
        data = new T[shape[0] * shape[1]];
        for(size_t i=0; i<(shape[0] * shape[1]); i++) data[i] = other.data[i];
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> & Matrix<T, M, N>::operator= (const Matrix<T, M, N> & other) {
        if (this != &other) {
            shape = other.shape;
            
            delete[] data;
            data = new T[shape[0] * shape[1]];
            
            for(size_t i=0; i<(shape[0] * shape[1]); i++) 
                data[i] = other.data[i];
        }
        return *this;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator+ (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator+ (T val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = data[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (T val, Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = other.data[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator* (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = data[i] * other.data[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator* (T val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = data[i] * val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (T val, Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = other.data[i] * val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::zeros() {
        Matrix<T, M, N> new_matrix = Matrix<T, M, N>();
        for(size_t i=0; i<(new_matrix.shape[0] * new_matrix.shape[1]); i++) 
            new_matrix.data[i] = 0;
        return new_matrix; 
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::ones() {
        Matrix<T, M, N> new_matrix = Matrix<T, M, N>();
        for(size_t i=0; i<(new_matrix.shape[0] * new_matrix.shape[1]); i++) 
            new_matrix.data[i] = 1.0;
        return new_matrix; 
    }

    namespace MatrixMath{

        template <size_t M>
        inline float dot(Matrix<float, M, 1> & a, Matrix<float, M, 1> & b) {
            float sum = 0;
            for(size_t i=0; i<M; i++) 
                sum += a.data[i] * b.data[i];
            return sqrtf(sum);
        }

        template <size_t M>
        inline Matrix<float, M, M> matmul(Matrix<float, M, M> & a, Matrix<float, M, M> & b) {
            Matrix<float, M, M> prod = Matrix<float, M, M>::zeros();
            for(size_t i = 0; i < M; i++)
                for(size_t j = 0; j < M; j++)
                    for(size_t k = 0; k < M; k++)
                        prod.data[i * M + j] += a.data[i * M + k] * b.data[k * M + j];
            return prod;
        }



    }

    typedef Matrix<float, 2, 1> Vec2;
    typedef Matrix<float, 3, 1> Vec3;
    typedef Matrix<float, 4, 1> Vec4;
    typedef Matrix<float, 2, 2> Mat2x2;
    typedef Matrix<float, 3, 3> Mat3x3;
    typedef Matrix<float, 4, 4> Mat4x4;
}