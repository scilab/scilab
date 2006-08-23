/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "ExportBmpEmf.h"
/*-----------------------------------------------------------------------------------*/
extern integer GetCurrentFigureWindows(void);
extern void SetCurrentFigureWindows(integer win);
extern void Setscig_buzyState(int state);
extern HDC TryToGetDC(HWND hWnd);
extern void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale);
/*-----------------------------------------------------------------------------------*/
int Interface_XS2BMP(int figurenum,char *filename)
{
	int bOK=0;
	struct BCG *ScilabGC=NULL;

	ScilabGC = GetWindowXgcNumber (figurenum);
	if (ScilabGC != (struct BCG *) 0)
	{
		ExportBMP(ScilabGC,filename);
		bOK=1;
	}
	else
	{
		Scierror(999,"xs2bmp : win_num invalid : %d \n",figurenum);
		bOK=0;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int Interface_XS2EMF(int figurenum,char *filename)
{
	int bOK=0;
	struct BCG *ScilabGC=NULL;

	ScilabGC = GetWindowXgcNumber (figurenum);
	if (ScilabGC != (struct BCG *) 0)
	{
		ExportEMF(ScilabGC,filename);
		bOK=1;
	}
	else
	{
		Scierror(999,"xs2emf : win_num invalid : %d \n",figurenum);
		bOK=0;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
void CopyToClipboardEMF (struct BCG *ScilabGC)
{
	TCHAR szDesc[] = "Scilab\0Image\0\0";
	LPGW lpgw;

	HANDLE hmf;

	HWND hwnd;
	HDC hdcEMF=NULL;
	HDC hDCtmp=NULL;
	integer SaveCurrentWindow=0;

	SaveCurrentWindow=GetCurrentFigureWindows();

	lpgw = ScilabGC->lpgw;
	hwnd = ScilabGC->CWindow;

	Setscig_buzyState(TRUE);

	SetCurrentFigureWindows (ScilabGC->CurWindow);

	hDCtmp = TryToGetDC (hwnd);
	if (hDCtmp) hdcEMF = CreateEnhMetaFile (hDCtmp, NULL, NULL, szDesc);

	if (hdcEMF)
	{
		/* view the window */
		if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
		BringWindowToTop (hwnd);
		UpdateWindow (hwnd);

		SetMapMode (hdcEMF, MM_TEXT);
		SetTextAlign (hdcEMF, TA_LEFT | TA_BOTTOM);
		SetWindowExtEx (hdcEMF, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight,(LPSIZE) NULL);
		Rectangle (hdcEMF, 0, 0, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight);

		/** fix hdc in the scilab driver **/
		scig_replay_hdc ('C', ScilabGC->CurWindow, hDCtmp, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
		ReleaseDC(hwnd,hDCtmp);

		scig_replay_hdc ('C', ScilabGC->CurWindow, hdcEMF, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
		hmf = CloseEnhMetaFile (hdcEMF);  
		ReleaseDC(hwnd,hdcEMF);


		OpenClipboard (hwnd);
		EmptyClipboard ();
		SetClipboardData (CF_ENHMETAFILE, hmf);
		CloseClipboard ();
	}

	SetCurrentFigureWindows (SaveCurrentWindow);
	Setscig_buzyState(FALSE);
	return;
}
/*-----------------------------------------------------------------------------------*/
/****************************************
* copy graph window to clipboard 
* copy a CF_BITMAP to the clipboard 
****************************************/
void CopyToClipboardBitmap (struct BCG *ScilabGC)
{
	LPGW lpgw;
	HDC hmemDC;
	HBITMAP hBitmap;
	
	HWND hwnd;
	HDC hdc=NULL;
	integer SaveCurrentWindow=0;

	SaveCurrentWindow=GetCurrentFigureWindows();

	Setscig_buzyState(TRUE);

	SetCurrentFigureWindows (ScilabGC->CurWindow);

	/* view the window */
	lpgw = ScilabGC->lpgw;
	hwnd = ScilabGC->CWindow;

	if (IsIconic (hwnd))   ShowWindow (hwnd, SW_SHOWNORMAL);
	BringWindowToTop (hwnd);
	UpdateWindow (hwnd);

	/* get the context */
	hdc = TryToGetDC (hwnd);

	/* make a bitmap and copy it there */
	if (hdc)
	{
		/** fix hdc in the scilab driver **/
		scig_replay_hdc ('C', ScilabGC->CurWindow, hdc, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
		ReleaseDC (hwnd, hdc);

		hmemDC = CreateCompatibleDC (hdc);
		hBitmap = CreateCompatibleBitmap (hdc, ScilabGC->CWindowWidth,ScilabGC->CWindowHeight);

		if (hBitmap)
		{
			SelectBitmap (hmemDC, hBitmap);
			scig_replay_hdc ('C', ScilabGC->CurWindow, hmemDC,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);

			OpenClipboard (hwnd);
			EmptyClipboard ();
			SetClipboardData (CF_BITMAP, hBitmap);
			CloseClipboard ();

			DeleteDC(hmemDC);
			DeleteBitmap(hBitmap);
		}
		else
		{
			MessageBeep (MB_ICONHAND);
			MessageBox (hwnd, MSG_WARNING16,lpgw->Title, MB_ICONHAND | MB_OK);
		}
	}
	else
	{
		MessageBox(NULL,MSG_ERROR36,MSG_ERROR20,MB_ICONWARNING);
	}

	SetCurrentFigureWindows (SaveCurrentWindow);

	Setscig_buzyState(FALSE);

	return;
}
/*-----------------------------------------------------------------------------------*/
BOOL HdcToBmpFile(HDC hdc, char *pszflname,POINT DeviceSize)
{
	HDC memdc;
	HANDLE hfl;
	DWORD dwBytes, dwWidth, dwHeight, dwNumColors, dwBPP, ColorSize;
	void *pBits;
	HBITMAP hbmp;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBQUAD colors[256];
	BITMAPINFO bmpinfo;
	HGDIOBJ hret;

	dwWidth = DeviceSize.x;
	dwHeight = DeviceSize.y;


	dwBPP = GetDeviceCaps(hdc, BITSPIXEL);

	if(dwBPP <= 8) dwNumColors = 256;
	else dwNumColors = 0;

	if(!(memdc = CreateCompatibleDC(hdc))) return 0;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = dwWidth;
	bmpinfo.bmiHeader.biHeight = dwHeight;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = (WORD) dwBPP;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = dwNumColors;
	bmpinfo.bmiHeader.biClrImportant = dwNumColors;
	hbmp = CreateDIBSection(hdc, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);

	if(!hbmp) goto errato;

	hret = SelectObject(memdc, hbmp);

	if(!hret || (hret == HGDI_ERROR)) goto errato;
	if(!BitBlt(memdc, 0, 0, dwWidth, dwHeight, hdc, 0, 0, SRCCOPY)) goto errato;
	if(dwNumColors) dwNumColors = GetDIBColorTable(memdc, 0, dwNumColors, colors);
	fileheader.bfType = 0x4D42;
	ColorSize = dwNumColors * sizeof(RGBQUAD);

	fileheader.bfSize = ((dwWidth*dwHeight*dwBPP) >> 3)+ColorSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	fileheader.bfReserved1 = fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = ColorSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	infoheader.biSize = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth = dwWidth;
	infoheader.biHeight = dwHeight;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = (WORD) dwBPP;
	infoheader.biCompression = BI_RGB;
	infoheader.biSizeImage = infoheader.biClrImportant = 0;
	infoheader.biXPelsPerMeter = infoheader.biYPelsPerMeter = 0;
	infoheader.biClrUsed = dwNumColors;

	hfl = CreateFile(pszflname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(hfl == INVALID_HANDLE_VALUE) {DeleteObject(hbmp); goto errato;}

	WriteFile(hfl, &fileheader, sizeof(BITMAPFILEHEADER), &dwBytes, 0);
	WriteFile(hfl, &infoheader, sizeof(BITMAPINFOHEADER), &dwBytes, 0);

	if(!dwNumColors) WriteFile(hfl, colors, ColorSize, &dwBytes, 0);
	ColorSize = (dwWidth*dwHeight*dwBPP) >> 3;
	WriteFile(hfl, pBits, ColorSize, &dwBytes, 0);

	CloseHandle(hfl);

	DeleteObject(hbmp);
	DeleteDC(memdc);

	return 1;
errato:
	DeleteDC(memdc); return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL HwndToBmpFile(HWND hwnd, char *pszflname)
{
	HDC memdc, hdc;
	HANDLE hfl;
	DWORD dwBytes, dwNumColors;
	void *pBits;
	HBITMAP hbmp;
	BITMAPFILEHEADER fileheader;
	RGBQUAD colors[256];
	BITMAPINFO bmpinfo;
	HGDIOBJ hret;
	RECT rct;

	hdc = TryToGetDC(hwnd);

	if(!hdc) return 0;

	GetWindowRect(hwnd, &rct);
	rct.bottom -= rct.top;
	rct.right -= rct.left;
	rct.top = GetDeviceCaps(hdc, BITSPIXEL);
	if(rct.top <= 8) dwNumColors = 256;
	else dwNumColors = 0;

	if(!(memdc = CreateCompatibleDC(hdc))) goto relHwndDc;

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = rct.right;
	bmpinfo.bmiHeader.biHeight = rct.bottom;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = (WORD) rct.top;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = dwNumColors;
	bmpinfo.bmiHeader.biClrImportant = dwNumColors;

	hbmp = CreateDIBSection(hdc, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);
	if(!hbmp) goto errato;

	hret = SelectObject(memdc, hbmp);
	if(!hret || (hret == HGDI_ERROR)) goto errato;
	if(!BitBlt(memdc, 0, 0, rct.right, rct.bottom, hdc, 0, 0, SRCCOPY)) goto errato;
	if(dwNumColors) dwNumColors = GetDIBColorTable(memdc, 0, dwNumColors, colors);
	fileheader.bfType = 0x4D42;
	rct.left = dwNumColors * sizeof(RGBQUAD);

	fileheader.bfSize = ((rct.right * rct.bottom * rct.top) >> 3) + rct.left + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileheader.bfReserved1 = fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = rct.left + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biClrImportant = 0;
	bmpinfo.bmiHeader.biClrUsed = dwNumColors;
	hfl = CreateFile(pszflname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(hfl == INVALID_HANDLE_VALUE) {DeleteObject(hbmp); goto errato;}
	WriteFile(hfl, &fileheader, sizeof(BITMAPFILEHEADER), &dwBytes, 0);
	WriteFile(hfl, &bmpinfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytes, 0);
	if(!dwNumColors) WriteFile(hfl, colors, rct.left, &dwBytes, 0);
	WriteFile(hfl, pBits, (rct.right * rct.bottom * rct.top) >> 3, &dwBytes, 0);
	CloseHandle(hfl);
	DeleteObject(hbmp);
	DeleteDC(memdc);
	return 1;
errato:
	DeleteDC(memdc);
relHwndDc:
	ReleaseDC(hwnd, hdc); return 0;
}

/*-----------------------------------------------------------------------------------*/
void ExportBMP(struct BCG *ScilabGC,char *pszflname)
{
	LPGW lpgw;
	HBITMAP hBitmap;
	HDC hmemDC=NULL;
	HWND hwnd;
	HDC hdc=NULL;
	integer SaveCurrentWindow=0;


	Setscig_buzyState(TRUE);

	SaveCurrentWindow=GetCurrentFigureWindows();
	SetCurrentFigureWindows (ScilabGC->CurWindow);

	/* view the window */
	lpgw = ScilabGC->lpgw;
	hwnd = ScilabGC->CWindow;

	if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
	BringWindowToTop (hwnd);
	UpdateWindow (hwnd);

	SetActiveWindow(ScilabGC->hWndParent);      
	
	/* get the context */
	hdc = TryToGetDC (hwnd);

	/* make a bitmap and copy it there */
	if (hdc)
	{
			/** fix hdc in the scilab driver **/
			scig_replay_hdc ('C', ScilabGC->CurWindow, hdc,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
			ReleaseDC (hwnd, hdc);

			hmemDC = CreateCompatibleDC (hdc);
			hBitmap = CreateCompatibleBitmap (hdc,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight);

			if (hBitmap)
			{
				POINT DCSize;
				SelectBitmap (hmemDC, hBitmap);
				scig_replay_hdc ('C', ScilabGC->CurWindow, hmemDC,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
				DCSize.x=ScilabGC->CWindowWidth;
				DCSize.y=ScilabGC->CWindowHeight;

				HdcToBmpFile(hmemDC,pszflname,DCSize);

				DeleteDC(hmemDC);
				DeleteBitmap(hBitmap);
			}
	}
	else
	{
		MessageBox(NULL,MSG_ERROR36,MSG_ERROR20,MB_ICONWARNING);
	}

	SetCurrentFigureWindows (SaveCurrentWindow);

	Setscig_buzyState(FALSE);

}
/*-----------------------------------------------------------------------------------*/
void ExportEMF(struct BCG *ScilabGC,char *pszflname)
{    
	RECT rect;
	HWND hwnd;
	HDC hdc;
	HDC hdcTmp;
	HENHMETAFILE metafile;

	integer SaveCurrentFigure=0; 

	Setscig_buzyState(TRUE);

	SaveCurrentFigure=GetCurrentFigureWindows();
	SetCurrentFigureWindows (ScilabGC->CurWindow);

	hwnd = ScilabGC->CWindow;

	/* view the window */
	if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
	BringWindowToTop (hwnd);
	UpdateWindow (hwnd);

	/* get the context */
	hdcTmp=TryToGetDC (hwnd);
	hdc = CreateEnhMetaFile (hdcTmp, pszflname, NULL, "Scilab Graphics");

	GetClientRect (hwnd, &rect);
	SetMapMode (hdc, MM_TEXT);
	SetTextAlign (hdc, TA_LEFT | TA_BOTTOM);
	
	SetWindowExtEx (hdc,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, (LPSIZE) NULL);
	Rectangle (hdc, 0, 0,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight);

	/** fix hdc in the scilab driver **/
	
	scig_replay_hdc ('C', ScilabGC->CurWindow,hdcTmp,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);
	ReleaseDC(hwnd,hdcTmp);

	scig_replay_hdc ('C', ScilabGC->CurWindow,hdc,ScilabGC->CWindowWidth,ScilabGC->CWindowHeight, 1);

	metafile = CloseEnhMetaFile(hdc);
	DeleteDC(hdc);
	DeleteEnhMetaFile( metafile );

	SetCurrentFigureWindows (SaveCurrentFigure);

	Setscig_buzyState(FALSE);
	return;
}
/*-----------------------------------------------------------------------------------*/
