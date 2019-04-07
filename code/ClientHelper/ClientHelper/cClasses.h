#pragma once
#include "cSDK.h"
#include "cClassTools.h"
#include "eUpdate.h"

class IEntity
{
public:
	EntityId m_EntityId()
	{
		return *(EntityId*)((DWORD)this + 0x14);
	}

	const char* GetName()
	{
		return To_CP1251(To_Unicode(*(const char**)((DWORD)this + 0xB8)));
	}
};

class IActor
{
public:
	IEntity *m_pEntity()
	{
		return *(IEntity**)((DWORD)this + 0x8);
	}

	EntityId m_entityId()
	{
		return *(EntityId*)((DWORD)this + 0x10);
	}

	int GetHealth()
	{
		typedef int(__thiscall* GetHealth)(PVOID);
		return vFun_Call<GetHealth>(this, 108)(this);
	}

	bool IsDead()
	{
		return this->GetHealth() <= 0;
	}
};

class IActorSystem
{
public:
	IActor* GetActor(EntityId entityId)
	{
		typedef IActor*(__thiscall* GetActor)(PVOID, EntityId);
		return vFun_Call<GetActor>(this, 60)(this, entityId);
	}
};

class IGameFramework
{
public:
	bool GetClientActor(IActor** pActor)
	{
		typedef bool(__thiscall* SetCountdown)(PVOID, IActor**);
		return vFun_Call<SetCountdown>(this, GetClientActorP)(this, pActor);
	}

	IActorSystem *GetIActorSystem()
	{
		typedef IActorSystem*(__thiscall* GetIActorSystem)(PVOID);
		return vFun_Call<GetIActorSystem>(this, GetIActorSystemP)(this);
	}
};

class IGame
{
public:
	IGameFramework* GetIGameFramework()
	{
		return *(IGameFramework**)((DWORD)this + 0x28);
	}
};

class SSystemGlobalEnvironment
{
public:
	static SSystemGlobalEnvironment* GetInstance()
	{
		return (SSystemGlobalEnvironment*)*(DWORD*)(dwGlobalAdr);
	}

	IGame* pIGame()
	{
		return (IGame*)*(DWORD*)((DWORD)this + pIGameP);
	}
};