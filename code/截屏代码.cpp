#pragma once
#include "common.h"
#include <io.h>
#include <tlhelp32.h>
#include <GdiPlus.h> 
#pragma comment( lib, "GdiPlus.lib" )  
using namespace Gdiplus;

 
//==================================全局函数=============================================
//获取程序路径
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

void ScreenSnap(HBITMAP hBitmap,TCHAR *bmpPath,HDC dc)  
{  
	BITMAP bmInfo;  
	DWORD bmDataSize;  
	char *bmData;													//位图数据  
	GetObject(hBitmap, sizeof(BITMAP), &bmInfo);					//根据位图句柄，获取位图信息  
	bmDataSize = bmInfo.bmWidthBytes * bmInfo.bmHeight;				//计算位图数据大小  
	bmData = new char[bmDataSize];									//分配数据  
	BITMAPFILEHEADER bfh;											//位图文件头  
	bfh.bfType = 0x4d42;  
	bfh.bfSize = bmDataSize+54;  
	bfh.bfReserved1 = 0;  
	bfh.bfReserved2 = 0;  
	bfh.bfOffBits = 54;  
	BITMAPINFOHEADER bih;											//位图信息头  
	bih.biSize = 40;  
	bih.biWidth = bmInfo.bmWidth;  
	bih.biHeight = bmInfo.bmHeight;  
	bih.biPlanes = 1;  
	bih.biBitCount = 24;  
	bih.biCompression = BI_RGB;  
	bih.biSizeImage = bmDataSize;  
	bih.biXPelsPerMeter = 0;  
	bih.biYPelsPerMeter = 0;  
	bih.biClrUsed = 0;  
	bih.biClrImportant = 0;  
	::GetDIBits(dc,hBitmap, 0, bmInfo.bmHeight, bmData, (BITMAPINFO *)&bih, DIB_RGB_COLORS);				//获取位图数据部分  
	HANDLE hFile=CreateFile(bmpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);		//创建文件  
	DWORD dwSize;  
	WriteFile(hFile, (void *)&bfh, sizeof(BITMAPFILEHEADER), &dwSize, 0);									//写入位图文件头  
	WriteFile(hFile, (void *)&bih, sizeof(BITMAPINFOHEADER), &dwSize, 0);									//写入位图信息头  
	WriteFile(hFile, (void *)bmData, bmDataSize, &dwSize, 0);												//写入位图数据  
	::CloseHandle(hFile);																					//关闭文件句柄 
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)   
{
	UINT  num = 0;
	UINT  size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}    
	}
	free(pImageCodecInfo);
	return -1;
}

//void PrintScreen()
//{
//	HWND hWnd = FindWindow(NULL, L"邮掌柜");
//	RECT rcWnd;  
//	::GetClientRect(hWnd, &rcWnd);											//获取窗口大小  
//	HDC hDC=GetDC(hWnd);													//获取窗口DC  
//	HBITMAP hBmp=::CreateCompatibleBitmap(hDC,rcWnd.right, rcWnd.bottom);	//兼容位图  
//	HDC memDC=::CreateCompatibleDC(hDC);									//兼容DC  
//	SelectObject(memDC, hBmp);												//把兼容位图选入兼容DC中  
//	BitBlt(memDC, 0, 0, rcWnd.right, rcWnd.bottom, hDC, 0, 0, SRCCOPY);		//拷贝DC  
//	ScreenSnap(hBmp, L"d:\\Screen.bmp", hDC); 
//}
void PrintScreen()
{
	HWND hWnd = FindWindow(NULL, L"邮掌柜");
	HDC hDC = GetDC(hWnd);  
	RECT rcWnd;  
	::GetClientRect(hWnd, &rcWnd);	   
	HBITMAP hBmp =  ::CreateCompatibleBitmap(hDC, rcWnd.right, rcWnd.bottom);   
	HDC hMemDC = ::CreateCompatibleDC(hDC);   
	HBITMAP hOld = (HBITMAP)::SelectObject(hMemDC,   hBmp);   
	BitBlt(hMemDC, 0, 0, rcWnd.right, rcWnd.bottom, hDC, 0, 0, SRCCOPY);   
	SelectObject(hMemDC, hOld);   


	
	Bitmap bit(rcWnd.right, rcWnd.bottom), bit2(hBmp, NULL);   
	Graphics g(&bit);   
	//g.ScaleTransform((float)xs/rcWnd.right,   (float)ys/rcWnd.bottom);   
	g.DrawImage(&bit2,   0,   0);   

	 
	EncoderParameters   encoderParameters;   
	int quality = 80;
	encoderParameters.Count   =   1;   
	encoderParameters.Parameter[0].Guid   =   EncoderQuality;   
	encoderParameters.Parameter[0].Type   =   EncoderParameterValueTypeLong;   
	encoderParameters.Parameter[0].NumberOfValues   =   1;   
	encoderParameters.Parameter[0].Value   =   &quality;   

	CLSID                           jpgClsid;  
	GetEncoderClsid(L"image/jpeg",   &jpgClsid);   
	bit.Save(L"D:\\1.jpg",   &jpgClsid,   &encoderParameters);   

	::DeleteObject(hBmp);   
	::DeleteObject(hMemDC);  

}