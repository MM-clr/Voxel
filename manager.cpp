#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "voxel.h" // ’Ç‰Á
#include "field.h"
#include "voxelEdit.h"
std::list<GameObject*>Manager:: mGameObject;
void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	AddGameObject<camera>();

	
	AddGameObject<Voxel>();
	AddGameObject<VoxelEdit>();
}


void Manager::Uninit()
{
	for (auto gameObject:mGameObject)
	{
		gameObject->Uninit();
		delete gameObject;
	}
	Input::Uninit();
	
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	for (auto gameObject : mGameObject)
	{

		gameObject->Update();
	}
	mGameObject.remove_if([](GameObject* gameObject)
		{
			return gameObject->Destroy();
		});
}

void Manager::Draw()
{
	Renderer::Begin();
	for (auto gameObject : mGameObject)
	{
		gameObject->Draw();
	}

	Renderer::End();
}
