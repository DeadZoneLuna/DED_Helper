#pragma once
#include "cClasses.h"

vector<HitInfo> HitsRegister;
bool HitsUsing = false;

int GetBulletTypeId(IActor* pIActor)
{
	IItem* pIItem = pIActor->GetCurrentItem();
	if (!pIItem)
		return -1;

	IWeapon* pIWeapon = pIItem->GetIWeapon();
	if (!pIWeapon)
		return -1;

	IWeaponGeneral* pIWeaponGeneral = pIWeapon->GetIWeaponGeneral();
	if (!pIWeaponGeneral)
		return -1;

	IWeaponParams1* pIWeaponParams1 = pIWeaponGeneral->GetIWeaponParams1();
	if (!pIWeaponParams1)
		return -1;

	SBulletParams* pSBulletParams = pIWeaponParams1->pSBulletParams();
	if (!pSBulletParams)
		return -1;

	const char* BulletName = pSBulletParams->NBulletType();

	if (strstr(BulletName, "bullet_ar_zsd01"))
		return 2;

	if (strstr(BulletName, "bullet_ar_turret"))
		return 2;

	if (strstr(BulletName, "bullet_ar_p"))
		return 2;

	if (strstr(BulletName, "bullet_ar11"))
		return 2;

	if (strstr(BulletName, "bullet_arl01_tutor"))
		return 2;

	if (strstr(BulletName, "bullet_arl01_zsd01"))
		return 2;

	if (strstr(BulletName, "bullet_arl01"))
		return 2;

	if (strstr(BulletName, "bullet_arl02"))
		return 2;

	if (strstr(BulletName, "bullet_arl03"))
		return 2;

	if (strstr(BulletName, "bullet_mg_p"))
		return 2;

	if (strstr(BulletName, "bullet_mg_pve01"))
		return 2;

	if (strstr(BulletName, "bullet_mg_zsd01"))
		return 2;

	if (strstr(BulletName, "bullet_mg01_s"))
		return 0;

	if (strstr(BulletName, "bullet_mg"))
		return 2;

	if (strstr(BulletName, "bullet_ar"))
		return 2;

	if (strstr(BulletName, "bullet_pt_fld01"))
		return 0;

	if (strstr(BulletName, "bullet_pt_p"))
		return 0;

	if (strstr(BulletName, "bullet_pt"))
		return 0;

	if (strstr(BulletName, "bullet_rg_zsd01"))
		return 1;

	if (strstr(BulletName, "bullet_rg"))
		return 4;

	if (strstr(BulletName, "bullet_shg_zsd01"))
		return 1;

	if (strstr(BulletName, "bullet_shg_p_01"))
		return 1;

	if (strstr(BulletName, "bullet_shg_p"))
		return 1;

	if (strstr(BulletName, "bullet_shg"))
		return 1;

	if (strstr(BulletName, "bullet_smg_zsd01"))
		return 0;

	if (strstr(BulletName, "bullet_smg_p"))
		return 0;

	if (strstr(BulletName, "bullet_smg"))
		return 0;

	if (strstr(BulletName, "bullet_sr_zsd01"))
		return 3;

	if (strstr(BulletName, "bullet_sr_p"))
		return 3;

	if (strstr(BulletName, "bullet_sr"))
		return 3;

	if (strstr(BulletName, "ammo_pack"))
		return 4;

	if (strstr(BulletName, "armorkit"))
		return 4;

	if (strstr(BulletName, "medkit"))
		return 4;

	return -1;
}

int GetMaterialId(IEntity* pIEntity, int partId)
{
	int zero_value = 0;

	if (partId > 0)
		zero_value = 1;

	return zero_value;
}

EntityId GetEntityIdByName(string name)
{
	SSystemGlobalEnvironment *pSSystemGlobalEnvironment = SSystemGlobalEnvironment::GetInstance();
	if (pSSystemGlobalEnvironment == nullptr)
		return 0;

	IGame* pIGame = pSSystemGlobalEnvironment->pIGame();
	if (pIGame == nullptr)
		return 0;

	IGameFramework* pIGameFramework = pIGame->GetIGameFramework();
	if (pIGameFramework == nullptr)
		return 0;

	IActorSystem* pIActorSystem = pIGameFramework->GetIActorSystem();
	if (pIActorSystem == nullptr)
		return 0;

	IActorIterator *pIActorIterator = nullptr;
	pIActorSystem->CreateActorIterator(&pIActorIterator);
	while (IActor* pIActor = pIActorIterator->Next())
	{
		if (pIActor->m_pEntity())
		{
			if (sTools->base64_encode(string(pIActor->m_pEntity()->GetName())) == name)
				return pIActor->m_entityId();
		}
	}

	return 0;
}

void HitsUpdate()
{
	if (HitsRegister.size())
	{
		if (!HitsUsing)
		{
			SSystemGlobalEnvironment *pSSystemGlobalEnvironment = SSystemGlobalEnvironment::GetInstance();
			if (pSSystemGlobalEnvironment == nullptr)
				return;

			IGame* pIGame = pSSystemGlobalEnvironment->pIGame();
			if (pIGame == nullptr)
				return;

			if (pIGame->GetICVar()->GetIGameRoomType() == 0)
				return;

			IGameFramework* pIGameFramework = pIGame->GetIGameFramework();
			if (pIGameFramework == nullptr)
				return;

			IGameRules* pIGameRules = pIGameFramework->GetIGameRules();
			if (pIGameRules == nullptr)
				return;

			if (!HitsUsing)
			{
				HitsUsing = true;

				for (int i = 0; i < HitsRegister.size(); i++)
					pIGameRules->ServerHit(HitsRegister[i]);

				//if (HitsRegister[HitsRegister.size() - 1].shooterId != 0)
				//	pIGameRules->ServerHit(HitsRegister[HitsRegister.size() - 1]);
				//HitsRegister.erase(HitsRegister.begin() + HitsRegister.size() - 1);

				HitsRegister.clear();
				//HitsRegister.resize(0);

				HitsUsing = false;
			}
		}
	}
}

int WINAPI ParamsRWI(SRWIParams &drinfo, const char* pNameTag, int i)
{
	int returnData = RWI(drinfo, pNameTag, i);

	__asm pushad

	if (drinfo.GetObjTypes() == 799)
	{
		if (drinfo.hits())
		{
			if (drinfo.hits()->pCollider)
			{
				if (drinfo.hits()->pCollider->pIEntity)
				{
					if (drinfo.pSIPhysicalEntity())
					{
						__int32 mlockEntityId = drinfo.hits()->pCollider->pIEntity->m_EntityId();
						__int32 mMyActorId = drinfo.pSIPhysicalEntity()->pIEntity->m_EntityId();

						if ((mlockEntityId != 0) && (mMyActorId != 0) && (mMyActorId != mlockEntityId))
						{
							SSystemGlobalEnvironment *pSSystemGlobalEnvironment = SSystemGlobalEnvironment::GetInstance();
							if (pSSystemGlobalEnvironment != nullptr)
							{
								IGame* pIGame = pSSystemGlobalEnvironment->pIGame();
								if (pIGame != nullptr)
								{
									if (pIGame->GetICVar()->GetIGameRoomType() != 0)
									{
										IGameFramework* pIGameFramework = pIGame->GetIGameFramework();
										if (pIGameFramework != nullptr)
										{
											IGameRules* pIGameRules = pIGameFramework->GetIGameRules();
											if (pIGameRules != nullptr)
											{
												IActorSystem* pIActorSystem = pIGameFramework->GetIActorSystem();
												if (pIActorSystem != nullptr)
												{
													if (!pIActorSystem->GetActor(mlockEntityId))
													{
														if (IActor* pIActor = pIActorSystem->GetActor(mMyActorId))
														{
															__int32 nweaponId = 0;

															if (IItem* pIItem = pIActor->GetCurrentItem())
																if (IWeapon* mCurrentWeapon = pIItem->GetIWeapon())
																	nweaponId = mCurrentWeapon->GetWeaponId();

															__int32 mtypeId = -999, mbulletTypeId = GetBulletTypeId(pIActor);

															if (drinfo.GetFlags() == 1024)
																mtypeId = pIGameRules->GetHitTypeId("bullet");
															else if (drinfo.GetFlags() == 128)
															{
																if (pIActor->GetWeaponSlotId() == 1718185579)
																{
																	mbulletTypeId = -1;
																	mtypeId = pIGameRules->GetHitTypeId("melee");
																}
															}

															HitInfo hinfo;
															RtlSecureZeroMemory(&hinfo, sizeof(HitInfo));
															hinfo.shooterId = mMyActorId;
															hinfo.targetId = mlockEntityId;
															hinfo.materialId = GetMaterialId(drinfo.hits()->pCollider->pIEntity, drinfo.hits()->partId);
															hinfo.typeId = mtypeId;
															hinfo.bulletType = mbulletTypeId;
															hinfo.partId = drinfo.hits()->partId;
															hinfo.pos = drinfo.hits()->pos;
															hinfo.dir = Vec3{ drinfo.dir().x / 100.0f, drinfo.dir().y / 100.0f, drinfo.dir().z / 100.0f };
															hinfo.meleeArmorAbsorb = 0;
															hinfo.weaponId = nweaponId;
															hinfo.projectileId = 0;
															hinfo.projectileClassId = 0;
															hinfo.normal = drinfo.hits()->normal;
															hinfo.distanceToHit = drinfo.hits()->dist;
															hinfo.remote = 0;

															if (hinfo.shooterId != 0)
																pIGameRules->ServerHit(hinfo);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (drinfo.GetObjTypes() == 7 || drinfo.GetObjTypes() == 287 || drinfo.GetObjTypes() == 799)
		HitsUpdate();

	__asm popad

	return returnData;
}