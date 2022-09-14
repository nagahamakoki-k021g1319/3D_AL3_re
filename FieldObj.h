#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"
#include "affin.h"

class FieldObj {
public:
	///< summary>
	///������
	///</summary>
	void Initialize(Model* model);

	///< summary>
	///�X�V
	///</summary>
	void Update();

	///< summary>
	///�`��
	///</summary>
	void Draw(ViewProjection viewProjection_);

	void SetTransform(Vector3 v);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
};

