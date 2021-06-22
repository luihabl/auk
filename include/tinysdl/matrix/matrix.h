#pragma once 

#include <cmath>
#include <algorithm>

// M : rows
// N : columns

namespace TinySDL {
    
    template <typename T, size_t M, size_t N>
    struct Matrix {

        T data[M * N];

        T length();
        Matrix normalized();
        size_t size() {return M * N;}

        void fill(T val) {std::fill_n(data, M * N, val);}

        T operator [](size_t i) const {return data[i];}
        T & operator [](size_t i) {return data[i];}

        static const Matrix zeros;
        static const Matrix ones;
        static const Matrix identity;
    };

    typedef Matrix<float, 2, 2> Mat2x2;
    typedef Matrix<float, 3, 2> Mat3x2;
    typedef Matrix<float, 3, 3> Mat3x3;
    typedef Matrix<float, 4, 4> Mat4x4;

    template <typename T, size_t M> using Vec = Matrix<T, M, 1>;
    typedef Vec<float, 2> Vec2;
    typedef Vec<float, 3> Vec3;
    typedef Vec<float, 4> Vec4;
    typedef Vec<uint8_t, 3> ByteVec3;
    typedef Vec<uint8_t, 4> ByteVec4;

    template <typename T, size_t M, size_t N>
    inline T Matrix<T, M, N>::length() {
        T sum_sqr = 0;
        for (const auto & e : data)
            sum_sqr += e * e;
        return sqrtf(sum_sqr);
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> Matrix<T, M, N>::normalized() {
        T len = length();
        return *this / len;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (const Matrix<T, M, N> & a, const Matrix<T, M, N> & b) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = a.data[i] + b.data[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator- (const Matrix<T, M, N> & a, const Matrix<T, M, N> & b) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = a.data[i] - b.data[i];
        return result;
    }
    
    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (const Matrix<T, M, N> & a, const Matrix<T, M, N> & b) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = a.data[i] * b.data[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (const Matrix<T, M, N> & other, const T & val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator+ (const T & val, const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] + val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (const Matrix<T, M, N> & other, const T & val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] * val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator* (const T & val, const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] * val;
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator/ (const T & val, const Matrix<T, M, N> & other) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = val / other[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    inline Matrix<T, M, N> operator/ (const Matrix<T, M, N> & other, const T & val) {
        Matrix<T, M, N> result;
        for (size_t i = 0; i < M * N; i++) 
            result[i] = other[i] / val;
        return result;
    }

    namespace MatrixMath{
        
        template <typename T, size_t M, size_t N>
        inline Matrix<T, M, N> filled (T val) {
            Matrix<T, M, N> result;
            result.fill(val);
            return result;
        }

        template <typename T, size_t M, size_t N>
        inline Matrix<T, M, N> identity() {
            Matrix<T, M, N> id_matrix;
            id_matrix.fill(0.0f);
            
            for (size_t i = 0; i < M; i++)
                for (size_t j = 0; j < M; j++)
                    if (i == j) id_matrix[i + j * M] = 1.0f;

            return id_matrix;
        }

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
            Matrix<float, M, M> prod = Matrix<float, M, M>::zeros;
            for(size_t j = 0; j < M; j++)
                for(size_t k = 0; k < M; k++)
                    for(size_t i = 0; i < M; i++) {
                        prod.data[i  + j * M] += a.data[i + k * M] * b.data[k + j * M];
                    }

            return prod;
        }

        //Optimized 4x4 matrix multiplication
        inline Mat4x4 matmul4x4(const Mat4x4 & a, const Mat4x4 & b) {
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

        inline Mat3x2 matmul_transform(const Mat3x2 & a, const Mat3x2 & b) {            
            return {
                a[0] * b[0] + a[2] * b[1],
                a[1] * b[0] + a[3] * b[1],
                a[0] * b[2] + a[2] * b[3],
                a[1] * b[2] + a[3] * b[3],
                a[0] * b[4] + a[2] * b[5] + a[4],
                a[1] * b[4] + a[3] * b[5] + a[5]
            };
        }

        inline void translate(Mat4x4 & mat, float x, float y, float z) {

            Mat4x4 trans = Mat4x4::identity;
            trans[0 + 3 * 4] = x;
            trans[1 + 3 * 4] = y;
            trans[2 + 3 * 4] = z;

            mat = matmul4x4(trans, mat);
        }

        inline void rotate(Mat4x4 & mat, float radians) {
            
            Mat4x4 rot = Mat4x4::identity;
            float c = cos(radians);
            float s = sin(radians);
            
            rot[0 + 0 * 4] =  c;
            rot[1 + 0 * 4] =  s;
            rot[0 + 1 * 4] = -s;
            rot[1 + 1 * 4] =  c;

            mat = matmul4x4(rot, mat);
        }


        inline void scale(Mat4x4 & mat, float sx, float sy, float sz) {

            Mat4x4 scl = Mat4x4::identity;
            scl[0 + 0 * 4] = sx;
            scl[1 + 1 * 4] = sy;
            scl[2 + 2 * 4] = sz; 

            mat = matmul4x4(scl, mat);
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


        inline Mat4x4 gen_transform(const Vec2 & pos, const Vec2 & scale, const Vec2 & origin, const float & rotation) {
            Mat4x4 model = Mat4x4::identity;

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

    template <typename T, size_t M, size_t N> const Matrix<T, M, N> Matrix<T, M, N>::zeros = MatrixMath::filled<T, M, N>(0);
    template <typename T, size_t M, size_t N> const Matrix<T, M, N> Matrix<T, M, N>::ones = MatrixMath::filled<T, M, N>(1);
    template <typename T, size_t M, size_t N> const Matrix<T, M, N> Matrix<T, M, N>::identity = MatrixMath::identity<T, M, N>();
}