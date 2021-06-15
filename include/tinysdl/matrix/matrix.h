#pragma once 

#include <cmath>
#include <array>

#include "tinysdl/platform/log.h"

// M : rows
// N : columns

namespace TinySDL {

    // template <typename T, size_t M, size_t N> using Matrix = std::array<T, M * N>;
    
    template <typename T, size_t M, size_t N>
    struct Matrix : std::array<T, M * N> {

        static Matrix zeros();
        static Matrix ones();
        static Matrix identity();
    };

    typedef Matrix<float, 2, 2> Mat2x2;
    typedef Matrix<float, 3, 3> Mat3x3;
    typedef Matrix<float, 4, 4> Mat4x4;

    template <typename T, size_t M> using Vec = Matrix<T, M, 1>;
    typedef Vec<float, 2> Vec2;
    typedef Vec<float, 3> Vec3;
    typedef Vec<float, 4> Vec4;
    typedef Vec<uint8_t, 3> ByteVec3;
    typedef Vec<uint8_t, 4> ByteVec4;

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> Matrix<T, M, N>::zeros() {
        Matrix<T, M, N> new_matrix;
        new_matrix.fill(0.0f);
        return new_matrix; 
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> Matrix<T, M, N>::ones() {
        Matrix<T, M, N> new_matrix;
        new_matrix.fill(1.0f);
        return new_matrix; 
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> Matrix<T, M, N>::identity() {
        Matrix<T, M, N> id_matrix;
        id_matrix.fill(0.0f);
        if( M != N ) {
            Log::warn("Trying to create non-square identity matrix");
            return id_matrix;
        }

        for(size_t i = 0; i < M; i++)
            for(size_t j = 0; j < M; j++)
                if(i==j) id_matrix[i + j * M] = 1.0f;
        
        return id_matrix; 
    }
    
    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = data()[i] + other[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator- (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = data()[i] - other[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (T val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = data()[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (T val, Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = data()[i] * other[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (T val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = data()[i] * val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (T val, Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] * val;
        return result;
    }

    namespace MatrixMath{

        static const Mat4x4 identity_4x4 = Mat4x4::identity();
        static const Mat4x4 zeros_4x4 = Mat4x4::zeros();

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
            for(size_t j = 0; j < M; j++)
                for(size_t k = 0; k < M; k++)
                    for(size_t i = 0; i < M; i++)
                        prod.data[i  + j * M] += a.data[i + k * M] * b.data[k + j * M];
            return prod;
        }

        //Optimized 4x4 matrix multiplication
        inline Mat4x4 matmul4x4(Mat4x4 & a_mat, Mat4x4 & b_mat) {
            Mat4x4 prod;
            float * p = prod.data();
            float * a = a_mat.data();
            float * b = b_mat.data();

            p[0] = a[0]  * b[0]  + a[4] * b[1]  + a[8]   * b[2]  + a[12] * b[3];
            p[1] = a[1]  * b[0]  + a[5] * b[1]  + a[9]   * b[2]  + a[13] * b[3];
            p[2] = a[2]  * b[0]  + a[6] * b[1]  + a[10]  * b[2]  + a[14] * b[3];
            p[3] = a[3]  * b[0]  + a[7] * b[1]  + a[11]  * b[2]  + a[15] * b[3];
            p[4] = a[0]  * b[4]  + a[4] * b[5]  + a[8]   * b[6]  + a[12] * b[7];
            p[5] = a[1]  * b[4]  + a[5] * b[5]  + a[9]   * b[6]  + a[13] * b[7];
            p[6] = a[2]  * b[4]  + a[6] * b[5]  + a[10]  * b[6]  + a[14] * b[7];
            p[7] = a[3]  * b[4]  + a[7] * b[5]  + a[11]  * b[6]  + a[15] * b[7];
            p[8] = a[0]  * b[8]  + a[4] * b[9]  + a[8]   * b[10] + a[12] * b[11];
            p[9] = a[1]  * b[8]  + a[5] * b[9]  + a[9]   * b[10] + a[13] * b[11];
            p[10] = a[2] * b[8]  + a[6] * b[9]  + a[10]  * b[10] + a[14] * b[11];
            p[11] = a[3] * b[8]  + a[7] * b[9]  + a[11]  * b[10] + a[15] * b[11];
            p[12] = a[0] * b[12] + a[4] * b[13] + a[8]   * b[14] + a[12] * b[15];
            p[13] = a[1] * b[12] + a[5] * b[13] + a[9]   * b[14] + a[13] * b[15];
            p[14] = a[2] * b[12] + a[6] * b[13] + a[10]  * b[14] + a[14] * b[15];
            p[15] = a[3] * b[12] + a[7] * b[13] + a[11]  * b[14] + a[15] * b[15];

        	return prod;
        }

        inline void translate(Mat4x4 & mat, float x, float y, float z) {

            Mat4x4 trans = identity_4x4;
            trans[0 + 3 * 4] = x;
            trans[1 + 3 * 4] = y;
            trans[2 + 3 * 4] = z;

            mat = matmul4x4(trans, mat);
        }

        inline void rotate(Mat4x4 & mat, float radians) {
            
            Mat4x4 rot = identity_4x4;
            float c = cos(radians);
            float s = sin(radians);
            
            rot[0 + 0 * 4] =  c;
            rot[1 + 0 * 4] =  s;
            rot[0 + 1 * 4] = -s;
            rot[1 + 1 * 4] =  c;

            mat = matmul4x4(rot, mat);
        }


        inline void scale(Mat4x4 & mat, float sx, float sy, float sz) {

            Mat4x4 scl = identity_4x4;
            scl[0 + 0 * 4] = sx;
            scl[1 + 1 * 4] = sy;
            scl[2 + 2 * 4] = sz; 

            mat = matmul4x4(scl, mat);
        }


        inline Mat4x4 ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
            Mat4x4 ortho_mat = identity_4x4;

            ortho_mat[0 + 0 * 4] =   2.0f / (right - left);
            ortho_mat[1 + 1 * 4] =   2.0f / (top   - bottom);
            ortho_mat[2 + 2 * 4] = - 2.0f / (z_far - z_near);

            ortho_mat[0 + 3 * 4] = - (right + left)   / (right - left);
            ortho_mat[1 + 3 * 4] = - (top   + bottom) / (top   - bottom);
            ortho_mat[2 + 3 * 4] = - (z_far + z_near) / (z_far - z_near);

            return ortho_mat;
        }


        inline Mat4x4 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation) {
            Mat4x4 model = identity_4x4;

            if(origin[0] != 0 || origin[1] != 0)
                MatrixMath::translate(model, -origin[0], -origin[1], 1.0f);

            if(scale[0] != 1 || scale[1] != 1)
                MatrixMath::scale(model, scale[0], scale[1], 1.0f);
            
            if(rotation != 0)
                MatrixMath::rotate(model, rotation);

            if(pos[0] != 0 || pos[1] != 0) 
                MatrixMath::translate(model, pos[0], pos[1], 1.0f);

            return model;
        }

    }
}