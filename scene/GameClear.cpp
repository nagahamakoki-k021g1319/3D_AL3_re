#include "GameClear.h"

void GameClear::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.9, 1.5, -20};
	worldTransform_.scale_ = {15, 5, 1};
}

void GameClear::Update() {
	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();
}

void GameClear::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
