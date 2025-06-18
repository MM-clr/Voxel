#include "main.h"
#include "renderer.h"
#include "voxel.h"
#include "AABB.h"
void Voxel::Init()
{
    // ���_�z��̐錾
    

    // ���_���Ƃɏ�����
    vertices[0].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
    vertices[0].Normal   = XMFLOAT3(0, 0, -1);
    vertices[0].Diffuse  = mColor;
    vertices[0].TexCoord = XMFLOAT2(0, 0);

    vertices[1].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
    vertices[1].Normal   = XMFLOAT3(0, 0, -1);
    vertices[1].Diffuse  = mColor;
    vertices[1].TexCoord = XMFLOAT2(1, 0);

    vertices[2].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
    vertices[2].Normal   = XMFLOAT3(0, 0, -1);
    vertices[2].Diffuse  = mColor;
    vertices[2].TexCoord = XMFLOAT2(1, 1);

    vertices[3].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
    vertices[3].Normal   = XMFLOAT3(0, 0, -1);
    vertices[3].Diffuse  = mColor;
    vertices[3].TexCoord = XMFLOAT2(0, 1);

    vertices[4].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
    vertices[4].Normal   = XMFLOAT3(0, 0, 1);
    vertices[4].Diffuse  = mColor;
    vertices[4].TexCoord = XMFLOAT2(0, 0);

    vertices[5].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
    vertices[5].Normal   = XMFLOAT3(0, 0, 1);
    vertices[5].Diffuse  = mColor;
    vertices[5].TexCoord = XMFLOAT2(1, 0);

    vertices[6].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
    vertices[6].Normal   = XMFLOAT3(0, 0, 1);
    vertices[6].Diffuse  = mColor;
    vertices[6].TexCoord = XMFLOAT2(1, 1);

    vertices[7].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
    vertices[7].Normal   = XMFLOAT3(0, 0, 1);
    vertices[7].Diffuse  = mColor;
    vertices[7].TexCoord = XMFLOAT2(0, 1);

    // ���_�o�b�t�@�쐬
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices;
    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

    // �C���f�b�N�X�o�b�t�@�͕s�v
    // �C���f�b�N�X�f�[�^�̒�`�i��: �����̂�6�ʕ��̃C���f�b�N�X�j
    UINT indices[] = {
        0, 2, 1, 3, 2, 0, // �O��
        4, 5, 6, 6, 7, 4, // �w��
        0, 4, 7, 7, 3, 0, // ����
        1, 6, 5, 2, 6, 1, // �E��
        3, 6, 2, 7, 6, 3, // ���
        0, 1, 5, 5, 4, 0  // ���
    };

    // �C���f�b�N�X�o�b�t�@�쐬
    D3D11_BUFFER_DESC ibd{};
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA isd{};
    isd.pSysMem = indices;
    Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &mIndexBuffer);
    // �e�N�X�`���E�V�F�[�_�[�͂��̂܂�
    TexMetadata metadata;
    ScratchImage image;
    LoadFromWICFile(L"asset\\texture\\QB_open2.jpg", WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &mTexture);
    assert(mTexture);

    Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, "unlitTextureVS.cso");
    Renderer::CreatePixelShader(&mPixelShader, "unlitTexturePS.cso");

    // ���[���h���W�� mPosition �Ő���
}

void Voxel::Uninit()
{
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mVertexLayout) mVertexLayout->Release();
    if (mVertexShader) mVertexShader->Release();
    if (mPixelShader) mPixelShader->Release();
	if (mIndexBuffer) mIndexBuffer->Release();
}

void Voxel::Update()
{
	//�����蔻��X�V����
    mAABB=AABB(Vector3(mPosition.x - 0.5f, mPosition.y - 0.5f, mPosition.z - 0.5f), 
		Vector3(mPosition.x + 0.5f, mPosition.y + 0.5f, mPosition.z + 0.5f));

    

}

void Voxel::Draw()
{
    // --- �ǉ�: �A���t�@�u�����h�L���� ---
    

    Renderer::GetDeviceContext()->IASetInputLayout(mVertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(mVertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(mPixelShader, NULL, 0);

    // ���[���h�s��
    XMMATRIX scale = XMMatrixScaling(1.0f,1.0f,1.0f);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
    XMMATRIX trans = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
    XMMATRIX world = scale * rot * trans;
    Renderer::SetWorldMatrix(world);

    MATERIAL mat;
    mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mat.TextureEnable = false;
    Renderer::SetMaterial(mat);
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mTexture);


   
    Renderer::GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Renderer::GetDeviceContext()->DrawIndexed(36, 0, 0);
}


void Voxel::SetColor(const XMFLOAT4& color)
{
    mColor = color;

    // ���_�o�b�t�@�����݂���ꍇ�A���_�J���[���X�V
    if (mVertexBuffer) {
        // ���݂̒��_�f�[�^���쐬
       
       
        for (int i = 0; i < 8; ++i) {
            vertices[i].Normal = XMFLOAT3(0, 0, 0);
            vertices[i].Diffuse = mColor;
            vertices[i].TexCoord = XMFLOAT2(0, 0);
        }
        // ���_�o�b�t�@�쐬
        D3D11_BUFFER_DESC bd{};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertices;
        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);
        // �o�b�t�@�ɏ�������
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (SUCCEEDED(hr) && mappedResource.pData) {
            memcpy(mappedResource.pData, vertices, sizeof(vertices));
            Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);
        }
       
       
    }
}
