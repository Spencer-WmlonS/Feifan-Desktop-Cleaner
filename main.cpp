#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>        // for strcat()
#include <io.h>
#include <shlobj.h>
#include <windows.h>
#include <Shobjidl.h>
#pragma comment(lib, "shell32.lib") 
#include <stdio.h>
#include <stdbool.h>
//#include <WinBase.h>
#include "enumfile.h"
using namespace std;
#define MAXLEN 512
char configN[] = "file.list";
//char configS[] = "filecheck.list";
TCHAR deskdest[255];
WCHAR dir[MAXLEN];
vector<wstring> allPath;

vector<char*>  getFilesList(const char* dir);

HRESULT Movefile(__in PCWSTR pszSrcItem, __in PCWSTR pszDest, PCWSTR pszNewName)
{
	//
	// Initialize COM as STA.
	//
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOperation* pfo;

		//
		// Create the IFileOperation interface
		//
		hr = CoCreateInstance(CLSID_FileOperation,
			NULL,
			CLSCTX_ALL,
			IID_PPV_ARGS(&pfo));
		if (SUCCEEDED(hr))
		{
			// Set the operation flags. Turn off all UI from being shown to the
			// user during the operation. This includes error, confirmation,
			// and progress dialogs.
			hr = pfo->SetOperationFlags(FOF_NO_UI);
			if (SUCCEEDED(hr))
			{
				// Create an IShellItem from the supplied source path.
				IShellItem* psiFrom = NULL;
				hr = SHCreateItemFromParsingName(pszSrcItem,
					NULL,
					IID_PPV_ARGS(&psiFrom));
				if (SUCCEEDED(hr))
				{
					IShellItem* psiTo = NULL;
					if (NULL != pszDest)
					{
						// Create an IShellItem from the supplied
						// destination path.
						hr = SHCreateItemFromParsingName(pszDest,
							NULL,
							IID_PPV_ARGS(&psiTo));
					}

					if (SUCCEEDED(hr))
					{
						// Add the operation
						hr = pfo->MoveItem(psiFrom, psiTo, pszNewName, NULL);
						if (NULL != psiTo)
						{
							psiTo->Release();
						}
					}
					psiFrom->Release();
				}

				if (SUCCEEDED(hr))
				{
					// Perform the operation to copy the file.
					hr = pfo->PerformOperations();
				}
			}
			// Release the IFileOperation interface.
			pfo->Release();
		}

		CoUninitialize();
	}
	return hr;
}

BOOL WINAPI myEnumerateFunc(PWCHAR lpFileOrPath, vector<wstring>& pUserData)
{
	//PWCHAR pdot;
	vector<wstring>& allPath = pUserData;
	//if ((pdot = wcsrchr(lpFileOrPath, L'.')) && wcsicmp(pdot, L".mp3") == 0)
	{
		//printf("%ls\n", lpFileOrPath);
		//PWCHAR filePath = new WCHAR[wcslen(lpFileOrPath)+1];
		//wcscpy(filePath, lpFileOrPath);
		//strcat(filePath, "\\");
		//strcat(filePath, findData.name);
		wstring filePath(lpFileOrPath);
		allPath.push_back(filePath);
	}
	return TRUE;
}

//bool isReadWriteable(const char* filename)
//{
//	FILE* fp = fopen(filename, "r+");   // 打开一个文件以用于读写
//	if (fp != NULL)                             // fopen()是否执行成功
//	{
//		fclose(fp);                                 // 成功：关闭文件，没有错误需要处理
//		return true;
//	}
//	else                                          // 失败
//		return false;
//}

bool checkfile(wstring perPath)
{
	int size = 0;
	wstring temp ;
	FILE* fp = freopen(configN, "r", stdin);
	cin >> size;
	for (size_t i = 0; i < size; i++)
	{
		wcin >> temp;
		if (perPath.compare(temp) == 0) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

void newconfig()
{
	SHGetSpecialFolderPath(0, deskdest, CSIDL_DESKTOPDIRECTORY, 0);

	//doFileEnumeration(dir, TRUE, TRUE, myEnumerateFunc, allPath);
	doFileEnumeration(deskdest, TRUE, TRUE, myEnumerateFunc, allPath);

	//wcout << deskdest << endl;
	//cout << "输出所有文件的路径：" << endl;
	FILE *fp = freopen(configN, "w+",stdout);
	wcout << allPath.size() << endl;
	for (size_t i = 0; i < allPath.size(); i++)
	{
		wstring perPath = allPath.at(i);
		wcout << perPath << endl;

	}
	fclose(fp);
}

void clean()
{
	SHGetSpecialFolderPath(0, deskdest, CSIDL_DESKTOPDIRECTORY, 0);

	//doFileEnumeration(dir, TRUE, TRUE, myEnumerateFunc, allPath);
	doFileEnumeration(deskdest, TRUE, TRUE, myEnumerateFunc, allPath);

	//wcout << deskdest << endl;
	//cout << "输出所有文件的路径：" << endl;
	//FILE* fp = freopen(configS, "w+", stdout);
	for (size_t i = 0; i < allPath.size(); i++)
	{
		wstring perPath = allPath.at(i);
		if (checkfile(perPath)) 
		{
			LPCWSTR temppath = perPath.c_str();
			Movefile(temppath , L"D:\\test", NULL);
			//system("pause");
		}
		//wcout << perPath << endl;

	}
	//fclose(fp);

}

int main()
{
	string con1;
	int con = 0;
	//cout << "Enter a directory: ";
	//cin.getline(dir, 200);
	//GetCurrentDirectory(MAXLEN,dir);
	FILE* sout;

	cout << "我唔使你炒而系我炒你！我 宜家 劈炮唔捞啦，你跪喺度翳我返黎我都唔返黎啊！" << endl << " A.放下枪" << endl << " B.开枪" << endl <<" C.吔屎啦！ 梁非凡 ！" <<endl<<" D.钝角"<<endl;
	while (1) {
		FILE* sin = freopen("CON","r",stdin);
		sout = freopen("CON", "w", stdout);
		//scanf("%d", &con);
		cin >> con1;
		fclose(sin);
		fclose(sout);
		
		if (con1.compare("A") == 0) {
			sout = freopen("CON", "w", stdout);
			cout<<"“得了！唔好讲，唔使担心我，个天帮我呃！ 我储够钱做小生意了， 我迟啲搵埋你哋出去帮手。”"<<endl;
			fclose(sout);
			newconfig();
		}
			
		else if (con1.compare("C")==0) {
			FILE* fp = fopen(configN, "r");
			if (fp == NULL) {
				fclose(fp);
				sout = freopen("CON", "w", stdout);
				cout<<"再乱点刘醒杀了你"<<endl;
				fclose(sout);
				//not exist, or you don't have read permission
			}
			else
			{
				fclose(fp);
				clean();
				sout = freopen("CON", "w", stdout);
				cout << "“醒哥，唔使等迟啲喇，你走咗我哋喺度做仲有咩意思啊，我哋都一齐劈炮”" << endl<<"“吔屎啦！ 梁非凡 ！”";
				//system("pause");
				fclose(sout);
				break;
			}
		}
		else if (con1.compare("B") == 0) {
			sout = freopen("CON", "w", stdout);
			printf("砰！\n");
			fclose(sout);
			//system("pause");
			break;
		}
		else
		{
			sout = freopen("CON", "w", stdout);
			printf("? \n");
			fclose(sout);
		}
	}
	

	return 0;
}

vector<char*> getFilesList(const char* dir)
{
	vector<char*> allPath;
	char dirNew[200];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) {// 检查是否成功
		cout << "can not found the file ... " << endl;
		return allPath;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR) //是否含有子目录
		{
			//若该子目录为"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;


		//cout << findData.name << "\t<dir>\n";
		// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
		strcpy(dirNew, dir);
		strcat(dirNew, "\\");
		strcat(dirNew, findData.name);
		vector<char*> tempPath = getFilesList(dirNew);
		allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else //不是子目录，即是文件，则输出文件名和文件的大小
		{
			char* filePath = new char[200];
			strcpy(filePath, dir);
			strcat(filePath, "\\");
			strcat(filePath, findData.name);
			allPath.push_back(filePath);
			//cout << filePath << "\t" << findData.size << " bytes.\n";

		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄
	return allPath;
}