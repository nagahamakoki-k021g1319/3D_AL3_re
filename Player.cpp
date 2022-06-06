#include "Player.h"
#include "Input.h"
#include "GameScene.h"
#include "DebugText.h"

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	//�L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	//�L�[�ŃL�����N�^�[�̈ړ��x�N�g����ύX
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	//���W�ړ�
	

	//�s��X�V


	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 150);

}

void Player::Draw() {
}
