#pragma once
#pragma warning(disable: 4996)
#include <Windows.h>
#include <stdio.h>
#include <vector>
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

using namespace std;

bool IsInGame = false;

#include "VMTHook.h"
#include "MH\MinHook.h"
#include "sTools.h"
#include "vFunctions.h"