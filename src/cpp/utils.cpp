#include "utils.hpp"
#include <cmath>

Vector3 SnapVectorToCardinal(Vector3 v) {
    float ax = fabsf(v.x);
    float ay = fabsf(v.y);
    float az = fabsf(v.z);

    if (ax >= ay && ax >= az) {
        return { v.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f };
    } else if (ay >= ax && ay >= az) {
        return { 0.0f, v.y > 0 ? 1.0f : -1.0f, 0.0f };
    } else {
        return { 0.0f, 0.0f, v.z > 0 ? 1.0f : -1.0f };
    }
}

Matrix SnapRotationToGrid(Matrix mat) {
    Vector3 right = { mat.m0, mat.m1, mat.m2 };
    Vector3 up = { mat.m4, mat.m5, mat.m6 };

    Vector3 snappedRight = SnapVectorToCardinal(right);
    Vector3 snappedUp = SnapVectorToCardinal(up);
    Vector3 snappedForward = Vector3CrossProduct(snappedRight, snappedUp);

    mat.m0 = snappedRight.x; mat.m1 = snappedRight.y; mat.m2 = snappedRight.z;
    mat.m4 = snappedUp.x;    mat.m5 = snappedUp.y;    mat.m6 = snappedUp.z;
    mat.m8 = snappedForward.x; mat.m9 = snappedForward.y; mat.m10 = snappedForward.z;

    return mat;
}

Matrix SnapTranslationToGrid(Matrix mat, float gridSize) {
    mat.m12 = roundf(mat.m12 / gridSize) * gridSize;
    mat.m13 = roundf(mat.m13 / gridSize) * gridSize;
    mat.m14 = roundf(mat.m14 / gridSize) * gridSize;
    return mat;
}

Matrix SnapMatrixToGrid(Matrix mat, float gridSize) {
    mat = SnapRotationToGrid(mat);
    mat = SnapTranslationToGrid(mat, gridSize);
    return mat;
}
