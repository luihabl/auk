#pragma once 

#include <cmath>
#include <array>

#include "tinysdl/platform/log.h"

// M : rows
// N : columns

namespace TinySDL {

    template <typename T, size_t M, size_t N>
    struct Matrix {  

        std::array<T, M * N> data;

        Matrix() = default;
        Matrix(const Matrix & other); 
        Matrix(const std::array<T, N*M> arr) {data=arr;}
        template <typename... A> 
        Matrix(A... values): data{values...} {};

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

        static Matrix zeros();
        static Matrix ones();
        static Matrix identity();
    };

    template <typename T, size_t M> using Vec = Matrix<T, M, 1>;

    typedef Matrix<float, 2, 1> Vec2;
    typedef Matrix<float, 3, 1> Vec3;
    typedef Matrix<float, 4, 1> Vec4;
    typedef Matrix<float, 2, 2> Mat2x2;
    typedef Matrix<float, 3, 3> Mat3x3;
    typedef Matrix<float, 4, 4> Mat4x4;
    
    template <typename T, size_t M, size_t N>
    Matrix<T, M, N>::Matrix(const Matrix<T, M, N> & other) {
        data = other.data;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> & Matrix<T, M, N>::operator= (const Matrix<T, M, N> & other) {
        if (this != &other) 
            data = other.data;
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
        Matrix<T, M, N> new_matrix;
        new_matrix.data.fill(0.0f);
        return new_matrix; 
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::ones() {
        Matrix<T, M, N> new_matrix;
        new_matrix.data.fill(1.0f);
        return new_matrix; 
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::identity() {
        Matrix<T, M, N> id_matrix;
        id_matrix.data.fill(0.0f);
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
        inline float dot(const Matrix<float, M, 1> & a, const Matrix<float, M, 1> & b) {
            float sum = 0;
            for(size_t i=0; i<M; i++) 
                sum += a.data[i] * b.data[i];
            return sqrtf(sum);
        }

        template <size_t M>
        inline Matrix<float, M, M> matmul(const Matrix<float, M, M> & a, const Matrix<float, M, M> & b) {
            // Only supports square matrices for the moment
            Matrix<float, M, M> prod = Matrix<float, M, M>::zeros();
            for(size_t i = 0; i < M; i++)
                for(size_t j = 0; j < M; j++)
                    for(size_t k = 0; k < M; k++)
                        prod.data[i  + j * M] += a.data[i + k * M] * b.data[k + j * M];
            return prod;
        }

        inline void translate(Mat4x4 & mat, float x, float y, float z) {

            Mat4x4 trans = Mat4x4::identity();
            trans.data[0 + 3 * 4] = x;
            trans.data[1 + 3 * 4] = y;
            trans.data[2 + 3 * 4] = z;

            mat = matmul(trans, mat);
        }

        inline void rotate(Mat4x4 & mat, float radians) {
            
            Mat4x4 rot = Mat4x4::identity();
            float c = cos(radians);
            float s = sin(radians);
            
            rot[0 + 0 * 4] =  c;
            rot[1 + 0 * 4] =  s;
            rot[0 + 1 * 4] = -s;
            rot[1 + 1 * 4] =  c;

            mat = matmul(rot, mat);
        }

        inline void scale(Mat4x4 & mat, float sx, float sy, float sz) {

            Mat4x4 scl = Mat4x4::identity();
            scl[0 + 0 * 4] = sx;
            scl[1 + 1 * 4] = sy;
            scl[2 + 2 * 4] = sz; 

            mat = matmul(mat, scl);
        }

        inline void scale2(Mat4x4 & mat, float sx, float sy, float sz) {

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

        inline Mat4x4 gen_model(const Vec4 & dst_rect, const float & rot) {
            Mat4x4 model = Mat4x4::identity();
            
            MatrixMath::translate(model, dst_rect[0], dst_rect[1], 0.0f); 
            if(rot != 0.0f){
                MatrixMath::translate(model, 0.5f * dst_rect[2], 0.5f * dst_rect[3], 0.0f); 
                MatrixMath::rotate(model, rot);
                MatrixMath::translate(model, -0.5f * dst_rect[2], -0.5f * dst_rect[3], 0.0f); 
            }
            MatrixMath::scale(model, dst_rect[2], dst_rect[3], 1.0f);

            return model;
        }


        inline Mat4x4 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation) 
        {
            Mat4x4 model = Mat4x4::identity();

            // MatrixMath::translate(model, pos.data[0], pos.data[1], 1.0f);
            
            MatrixMath::translate(model, -origin.data[0], -origin.data[1], 1.0f);
            MatrixMath::scale2(model, scale.data[0], scale.data[1], 1.0f);
            
            // MatrixMath::translate(model, pos.data[0], pos.data[1], 1.0f);
            MatrixMath::rotate(model, rotation);
            MatrixMath::translate(model, pos.data[0], pos.data[1], 1.0f);
            // MatrixMath::translate(model, origin.data[0], origin.data[1], 1.0f);

            
            
            
            
            
            return model;
        }




    }
}