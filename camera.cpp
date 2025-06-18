#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "manager.h"
#include"input.h"

void camera::Init()
{
	mPosition = Vector3(0.0f, 1.0f, -5.0f);
}

void camera::Uninit()
{
}

void camera::Update()
{
	if (Input::GetKeyPress('A'))
	{
		mRotation.y += 0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		mRotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('W'))
	{
		mZ -= 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		mZ += 0.1f;
	}
	if (Input::GetKeyPress(VK_SPACE))
	{
		mPosition.y += 0.1f;
	}
	if (Input::GetKeyPress(VK_LSHIFT))
	{
		mPosition.y -= 0.1f;
	}

	
	mTarget.y = mPosition.y;

	
	mPosition.x = mTarget.x + -sinf(mRotation.y)* mZ;

	mPosition.z = mTarget.z + -cosf(mRotation.y) * mZ;
	
	
	
	
		
}

void camera::Draw()
{
	mProjection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(mProjection);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mView = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*) & mPosition), XMLoadFloat3((XMFLOAT3*)&mTarget), XMLoadFloat3(&up));
	Renderer::SetViewMatrix(mView);
}
