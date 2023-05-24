#include "Skydome.h"

void Skydome::Initialize(Model* model, const Vector3& position) { 
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void Skydome::Update() {


worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}