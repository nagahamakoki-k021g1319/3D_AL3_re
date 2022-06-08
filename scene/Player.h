#include "Model.h"
#include "WorldTransform.h"
#include "input/input.h"
#include "2d/DebugText.h"
#include <cassert>


#pragma once
class Player {
  public:
	  /// <summary>
	  /// 初期化
	  /// <summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 初期化
	/// <summary>
	void Update();

	/// <summary>
	/// 初期化
	/// <summary>
	void Draw();

	private:
	   //ワールド変換データ
	  WorldTransform worldTransform_;
	  //モデル
	  Model* model_ = nullptr;
	  //テクスチャハンドル
	  uint32_t textureHandle_ = 0u;
	  
	  Input* input_ = nullptr;
	  DebugText* debugText_ = nullptr;

	  
};