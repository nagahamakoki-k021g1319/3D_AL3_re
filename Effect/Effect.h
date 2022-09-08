#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include <random>
#include "affin.h"

///<summary>
///�G�t�F�N�g
///</summary>
class Effect {
public:
	///<summary>
	///������
	///</summary>

	void Initialize(Model* model, const Vector3& position);;



	///<summary>
	///�X�V
	///</summary>

	void Update();

	///<summary>
	///�`��
	///</summary>

	void Draw(const ViewProjection& viewProjection_);

	bool IsDead() const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	float becomeSmaller = 0.1;


	//�f�X�t���O
	bool isDead_ = false;
};
