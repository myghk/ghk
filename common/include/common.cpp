#pragma once
#include "common.h"

//=================================全局变量定义====================================

//=================================全局函数定义=====================================

string WstringToString(const wstring& _src)
{
	int nBufSize = WideCharToMultiByte(GetACP(), 0, _src.c_str(),-1, NULL, 0, 0, FALSE);

	char *szBuf = new char[nBufSize];

	WideCharToMultiByte(GetACP(), 0, _src.c_str(),-1, szBuf, nBufSize, 0, FALSE);

	std::string strRet(szBuf);

	DEL_PTR_ARRAY(szBuf);

	return strRet;
}

string WstringToString_UTF8(const wstring& _src)
{
	int nBufSize = WideCharToMultiByte(CP_UTF8, 0, _src.c_str(),-1, NULL, 0, 0, FALSE);

	char *szBuf = new char[nBufSize];

	WideCharToMultiByte(CP_UTF8, 0, _src.c_str(),-1, szBuf, nBufSize, 0, FALSE);

	std::string strRet(szBuf);

	DEL_PTR_ARRAY(szBuf);

	return strRet;
}

wstring StringToWstring(const string& _src)
{
	//计算字符串 string 转成 wchar_t 之后占用的内存字节数
	int nBufSize = MultiByteToWideChar(GetACP(), 0, _src.c_str(), -1, NULL, 0); 

	//为 wsbuf 分配内存 BufSize 个字节
	wchar_t *wsBuf = new wchar_t[nBufSize];

	//转化为 unicode 的 WideString
	MultiByteToWideChar(GetACP(), 0, _src.c_str() , -1, wsBuf, nBufSize); 

	std::wstring wstrRet(wsBuf);

	DEL_PTR_ARRAY(wsBuf);

	return wstrRet;
}

wstring StringToWstring_UTF8(const string& _src)
{
	//计算字符串 string 转成 wchar_t 之后占用的内存字节数
	int nBufSize = MultiByteToWideChar(CP_UTF8, 0, _src.c_str(), -1, NULL, 0); 

	//为 wsbuf 分配内存 BufSize 个字节
	wchar_t *wsBuf = new wchar_t[nBufSize];

	//转化为 unicode 的 WideString
	MultiByteToWideChar(CP_UTF8, 0, _src.c_str() , -1, wsBuf, nBufSize); 

	std::wstring wstrRet(wsBuf);

	DEL_PTR_ARRAY(wsBuf);

	return wstrRet;
}

string  ReadFile(const string& _filename)
{
	std::string contents = "";
	FILE *fp = fopen(_filename.c_str(), "rb");

	if (fp)
	{
	std::fseek(fp, 0, SEEK_END);
	contents.resize(std::ftell(fp));
	std::rewind(fp);
	std::fread(&contents[0], 1, contents.size(), fp);
	std::fclose(fp);
	}

	return contents;
}

wstring GetCurrentPath()
{
	TCHAR path[1000] = {0};
	GetModuleFileName(NULL, path, 1000);
	wstring strPath = path;
	int nlength = strPath.find_last_of(L"\\");
	strPath = strPath.substr(0, nlength);
	strPath += L"\\";

	return strPath;
}

void RunCmd()
{
	wstring strPath = GetCurrentPath();
	wstring strParam = strPath + L"reg.bat";
	ShellExecute(NULL, L"open", strParam.c_str(), NULL, strPath.c_str(), SW_HIDE);
}

//字符串分割函数  
vector<string> StringSplit(string str, string pattern)  
{  
	string::size_type pos;  
	vector<string> result;  
	str+=pattern;//扩展字符串以方便操作  
	int size=str.size();  

	for(int i=0; i<size; i++)  
	{  
		pos=str.find(pattern,i);  
		if(pos<size)  
		{  
			string s=str.substr(i,pos-i);  
			result.push_back(s);  
			i=pos+pattern.size()-1;  
		}  
	}  
	return result;  
} 

//判断文件是否存在
bool IsFileExist(wstring strPath)
{
	bool bRet = false;
	if ( _access(WstringToString(strPath).c_str(), 0) != -1 )
	{
		bRet = true;
	}

	return bRet;
}

vector<wstring> wstringSplit(wstring str, wstring pattern) 
{
	wstring::size_type pos,size;
	vector<std::wstring> result;
	str += pattern;
	size = str.size();
	for(unsigned int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if(pos < size) {
			wstring s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;

}

wstring FormatString(const wchar_t *fmt,...)
{
	std::wstring strResult=L"";
	if (NULL != fmt)
	{
		va_list marker;            
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<wchar_t> vBuffer(nLength, L'\0');     //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker); 
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
}


