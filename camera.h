#pragma once
#include "gameObject.h"
class camera :public GameObject
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	XMMATRIX GetProjection() const { return mProjection; }
	XMMATRIX GetView() const { return mView; }

private:
	XMMATRIX mProjection;
	XMMATRIX mView;
	Vector3 mTarget{ 0.0f,0.0f,0.0f };
	float mZ = 5.0f; // ÉJÉÅÉâÇÃZç¿ïW
};

