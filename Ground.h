#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "affin.h"
#include <assert.h>

class Ground {

  public:
	/// <summary>
	/// ������
	/// <summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// <summary>
	void Update();

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
};