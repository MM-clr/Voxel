#pragma once
#include"gameObject.h"
class Polygon2D :public GameObject
{
private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11InputLayout* mVertexLayout;
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;

	ID3D11ShaderResourceView* mTexture;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};

