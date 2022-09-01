#pragma once
#include "math/Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include <affin.h>

class EnemyBullet {
  public:
	///< summary>
	///������
	///</summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///< summary>
	///������
	///</summary>
	void Update();

	///< summary>
	///������
	///</summary>
	void Draw(const ViewProjection& viewProjection);

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();


   public:
	bool InDead() const { return isDead_; }

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�喃
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
	
};
