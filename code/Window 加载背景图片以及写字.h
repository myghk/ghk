
LRESULT CALLBACK WndProcCallBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case UPDATE_FILE:
		{
			RECT        rect ;

			GetClientRect (hWnd, &rect) ;
			InvalidateRect(hWnd, &rect, TRUE); //�����ػ�
		} 
		break;
	case WM_PAINT:
		{

			HDC hdc;
			PAINTSTRUCT ps;
			HDC hdcMem;
			HBITMAP hbmp;
			BITMAP bmp;
			RECT   rect ;

			hdc = BeginPaint(hWnd,&ps);	
			hdcMem  = CreateCompatibleDC(hdc);	//����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
			//wstring strbk = CDM->GetAppWorkingDirectory() + L"updating.bmp";
			hbmp = (HBITMAP)LoadBitmap(::hInstance, (LPCTSTR)IDB_BITMAP_UPDATE);
			GetObject(hbmp, sizeof(BITMAP), &bmp);

			SelectObject(hdcMem, hbmp);
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);		//��ʾλͼ

			DeleteDC(hdcMem);
			DeleteObject(hbmp);

			HFONT hfNew, hfOld;
			LOGFONT lf;
			lf.lfHeight = 0;
			lf.lfWidth = 0;
			lf.lfEscapement = 0;
			lf.lfItalic = false;
			lf.lfUnderline = false;
			lf.lfStrikeOut = false;
			hfNew = CreateFontIndirect(&lf);
			hfOld = (HFONT)SelectObject(hdc,hfNew);

			if (bUnZip) //��ѹ��ɺ�
			{	
				GetClientRect (hWnd, &rect) ;
				rect.bottom += 150;
				wstringstream ssum;
				ssum << nFileSum;

				wstringstream sno;
				sno << nFileNo;
				wstring strText =  L"һ���� " + ssum.str() + L" ����Դ����ǰ���������� " + sno.str() + L" ��";
				SetBkColor(hdc, RGB(242, 242, 242));
				DrawText (hdc, strText.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
			}
			else  //���ڽ�ѹ
			{
				GetClientRect (hWnd, &rect) ;
				rect.bottom += 150;
				SetBkColor(hdc, RGB(242, 242, 242));
				DrawText (hdc, TEXT ("���ڽ�ѹ�ļ������Ե�..."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
			}

			SelectObject(hdc, hfOld);
			DeleteObject(hfNew);
			EndPaint (hWnd, &ps);
		}
		break;
	default:
		{
			DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 1;
}

HWND CreateWindowUpdate()
{
	static const wchar_t WndClassName[] = L"YZGUpdateWindow";

	WNDCLASSEX wc = {0};
	//wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProcCallBack;
	wc.hInstance = ::hInstance;
	wc.lpszClassName = WndClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	RegisterClassEx(&wc);

	int width = GetSystemMetrics ( SM_CXSCREEN ) / 2; 
	int height= GetSystemMetrics ( SM_CYSCREEN ) / 2;

	hWndUpdateTip = CreateWindow(WndClassName, L"���ƹ����",  WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, width - 195, height - 138, 390, 275, 0, 0, ::hInstance, 0);
	return hWndUpdateTip;
}

void ShowUpdateTip()
{
	CreateWindowUpdate();
	
	ShowWindow(hWndUpdateTip, SW_SHOWNORMAL);
	UpdateWindow(hWndUpdateTip);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void GetFileList( string path, vector<string>& files )  
{  
	//�ļ����  
	long   hFile   =   0;  
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) !=  -1)  
	{  
		do  
		{  
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
					GetFileList( p.assign(path).append("\\").append(fileinfo.name), files );  
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  
		_findclose(hFile);  
	}  
} 

void UpdateFileNo() //�����ļ�����  �ص�
{
	//Sleep(1000);
	if ( nFileSum > nFileNo)
	{
		bUnZip = true;
		nFileNo++;
		PostMessage(hWndUpdateTip, UPDATE_FILE, nFileSum, nFileNo);
	}
}
