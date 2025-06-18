#pragma once

#include "gameObject.h"
#include <list>

class Manager
{
private:
	static std::list<GameObject*> mGameObject;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	template <class T>
	static T* AddGameObject()
	{
		T* obj = new T();
		mGameObject.push_back(obj);
		obj->Init();
		return obj;
	}

	template<typename T>
	static T* GetGameObject()
	{

		for (auto& obj : mGameObject)
		{
			T* t = dynamic_cast<T*>(obj);
			if (t != nullptr)
			{
				return t;
			}
		}
		return nullptr;
	}
	template<typename T>
	static std::list<T*> GetGameObjects()
	{
		std::list<T*> result;
		for (auto& obj : mGameObject)
		{
			T* t = dynamic_cast<T*>(obj);
			if (t != nullptr)
			{
				result.push_back(t);
			}

		}
		return result;
	}
};
