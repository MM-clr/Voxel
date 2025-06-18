#pragma once


class GameObject
{
public:
	virtual void Init()=0;
	virtual void Uninit()=0;
	virtual void Update()=0;
	virtual void Draw() = 0;

	Vector3 GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& position) { mPosition = position; }
	Vector3 GetRotation() const { return mRotation; }
	void SetRotation(const Vector3& rotation) { mRotation = rotation; }
	Vector3 GetScale() const { return mScale; }
	void SetScale(const Vector3& scale) { mScale = scale; }
	void SetPosition(float x, float y, float z) { mPosition = Vector3(x, y, z); }
	void SetRotation(float x, float y, float z) { mRotation = Vector3(x, y, z); }
	void SetScale(float x, float y, float z) { mScale = Vector3(x, y, z); }
	void SetPosition(const XMFLOAT3& position) { mPosition = Vector3(position.x, position.y, position.z); }
	void SetRotation(const XMFLOAT3& rotation) { mRotation = Vector3(rotation.x, rotation.y, rotation.z); }
	void SetScale(const XMFLOAT3& scale) { mScale = Vector3(scale.x, scale.y, scale.z); }
	XMFLOAT4 GetColor() const { return mColor; }
	void SetColor(const XMFLOAT4& color) { mColor = color; }
	void SetColor(float r, float g, float b, float a) { mColor = XMFLOAT4(r, g, b, a); }


	

	void SetDestroy() { 
		mDestroy = true;
	}
	bool Destroy() {
		if (mDestroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
	Vector3 GetRight() const
	{
		XMMATRIX mat = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, mat.r[0]);
		return right;
	}

	Vector3 GetUp() const
	{
		XMMATRIX mat = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		Vector3 up;
		XMStoreFloat3((XMFLOAT3*)&up, mat.r[1]);
		return up;
	}
	Vector3 GetForward() const
	{
		XMMATRIX mat = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, mat.r[2]);
		return forward;
	}

protected:	
	Vector3 mPosition{ 0.0f,0.0f,0.0f };
	Vector3 mRotation{ 0.0f,0.0f,0.0f };
	Vector3 mScale{ 1.0f,1.0f,1.0f };
	XMFLOAT4 mColor{ 1.0f,1.0f,1.0f,1.0f };

	bool mDestroy = false;
};

