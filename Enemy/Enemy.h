#pragma once
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include <cassert>
#include "EnemyBullet.h"
#include <list>
class Player;

class Enemy {
  public:
	///< summary>
	///������
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///������
	///</summary>
	void Update();

	///< summary>
	///������
	///</summary>
	void Draw(ViewProjection viewProjection_);

	///< summary>
	///������
	///</summary>
	void Fire();

	//���ˊu��
	static const int kFireInterval = 60;

	//�ڋ߃t�F�[�Y������
	void Approach();
	
	void SetPlayer(Player* player) { player_ = player;}

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���L����
	Player* player_ = nullptr;

	//�e
	//����
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���˃^�C�}�[
	int32_t shotTimer = 0;

};
