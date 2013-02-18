#ifndef SRC_TRANSFORM_H
#define SRC_TRANSFORM_H


#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


class Matrix4 {
  protected:
    P_FLT data[4][4];

  public:
    Matrix4() {}
    Matrix4(
      const P_FLT p00, const P_FLT p01, const P_FLT p02, const P_FLT p03,
      const P_FLT p10, const P_FLT p11, const P_FLT p12, const P_FLT p13,
      const P_FLT p20, const P_FLT p21, const P_FLT p22, const P_FLT p23,
      const P_FLT p30, const P_FLT p31, const P_FLT p32, const P_FLT p33) {
      data[0][0] = p00, data[0][1] = p01, data[0][2] = p02, data[0][3] = p03,
      data[1][0] = p10, data[1][1] = p11, data[1][2] = p12, data[1][3] = p13,
      data[2][0] = p20, data[2][1] = p21, data[2][2] = p22, data[2][3] = p23,
      data[3][0] = p30, data[3][1] = p31, data[3][2] = p32, data[3][3] = p33;
    }

    inline P_FLT * operator[](const unsigned int i) const {
      return const_cast<P_FLT *>(data[i]);
    }

    friend Vector3D operator *(const Vector3D &vector);
    friend Matrix4 operator +(const Matrix4 &A, const Matrix4&B);
    friend Matrix4 operator -(const Matrix4 &A, const Matrix4&B);
    friend Matrix4 operator *(const Matrix4 &A, const Matrix4&B);
};


class Transform: Matrix4 {
  public:
    Transform() {}
    Transform(
      const P_FLT p00, const P_FLT p01, const P_FLT p02, const P_FLT p03,
      const P_FLT p10, const P_FLT p11, const P_FLT p12, const P_FLT p13,
      const P_FLT p20, const P_FLT p21, const P_FLT p22, const P_FLT p23,
      const P_FLT p30, const P_FLT p31, const P_FLT p32, const P_FLT p33):
      Matrix4(p00, p01, p02, p03,
              p10, p11, p12, p13,
              p20, p21, p22, p23,
              p30, p31, p32, p33) {}

    static Transform identity() {
      return Transform(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Transform zero() {
      return Transform(0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f);
    }

    static Transform rotation() {
      return Transform();
    }

    static Transform translation(const Vector3D &direction) {
      return Transform(1.0f, 0.0f, 0.0f, direction.x,
                       0.0f, 1.0f, 0.0f, direction.y,
                       0.0f, 0.0f, 1.0f, direction.z,
                       0.0f, 0.0f, 0.0f, 1.0f);
    }


    void apply(Vector3D * vector) const {
      *vector = Vector3D(data[0][0] * vector->x + data[0][1] * vector->y +
                         data[0][2] * vector->z + data[0][3],
                         data[1][0] * vector->x + data[1][1] * vector->y +
                         data[1][2] * vector->z + data[1][3],
                         data[2][0] * vector->x + data[2][1] * vector->y +
                         data[2][2] * vector->z + data[2][3]);
      *vector *= 1.0f / (data[3][0] * vector->x + data[3][1] * vector->y +
                         data[3][2] * vector->z + data[3][3]);
    }
};


Vector3D operator *(const Matrix4 &matrix, const Vector3D &vector) {
  Vector3D result(matrix[0][0] * vector.x + matrix[0][1] * vector.y +
                  matrix[0][2] * vector.z + matrix[0][3],
                  matrix[1][0] * vector.x + matrix[1][1] * vector.y +
                  matrix[1][2] * vector.z + matrix[1][3],
                  matrix[2][0] * vector.x + matrix[2][1] * vector.y +
                  matrix[2][2] * vector.z + matrix[2][3]);
  result *= 1.0f / (matrix[3][0] * vector.x + matrix[3][1] * vector.y +
                    matrix[3][2] * vector.z + matrix[3][3]);

  return result;
}


Matrix4 operator +(const Matrix4 &A, const Matrix4 &B) {
  return Matrix4(A[0][0] + B[0][0], A[0][1] + B[0][1],
                 A[0][2] + B[0][2], A[0][3] + B[0][3],

                 A[1][0] + B[1][0], A[1][1] + B[1][1],
                 A[1][2] + B[1][2], A[1][3] + B[1][3],

                 A[2][0] + B[2][0], A[2][1] + B[2][1],
                 A[2][2] + B[2][2], A[2][3] + B[2][3],

                 A[3][0] + B[3][0], A[3][1] + B[3][1],
                 A[3][2] + B[3][2], A[3][3] + B[3][3]);
}

Matrix4 operator -(const Matrix4 &A, const Matrix4 &B) {
  return Matrix4(A[0][0] - B[0][0], A[0][1] - B[0][1],
                 A[0][2] - B[0][2], A[0][3] - B[0][3],

                 A[1][0] - B[1][0], A[1][1] - B[1][1],
                 A[1][2] - B[1][2], A[1][3] - B[1][3],

                 A[2][0] - B[2][0], A[2][1] - B[2][1],
                 A[2][2] - B[2][2], A[2][3] - B[2][3],

                 A[3][0] - B[3][0], A[3][1] - B[3][1],
                 A[3][2] - B[3][2], A[3][3] - B[3][3]);
}

Matrix4 operator *(const Matrix4 &A, const Matrix4 &B) {
  return Matrix4(A[0][0] * B[0][0] + A[0][1] * B[1][0] +
                 A[0][2] * B[2][0] + A[0][3] * B[3][0],
                 A[0][0] * B[0][1] + A[0][1] * B[1][1] +
                 A[0][2] * B[2][1] + A[0][3] * B[3][1],
                 A[0][0] * B[0][2] + A[0][1] * B[1][2] +
                 A[0][2] * B[2][2] + A[0][3] * B[3][2],
                 A[0][0] * B[0][3] + A[0][1] * B[1][3] +
                 A[0][2] * B[2][3] + A[0][3] * B[3][3],

                 A[1][0] * B[0][0] + A[1][1] * B[1][0] +
                 A[1][2] * B[2][0] + A[1][3] * B[3][0],
                 A[1][0] * B[0][1] + A[1][1] * B[1][1] +
                 A[1][2] * B[2][1] + A[1][3] * B[3][1],
                 A[1][0] * B[0][2] + A[1][1] * B[1][2] +
                 A[1][2] * B[2][2] + A[1][3] * B[3][2],
                 A[1][0] * B[0][3] + A[1][1] * B[1][3] +
                 A[1][2] * B[2][3] + A[1][3] * B[3][3],

                 A[2][0] * B[0][0] + A[2][1] * B[1][0] +
                 A[2][2] * B[2][0] + A[2][3] * B[3][0],
                 A[2][0] * B[0][1] + A[2][1] * B[1][1] +
                 A[2][2] * B[2][1] + A[2][3] * B[3][1],
                 A[2][0] * B[0][2] + A[2][1] * B[1][2] +
                 A[2][2] * B[2][2] + A[2][3] * B[3][2],
                 A[2][0] * B[0][3] + A[2][1] * B[1][3] +
                 A[2][2] * B[2][3] + A[2][3] * B[3][3],

                 A[3][0] * B[0][0] + A[3][1] * B[1][0] +
                 A[3][2] * B[2][0] + A[3][3] * B[3][0],
                 A[3][0] * B[0][1] + A[3][1] * B[1][1] +
                 A[3][2] * B[2][1] + A[3][3] * B[3][1],
                 A[3][0] * B[0][2] + A[3][1] * B[1][2] +
                 A[3][2] * B[2][2] + A[3][3] * B[3][2],
                 A[3][0] * B[0][3] + A[3][1] * B[1][3] +
                 A[3][2] * B[2][3] + A[3][3] * B[3][3]);
}


END_RAYTRACER


#endif  // SRC_TRANSFORM_H
