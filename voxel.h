#pragma once
#include "gameObject.h"
#include "AABB.h"

class Voxel : public GameObject
{
private:
    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11InputLayout* mVertexLayout = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
    ID3D11Buffer* mIndexBuffer = nullptr;
    ID3D11ShaderResourceView* mTexture = nullptr;
    XMFLOAT4 mColor = { 1, 1, 1, 1 };
    AABB mAABB;
    VERTEX_3D vertices[8];
public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;
    void SetColor(const XMFLOAT4& color);
    XMFLOAT4 GetColor() const { return mColor; }
    AABB GetAABB() const { return mAABB; }
};