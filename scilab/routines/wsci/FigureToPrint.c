/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "FigureToPrint.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern HINSTANCE hdllInstance;
extern int WindowsPrintScreen;
extern int Printer_XRes;
extern int Printer_YRes;
extern void dos2win32 (char *filename, char *filename1);
extern char GetPrinterOrientation(void);
extern void Setscig_buzyState(BOOL state);
/*-----------------------------------------------------------------------------------*/
#define MAXSTR 255
static char filename[MAXSTR], filename1[MAXSTR];
extern LS ls;
/*-----------------------------------------------------------------------------------*/
BOOL FigureToPrint(int figurenumber,BOOL Postscript)
{
	BOOL bOK=FALSE;
	struct BCG *ScilabGC=NULL;
	ScilabGC = GetWindowXgcNumber (figurenumber);

	if (ScilabGC)
	{
		if (Postscript)
		{
			 PrintPs (ScilabGC);
		}
		else
		{
			CopyPrint (ScilabGC);
		}
		bOK=TRUE;
	}
	else bOK=FALSE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int CopyPrint (struct BCG *ScilabGC)
{
	int xPage, yPage;
	static DOCINFO di = {sizeof (DOCINFO), MSG_SCIMSG12, NULL};
	BOOL bError = FALSE;
	HDC hDCfromhWnd=NULL;
	HDC printer=NULL;
	LPGW lpgw;
	ABORTPROC lpfnAbortProc;
	DLGPROC lpfnPrintDlgProc;
	static PRINTDLG pd;
	HWND hwnd;
	RECT rect,RectRestore;
	PRINT pr;

	integer SaveCurrentWindow=0;

	SaveCurrentWindow=GetCurrentFigureWindows();

	Setscig_buzyState(TRUE);

	SetCurrentFigureWindows (ScilabGC->CurWindow);

	lpgw = ScilabGC->lpgw;
	hwnd = ScilabGC->CWindow;
	hDCfromhWnd=TryToGetDC (hwnd);

	printer = (HDC)GetPrinterDC();

	if (NULL == printer)
	{
		/* Redessine si Cancel Impression */
		GetClientRect (hwnd, &RectRestore);
		scig_replay_hdc ('W', ScilabGC->CurWindow, hDCfromhWnd,RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
		Setscig_buzyState(FALSE);
		return TRUE;		/* abort */
	}

	pr.hdcPrn = printer;
	xPage = GetDeviceCaps (pr.hdcPrn, HORZRES);
	yPage = GetDeviceCaps (pr.hdcPrn, VERTRES);

	GetClientRect (hwnd, &rect);
	SetLastError (0);
	if (SetWindowLong (hwnd, 4, (LONG) ((LP_PRINT) & pr)) == 0 && GetLastError () != 0)
	{
		sciprint (MSG_ERROR34);
		Setscig_buzyState(FALSE);
		return TRUE;
	}
	if (SetWindowLong(ScilabGC->hWndParent, 4, (LONG) ((LP_PRINT) & pr)) == 0	&& GetLastError () != 0)
	{
		sciprint (MSG_ERROR34);
		Setscig_buzyState(FALSE);
		return TRUE;
	}
	PrintRegister ((LP_PRINT) & pr);
	EnableWindow (hwnd, FALSE);

	pr.bUserAbort = FALSE;
	lpfnPrintDlgProc = (DLGPROC) MyGetProcAddress ("PrintDlgProc",PrintDlgProc);
	lpfnAbortProc = (ABORTPROC) MyGetProcAddress ("PrintAbortProc",PrintAbortProc);
	pr.hDlgPrint = CreateDialogParam (hdllInstance, "PrintDlgBox", hwnd, lpfnPrintDlgProc, (LPARAM) lpgw->Title);
	SetAbortProc (pr.hdcPrn, lpfnAbortProc);

	if (StartDoc (pr.hdcPrn, &di) > 0)
	{
		if (StartPage (pr.hdcPrn) > 0)
		{
			int scalef = 1;
			SetMapMode (pr.hdcPrn, MM_TEXT);
			SetBkMode (pr.hdcPrn, TRANSPARENT);
			SetTextAlign (pr.hdcPrn, TA_LEFT | TA_BOTTOM);
			scalef = (int) (10.0 * ((double) xPage * yPage) / (6800.0 * 4725.0));
			GetClientRect (hwnd, &RectRestore);
			/* Evite bug lorsque l'on selectionne la fenetre & que l'on imprime apres */
			scig_replay_hdc ('P', ScilabGC->CurWindow, hDCfromhWnd,RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
			WindowsPrintScreen = 1;
			Printer_XRes = GetDeviceCaps (pr.hdcPrn, LOGPIXELSX);
			Printer_YRes = GetDeviceCaps (pr.hdcPrn, LOGPIXELSY);
			scig_replay_hdc ('P', ScilabGC->CurWindow, printer,xPage, yPage, scalef);
			WindowsPrintScreen = 0;
			/* Redessine à l'ecran apres l'impression */
			scig_replay_hdc ('W', ScilabGC->CurWindow,hDCfromhWnd,RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
			if (EndPage (pr.hdcPrn) > 0)  EndDoc (pr.hdcPrn);
			else bError = TRUE;
		}
	}
	else bError = TRUE;

	if (!pr.bUserAbort)
	{
		EnableWindow (hwnd, TRUE);
		DestroyWindow (pr.hDlgPrint);
	}
	if (printer) {DeleteDC (printer); printer=NULL;}
	SetWindowLong (hwnd, 4, (LONG) (0L));
	SetWindowLong (ScilabGC->hWndParent, 4, (LONG) (0L));
	PrintUnregister ((LP_PRINT) & pr);

	ReleaseDC (hwnd, hDCfromhWnd);
	SetCurrentFigureWindows (SaveCurrentWindow);

	Setscig_buzyState(FALSE);
	InvalidateRect(hwnd,NULL,TRUE);

	return bError || pr.bUserAbort;
}
/*-----------------------------------------------------------------------------------*/
void PrintPs (struct BCG *ScilabGC)
{
	HDC hPrinterDC=NULL;
	char *p1;
	char ori;
	/** getting ls flags **/
	ls.use_printer = 1;
	ls.colored = 1;

	hPrinterDC=GetPrinterDC(); // Just to Configure Printer
	if (hPrinterDC) {DeleteDC(hPrinterDC);hPrinterDC=NULL;}

	/** getting filename **/
	if ((p1 = getenv ("TMPDIR")) == (char *) 0)
	{
		sciprint (MSG_WARNING15);
		return;
	}
	sprintf (filename, "%s/scilab-%d", p1, (int) ScilabGC->CurWindow);

	dos2win32 (filename, filename1);

	scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");

	ori =GetPrinterOrientation();

	if (ScilabPsToEps (ori, filename1, filename) == 0)
	{
		if (gp_printfile (hdllInstance, ScilabGC->hWndParent, filename,(char *) 0) == FALSE)	sciprint (MSG_ERROR78);
	}
	/** filename is destroyed when we quit scilab **/

}
/*-----------------------------------------------------------------------------------*/
