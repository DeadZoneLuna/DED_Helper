#pragma once
#include "cClasses.h"

const char* myNickName = "";
EntityId myEntId = 0;

void Update1Info()
{
	SSystemGlobalEnvironment *pSSystemGlobalEnvironment = SSystemGlobalEnvironment::GetInstance();
	if (!pSSystemGlobalEnvironment)
		return;

	if (pSSystemGlobalEnvironment == nullptr)
		return;

	IGame *pIGame = pSSystemGlobalEnvironment->pIGame();
	if (!pIGame)
		return;

	if (pIGame == nullptr)
		return;

	IGameFramework *pIGameFramework = pIGame->GetIGameFramework();
	if (!pIGameFramework)
		return;

	if (pIGameFramework == nullptr)
		return;

	IActor* MyActor = nullptr;
	if (pIGameFramework->GetClientActor(&MyActor))
	{
		if (!MyActor)
			return;

		if (MyActor == nullptr)
			return;

		if (!MyActor->m_pEntity())
			return;

		if (MyActor->m_pEntity() == nullptr)
			return;

		if (MyActor->IsDead())
			return;

		if (myEntId != MyActor->m_entityId())
			myEntId = MyActor->m_entityId();

		const char* newMyName = MyActor->m_pEntity()->GetName();
		if (myNickName != newMyName && newMyName != "")
			myNickName = newMyName;
	}
}

typedef char(__stdcall * nSessionJoin)(int, int);
nSessionJoin mSessionJoin;

char __stdcall SessionJoinHook(int a1, int a2)
{
	char result = mSessionJoin(a1, a2);

	__asm pushad;

	const char* helper_server = "192.168.0.105";
	int helper_port = 64083;

	dedicHelper = sTools->ConnectToDedicHelper(helper_server, helper_port);

	__asm popad;

	return result;
}

typedef int(__thiscall* nHitInfoParser)(void*, int, const HitInfo &);
nHitInfoParser mHitInfoParser;

char dataToSend1[1024];
int __fastcall HitInfoParserHook(void* this1, void *Unknown, int a1, const HitInfo &hitInfo)
{
	int result = mHitInfoParser(this1, a1, hitInfo);

	__asm pushad;

	if (hitInfo.remote == 0)
	{
		if (IsSocketAvai)
		{
			if (myEntId == hitInfo.shooterId)
			{
				if (SSystemGlobalEnvironment *pSSystemGlobalEnvironment = SSystemGlobalEnvironment::GetInstance())
				{
					if (IGame *pIGame = pSSystemGlobalEnvironment->pIGame())
					{
						if (IGameFramework *pIGameFramework = pIGame->GetIGameFramework())
						{
							IActor* targetActor = nullptr;
							if (targetActor = pIGameFramework->GetIActorSystem()->GetActor(hitInfo.targetId))
							{
								ZeroMemory(dataToSend1, 1024);
								sprintf(dataToSend1, "{<send_msg msg_type='regdmg' mnick='%s' tnick='%s' meId='%d' teId='%d' pId='%d' mId='%d' wpId='%d' dTp='%f' bType='%d' tId='%d'><pdi p_x='%f' p_y='%f' p_z='%f' d_x='%f' d_y='%f' d_z='%f' n_x='%f' n_y='%f' n_z='%f'/></send_msg>}", sTools->base64_encode(myNickName).c_str(), sTools->base64_encode(targetActor->m_pEntity()->GetName()).c_str(), hitInfo.shooterId, hitInfo.targetId, hitInfo.partId, hitInfo.materialId, hitInfo.weaponId, hitInfo.distanceToHit, hitInfo.bulletType, hitInfo.typeId, hitInfo.pos.x, hitInfo.pos.y, hitInfo.pos.z, hitInfo.dir.x, hitInfo.dir.y, hitInfo.dir.z, hitInfo.normal.x, hitInfo.normal.y, hitInfo.normal.z);
								send(dedicHelper, dataToSend1, strlen(dataToSend1), 0);
							}
						}
					}
				}
			}
		}
	}

	__asm popad;

	return result;
}