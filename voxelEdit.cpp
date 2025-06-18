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
    // 色リスト
    const XMFLOAT4 kVoxelColors[] = {
        {1, 1, 1, 1},    // 白
        {1, 0, 0, 1},    // 赤
        {0, 1, 0, 1},    // 緑
        {0, 0, 1, 1},    // 青
        {1, 1, 0, 1},    // 黄
        {1, 0, 1, 1},    // マゼンタ
        {0, 1, 1, 1},    // シアン
    };
    int gColorIndex = 0;
    const int kColorCount = sizeof(kVoxelColors) / sizeof(kVoxelColors[0]);
}

Mouse_State gMouseState;
XMMATRIX gViewMatrix;
XMMATRIX gProjectionMatrix;
int color = 0; // 色のインデックス（0: 赤, 1: 緑, 2: 青, 3: 白）

// 画面座標をワールド座標に変換する関数
Vector3 ScreenToWorld(int mouseX, int mouseY, const XMMATRIX& view, const XMMATRIX& proj)
{
    // スクリーン座標をNDC（正規化デバイス座標）に変換
    float ndcX = (2.0f * mouseX / SCREEN_WIDTH) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / SCREEN_HEIGHT);
    float ndcZ = 0.0f; // ニアクリップ面上

    XMVECTOR ndcPos = XMVectorSet(ndcX, ndcY, ndcZ, 1.0f);

    // ビュー・プロジェクション逆行列
    XMMATRIX invViewProj = XMMatrixInverse(nullptr, view * proj);

    // NDC座標をワールド座標に変換
    XMVECTOR worldPos = XMVector3TransformCoord(ndcPos, invViewProj);

    XMFLOAT3 result;
    XMStoreFloat3(&result, worldPos);
    return Vector3{ result.x, result.y, result.z };
}

void ScreenToWorldRay(int mouseX, int mouseY, float screenWidth, float screenHeight, const XMMATRIX& view, const XMMATRIX& proj, Vector3& rayOrigin, Vector3& rayDir)
{
    // スクリーン座標をNDC（正規化デバイス座標）に変換
    float ndcX = (2.0f * mouseX / screenWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / screenHeight);

    // ニアクリップ面とファークリップ面の座標を計算
    XMVECTOR nearPoint = XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
    XMVECTOR farPoint = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);

    // ビュー・プロジェクション逆行列
    XMMATRIX invViewProj = XMMatrixInverse(nullptr, view * proj);

    // NDC座標をワールド座標に変換
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

        // スクロールで色インデックス変更
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
            case 0: // 赤
                mColor={ 1.0f, 0.0f, 0.0f, 1.0f };
			break;
            case 1: // 緑
				mColor = { 0.0f, 1.0f, 0.0f, 1.0f };
				break;
			case 2: // 青
				mColor = { 0.0f, 0.0f, 1.0f, 1.0f };
				break;
                case 3: // 白
                mColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				break;
            default:
            break;
        }
        // すべてのボクセルとレイの交差を調べ、最も近い交点を探す
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
                // 配置位置を整数グリッドにスナップ
                Vector3 intPos(
                    std::round(hitPos.x),
                    std::round(hitPos.y),
                    std::round(hitPos.z)
                );

                Voxel* voxel = Manager::AddGameObject<Voxel>();
                voxel->SetPosition(intPos);
                voxel->SetScale(Vector3(1.0f, 1.0f, 1.0f));
                voxel->SetColor(kVoxelColors[gColorIndex]); // Init()の後に呼ぶ
            }
			//voxelsがnullptrなら

            if (voxels.empty())
            {
                Voxel* voxel = Manager::AddGameObject<Voxel>();
                voxel->SetPosition(Vector3{0.0f,0.0f,0.0f});
                voxel->SetScale(Vector3(1.0f, 1.0f, 1.0f));
                voxel->SetColor(kVoxelColors[gColorIndex]); // Init()の後に呼ぶ
            }
            
        }
        if (Input::GetKeyTrigger(VK_RBUTTON))
        {
           // 右クリックでボクセルを削除
            for (auto& v : voxels)
            {
                float t;
                AABB box = v->GetAABB();
                if (box.IntersectRay(rayOrigin, rayDir, t))
                {
					
                        // カメラに一番近いボクセルを削除
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
    // ここでは特に描画処理は行わない
}

void VoxelEdit::Uninit()
{
    mVoxel = nullptr;
    mTexture = nullptr;
}

