#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Input.h"
#include "DebugText.h"

///< summary>
///���[���J����
///</summary>
class RailCamera {
  public:
	///< summary>
	///������
	///</summary>
	void Initialize(const Vector3 position, const Vector3 rota);

	///< summary>
	///�X�V
	///</summary>
	void Update();

	ViewProjection& GetViewProjection();

	//���[���h���W���擾
	WorldTransform* GetWorldPosition();

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};
