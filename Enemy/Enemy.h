#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include <cassert>

class Enemy {
  public:
	///< summary>
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

	//行動フェーズ
	enum class Phase {
		Approach, //接近する
		Leave,    //離脱する
	};

	
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//フェーズ
	Phase phase_ = Phase::Approach;
	
};
