#pragma once
#include "GameScene.h"

Matrix4 scale(float scaleX, float scaleY, float scaleZ);

Matrix4 rotationZ(float angleZ);

Matrix4 rotationX(float angleX);

Matrix4 rotationY(float angleY);

Matrix4 translation(float translationX, float translationY, float translationZ);