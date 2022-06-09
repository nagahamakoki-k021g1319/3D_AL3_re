#include "GameScene.h"
#include "affin.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

const float PI = 3.141592f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, MathUtility::PI2);

	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X,Y,Z 方向のスケーリングを設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		// X,Y,Z 軸回りの回転角を設定
		worldTransform.rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		// X,Y,Z軸の周りの平行移動を設定
		worldTransform.translation_ = {dist(engine), dist(engine), dist(engine)};

#pragma region スケーリング

	Matrix4 mat = MathUtility::Matrix4Identity();

	worldTransform.matWorld_ *= AffinTrans::Scale(
	  vector3(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z));

	
#pragma endregion

//#pragma region Rotation_X
//	////X,Y,Z軸周りの回転角を設定
//	// worldTransform_.rotation_ = { PI / 4,0.0f,0.0f };
//	////Z軸回転行列を宣言
//	// Matrix4 matRotX;
//	// matRotX = {
//	//	1.0f,0.0f,0.0f,0.0f,
//	//	0.0f,cos(worldTransform_.rotation_.x),  sin(worldTransform_.rotation_.x),0.0f,
//	//	0.0f,-sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x),0.0f,
//	//	0.0f,0.0f,0.0f,1.0f
//	// };
//
//	// Matrix4 matRotX2;
//	// matRotX2 = {
//	//	1.0f,0.0f,0.0f,0.0f,
//	//	0.0f,1.0f,0.0f,0.0f,
//	//	0.0f,0.0f,1.0f,0.0f,
//	//	0.0f,0.0f,0.0f,1.0f
//	// };
//
//	//worldTransform_.matWorld_ = AffinTrans::Initialize();
//	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(PI / 4, 0.0f, 0.0f), 1);
//
//	//行列の転送
//	worldTransform_.TransferMatrix();
//#pragma endregion
//
//#pragma region Rotation_Y
//	//	//X,Y,Z軸周りの回転角を設定
//	//	worldTransform_.rotation_ = { 0.0f,PI / 4,0.0f };
//	//	//Y軸回転行列を宣言
//	//	Matrix4 matRotY;
//	//	matRotY = {
//	//		cos(worldTransform_.rotation_.y),0.0f,-sin(worldTransform_.rotation_.y),0.0f,
//	//		0.0f,1.0f,0.0f,0.0f,
//	//		sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y),0.0f,
//	//		0.0f,0.0f,0.0f,1.0f
//	//	};
//	//
//	//	Matrix4 matRotY2;
//	//	matRotY2 = {
//	//		1.0f,0.0f,0.0f,0.0f,
//	//		0.0f,1.0f,0.0f,0.0f,
//	//		0.0f,0.0f,1.0f,0.0f,
//	//		0.0f,0.0f,0.0f,1.0f
//	//	};
//	//
//	//worldTransform_.matWorld_ = AffinTrans::Initialize();
//	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, PI / 4, 0.0f), 2);
//
//	//行列の転送
//	worldTransform_.TransferMatrix();
//#pragma endregion
//
//#pragma region Rotation_Z
//	//	//X,Y,Z軸周りの回転角を設定
//	//	worldTransform_.rotation_ = { 0.0f,0.0f,PI / 4 };
//	//	//Z軸回転行列を宣言
//	//	Matrix4 matRotZ;
//	//	matRotZ = {
//	//		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
//	//		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z), 0.0f,0.0f,
//	//		0.0f,0.0f, 1.0f,0.0f,
//	//		0.0f,0.0f,0.0f,1.0f
//	//	};
//	//
//	//	Matrix4 matRotZ2;
//	//	matRotZ2 = {
//	//		1.0f,0.0f,0.0f,0.0f,
//	//		0.0f,1.0f,0.0f,0.0f,
//	//		0.0f,0.0f,1.0f,0.0f,
//	//		0.0f,0.0f,0.0f,1.0f
//	//	};
//	//
//	//worldTransform_.matWorld_ = AffinTrans::Initialize();
//	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, 0.0f, PI / 4), 3);
//
//	//行列の転送
//	worldTransform_.TransferMatrix();
//#pragma endregion

#pragma region Rotation_X_Y_Z

	worldTransform.matWorld_ *= AffinTrans::Rotation(
	  vector3(worldTransform.rotation_.x, worldTransform.rotation_.y, worldTransform.rotation_.z),
	  6);

	
#pragma endregion

#pragma region move
	worldTransform.matWorld_ *= AffinTrans::Move(vector3(
	  worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z));

	//行列の転送
	worldTransform.TransferMatrix();
	}

	//カメラ注視点座標を設定
	viewProjection_.target = {10, 0, 0};
	//カメラ上方向ベクトルを設定
	viewProjection_.up = {cosf(PI / 4.0f), sinf(PI / 4.0f), 0.0f};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

#pragma endregion
}

void GameScene::Update() { 
	debugCamera_->Update(); 
	//注視点の移動ベクトル
	Vector3 move = {0, 0, 0};

	//注視点の移動速さ
	const float kTargetSpeed = 0.2f;
	
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kTargetSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kTargetSpeed;
	}
	//注視点の移動速さ
	const float kUpRotSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2PIを超えたら0に戻す
		viewAngle = fmodf(viewAngle, PI * 2.0f);
	}
	//注視点移動
	viewProjection_.target += move;
	//行列の掛け算
	viewProjection_.UpdateMatrix();
	//上方向ベクトルを計算
	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
	// 行列の再計算
	viewProjection_.UpdateMatrix();
	//デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", 
	   viewProjection_.target.x, 
	   viewProjection_.target.y,
	   viewProjection_.target.z );
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)", 
		viewProjection_.up.x, 
		viewProjection_.up.y,
	  viewProjection_.up.z);

}

void GameScene::Draw() {

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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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

Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }