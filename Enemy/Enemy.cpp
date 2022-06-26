#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�������W���Z�b�g
	worldTransform_.translation_ = {0,3,0};

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Enemy::Update() {
	//�G�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�G�̈ړ��̑���
	const float kCharacterSpeed = 0.1f;
	//�G�̈ړ�����
	move = {0, 0, -kCharacterSpeed};
	worldTransform_.translation_ += move;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	

	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "Enemytranslation : %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);



}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}