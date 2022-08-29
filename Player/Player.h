#pragma once
#include "Input.h"
#include <cassert>
#include "DebugText.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <Vector3.h>
#include <WinApp.h>

class Player {
  public:
	///<summary>
	///������
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///������
	///</summary>
	void Update(ViewProjection viewProjection_);

	///< summary>
	///������
	///</summary>
	void Draw(ViewProjection viewProjection_);

	///< summary>
	///������
	///</summary>
	void Attack();

	///< summary>
	/// UI�`��
	///</summary>
	void DrawUI();


	//�L�����̌����ɉ����������ɋ�������
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//���[���h���W���擾
	Vector3 GetWorldPosition2();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	void setparent(WorldTransform* worldTransform);



	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;


	//�e
	//���� 
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

};
