#include <iostream>
#include <vector>
#include <cstring>        // for strcat()
#include <io.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib") 
#include <stdio.h>
#include <stdbool.h>
//#include <WinBase.h>
#include "enumfile.h"
using namespace std;
#define MAXLEN 512
char configN[] = "file.list";
TCHAR deskdest[255];
WCHAR dir[MAXLEN];
vector<wstring> allPath;

vector<char*>  getFilesList(const char* dir);

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

void newconfig()
{
	SHGetSpecialFolderPath(0, deskdest, CSIDL_DESKTOPDIRECTORY, 0);

	//doFileEnumeration(dir, TRUE, TRUE, myEnumerateFunc, allPath);
	doFileEnumeration(deskdest, TRUE, TRUE, myEnumerateFunc, allPath);

	wcout << deskdest << endl;
	cout << "��������ļ���·����" << endl;
	FILE *fp = freopen(configN, "w+",stdout);
	for (size_t i = 0; i < allPath.size(); i++)
	{
		wstring perPath = allPath.at(i);
		wcout << perPath << endl;

	}
	fclose(fp);
}

int main()
{
	
	//cout << "Enter a directory: ";
	//cin.getline(dir, 200);
	//GetCurrentDirectory(MAXLEN,dir);

	newconfig();
	

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