#include "Model.h"
#include "WorldTransform.h"
#include "input/input.h"
#include "2d/DebugText.h"
#include <cassert>


#pragma once
class Player {
  public:
	  /// <summary>
	  /// ������
	  /// <summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// ������
	/// <summary>
	void Update();

	/// <summary>
	/// ������
	/// <summary>
	void Draw();

	private:
	   //���[���h�ϊ��f�[�^
	  WorldTransform worldTransform_;
	  //���f��
	  Model* model_ = nullptr;
	  //�e�N�X�`���n���h��
	  uint32_t textureHandle_ = 0u;
	  
	  Input* input_ = nullptr;
	  DebugText* debugText_ = nullptr;

	  
};