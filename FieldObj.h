#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"
#include "affin.h"

class FieldObj {
public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(Model* model);

	///< summary>
	///更新
	///</summary>
	void Update();

	///< summary>
	///描画
	///</summary>
	void Draw(ViewProjection viewProjection_);

	void SetTransform(Vector3 v);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
};

