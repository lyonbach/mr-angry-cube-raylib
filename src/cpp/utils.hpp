#ifndef UTILS_HPP
#define UTILS_HPP

#include "raylib.h"
#include "raymath.h"

Vector3 SnapVectorToCardinal(Vector3 v);
Matrix SnapRotationToGrid(Matrix mat);
Matrix SnapTranslationToGrid(Matrix mat, float gridSize = 1.0f);
Matrix SnapMatrixToGrid(Matrix mat, float gridSize = 1.0f);

#endif
