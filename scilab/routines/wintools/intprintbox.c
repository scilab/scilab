/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intprintbox.h"

#if WIN32
	static HDC PrinterHDC=NULL;
	static char PrinterName[2048];
	static char PrinterOrientation;
#endif

/*-----------------------------------------------------------------------------------*/
#if WIN32
	BOOL ConfigurePrinterDialogBox(void);
#endif
/*-----------------------------------------------------------------------------------*/
/* Open Print DialogBox*/
int C2F(intprintsetupbox) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(0,1);

	#if WIN32
		if (ConfigurePrinterDialogBox())
		{
			*paramoutINT=(int)(TRUE);
		}
		else
		{
			*paramoutINT=(int)(FALSE);
		}
	#else
		*paramoutINT=(int)(FALSE);
	#endif

	n1=1;
	CreateVarFromPtr(1, "b", &n1, &n1,&paramoutINT);
	LhsVar(1)=1;
	FREE(paramoutINT);
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
#if WIN32
BOOL ConfigurePrinterDialogBox(void)
{
	BOOL bOK=FALSE;

	PRINTDLG pd;

	if (PrinterHDC) {DeleteDC(PrinterHDC);PrinterHDC=NULL;}
	wsprintf(PrinterName,"%s","EMPTY");
	PrinterOrientation='p';

	memset (&pd, 0, sizeof (PRINTDLG));
	pd.lStructSize = sizeof (PRINTDLG);
	pd.hwndOwner = NULL;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	pd.hDC = NULL;
	pd.Flags = PD_ALLPAGES | PD_COLLATE | PD_RETURNDC| PD_USEDEVMODECOPIESANDCOLLATE|PD_NOSELECTION|PD_HIDEPRINTTOFILE|PD_NONETWORKBUTTON;
	pd.nFromPage = 0;
	pd.nToPage = 0;
	pd.nMinPage = 0;
	pd.nMaxPage = 0;
	pd.nCopies = 1;
	pd.hInstance = NULL;
	pd.lCustData = 0L;
	pd.lpfnPrintHook = NULL;
	pd.lpfnSetupHook = NULL;
	pd.lpPrintTemplateName = NULL;
	pd.lpSetupTemplateName = NULL;
	pd.hPrintTemplate = NULL;
	pd.hSetupTemplate = NULL;
	if (PrintDlg (&pd) == FALSE)
	{
		wsprintf(PrinterName,"%s","EMPTY");
		PrinterOrientation='p';
		PrinterHDC=NULL;
		bOK=FALSE;
	}
	else
	{
		LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(pd.hDevNames);
		LPDEVMODE lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);
		LPCTSTR lpszDevice = (LPCTSTR)lpDev + lpDev->wDeviceOffset;
		
		wsprintf(PrinterName,"%s",lpszDevice);
		GlobalUnlock(pd.hDevNames);

		if (lpDevMode->dmOrientation==DMORIENT_PORTRAIT ) PrinterOrientation='p';
		else PrinterOrientation='l';
		GlobalUnlock(pd.hDevMode);

		PrinterHDC=pd.hDC;
		bOK=TRUE;
	}

	return bOK;
}
#endif
/*-----------------------------------------------------------------------------------*/
#if WIN32
HDC GetPrinterDC(void)
{
	if (PrinterHDC == NULL)
	{
		ConfigurePrinterDialogBox();
	}

	return PrinterHDC;
}
#endif
/*-----------------------------------------------------------------------------------*/
#if WIN32
char GetPrinterOrientation(void)
{
	return PrinterOrientation;
}
#endif
/*-----------------------------------------------------------------------------------*/
#if WIN32
char* GetPrinterName(void)
{
	char *ReturnPrinterName=NULL;

	ReturnPrinterName=MALLOC(strlen(PrinterName)*sizeof(char));
	wsprintf(ReturnPrinterName,"%s",PrinterName);

	return ReturnPrinterName;
}
#endif
/*-----------------------------------------------------------------------------------*/