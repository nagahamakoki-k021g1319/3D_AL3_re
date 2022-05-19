#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);

	//軸方向表示を有効にする

	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するとビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	vector4 = Vector4(255, 0, 13, 1);

	//平行移動
	/*for (int i = 0; i < 4; i++) {
	    boxMoved[i][0] = move[0][0] * box[i][0] + move[0][1] * box[i][1] + move[0][2] * box[i][2];
	    boxMoved[i][1] = move[1][0] * box[i][0] + move[1][1] * box[i][1] + move[1][2] * box[i][2];
	    boxMoved[i][2] = move[2][0] * box[i][0] + move[2][1] * box[i][1] + move[2][2] * box[i][2];
	}*/

	vector3Box[0].x =
	  move[0].x * vector3[0].x + move[0].y * vector3[0].y + move[0].z * vector3[0].z;

	
	//拡大処理
	vector3aBox[0].x =
	  Scale[0].x * vector3a[0].x + Scale[0].y * vector3a[0].y + Scale[0].z * vector3a[0].z;

	for (int i = 0; i < 8; i++) {
		vector3aBox[i].x = Scale[0].x * vector3a[i].x + Scale[0].y * vector3a[i].y +
		                   Scale[0].z * vector3a[i].z + 0*0;
		vector3aBox[i].y = Scale[1].x * vector3a[i].x + Scale[1].y * vector3a[i].y + 
			               Scale[1].z * vector3a[i].z + 0*0;
		vector3aBox[i].z = Scale[2].x * vector3a[i].x + Scale[2].y * vector3a[i].y + 
						   Scale[2].z * vector3a[i].z + 0*0;
		
	}

	//X軸回転処理
	vector3bXBox[0].x =
	  rotateX[0].x * vector3b[0].x + rotateX[0].y * vector3b[0].y + rotateX[0].z * vector3b[0].z;

	for (int i = 0; i < 8; i++) {
		vector3bXBox[i].x = rotateX[0].x * vector3b[i].x + rotateX[0].y * vector3b[i].y +
		                   rotateX[0].z * vector3b[i].z + 0 * 0;
		vector3bXBox[i].y = rotateX[1].x * vector3b[i].x + rotateX[1].y * vector3b[i].y +
		                   rotateX[1].z * vector3b[i].z + 0 * 0;
		vector3bXBox[i].z = rotateX[2].x * vector3b[i].x + rotateX[2].y * vector3b[i].y +
		                   rotateX[2].z * vector3b[i].z + 0 * 0;
	}

	// Y軸回転処理
	vector3bYBox[0].x =
	  rotateY[0].x * vector3b[0].x + rotateY[0].y * vector3b[0].y + rotateY[0].z * vector3b[0].z;

	for (int i = 0; i < 8; i++) {
		vector3bYBox[i].x = rotateY[0].x * vector3b[i].x + rotateY[0].y * vector3b[i].y +
		                   rotateY[0].z * vector3b[i].z + 0 * 0;
		vector3bYBox[i].y = rotateY[1].x * vector3b[i].x + rotateY[1].y * vector3b[i].y +
		                   rotateY[1].z * vector3b[i].z + 0 * 0;
		vector3bYBox[i].z = rotateY[2].x * vector3b[i].x + rotateY[2].y * vector3b[i].y +
		                   rotateY[2].z * vector3b[i].z + 0 * 0;
	}
	
	// Z軸回転処理
	vector3bZBox[0].x =
	  rotateZ[0].x * vector3b[0].x + rotateZ[0].y * vector3b[0].y + rotateZ[0].z * vector3b[0].z;

	for (int i = 0; i < 8; i++) {
		vector3bZBox[i].x = rotateZ[0].x * vector3b[i].x + rotateZ[0].y * vector3b[i].y +
		                   rotateZ[0].z * vector3b[i].z + 0 * 0;
		vector3bZBox[i].y = rotateZ[1].x * vector3b[i].x + rotateZ[1].y * vector3b[i].y +
		                   rotateZ[1].z * vector3b[i].z + 0 * 0;
		vector3bZBox[i].z = rotateZ[2].x * vector3b[i].x + rotateZ[2].y * vector3b[i].y +
		                   rotateZ[2].z * vector3b[i].z + 0 * 0;
	}


	//平行移動処理
	vector3cBox[0].x = parallelMove[0].x * vector3c[0].x + parallelMove[0].y * vector3c[0].y +
	                   parallelMove[0].z * vector3c[0].z+1 * parallelMove[0].w;

	for (int i = 0; i < 8; i++) {
		vector3cBox[i].x = parallelMove[0].x * vector3c[i].x + parallelMove[0].y * vector3c[i].y +
		                   parallelMove[0].z * vector3c[i].z + 1 * parallelMove[0].w;
		vector3cBox[i].y = parallelMove[1].x * vector3c[i].x + parallelMove[1].y * vector3c[i].y +
		                   parallelMove[1].z * vector3c[i].z + 1 * parallelMove[1].w;
		vector3cBox[i].z = parallelMove[2].x * vector3c[i].x + parallelMove[2].y * vector3c[i].y +
		                   parallelMove[2].z * vector3c[i].z + 1 * parallelMove[2].w;
		
		
	}


}

void GameScene::Update() { debugCamera_->Update(); }

void GameScene::Draw() {

	//初期ボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3[eqgeList[i][0]], vector3[eqgeList[i][1]], 
			Vector4(0xff,0xff,0xff,0xff));
	}
	//拡大ボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3aBox[eqgeList[i][0]], vector3aBox[eqgeList[i][1]],
		  Vector4(0x00, 0xff, 0xaa, 0xff));
	}
	//平行移動したボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3cBox[eqgeList[i][0]], vector3cBox[eqgeList[i][1]],
		  Vector4(0xff, 0xff, 0xff, 0xff));
	}
	//回転したボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3bXBox[eqgeList[i][0]], vector3bXBox[eqgeList[i][1]], vector4);
	}
	//回転したボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3bYBox[eqgeList[i][0]], vector3bYBox[eqgeList[i][1]], vector4);
	}
	//回転したボックス
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vector3bZBox[eqgeList[i][0]], vector3bZBox[eqgeList[i][1]], vector4);
	}

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}