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


class Player {
  public:
	  ///<summary>
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
	void Attack();

	//�L�����̌����ɉ����������ɋ�������
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);


  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�e
	//���� 
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�P��
	/*std::unique_ptr<PlayerBullet> bullet_ = nullptr;*/
};
