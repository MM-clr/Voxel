#pragma once
#include<math.h>
class Vector3
{
public:
	float x, y, z;
	Vector3() {}
	Vector3(const Vector3 &a):x(a.x),y(a.y),z(a.z){}
	Vector3(float nx,float ny,float nz):x(nx),y(ny),z(nz){}

	

	Vector3 &operator = (const Vector3 & a)
	{
		x = a.x; y = a.y; z = a.z;
		return *this;
	}
	bool operator==(const Vector3& a)const
	{
		return x == a.x && y == a.y && z == a.z;
	}
	bool operator!=(const Vector3& a)const
	{
		return x != a.x || y != a.y || z != a.z;
	}

	void zero() { x = y = z = 0.0f; }

	Vector3 operator-()const { return Vector3(-x, -y, -z); }

	Vector3 operator+(const Vector3& a)const {
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator-(const Vector3& a)const {
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	Vector3 operator*(float a)const {
		return Vector3(x * a, y * a, z * a);
	}
	Vector3 operator/(float a)const {
		
		a = 1.0f / a;
		return Vector3(x * a, y * a, z * a);
	}
	Vector3& operator += (const Vector3& a)
	{
		x += a.x; y += a.y; z += a.z;
		return *this;
	}
	Vector3& operator -= (const Vector3& a)
	{
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}
	Vector3& operator *= ( float a)
	{
		x *= a; y *= a; z *= a;
		return *this;
	}
	Vector3& operator /= ( float a)
	{
		a = 1.0f / a;
		x *= a; y *= a; z *= a;
		return *this;
	}
	void normalize() {
		float magsq = x * x + y * y + z * z;
		if (magsq>0.0f)
		{
			float oneOverMag = 1.0f / sqrt(magsq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	// 新規追加: 正規化したベクトルを返す（自身は変更しない）
	Vector3 normalized() const {
		float magsq = x * x + y * y + z * z;
		if (magsq > 0.0f) {
			float oneOverMag = 1.0f / sqrt(magsq);
			return Vector3(x * oneOverMag, y * oneOverMag, z * oneOverMag);
		}
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	float length(const Vector3 &a) const {
		return sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y) + (z - a.z) * (z - a.z));
	}

	/*float operator*(const Vector3 &a)const {
		return x * a.x * y * a.y + z * a.z;
	}*/
};