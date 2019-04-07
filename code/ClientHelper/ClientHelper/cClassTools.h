#pragma once
class IActor;

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
	EntityId shooterId; //0x0000 
	EntityId targetId; //0x0004 
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

wchar_t *To_Unicode(const char*utf8_string)
{
	int err;
	wchar_t * res;
	int res_len = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, NULL, 0);
	if (res_len == 0) { return NULL; }
	res = (wchar_t *)calloc(sizeof(wchar_t), res_len);
	if (res == NULL) { return NULL; }
	err = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, res, res_len);
	if (err == 0)
	{
		free(res);
		return NULL;
	}
	return res;
}

char *To_CP1251(wchar_t *unicode_string)
{
	int err;
	char * res;
	int res_len = WideCharToMultiByte(1251, 0, unicode_string, -1, NULL, 0, NULL, NULL);
	if (res_len == 0) return NULL;
	res = (char *)calloc(sizeof(char), res_len);
	if (res == NULL) return NULL;
	err = WideCharToMultiByte(1251, 0, unicode_string, -1, res, res_len, NULL, NULL);
	if (err == 0)
	{
		free(res);
		return NULL;
	}
	return res;
}