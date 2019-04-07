#pragma once
class IActor;
class IPhysicalWorld;
class IEntity;

typedef unsigned int EntityId;

template< typename cData >
cData vFun_Call(PVOID BaseClass, DWORD vIndex)
{
	PDWORD* vPointer = (PDWORD*)BaseClass;
	PDWORD vFunction = *vPointer;
	DWORD dwAddress = vFunction[(vIndex / 4)];
	return (cData)(dwAddress);
}

struct HitInfo
{
	__int32 shooterId; //0x0000 
	__int32 targetId; //0x0004 
	__int32 materialId; //0x0008 
	__int32 typeId; //0x000C 
	__int32 bulletType; //0x0010 
	__int32 partId; //0x0014 
	Vec3 pos; //0x0018 
	Vec3 dir; //0x0024 
	BYTE meleeArmorAbsorb; //0x0030 
	char _0x0031[7];
	__int32 weaponId; //0x0038 
	__int32 projectileId; //0x003C 
	__int16 projectileClassId; //0x0040 
	char _0x0042[6];
	Vec3 normal; //0x0048 
	char _0x0054[20];
	float distanceToHit; //0x0068 
	char _0x006C[8];
	BYTE remote; //0x0074 
};

struct SIPhysicalEntity
{
public:
	char _0x0000[28];
	IEntity* pIEntity; //0x001C 
};

struct SRay_hit
{
public:
	char _0x0000[64];
	float dist; //0x0040 
	SIPhysicalEntity *pCollider; //0x0044 
	char _0x0048[4];
	__int32 partId; //0x004C 
	char _0x0050[12];
	Vec3 pos; //0x005C 
	Vec3 normal; //0x0068 
};//Size=0x0074

class SRWIParams
{
public:
	SRWIParams() { memset(this, 0, sizeof(*this)); }

	__int32 GetObjTypes()
	{
		return *(__int32*)((DWORD)this + 0x0);
	}

	__int32 GetFlags()
	{
		return *(__int32*)((DWORD)this + 0x4);
	}

	Vec3 org()
	{
		return *(Vec3*)((DWORD)this + 0x28);
	}

	Vec3 dir()
	{
		return *(Vec3*)((DWORD)this + 0x1C);
	}

	SRay_hit* hits()
	{
		return *(SRay_hit**)((DWORD)this + 0x14);
	}

	SIPhysicalEntity *pSIPhysicalEntity()
	{
		return **(SIPhysicalEntity***)((DWORD)this + 0x8);
	}
};