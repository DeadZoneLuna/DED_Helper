#include "cBase.h"
#include <fstream>

vector<SOCKET> mClients;

char recvData1[2048];

bool IsSocketUse = false;



void RemoteClient()
{
	for (int i = 0; i < mClients.size(); i++)
	{
		//Beep(1000,200);

		fd_set readfds;
		FD_ZERO(&readfds);

		FD_SET(mClients[i], &readfds);
		timeval tv = { 0, 0 };
		select(0, &readfds, 0, 0, &tv);

		if (FD_ISSET(mClients[i], &readfds))
		{
			ZeroMemory(recvData1, sizeof(recvData1));

			if (recv(mClients[i], recvData1, sizeof(recvData1), 0) == SOCKET_ERROR)
				continue;

			if (recvData1 == NULL)
				continue;

			if (!strstr(recvData1, "{"))
				continue;

			if (!strstr(recvData1, "}"))
				continue;

			string recvData = string(sTools->To_CP1251(sTools->To_Unicode(recvData1)));

			if (recvData.size() > sizeof(recvData1))
				continue;

			if (recvData.c_str() == NULL)
				continue;

			if (!strstr(recvData.c_str(), "{"))
				continue;

			if (!strstr(recvData.c_str(), "}"))
				continue;

			if (!recvData.size())
				continue;

			int read_count = 0;
			while (recvData.size() > read_count)
			{
				if (!string(recvData.begin() + read_count, recvData.end()).size())
					break;

				if (!strstr(string(recvData.begin() + read_count, recvData.end()).c_str(), "{"))
					break;

				if (!strstr(string(recvData.begin() + read_count, recvData.end()).c_str(), "}"))
					break;

				string current_data = sTools->GetData2C(string(recvData.begin() + read_count, recvData.end()).c_str(), "{", "}");

				if (sTools->GetData(current_data.c_str(), "msg_type='", '\'') == "clean_data")
				{
					while (HitsUsing) Sleep(1);
					HitsUsing = true;
					if (HitsRegister.size())
						HitsRegister.clear();
					HitsUsing = false;

					while (IsSocketUse) Sleep(1);
					IsSocketUse = true;

					for (int i2 = 0; i2 < mClients.size(); i2++)
						closesocket(mClients[i2]);

					if (mClients.size())
						mClients.clear();

					IsSocketUse = false;

					return;
				}
				else if (sTools->GetData(current_data.c_str(), "msg_type='", '\'') == "regdmg")
				{
					while (HitsUsing) Sleep(1);

					HitsUsing = true;

					string my_Nick = sTools->GetData(current_data.c_str(), "mnick='", '\'');
					string target_Nick = sTools->GetData(current_data.c_str(), "tnick='", '\'');
					int m_entityId = atoi(sTools->GetData(current_data.c_str(), "meId='", '\'').c_str());
					int target_entId = atoi(sTools->GetData(current_data.c_str(), "teId='", '\'').c_str());
					int partId = atoi(sTools->GetData(current_data.c_str(), "pId='", '\'').c_str());
					int materialId = atoi(sTools->GetData(current_data.c_str(), "mId='", '\'').c_str());
					Vec3 target_pos = Vec3{ (float)atof(sTools->GetData(current_data.c_str(), "p_x='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "p_y='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "p_z='", '\'').c_str()) };
					Vec3 target_normal = Vec3{ (float)atof(sTools->GetData(current_data.c_str(), "n_x='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "n_y='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "n_z='", '\'').c_str()) };
					Vec3 m_dir = Vec3{ (float)atof(sTools->GetData(current_data.c_str(), "d_x='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "d_y='", '\'').c_str()), (float)atof(sTools->GetData(current_data.c_str(), "d_z='", '\'').c_str()) };
					int weaponId = atoi(sTools->GetData(current_data.c_str(), "wpId='", '\'').c_str());
					float distToPoint = (float)atof(sTools->GetData(current_data.c_str(), "dTp='", '\'').c_str());
					int bulletType = atoi(sTools->GetData(current_data.c_str(), "bType='", '\'').c_str());
					int typeId = atoi(sTools->GetData(current_data.c_str(), "tId='", '\'').c_str());


					HitInfo hinfo;
					RtlSecureZeroMemory(&hinfo, sizeof(HitInfo));
					hinfo.shooterId = GetEntityIdByName(my_Nick);
					hinfo.targetId = GetEntityIdByName(target_Nick);
					hinfo.materialId = materialId;
					hinfo.typeId = typeId;
					hinfo.bulletType = bulletType;
					hinfo.partId = partId;
					hinfo.pos = target_pos;
					hinfo.dir = m_dir;
					hinfo.meleeArmorAbsorb = 0;
					hinfo.weaponId = weaponId;
					hinfo.projectileId = 0;
					hinfo.projectileClassId = 0;
					hinfo.normal = target_normal;
					hinfo.distanceToHit = distToPoint;
					hinfo.remote = 0;

					HitsRegister.push_back(hinfo);
					HitsUsing = false;
				}
				read_count += current_data.size() + 2;
			}
		}

		FD_CLR(mClients[i], &readfds);
	}
}

DWORD WINAPI ClientsThread()
{

	while (1)
	{
		if (!IsSocketUse)
		{
			IsSocketUse = true;
			RemoteClient();
			IsSocketUse = false;
		}
		Sleep(10);
	}
	return NULL;
}

DWORD WINAPI acceptClients()
{
	ofstream fout("acceptClients.txt");
	fout << "StartLog:" << mClients.size() << "\n";
	while (1)
	{
		Beep(1000,200);
		fout << "Cicl:" << mClients.size() << "\n";
		if (listen(mServer, SOMAXCONN) != SOCKET_ERROR)
		{
			fout << "Sock no err" << "\n";
			SOCKET newClient = accept(mServer, (sockaddr*)&sadr_in, 0);
			fout << "Accept" << "\n";
			if (newClient != INVALID_SOCKET)
			{
				fout << "New client" << "\n";
				while (IsSocketUse) Sleep(1);
				IsSocketUse = true;

				mClients.push_back(newClient);

				IsSocketUse = false;
			}
		}
		
	}
	return NULL;
}

DWORD WINAPI SetRWIHook()
{
	while (1)
	{
		if (SSystemGlobalEnvironment::GetInstance()->pIPhysicalWorld())
		{
			if (CVMTHookManager* RayTraceHook = new CVMTHookManager((DWORD**)SSystemGlobalEnvironment::GetInstance()->pIPhysicalWorld()))
			{
				RWI = (PRWI)RayTraceHook->dwGetMethodAddress(pSRWI / 4);
				RayTraceHook->dwHookMethod((DWORD)ParamsRWI, pSRWI / 4);
				break;
			}
		}
		Sleep(100);
	}
	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		myHelperPort = atoi(sTools->GetData(GetCommandLineA(), "sv_port=", ' ').c_str()) + 1;
		if (myHelperPort != 0)
		{
			ofstream fout("logfile.txt");
			fout << "Starting helper server on port:" << myHelperPort << "\n";
			mServer = sTools->createSocket(myHelperPort);
			if (mServer != INVALID_SOCKET)
			{
				fout << "Server started Sucess\n";
				CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetRWIHook, 0, 0, 0));
				CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acceptClients, 0, 0, 0));
				CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientsThread, 0, 0, 0));
			}
			fout.close();
		}
		return TRUE;
	}
	return FALSE;
}