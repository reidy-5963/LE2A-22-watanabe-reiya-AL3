#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MyMath.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:


	~Player();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection &viewProjection);


	void Rotate();

	void Attack();

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent;
	}
	WorldTransform& GetWorldMatrix() { 
		/*if (worldTransform_.parent_) {
			worldTransform_.matWorld_ =
			    MyMath::MMMultiply(worldTransform_.matWorld_, worldTransform_.parent_->matWorld_);

		}*/
		return worldTransform_;
	}

	void SetTexReticle(uint32_t texReticle) { 
		texReticle_ = texReticle;
	}

	void DrawUI();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	//
	WorldTransform worldTransform3DReticle_;
	uint32_t texReticle_ = 0;
	Sprite* sprite2DReticle_ = nullptr;
};