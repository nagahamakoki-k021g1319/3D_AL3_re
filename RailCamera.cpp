#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 position, const Vector3 rota) {
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rota;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	Vector3 rotate = {0, 0, 0};
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_W)) {
		move = {0, kCharacterSpeed, 0};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, -kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_A)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_D)) {
		move = {kCharacterSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//��]
	const float kChestRotSpeed = 0.02f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_U)) {
		rotate = {0, kChestRotSpeed, 0};
	} else if (input_->PushKey(DIK_I)) {
		rotate = {0, -kChestRotSpeed, 0};
	}
	worldTransform_.rotation_ += rotate;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = AffinTrans::MatVector(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f
	viewProjection_.up = AffinTrans::MatVector(up, worldTransform_.matWorld_);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

}

ViewProjection& RailCamera::GetViewProjection() { 
	return viewProjection_; 
}

WorldTransform* RailCamera::GetWorldPosition() { 
    

	return &worldTransform_;
}

