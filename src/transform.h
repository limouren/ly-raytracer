#ifndef SRC_TRANSFORM_H
#define SRC_TRANSFORM_H


#include <math.h>

#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


// TODO(kent): Optimize this class
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

    Matrix4 inverse() {
      // TODO(kent): Implement fast gaussian elimination
    }

    friend Vector3D operator *(const Vector3D &vector);
    friend Matrix4 operator +(const Matrix4 &A, const Matrix4&B);
    friend Matrix4 operator -(const Matrix4 &A, const Matrix4&B);
    friend Matrix4 operator *(const Matrix4 &A, const Matrix4&B);

    static Matrix4 Identity() {
      return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Matrix4 Zero() {
      return Matrix4(0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f);
    }
};


class Transform {
  protected:
    P_FLT xScale, yScale, zScale, radians;
    Vector3D rotateAxis, translate;

    Matrix4 * matrix,
            * scale,
            * rotation,
            * translation,
            * inverse,
            * invScale,
            * invRotation,
            * invTranslation;

  public:
    Transform() {}
    Transform(const P_FLT xScale, const P_FLT yScale, const P_FLT zScale,
              const P_FLT radians, const Vector3D &rotateAxis,
              const Vector3D &translate):
      xScale(xScale), yScale(yScale), zScale(zScale), radians(radians),
      rotateAxis(rotateAxis), translate(translate),
      matrix(new Matrix4()), inverse(new Matrix4()) {
      scale = new Matrix4(xScale, 0.0f, 0.0f, 0.0f,
                          0.0f, yScale, 0.0f, 0.0f,
                          0.0f, 0.0f, zScale, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

      invScale = new Matrix4(1.0f / xScale, 0.0f, 0.0f, 0.0f,
                             0.0f, 1.0f / yScale, 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f / zScale, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f);

      this->rotateAxis.normalize();
      P_FLT x = this->rotateAxis.x,
            y = this->rotateAxis.y,
            z = this->rotateAxis.z,
            cosine = cos(radians),
            sine = sin(radians);

      rotation = new Matrix4(cosine + x * x * (1 - cosine),
                             x * y * (1 - cosine) - z * sine,
                             x * z * (1 - cosine) + y * sine,
                             0.0f,

                             x * y * (1 - cosine) + z * sine,
                             cosine + y * y * (1 - cosine),
                             y * z * (1 - cosine) - x * sine,
                             0.0f,

                             x * z * (1 - cosine) - y * sine,
                             y * z * (1 - cosine) + x * sine,
                             cosine + z * z * (1 - cosine),
                             0.0f,

                             0.0f, 0.0f, 0.0f, 1.0f);

      invRotation = new Matrix4(cosine + x * x * (1 - cosine),
                              x * y * (1 - cosine) + z * sine,
                              x * z * (1 - cosine) - y * sine,
                              0.0f,

                              x * y * (1 - cosine) - z * sine,
                              cosine + y * y * (1 - cosine),
                              y * z * (1 - cosine) + x * sine,
                              0.0f,

                              x * z * (1 - cosine) + y * sine,
                              y * z * (1 - cosine) - x * sine,
                              cosine + z * z * (1 - cosine),
                              0.0f,

                              0.0f, 0.0f, 0.0f, 1.0f);

      translation = new Matrix4(1.0f, 0.0f, 0.0f, translate.x,
                                0.0f, 1.0f, 0.0f, translate.y,
                                0.0f, 0.0f, 1.0f, translate.z,
                                0.0f, 0.0f, 0.0f, 1.0f);

      invTranslation = new Matrix4(1.0f, 0.0f, 0.0f, -translate.x,
                                   0.0f, 1.0f, 0.0f, -translate.y,
                                   0.0f, 0.0f, 1.0f, -translate.z,
                                   0.0f, 0.0f, 0.0f, 1.0f);

      *matrix = *translation * *rotation * *scale;

      *inverse = *invScale * *invRotation * *invTranslation;
    }

    ~Transform() {
      delete matrix,
      delete scale,
      delete rotation,
      delete translation,
      delete inverse,
      delete invScale,
      delete invRotation,
      delete invTranslation;
    }

    void transformVector(Vector3D * vector) const {
      Vector3D result(
        *rotation[0][0] * vector->x + *rotation[0][1] * vector->y +
        *rotation[0][2] * vector->z + *rotation[0][3],
        *rotation[1][0] * vector->x + *rotation[1][1] * vector->y +
        *rotation[1][2] * vector->z + *rotation[1][3],
        *rotation[2][0] * vector->x + *rotation[2][1] * vector->y +
        *rotation[2][2] * vector->z + *rotation[2][3]);
      result *= 1.0f / (*rotation[3][0] * vector->x +
                        *rotation[3][1] * vector->y +
                        *rotation[3][2] * vector->z +
                        *rotation[3][3]);

      *vector = result;
    }

    void transformPoint(Point3D * point) const {
      Point3D result(*matrix[0][0] * point->x + *matrix[0][1] * point->y +
                     *matrix[0][2] * point->z + *matrix[0][3],
                     *matrix[1][0] * point->x + *matrix[1][1] * point->y +
                     *matrix[1][2] * point->z + *matrix[1][3],
                     *matrix[2][0] * point->x + *matrix[2][1] * point->y +
                     *matrix[2][2] * point->z + *matrix[2][3]);
      result *= 1.0f / (*matrix[3][0] * point->x + *matrix[3][1] * point->y +
                        *matrix[3][2] * point->z + *matrix[3][3]);

      *point = result;
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
