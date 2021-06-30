#pragma once

#include "tinysdl/numerics/matrix.h"

namespace TinySDL::LinAlg2D{
    Mat3x2 matmul(const Mat3x2 & a, const Mat3x2 & b);
    Mat3x2 gen_translation (float x, float y);
    Mat3x2 gen_rotation (float radians);
    Mat3x2 gen_scale (float sx, float sy);
    Mat3x2 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation);
}

namespace TinySDL::LinAlg3D {
    Mat4x4 matmul(const Mat4x4 & a, const Mat4x4 & b);
    void gen_translate(Mat4x4 & mat, float x, float y, float z);
    void gen_rotate(Mat4x4 & mat, float radians);
    void gen_scale(Mat4x4 & mat, float sx, float sy, float sz);
    Mat4x4 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation);
}

namespace TinySDL::LinAlg {

    template <size_t M>
    inline float dot(const Matrix<float, M, 1> & a, const Matrix<float, M, 1> & b) {
        float sum = 0;
        for(size_t i=0; i<M; i++) 
            sum += a.data[i] * b.data[i];
        return sum;
    }

    template <size_t M>
    inline Matrix<float, M, M> matmul(const Matrix<float, M, M> & a, const Matrix<float, M, M> & b) {
        // Only supports square matrices for the moment
        Matrix<float, M, M> prod = Matrix<float, M, M>::zeros;
        for(size_t j = 0; j < M; j++)
            for(size_t k = 0; k < M; k++)
                for(size_t i = 0; i < M; i++) {
                    prod.data[i  + j * M] += a.data[i + k * M] * b.data[k + j * M];
                }

        return prod;
    }

    inline Mat4x4 ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
        Mat4x4 ortho_mat = Mat4x4::identity;

        ortho_mat[0 + 0 * 4] =   2.0f / (right - left);
        ortho_mat[1 + 1 * 4] =   2.0f / (top   - bottom);
        ortho_mat[2 + 2 * 4] = - 2.0f / (z_far - z_near);

        ortho_mat[0 + 3 * 4] = - (right + left)   / (right - left);
        ortho_mat[1 + 3 * 4] = - (top   + bottom) / (top   - bottom);
        ortho_mat[2 + 3 * 4] = - (z_far + z_near) / (z_far - z_near);

        return ortho_mat;
    }

}

