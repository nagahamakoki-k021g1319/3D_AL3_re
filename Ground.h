#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "affin.h"
#include <assert.h>

class Ground {

  public:
	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
};