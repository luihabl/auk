#pragma once

#include "tinysdl/numerics/matrix.h"


namespace TinySDL::MatrixMath {

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


namespace TinySDL::MatrixMath3D {
        
    inline Mat4x4 matmul(const Mat4x4 & a, const Mat4x4 & b) {
        //Unrolled 4x4 matrix multiplication
        return {
            a[0]  * b[0]  + a[4] * b[1]  + a[8]   * b[2]  + a[12] * b[3],
            a[1]  * b[0]  + a[5] * b[1]  + a[9]   * b[2]  + a[13] * b[3],
            a[2]  * b[0]  + a[6] * b[1]  + a[10]  * b[2]  + a[14] * b[3],
            a[3]  * b[0]  + a[7] * b[1]  + a[11]  * b[2]  + a[15] * b[3],
            a[0]  * b[4]  + a[4] * b[5]  + a[8]   * b[6]  + a[12] * b[7],
            a[1]  * b[4]  + a[5] * b[5]  + a[9]   * b[6]  + a[13] * b[7],
            a[2]  * b[4]  + a[6] * b[5]  + a[10]  * b[6]  + a[14] * b[7],
            a[3]  * b[4]  + a[7] * b[5]  + a[11]  * b[6]  + a[15] * b[7],
            a[0]  * b[8]  + a[4] * b[9]  + a[8]   * b[10] + a[12] * b[11],
            a[1]  * b[8]  + a[5] * b[9]  + a[9]   * b[10] + a[13] * b[11],
            a[2] * b[8]  + a[6] * b[9]  + a[10]  * b[10] + a[14] * b[11],
            a[3] * b[8]  + a[7] * b[9]  + a[11]  * b[10] + a[15] * b[11],
            a[0] * b[12] + a[4] * b[13] + a[8]   * b[14] + a[12] * b[15],
            a[1] * b[12] + a[5] * b[13] + a[9]   * b[14] + a[13] * b[15],
            a[2] * b[12] + a[6] * b[13] + a[10]  * b[14] + a[14] * b[15],
            a[3] * b[12] + a[7] * b[13] + a[11]  * b[14] + a[15] * b[15]
        };
    }

    inline void gen_translate(Mat4x4 & mat, float x, float y, float z) {

        Mat4x4 trans = Mat4x4::identity;
        trans[0 + 3 * 4] = x;
        trans[1 + 3 * 4] = y;
        trans[2 + 3 * 4] = z;

        mat = matmul(trans, mat);
    }

    inline void gen_rotate(Mat4x4 & mat, float radians) {
        
        Mat4x4 rot = Mat4x4::identity;
        float c = cos(radians);
        float s = sin(radians);
        
        rot[0 + 0 * 4] =  c;
        rot[1 + 0 * 4] =  s;
        rot[0 + 1 * 4] = -s;
        rot[1 + 1 * 4] =  c;

        mat = matmul(rot, mat);
    }


    inline void gen_scale(Mat4x4 & mat, float sx, float sy, float sz) {

        Mat4x4 scl = Mat4x4::identity;
        scl[0 + 0 * 4] = sx;
        scl[1 + 1 * 4] = sy;
        scl[2 + 2 * 4] = sz; 

        mat = matmul(scl, mat);
    }

    inline Mat4x4 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation) {
        Mat4x4 model = Mat4x4::identity;

        if(origin[0] != 0 || origin[1] != 0)
            gen_translate(model, -origin[0], -origin[1], 1.0f);

        if(scale[0] != 1 || scale[1] != 1)
            gen_scale(model, scale[0], scale[1], 1.0f);
        
        if(rotation != 0)
            gen_rotate(model, rotation);

        if(pos[0] != 0 || pos[1] != 0) 
            gen_translate(model, pos[0], pos[1], 1.0f);

        return model;
    }
}

namespace TinySDL::MatrixMath2D{

    inline Mat3x2 matmul(const Mat3x2 & a, const Mat3x2 & b) {      
        //This is an optimized 3x3 matrix multiplication that ignores the last row
        return {
            a[0] * b[0] + a[2] * b[1],
            a[1] * b[0] + a[3] * b[1],
            a[0] * b[2] + a[2] * b[3],
            a[1] * b[2] + a[3] * b[3],
            a[0] * b[4] + a[2] * b[5] + a[4],
            a[1] * b[4] + a[3] * b[5] + a[5]
        };
    }

    inline Mat3x2 gen_translation (float x, float y) {
        return {1, 0, 0, 1, x, y};
    }

    inline Mat3x2 gen_rotation (float radians) {
        const float c = cosf(radians);
        const float s = sinf(radians);
        return {c, s, -s, c, 0, 0};
    }

    inline Mat3x2 gen_scale (float sx, float sy) {
        return {sx, 0, 0, sy, 0, 0};
    }


    inline Mat3x2 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation) {
        Mat3x2 model{1, 0, 0, 1, 0, 0};

        if(origin[0] != 0 || origin[1] != 0)
            model = matmul(gen_translation(-origin[0], -origin[1]), model);

        if(scale[0] != 1 || scale[1] != 1)
            model = matmul(gen_scale(scale[0], scale[1]), model);
        
        if(rotation != 0)
            model = matmul(gen_rotation(rotation), model);

        if(pos[0] != 0 || pos[1] != 0) 
            model = matmul(gen_translation(pos[0], pos[1]), model);

        return model;
    }


}