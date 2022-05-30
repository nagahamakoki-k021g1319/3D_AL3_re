#include "affin.h"

Matrix4 scale(float scaleX, float scaleY, float scaleZ) {

	//�}�g���b�N�X�̐錾
	Matrix4 matscale;

	//�g��̕ϐ�
	matscale.m[0][0] = scaleX;
	matscale.m[1][1] = scaleY;
	matscale.m[2][2] = scaleZ;
	matscale.m[3][3] = 1.0f;

	return matscale;
}

Matrix4 rotationZ(float rotationX, float rotationY, float rotationZ) {

	//�}�g���b�N�X�̐錾
	Matrix4 matrotation;

	//�g��̕ϐ�
	matrotation.m[0][0] = rotationX;
	matrotation.m[0][1] = rotationX;
	matrotation.m[1][0] = rotationY;
	matrotation.m[1][1] = rotationY;
	matrotation.m[2][2] = 1.0f;
	matrotation.m[3][3] = 1.0f;

	return matrotation;
}