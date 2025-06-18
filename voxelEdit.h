#pragma once
#include "GameObject.h"
class VoxelEdit : public GameObject
{
private:
	Voxel* mVoxel ;
    ID3D11ShaderResourceView* mTexture;
    XMFLOAT4 mColor = { 1, 1, 1,1 };
public:
    void Init() override;
    void Uninit() override;
    void Update() override;
	void Draw() override;
    void SetColor(const XMFLOAT4& color) { mColor = color; }
};