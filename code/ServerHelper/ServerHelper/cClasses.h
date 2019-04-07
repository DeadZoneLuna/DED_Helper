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

	char* GetName()
	{
		return sTools->To_CP1251(sTools->To_Unicode(*(const char**)((DWORD)this + 0xB8)));
	}
};

class SBulletParams
{
public:
	const char* NBulletType()
	{
		return *(const char**)((DWORD)this + 0x8);
	}
};

class IWeaponParams1
{
public:
	SBulletParams *pSBulletParams()
	{
		return *(SBulletParams **)((DWORD)this + 0x4);
	}
};

class IWeaponGeneral
{
public:
	IWeaponParams1* GetIWeaponParams1()
	{
		return *(IWeaponParams1**)((DWORD)this + 0x14);
	}
};

class IWeapon
{
public:
	IWeaponGeneral* GetIWeaponGeneral()
	{
		return *(IWeaponGeneral**)((DWORD)this + 0x8);
	}

	__int32 GetWeaponId()
	{
		return *(__int32*)((DWORD)this + 0x18);
	}
};

class IItem
{
public:
	IWeapon* GetIWeapon()
	{
		typedef IWeapon*(__thiscall* GetIWeapon)(PVOID);
		return vFun_Call<GetIWeapon>(this, 316)(this);
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

	void SetHealth(float value)
	{
		typedef void *(__thiscall* SetHealth)(PVOID, float);
		vFun_Call<SetHealth>(this, 112)(this, value);
	}

	EntityId GetCurrentItemId()
	{
		return *(EntityId*)((DWORD)this + 0x32C);
	}

	__int32 GetWeaponSlotId()
	{
		return *(__int32*)((DWORD)this + 0x9D8);
	}

	IItem *GetCurrentItem(bool includeVehicle = false)
	{
		typedef IItem *(__thiscall* GetCurrentItem)(PVOID, bool);
		return vFun_Call<GetCurrentItem>(this, 224)(this, includeVehicle = false);
	}
};

class IActorIterator
{
public:
	IActor* Next()
	{
		typedef IActor*(__thiscall* aNext)(PVOID);
		return vFun_Call<aNext>(this, 4)(this);
	}
};

class IActorSystem
{
public:
	void CreateActorIterator(IActorIterator** ActorIterator)
	{
		typedef void(__thiscall* CreateActorIterator)(PVOID, IActorIterator**);
		return vFun_Call<CreateActorIterator>(this, 72)(this, ActorIterator);
	}

	IActor* GetActor(EntityId entityId)
	{
		typedef IActor*(__thiscall* GetActor)(PVOID, EntityId);
		return vFun_Call<GetActor>(this, 60)(this, entityId);
	}
};

class IGameRules
{
public:
	int GetHitTypeId(const char *type)
	{
		typedef int(__thiscall* GetHitTypeId)(PVOID, const char *);
		return vFun_Call<GetHitTypeId>(this, 164)(this, type);
	}

	void ServerHit(const HitInfo &hitInfo)
	{
		typedef void(__thiscall* ServerHit)(PVOID, const HitInfo &);
		return vFun_Call<ServerHit>(this, 612)(this, hitInfo);
	}
};

class IGameFramework
{
public:
	IActorSystem *GetIActorSystem()
	{
		typedef IActorSystem*(__thiscall* GetIActorSystem)(PVOID);
		return vFun_Call<GetIActorSystem>(this, GetIActorSystemP)(this);
	}

	IGameRules *GetIGameRules()
	{
		typedef IGameRules *(__thiscall* GetCurrentGameRules)(PVOID);
		return vFun_Call<GetCurrentGameRules>(this, GetIGameRulesP)(this);
	}
};

class ICVar
{
public:
	__int32 GetIGameRoomType()
	{
		return *(__int32*)((DWORD)this + 1200);
	}
};

class IGame
{
public:
	ICVar* GetICVar()
	{
		return *(ICVar**)((DWORD)this + 0x98);
	}

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

	IPhysicalWorld* pIPhysicalWorld()
	{
		return (IPhysicalWorld*)*(DWORD*)((DWORD)this + pIPhysicalWorldP);
	}

	IGame* pIGame()
	{
		return (IGame*)*(DWORD*)((DWORD)this + pIGameP);
	}
};

using PRWI = int(__stdcall*)(SRWIParams&, const char*, int);
PRWI RWI = nullptr;