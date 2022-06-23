#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) { 
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("Black.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	
}

void PlayerBullet::Update() {
	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
