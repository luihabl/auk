#pragma once 

#include <cmath>
#include <array>

#include "tinysdl/core/log.h"

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
        Matrix operator- (const Matrix & other);
        Matrix operator+ (T val);
        Matrix operator* (const Matrix & other);
        Matrix operator* (T val);

        size_t memsize(){return sizeof(T) * M * N;}
        void print();

        std::array<size_t, 2> shape = {0, 0};
        T * data = nullptr;

        static Matrix zeros();
        static Matrix ones();
        static Matrix identity();
    };

    typedef Matrix<float, 2, 1> Vec2;
    typedef Matrix<float, 3, 1> Vec3;
    typedef Matrix<float, 4, 1> Vec4;
    typedef Matrix<float, 2, 2> Mat2x2;
    typedef Matrix<float, 3, 3> Mat3x3;
    typedef Matrix<float, 4, 4> Mat4x4;
    
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
    Matrix<T, M, N> Matrix<T, M, N>::operator- (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result.data[i] = data[i] - other.data[i];
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
        for(size_t i=0; i < M * N; i++) 
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

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::identity() {
        Matrix<T, M, N> id_matrix = Matrix<T, M, N>::zeros();
        if( M != N ) {
            Log::warn("Trying to create non-square identity matrix");
            return id_matrix;
        }

        for(size_t i = 0; i < M; i++)
            for(size_t j = 0; j < M; j++)
                if(i==j) id_matrix.data[i + j * M] = 1.0f;
        
        return id_matrix; 
    }

    template <typename T, size_t M, size_t N>
    void Matrix<T, M, N>::print() {
        for(size_t i = 0; i < M; i++){
            for(size_t j = 0; j < N; j++) {
                Log::print("%f\t", data[i + j * N]);
            }
            Log::print("\n");
        }
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
            // Only supports square matrices for the moment
            Matrix<float, M, M> prod = Matrix<float, M, M>::zeros();
            for(size_t i = 0; i < M; i++)
                for(size_t j = 0; j < M; j++)
                    for(size_t k = 0; k < M; k++)
                        prod.data[i * M + j] += a.data[i * M + k] * b.data[k * M + j];
            return prod;
        }

        inline void translate(Mat4x4 & mat, float x, float y, float z) {

            Mat4x4 trans = Mat4x4::identity();
            trans[0 + 3 * 4] += x;
            trans[1 + 3 * 4] += y;
            trans[2 + 3 * 4] += z;

            mat = matmul(trans, mat);
        }

        inline void rotate(Mat4x4 & mat, float radians) {
            
            Mat4x4 rot = Mat4x4::identity();
            rot[0 + 0 * 4] = cos(radians);
            rot[1 + 0 * 4] = sin(radians);
            rot[0 + 1 * 4] = -sin(radians);
            rot[1 + 1 * 4] = cos(radians);

            mat = matmul(rot, mat);
        }

        inline void scale(Mat4x4 & mat, float sx, float sy, float sz) {

            Mat4x4 scl = Mat4x4::identity();
            scl[0 + 0 * 4] = sx;
            scl[1 + 1 * 4] = sy;
            scl[2 + 2 * 4] = sz; 

            mat = matmul(scl, mat);
        }

        inline Mat4x4 ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
            Mat4x4 ortho_mat = Mat4x4::identity();

            ortho_mat[0 + 0 * 4] =   2.0f / (right - left);
            ortho_mat[1 + 1 * 4] =   2.0f / (top   - bottom);
            ortho_mat[2 + 2 * 4] = - 2.0f / (z_far - z_near);

            ortho_mat[0 + 3 * 4] = - (right + left)   / (right - left);
            ortho_mat[1 + 3 * 4] = - (top   + bottom) / (top   - bottom);
            ortho_mat[2 + 3 * 4] = - (z_far + z_near) / (z_far - z_near);

            return ortho_mat;
        }

    }
}