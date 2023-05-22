#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_; 
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	//3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	//
	debugCamera_ = new DebugCamera(1280, 720);


	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラの生成
	player_ = new Player();

	enemy_ = new Enemy();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

		// 自キャラの初期化
	enemy_->Initialize(model_,  {0, 2, 10});
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_W)) {
		isDebugCameraActive_ = true;
	}
#endif
	ImGui::Begin("Debug");
	ImGui::Text("isDebugCameraActive : %d", isDebugCameraActive_);
	ImGui::End();

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.UpdateMatrix();
	}

	// 自キャラの更新
	player_->Update();

	enemy_->Update();

	CheckAllCollisions();
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
	// 自キャラの描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 
	Vector3 posA, posB;


	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) +
					  (posB.z - posA.z) * (posB.z - posA.z);

		float playerRad = 2.5f;
		float enemyRad = 2.5f;
		if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラ
	

	for (PlayerBullet* plaBullet : playerBullets) {
		posB = plaBullet->GetWorldPosition();
		for (EnemyBullet* eneBullet : enemyBullets) {
			posA = eneBullet->GetWorldPosition();

			float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float playerRad = 2.5f;
			float enemyRad = 2.5f;
			if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
				plaBullet->OnCollision();
				eneBullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾
	posB = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posA = bullet->GetWorldPosition();

		float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float playerRad = 2.5f;
		float enemyRad = 2.5f;
		if (Judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion
}