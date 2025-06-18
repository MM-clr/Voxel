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

    // AABB���m�̓����蔻��
    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    // �_��AABB�̓����蔻��
    bool Contains(const Vector3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }
	//����AABB�̓����蔻��
    bool Intersects(const Vector3& lineStart, const Vector3& lineEnd) const {
        // �����̎n�_�ƏI�_��AABB�̍ŏ��l�ƍő�l�Ɋ�Â��ă`�F�b�N
        return (lineStart.x <= max.x && lineEnd.x >= min.x) &&
               (lineStart.y <= max.y && lineEnd.y >= min.y) &&
               (lineStart.z <= max.z && lineEnd.z >= min.z);
    }
    // AABB�̊g��
    void Expand(const Vector3& point) {
        if (point.x < min.x) min.x = point.x;
        if (point.y < min.y) min.y = point.y;
        if (point.z < min.z) min.z = point.z;
        if (point.x > max.x) max.x = point.x;
        if (point.y > max.y) max.y = point.y;
        if (point.z > max.z) max.z = point.z;
	}



	// AABB�̒��S���擾
    Vector3 GetCenter() const {
        return Vector3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    }
    // AABB�̃T�C�Y���擾
    Vector3 GetSize() const {
        return Vector3(max.x - min.x, max.y - min.y, max.z - min.z);
    }
    // AABB�̖ʐς��擾
    float GetVolume() const {
        Vector3 size = GetSize();
        return size.x * size.y * size.z;
	}

    // ���C��AABB�̌�������i��_t��Ԃ��B�������Ȃ����false�j
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