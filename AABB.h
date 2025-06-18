#pragma once
#include "vector3.h"

// Axis-Aligned Bounding Box
class AABB
{
public:
    Vector3 min;
    Vector3 max;

    AABB() : min(0, 0, 0), max(0, 0, 0) {}
    AABB(const Vector3& min_, const Vector3& max_) : min(min_), max(max_) {}

    // AABB同士の当たり判定
    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    // 点とAABBの当たり判定
    bool Contains(const Vector3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }
	//線とAABBの当たり判定
    bool Intersects(const Vector3& lineStart, const Vector3& lineEnd) const {
        // 直線の始点と終点をAABBの最小値と最大値に基づいてチェック
        return (lineStart.x <= max.x && lineEnd.x >= min.x) &&
               (lineStart.y <= max.y && lineEnd.y >= min.y) &&
               (lineStart.z <= max.z && lineEnd.z >= min.z);
    }
    // AABBの拡張
    void Expand(const Vector3& point) {
        if (point.x < min.x) min.x = point.x;
        if (point.y < min.y) min.y = point.y;
        if (point.z < min.z) min.z = point.z;
        if (point.x > max.x) max.x = point.x;
        if (point.y > max.y) max.y = point.y;
        if (point.z > max.z) max.z = point.z;
	}



	// AABBの中心を取得
    Vector3 GetCenter() const {
        return Vector3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    }
    // AABBのサイズを取得
    Vector3 GetSize() const {
        return Vector3(max.x - min.x, max.y - min.y, max.z - min.z);
    }
    // AABBの面積を取得
    float GetVolume() const {
        Vector3 size = GetSize();
        return size.x * size.y * size.z;
	}

    // レイとAABBの交差判定（交点tを返す。交差しなければfalse）
    bool IntersectRay(const Vector3& rayOrigin, const Vector3& rayDir, float& t) const
    {
        float tmin = (min.x - rayOrigin.x) / rayDir.x;
        float tmax = (max.x - rayOrigin.x) / rayDir.x;
        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.y - rayOrigin.y) / rayDir.y;
        float tymax = (max.y - rayOrigin.y) / rayDir.y;
        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min.z - rayOrigin.z) / rayDir.z;
        float tzmax = (max.z - rayOrigin.z) / rayDir.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        t = tmin;
        return t >= 0.0f;
    }
};