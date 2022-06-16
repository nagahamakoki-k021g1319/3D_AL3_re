#pragma once
#include "Input.h"
#include <cassert>
#include "DebugText.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include "ViewProjection.h"

class Player {
  public:
	  ///<summary>
	  ///初期化
	  ///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///初期化
	///</summary>
	void Update();

	///< summary>
	///初期化
	///</summary>
	void Draw(ViewProjection viewProjection_);

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
