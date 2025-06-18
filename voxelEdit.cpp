#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "voxel.h"
#include "voxelEdit.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "AABB.h"
#include <cmath>

using namespace DirectX;

namespace {
    // �F���X�g
    const XMFLOAT4 kVoxelColors[] = {
        {1, 1, 1, 1},    // ��
        {1, 0, 0, 1},    // ��
        {0, 1, 0, 1},    // ��
        {0, 0, 1, 1},    // ��
        {1, 1, 0, 1},    // ��
        {1, 0, 1, 1},    // �}�[���^
        {0, 1, 1, 1},    // �V�A��
    };
    int gColorIndex = 0;
    const int kColorCount = sizeof(kVoxelColors) / sizeof(kVoxelColors[0]);
}

Mouse_State gMouseState;
XMMATRIX gViewMatrix;
XMMATRIX gProjectionMatrix;
int color = 0; // �F�̃C���f�b�N�X�i0: ��, 1: ��, 2: ��, 3: ���j

// ��ʍ��W�����[���h���W�ɕϊ�����֐�
Vector3 ScreenToWorld(int mouseX, int mouseY, const XMMATRIX& view, const XMMATRIX& proj)
{
    // �X�N���[�����W��NDC�i���K���f�o�C�X���W�j�ɕϊ�
    float ndcX = (2.0f * mouseX / SCREEN_WIDTH) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / SCREEN_HEIGHT);
    float ndcZ = 0.0f; // �j�A�N���b�v�ʏ�

    XMVECTOR ndcPos = XMVectorSet(ndcX, ndcY, ndcZ, 1.0f);

    // �r���[�E�v���W�F�N�V�����t�s��
    XMMATRIX invViewProj = XMMatrixInverse(nullptr, view * proj);

    // NDC���W�����[���h���W�ɕϊ�
    XMVECTOR worldPos = XMVector3TransformCoord(ndcPos, invViewProj);

    XMFLOAT3 result;
    XMStoreFloat3(&result, worldPos);
    return Vector3{ result.x, result.y, result.z };
}

void ScreenToWorldRay(int mouseX, int mouseY, float screenWidth, float screenHeight, const XMMATRIX& view, const XMMATRIX& proj, Vector3& rayOrigin, Vector3& rayDir)
{
    // �X�N���[�����W��NDC�i���K���f�o�C�X���W�j�ɕϊ�
    float ndcX = (2.0f * mouseX / screenWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / screenHeight);

    // �j�A�N���b�v�ʂƃt�@�[�N���b�v�ʂ̍��W���v�Z
    XMVECTOR nearPoint = XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
    XMVECTOR farPoint = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);

    // �r���[�E�v���W�F�N�V�����t�s��
    XMMATRIX invViewProj = XMMatrixInverse(nullptr, view * proj);

    // NDC���W�����[���h���W�ɕϊ�
    XMVECTOR worldNearPoint = XMVector3TransformCoord(nearPoint, invViewProj);
    XMVECTOR worldFarPoint = XMVector3TransformCoord(farPoint, invViewProj);

    XMFLOAT3 nearPos, farPos;
    XMStoreFloat3(&nearPos, worldNearPoint);
    XMStoreFloat3(&farPos, worldFarPoint);

    rayOrigin = Vector3{ nearPos.x, nearPos.y, nearPos.z };
    rayDir = Vector3{ farPos.x - nearPos.x, farPos.y - nearPos.y, farPos.z - nearPos.z };
    rayDir.normalize();
}

void VoxelEdit::Init()
{
    mVoxel = Manager::AddGameObject<Voxel>();
}

void VoxelEdit::Update()
{
    
        Mouse_State mouseState;
        Mouse_GetState(&mouseState);

        float screenWidth = static_cast<float>(SCREEN_WIDTH);
        float screenHeight = static_cast<float>(SCREEN_HEIGHT);

		gViewMatrix = Manager::GetGameObject<camera>()->GetView();
		gProjectionMatrix = Manager::GetGameObject<camera>()->GetProjection();

        Vector3 rayOrigin, rayDir;
        ScreenToWorldRay(mouseState.x, mouseState.y, screenWidth, screenHeight, gViewMatrix, gProjectionMatrix, rayOrigin, rayDir);

        // �X�N���[���ŐF�C���f�b�N�X�ύX
        static int prevScroll = 0;
        if (mouseState.scrollWheelValue != prevScroll) {
            int delta = mouseState.scrollWheelValue - prevScroll;
            if (delta > 0) {
                gColorIndex = (gColorIndex + 1) % kColorCount;
            } else if (delta < 0) {
                gColorIndex = (gColorIndex - 1 + kColorCount) % kColorCount;
            }
            prevScroll = mouseState.scrollWheelValue;
        }

        switch (color)
        {
            case 0: // ��
                mColor={ 1.0f, 0.0f, 0.0f, 1.0f };
			break;
            case 1: // ��
				mColor = { 0.0f, 1.0f, 0.0f, 1.0f };
				break;
			case 2: // ��
				mColor = { 0.0f, 0.0f, 1.0f, 1.0f };
				break;
                case 3: // ��
                mColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				break;
            default:
            break;
        }
        // ���ׂẴ{�N�Z���ƃ��C�̌����𒲂ׁA�ł��߂���_��T��
        float minT = FLT_MAX;
        Vector3 hitPos;
        bool hit = false;
        auto voxels = Manager::GetGameObjects<Voxel>();
        for (auto& v : voxels)
        {
            float t;
            AABB box = v->GetAABB();
            if (box.IntersectRay(rayOrigin, rayDir, t))
            {
                if (t < minT)
                {
                    minT = t;
                    hitPos = rayOrigin + rayDir * t;
                    hit = true;
                }
            }
        }
        if (Input::GetKeyTrigger(VK_LBUTTON))
        {
            if (hit)
            {
                // �z�u�ʒu�𐮐��O���b�h�ɃX�i�b�v
                Vector3 intPos(
                    std::round(hitPos.x),
                    std::round(hitPos.y),
                    std::round(hitPos.z)
                );

                Voxel* voxel = Manager::AddGameObject<Voxel>();
                voxel->SetPosition(intPos);
                voxel->SetScale(Vector3(1.0f, 1.0f, 1.0f));
                voxel->SetColor(kVoxelColors[gColorIndex]); // Init()�̌�ɌĂ�
            }
			//voxels��nullptr�Ȃ�

            if (voxels.empty())
            {
                Voxel* voxel = Manager::AddGameObject<Voxel>();
                voxel->SetPosition(Vector3{0.0f,0.0f,0.0f});
                voxel->SetScale(Vector3(1.0f, 1.0f, 1.0f));
                voxel->SetColor(kVoxelColors[gColorIndex]); // Init()�̌�ɌĂ�
            }
            
        }
        if (Input::GetKeyTrigger(VK_RBUTTON))
        {
           // �E�N���b�N�Ń{�N�Z�����폜
            for (auto& v : voxels)
            {
                float t;
                AABB box = v->GetAABB();
                if (box.IntersectRay(rayOrigin, rayDir, t))
                {
					
                        // �J�����Ɉ�ԋ߂��{�N�Z�����폜
                    if (t == minT)
                        {
                        v->SetDestroy();
					}


                       


                }
            }
        }
        
    
}

void VoxelEdit::Draw()
{
    // �����ł͓��ɕ`�揈���͍s��Ȃ�
}

void VoxelEdit::Uninit()
{
    mVoxel = nullptr;
    mTexture = nullptr;
}

