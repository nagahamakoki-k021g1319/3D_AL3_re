#include "affin.h"

//�g��
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

//Z����]
Matrix4 rotationZ(float angleZ) {

	//�}�g���b�N�X�̐錾
	Matrix4 matRotZ;

	//�g��̕ϐ�
	matRotZ.m[0][0] = cosf(angleZ);
	matRotZ.m[0][1] = sinf(angleZ);
	matRotZ.m[1][0] = -sinf(angleZ);
	matRotZ.m[1][1] = cosf(angleZ);
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	return matRotZ;
}

//X����]
Matrix4 rotationX(float angleX) {

	//�}�g���b�N�X�̐錾
	Matrix4 matRotX;

	//�g��̕ϐ�
	matRotX.m[0][0] = 1.0f;
	matRotX.m[1][1] = cosf(angleX);
	matRotX.m[1][2] = sinf(angleX);
	matRotX.m[2][1] = -sinf(angleX);
	matRotX.m[2][2] = cosf(angleX);
	matRotX.m[3][3] = 1.0f;

	return matRotX;
}

//Y����]
Matrix4 rotationY(float angleY) {

	//�}�g���b�N�X�̐錾
	Matrix4 matRotY;

	//�g��̕ϐ�
	matRotY.m[0][0] = cosf(angleY);
	matRotY.m[0][2] = -sinf(angleY);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[2][0] = sinf(angleY);
	matRotY.m[2][2] = cosf(angleY);
	matRotY.m[3][3] = 1.0f;

	return matRotY;
}

//���s�ړ�
Matrix4 translation(float translationX, float translationY, float translationZ) {

	//�}�g���b�N�X�̐錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�g��̕ϐ�
	matTrans.m[0][0] = 1.0f;
	matTrans.m[1][1] = 1.0f;
	matTrans.m[2][2] = 1.0f;
	matTrans.m[3][0] = translationX;
	matTrans.m[3][1] = translationY;
	matTrans.m[3][2] = translationZ;
	matTrans.m[3][3] = 1.0f;

	return matTrans;
}