#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>
#include "push.h"
#include "FieldObj.h"


#include "Effect.h"

#include "Ground.h"



/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: 
	// メンバ関数
	/// <summary>
	/// コンストクラタ
	GameScene();
	

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	void AddEffect(std::unique_ptr<Effect>& effect);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();
	void LoadEnemyPopData2();


	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdataEnemyPopCommands();
	void UpdataEnemyPopCommands2();


	/// <summary>
	/// 敵の発生
	/// </summary>
	void GenerEnemy(Vector3 EnemyPos,int ID);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemybullets_; }



	//エフェクトリストを取得
	const std::list<std::unique_ptr<Effect>>& GetEffect() { return effects_; }


	//シーン切り替え
	enum class SceneNo {
		Title, //タイトル
		Operate, //操作説明(チュートリアル)
		Game,  //射撃
		Clear, //ゲームクリア
		Over   //ゲメオーバー
	};

	void EnemyReset();
	void EnemyReset2();

	void EnemyTarget(Vector3 targetPos,Vector3 playerPos,float distance);


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	//音
	Audio* audio_ = nullptr;
	//タイトル
	uint32_t bgmHandle = 0;
	int soundHandle = -1;
	//戦闘
	uint32_t bgmHandle2 = 0;
	int soundHandle2 = -1;
	//クリア
	uint32_t bgmHandle3 = 0;
	int soundHandle3 = -1;
	//オーバー
	uint32_t bgmHandle4 = 0;
	int soundHandle4 = -1;
	// UI音
	uint32_t bgmDecision = 0;
	uint32_t bgmRock = 0;



	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle1_ = 0;
	// タイトル
	std::unique_ptr<Sprite> spriteTitle;
	//ゲームクリア
	std::unique_ptr<Sprite> spriteClear;
	//ゲームオーバー
	std::unique_ptr<Sprite> spriteOver; 

	//UI
	std::unique_ptr<Sprite> spriteUI; 
	std::unique_ptr<Sprite> spriteG;
	std::unique_ptr<Sprite> spriteCot;

	//敵2Dレティクル用スプライト
	uint32_t textureHandleEnemyReticle_ = 0;
	std::unique_ptr<Sprite> enemy2DReticle_;
	// 赤ロック
	std::unique_ptr<Sprite> spriterock;
	//Go
	std::unique_ptr<Sprite> spriteGO1;
	std::unique_ptr<Sprite> spriteGO2;

	std::unique_ptr<Sprite> spriteHP[10];
	// 3Dモデル
	Model* model_ = nullptr;

	
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//カメラ上方向
	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;
	int playerRadius = 14;
	int playerBulletRadius = 1;
	//自機のの撃破カウント
	int playerTimer = 1000;
	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	int enemyRadius = 1;
	int enemyBulletRadius = 3;
	//敵の撃破カウント
	int enemyDefeat = 0;
	//弾 複数
	std::list<std::unique_ptr<EnemyBullet>> enemybullets_;

	//自弾リストの取得
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//エフェクト 複数
	std::list<std::unique_ptr<Effect>> effects_;

	//スカイドーム
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//地面
	FieldObj* fieldObj_ = nullptr;
	Ground* ground_ = nullptr;
	Model* modelGround_ = nullptr;


	Model* modelPlayer1_ = nullptr;
	Model* modelPlayer2_ = nullptr;
	Model* modelField1_ = nullptr;
	Model* enemyBox_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	std::stringstream enemyPopCommands2;

	Vector3 vector3(float x, float y, float z);
	Vector4 vector4(int x, int y, int z, int w);

	//待機中フラグ
	bool isStand_ = false;
	//待機タイマー
	int standTime_ = 0;

	//待機中フラグ
	bool isStand2_ = false;
	//待機タイマー
	int standTime2_ = 0;

	SceneNo sceneNo_ = SceneNo::Title;
	
	//ロックオンカメラのフラグ
	int cameraFlag_ = 0;
	Vector3 lastEnemyPos;
	
	
	


	int targetChange = 0;

	int targetMax = 0;

	int goThrough = 0;
	
	int noEnemy = 0;

	float goTexPosX = -40.0f;	//textureGo
};