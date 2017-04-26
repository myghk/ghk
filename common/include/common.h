#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <io.h>

using namespace std;

//ɾ������ָ��
#ifndef DEL_PTR
#define DEL_PTR(p)              if (p != NULL) { delete p; p = NULL; }
#endif

//ɾ������ָ��
#ifndef DEL_PTR_ARRAY
#define DEL_PTR_ARRAY(p)        if (p != NULL) { delete [] p; p = NULL; }
#endif
//===================================xml����=========================================
#define CONFIGURE_FILE			L"config.xml"
#define NODE_CONFIG_ROOT		"config"
#define NODE_UPDATEURL			"updateurl"
#define NODE_DATASOURCEURL		"datasourceurl"

//===================================��������================================================
#define DOWNLOAD_INFO			L"download_info"
#define STATUS					L"status"
#define VERSION					L"version"
#define DEFAULT_VERSION			L"1.0.0.0"
#define SERVER_VERSION			L"server_version"
#define VERSION_URL				L"version_url"


#define WAIT_TIME_INFO			L"waittimeinfo"	
#define GET_VERSION_TIME		L"get_version_time"
#define UPDATE_TIME				L"update_time"

//================================ȫ�ֱ�������=======================================
//==================================�ṹ�嶨��=======================================
typedef struct tagStudent{
	int num;
	int age;
	int city;
	tagStudent(){ memset( this ,0 ,sizeof( struct  tagStudent ) );}
}Student;

//================================ȫ�ֺ�������=======================================

extern string WstringToString(const wstring& _src);

extern string WstringToString_UTF8(const wstring& _src);

extern wstring StringToWstring(const string& _src);

extern wstring StringToWstring_UTF8(const string& _src);

extern string  ReadFile(const string& _filename);

extern wstring GetCurrentPath();

extern void RunCmd();

extern vector<string> StringSplit(string str, string pattern);

extern bool IsFileExist(wstring strPath);

extern vector<wstring> wstringSplit(wstring str, wstring pattern);

extern wstring FormatString(const wchar_t *fmt,...);

extern vector<wstring> GetFileList( wstring path, wstring pattern);

//дIni
extern void WriteIni(wstring Field, wstring Key, wstring KeyValue);

//��Ini
extern void ReadIni(wstring Field, wstring Key, wstring &strValue, wstring DefaultValue = L"0");




