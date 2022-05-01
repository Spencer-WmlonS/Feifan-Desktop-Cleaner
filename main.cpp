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
//	FILE* fp = fopen(filename, "r+");   // ��һ���ļ������ڶ�д
//	if (fp != NULL)                             // fopen()�Ƿ�ִ�гɹ�
//	{
//		fclose(fp);                                 // �ɹ����ر��ļ���û�д�����Ҫ����
//		return true;
//	}
//	else                                          // ʧ��
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
	//cout << "��������ļ���·����" << endl;
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
	//cout << "��������ļ���·����" << endl;
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

	cout << "����ʹ�㳴��ϵ�ҳ��㣡�� �˼� ���������������ն����ҷ����Ҷ����谡��" << endl << " A.����ǹ" << endl << " B.��ǹ" << endl <<" C.��ʺ���� ���Ƿ� ��" <<endl<<" D.�۽�"<<endl;
	while (1) {
		FILE* sin = freopen("CON","r",stdin);
		sout = freopen("CON", "w", stdout);
		//scanf("%d", &con);
		cin >> con1;
		fclose(sin);
		fclose(sout);
		
		if (con1.compare("A") == 0) {
			sout = freopen("CON", "w", stdout);
			cout<<"�����ˣ���ý�����ʹ�����ң������������ �Ҵ���Ǯ��С�����ˣ� �ҳن�������O��ȥ���֡���"<<endl;
			fclose(sout);
			newconfig();
		}
			
		else if (con1.compare("C")==0) {
			FILE* fp = fopen(configN, "r");
			if (fp == NULL) {
				fclose(fp);
				sout = freopen("CON", "w", stdout);
				cout<<"���ҵ�����ɱ����"<<endl;
				fclose(sout);
				//not exist, or you don't have read permission
			}
			else
			{
				fclose(fp);
				clean();
				sout = freopen("CON", "w", stdout);
				cout << "���Ѹ磬��ʹ�ȳن��������߅��҆O�ն�����������˼�����҆O��һ�����ڡ�" << endl<<"����ʺ���� ���Ƿ� ����";
				//system("pause");
				fclose(sout);
				break;
			}
		}
		else if (con1.compare("B") == 0) {
			sout = freopen("CON", "w", stdout);
			printf("�飡\n");
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
	strcat(dirNew, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) {// ����Ƿ�ɹ�
		cout << "can not found the file ... " << endl;
		return allPath;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR) //�Ƿ�����Ŀ¼
		{
			//������Ŀ¼Ϊ"."��".."���������һ��ѭ�������������Ŀ¼������������һ������
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;


		//cout << findData.name << "\t<dir>\n";
		// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
		strcpy(dirNew, dir);
		strcat(dirNew, "\\");
		strcat(dirNew, findData.name);
		vector<char*> tempPath = getFilesList(dirNew);
		allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else //������Ŀ¼�������ļ���������ļ������ļ��Ĵ�С
		{
			char* filePath = new char[200];
			strcpy(filePath, dir);
			strcat(filePath, "\\");
			strcat(filePath, findData.name);
			allPath.push_back(filePath);
			//cout << filePath << "\t" << findData.size << " bytes.\n";

		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // �ر��������
	return allPath;
}