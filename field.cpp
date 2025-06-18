#include "main.h"
#include "renderer.h"
#include "field.h"

void Field::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-10.0f, 0.0f, 10.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(10.0f, 0.0f, 10.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-10.0f, 0.0f, -10.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0, 1.0f);

	vertex[3].Position = XMFLOAT3(10.0f, 0.0f, -10.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\grass.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &mTexture);
	assert(mTexture);

	Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, "unlitTextureVS.cso");

	Renderer::CreatePixelShader(&mPixelShader, "unlitTexturePS.cso");
}

void Field::Uninit()
{
	mVertexBuffer->Release();
	mVertexLayout->Release();
	mVertexShader->Release();
	mPixelShader->Release();
	mTexture->Release();
}

void Field::Update()
{
}

void Field::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(mVertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(mVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(mPixelShader, NULL, 0);
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	trans = XMMatrixTranslation(0.0f, 0.0f, 0.0);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	MATERIAL mat;
	mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.TextureEnable = true;
	Renderer::SetMaterial(mat);
	// 定数バッファ設定

	
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mTexture);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	Renderer::GetDeviceContext()->Draw(4, 0);
	// テクスチャ解除（スロット0をnullにする）
	//ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	//Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, nullSRV);
}
