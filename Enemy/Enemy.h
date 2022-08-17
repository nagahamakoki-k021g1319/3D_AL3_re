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

	///< summary>
	///初期化
	///</summary>
	void Fire();

	//発射隔離
	static const int kFireInterval = 60;

	//接近フェーズ初期化
	void Approach();
	
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//弾
	//複数
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t shotTimer = 0;

};
