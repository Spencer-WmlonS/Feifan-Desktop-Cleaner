#pragma once
#include <windows.h>
#include <vector>
#include <string>
using namespace std;
typedef BOOL(WINAPI* EnumerateFunc) (PWCHAR lpFileOrPath, vector<wstring> &pUserData);
void doFileEnumeration(PWCHAR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, vector<wstring>& pUserData);