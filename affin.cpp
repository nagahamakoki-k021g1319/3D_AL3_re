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

Matrix4 rotationZ(float angleZ) {

	//マトリックスの宣言
	Matrix4 matRotZ;

	//拡大の変数
	matRotZ.m[0][0] = cosf(π/4);
	matRotZ.m[0][1] = sinf(π/4);
	matRotZ.m[1][0] = -sinf(π/4);
	matRotZ.m[1][1] = cosf(π/4);
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	return matRotZ;
}

Matrix4 rotationX(float angleX) {

	//マトリックスの宣言
	Matrix4 matRotX;

	//拡大の変数
	matRotX.m[0][0] = 1.0f;
	matRotX.m[1][1] = cosf(π/4);
	matRotX.m[1][2] = sinf(π/4);
	matRotX.m[2][1] = -sinf(π/4);
	matRotX.m[2][2] = cosf(π/4);
	matRotX.m[3][3] = 1.0f;

	return matRotX;
}

Matrix4 rotationY(float angleY) {

	//マトリックスの宣言
	Matrix4 matRotY;

	//拡大の変数
	matRotY.m[0][0] = cosf(π / 4);
	matRotY.m[0][2] = -sinf(π / 4);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[2][0] = sinf(π / 4);
	matRotY.m[2][2] = cosf(π / 4);
	matRotY.m[3][3] = 1.0f;

	return matRotY;
}

Matrix4 translation(float translationX, float translationY, float translationZ) {

	//マトリックスの宣言
	Matrix4 matTrans;

	//拡大の変数
	matTrans.m[0][0] = 1.0f;
	matTrans.m[1][1] = 1.0f;
	matTrans.m[2][2] = 1.0f;
	matTrans.m[3][0] = translationX;
	matTrans.m[3][1] = translationY;
	matTrans.m[3][2] = translationZ;
	matTrans.m[3][3] = 1.0f;

	return matTrans;
}
