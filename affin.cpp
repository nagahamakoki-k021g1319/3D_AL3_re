#include "affin.h"

Matrix4 scale(float scaleX, float scaleY, float scaleZ) {

	//マトリックスの宣言
	Matrix4 matscale;

	//拡大の変数
	matscale.m[0][0] = scaleX;
	matscale.m[1][1] = scaleY;
	matscale.m[2][2] = scaleZ;
	matscale.m[3][3] = 1.0f;

	return matscale;
}

Matrix4 rotationZ(float rotationX, float rotationY, float rotationZ) {

	//マトリックスの宣言
	Matrix4 matrotation;

	//拡大の変数
	matrotation.m[0][0] = rotationX;
	matrotation.m[0][1] = rotationX;
	matrotation.m[1][0] = rotationY;
	matrotation.m[1][1] = rotationY;
	matrotation.m[2][2] = 1.0f;
	matrotation.m[3][3] = 1.0f;

	return matrotation;
}