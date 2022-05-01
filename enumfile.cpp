#include "enumfile.h"

void doFileEnumeration(PWCHAR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, vector<wstring>& pUserData)
{

	static BOOL s_bUserBreak = FALSE;
	try {
		//-------------------------------------------------------------------------
		if (s_bUserBreak) return;

		size_t len = wcslen(lpPath);
		if (lpPath == NULL || len <= 0) return;

		//NotifySys(NRS_DO_EVENTS, 0,0);

		WCHAR path[MAX_PATH];
		wcscpy(path, lpPath);
		if (lpPath[len - 1] != L'\\') wcscat(path, L"\\");
		wcscat(path, L"*");

		WIN32_FIND_DATA fd;
		HANDLE hFindFile = FindFirstFile(path, &fd);
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			::FindClose(hFindFile); return;
		}

		WCHAR tempPath[MAX_PATH]; BOOL bUserReture = TRUE; BOOL bIsDirectory;

		BOOL bFinish = FALSE;
		while (!bFinish)
		{
			wcscpy(tempPath, lpPath);
			if (lpPath[len - 1] != L'\\') wcscat(tempPath, L"\\");
			wcscat(tempPath, fd.cFileName);

			bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

			//如果是.或..
			if (bIsDirectory
				&& (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0))
			{
				bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
				continue;
			}

			if (pFunc)// && bEnumFiles != bIsDirectory)
			{
				if (bEnumFiles == bIsDirectory) {
					wcscat(tempPath, L"\\");
				}
				bUserReture = pFunc(tempPath, pUserData);
				if (bUserReture == FALSE)
				{
					s_bUserBreak = TRUE; 
					::FindClose(hFindFile); 
					return;
				}
			}

			//NotifySys(NRS_DO_EVENTS, 0,0);

			if (bIsDirectory && bRecursion) //是子目录
			{

				//doFileEnumeration(tempPath, bRecursion, bEnumFiles, pFunc, pUserData);
			}

			bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
		}

		::FindClose(hFindFile);

		//-------------------------------------------------------------------------
	}
	catch (...) { 
		//ASSERT(0); 
		return; 
	}
}


