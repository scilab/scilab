/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <ShlObj.h>
#pragma comment(lib, "shell32.lib")
#include "FilesAssociationBox.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*fptr)(void);
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK FilesAssociationDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
BOOL ON_FILESASSOCIATIONBOX_WM_INITDIALOG(HWND hDlg,HWND hwndFocus, LPARAM lParam);
BOOL ON_FILESASSOCIATIONBOX_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void ON_FILESASSOCIATIONBOX_WM_CLOSE(HWND hwnd);

BOOL TestRegistryKey(char *Key,char *ValueToCompare);

void InitCheckSCI(HWND hDlgParent);
void InitCheckSCE(HWND hDlgParent);
void InitCheckTST(HWND hDlgParent);
void InitCheckDEM(HWND hDlgParent);
void InitCheckCOS(HWND hDlgParent);
void InitCheckCOSF(HWND hDlgParent);
void InitCheckSAV(HWND hDlgParent);
void InitCheckBIN(HWND hDlgParent);
void InitCheckGRAPH(HWND hDlgParent);
void InitCheckGRAPHB(HWND hDlgParent);
BOOL InitCheckBox(HWND hDlgParent,char *extension);

void UpdateFromCheckBox(HWND hDlgParent,fptr Update,fptr Delete);
BOOL UpdateSCE(void);
BOOL UpdateSCI(void);
BOOL UpdateTST(void);
BOOL UpdateDEM(void);
BOOL UpdateCOS(void);
BOOL UpdateCOSF(void);
BOOL UpdateSAV(void);
BOOL UpdateBIN(void);
BOOL UpdateGRAPH(void);
BOOL UpdateGRAPHB(void);
BOOL UpdateAKey(char *Clef,char *line);

BOOL DeleteSCE(void);
BOOL DeleteSCI(void);
BOOL DeleteTST(void);
BOOL DeleteDEM(void);
BOOL DeleteCOS(void);
BOOL DeleteCOSF(void);
BOOL DeleteSAV(void);
BOOL DeleteBIN(void);
BOOL DeleteGRAPH(void);
BOOL DeleteGRAPHB(void);
BOOL DeleteAKey(char *Clef);

char * GetWhereIsThisExe(void);
/*-----------------------------------------------------------------------------------*/
extern BOOL IsWindowInterface(void);
extern LPTW GetTextWinScilab(void);
extern int GetCurrentLanguage(void);
/*-----------------------------------------------------------------------------------*/
#define PATH_MAX 512
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI FilesAssociationBox (HWND hwnd)
{
	DLGPROC FilesAssociationDlgProc ;

	FilesAssociationDlgProc = (DLGPROC) GetProcAddress((HINSTANCE)GetModuleHandle("LibScilab"),"FilesAssociationDlgProc");
	DialogBox((HINSTANCE)GetModuleHandle("LibScilab"),(LPCSTR)IDD_FILEASSOCIATION,hwnd,FilesAssociationDlgProc) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK FilesAssociationDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wMsg)
	{
		case WM_INITDIALOG:
			return HANDLE_WM_INITDIALOG(hDlg,wParam,lParam,ON_FILESASSOCIATIONBOX_WM_INITDIALOG);

		case WM_COMMAND:
			return HANDLE_WM_COMMAND(hDlg,wParam,lParam,ON_FILESASSOCIATIONBOX_WM_COMMAND);	

		case WM_CLOSE:
			HANDLE_WM_DESTROY(hDlg,wParam,lParam,ON_FILESASSOCIATIONBOX_WM_CLOSE);
			return TRUE;
	}

	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_FILESASSOCIATIONBOX_WM_INITDIALOG(HWND hDlg,HWND hwndFocus, LPARAM lParam)
{
	int CodeLanguage=GetCurrentLanguage();
	
	switch (CodeLanguage)
	{
		case 0:default:
			SetWindowText(hDlg,"File's Association");
		break;

		case 1:
		SetWindowText(hDlg,"Association des fichiers");
		break;
	}

	InitCheckBox(hDlg,".sci");
	InitCheckBox(hDlg,".sce");
	InitCheckBox(hDlg,".tst");
	InitCheckBox(hDlg,".dem");
	InitCheckBox(hDlg,".cos");
	InitCheckBox(hDlg,".cosf");
	InitCheckBox(hDlg,".sav");
	InitCheckBox(hDlg,".bin");
	InitCheckBox(hDlg,".graph");
	InitCheckBox(hDlg,".graphb");

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_FILESASSOCIATIONBOX_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case IDOK:
		{
			HWND hControl[10];
			hControl[0] = GetDlgItem(hwnd, IDC_CHECKSCI);
			hControl[1] = GetDlgItem(hwnd, IDC_CHECKSCE);
			hControl[2] = GetDlgItem(hwnd, IDC_CHECKTST);
			hControl[3] = GetDlgItem(hwnd, IDC_CHECKDEM);
			hControl[4] = GetDlgItem(hwnd, IDC_CHECKCOS);
			hControl[5] = GetDlgItem(hwnd, IDC_CHECKCOSF);
			hControl[6] = GetDlgItem(hwnd, IDC_CHECKBIN);
			hControl[7] = GetDlgItem(hwnd, IDC_CHECKSAV);
			hControl[8] = GetDlgItem(hwnd, IDC_CHECKGRAPH);
			hControl[9] = GetDlgItem(hwnd, IDC_CHECKGRAPHB);

			UpdateFromCheckBox(hControl[0],&UpdateSCI,&DeleteSCI);
			UpdateFromCheckBox(hControl[1],&UpdateSCE,&DeleteSCE);
			UpdateFromCheckBox(hControl[2],&UpdateTST,&DeleteTST);
			UpdateFromCheckBox(hControl[3],&UpdateDEM,&DeleteDEM);
			UpdateFromCheckBox(hControl[4],&UpdateCOS,&DeleteCOS);
			UpdateFromCheckBox(hControl[5],&UpdateCOSF,&DeleteCOSF);
			UpdateFromCheckBox(hControl[6],&UpdateBIN,&DeleteBIN);
			UpdateFromCheckBox(hControl[7],&UpdateSAV,&DeleteSAV);
			UpdateFromCheckBox(hControl[8],&UpdateGRAPH,&DeleteGRAPH);
			UpdateFromCheckBox(hControl[9],&UpdateGRAPHB,&DeleteGRAPHB);

			SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);

			EndDialog(hwnd, IDOK);
			return TRUE;
		}

		case IDCANCEL:
		{
			EndDialog(hwnd, IDCANCEL);
			return TRUE;
		}
	}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_FILESASSOCIATIONBOX_WM_CLOSE(HWND hwnd)
{
	EndDialog (hwnd, 0L);
}
/*-----------------------------------------------------------------------------------*/
BOOL InitCheckBox(HWND hDlgParent,char *extension)
{
	if ( strcmp(extension,".sci")==0 )
	{
		InitCheckSCI(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".sce")==0 )
	{
		InitCheckSCE(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".tst")==0 )
	{
		InitCheckTST(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".dem")==0 )
	{
		InitCheckDEM(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".cos")==0 )
	{
		InitCheckCOS(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".cosf")==0 )
	{
		InitCheckCOSF(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".graph")==0 )
	{
		InitCheckGRAPH(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".graphb")==0 )
	{
		InitCheckGRAPHB(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".bin")==0 )
	{
		InitCheckBIN(hDlgParent);
		return TRUE;
	}

	if ( strcmp(extension,".sav")==0 )
	{
		InitCheckSAV(hDlgParent);
		return TRUE;
	}

	return FALSE;	
}
/*-----------------------------------------------------------------------------------*/
BOOL TestRegistryKey(char *Key,char *ValueToCompare)
{
	BOOL bOK=TRUE;
	HKEY key;
	DWORD Length=MAX_PATH;

	if ( RegOpenKeyEx(HKEY_CLASSES_ROOT, Key, 0, KEY_QUERY_VALUE , &key) == ERROR_SUCCESS )
	{
		char Line[MAX_PATH];
		if ( RegQueryValueEx(key,"", NULL, NULL, (LPBYTE)&Line, &Length) != ERROR_SUCCESS )
		{
			bOK=FALSE;
		}
		else
		{
			if ( stricmp(Line,ValueToCompare) != 0 ) bOK=FALSE;
		}
		RegCloseKey(key);

	}
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
void UpdateFromCheckBox(HWND hwnd,fptr Update,fptr Delete)
{
	LRESULT lResult;

	lResult = SendMessage(  hwnd ,(UINT) BM_GETCHECK, (WPARAM) 0,(LPARAM) 0); 

	if (lResult == BST_CHECKED)
	{
		(Update)();
	}

	if (lResult == BST_UNCHECKED)
	{
		(Delete)();
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateSCE(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteSCE();

	bOK=UpdateAKey(".sce","SCE_scilab_file");
	if (bOK) bOK=UpdateAKey("SCE_scilab_file","SCE_scilab_file");

	wsprintf(Line,"\"%s%s\",9",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
		case 1:
			wsprintf(KeyRegistryWithLanguage,"SCE_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

		default :case 0:
			wsprintf(KeyRegistryWithLanguage,"SCE_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);

	wsprintf(Line,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\shell\\print\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateSCI(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteSCI();

	bOK=UpdateAKey(".sci","SCI_scilab_file");
	if (bOK) bOK=UpdateAKey("SCI_scilab_file","SCI_scilab_file");

	wsprintf(Line,"\"%s%s\",10",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
	case 1:
		wsprintf(KeyRegistryWithLanguage,"SCI_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

	default :case 0:
		wsprintf(KeyRegistryWithLanguage,"SCI_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);


	wsprintf(Line,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\shell\\print\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateTST(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteTST();

	bOK=UpdateAKey(".tst","TST_scilab_file");
	if (bOK) bOK=UpdateAKey("TST_scilab_file","TST_scilab_file");

	wsprintf(Line,"\"%s%s\",11",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
	case 1:
		wsprintf(KeyRegistryWithLanguage,"TST_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

	default :case 0:
		wsprintf(KeyRegistryWithLanguage,"TST_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);


	wsprintf(Line,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\shell\\print\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateDEM(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteDEM();

	bOK=UpdateAKey(".dem","DEM_scilab_file");
	if (bOK) bOK=UpdateAKey("DEM_scilab_file","DEM_scilab_file");

	wsprintf(Line,"\"%s%s\",6",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
	case 1:
		wsprintf(KeyRegistryWithLanguage,"DEM_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

	default :case 0:
		wsprintf(KeyRegistryWithLanguage,"DEM_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);


	wsprintf(Line,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\shell\\print\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateCOS(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteCOS();

	bOK=UpdateAKey(".cos","COS_scilab_file");
	if (bOK) bOK=UpdateAKey("COS_scilab_file","COS_scilab_file");

	wsprintf(Line,"\"%s%s\",4",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("COS_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
	case 1:
		wsprintf(KeyRegistryWithLanguage,"COS_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

	default :case 0:
		wsprintf(KeyRegistryWithLanguage,"COS_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);


	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateCOSF(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();
	int Language=GetCurrentLanguage();
	char KeyRegistryWithLanguage[1024];

	DeleteCOSF();

	bOK=UpdateAKey(".cosf","COSF_scilab_file");
	if (bOK) bOK=UpdateAKey("COSF_scilab_file","COSF_scilab_file");

	wsprintf(Line,"\"%s%s\",5",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("COSF_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("COSF_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	switch (Language)
	{
	case 1:
		wsprintf(KeyRegistryWithLanguage,"COSF_scilab_file\\shell\\Executer avec %s\\command",VERSION);
		break;

	default :case 0:
		wsprintf(KeyRegistryWithLanguage,"COSF_scilab_file\\shell\\Run with %s\\command",VERSION);
		break;
	}
	if (bOK) bOK=UpdateAKey(KeyRegistryWithLanguage,Line);


	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateSAV(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	DeleteSAV();

	bOK=UpdateAKey(".sav","SAV_scilab_file");
	if (bOK) bOK=UpdateAKey("SAV_scilab_file","SAV_scilab_file");

	wsprintf(Line,"\"%s%s\",8",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SAV_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("SAV_scilab_file\\shell\\open\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateBIN(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	DeleteBIN();

	bOK=UpdateAKey(".bin","BIN_scilab_file");
	if (bOK) bOK=UpdateAKey("BIN_scilab_file","BIN_scilab_file");

	wsprintf(Line,"\"%s%s\",3",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("BIN_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("BIN_scilab_file\\shell\\open\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateGRAPH(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	DeleteGRAPH();

	bOK=UpdateAKey(".graph","GRAPH_scilab_file");
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file","GRAPH_scilab_file");

	wsprintf(Line,"\"%s%s\",7",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file\\shell\\open\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateGRAPHB(void)
{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	DeleteGRAPHB();

	bOK=UpdateAKey(".graphb","GRAPHB_scilab_file");
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file","GRAPHB_scilab_file");

	wsprintf(Line,"\"%s%s\",7",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file\\shell\\open\\command",Line);

	FREE(path);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateAKey(char *Clef,char *line)
{
	HKEY key;
	DWORD result=0;
	BOOL bOK=TRUE;

	if ( RegCreateKeyEx(HKEY_CLASSES_ROOT, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result) == ERROR_SUCCESS ) 
		{
			if ( RegSetValueEx(key, "", 0, REG_SZ, (LPBYTE)line, (DWORD)(strlen(line)+1)) != ERROR_SUCCESS ) bOK=FALSE;
			RegFlushKey(key);
			RegCloseKey(key);
		}
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteSCE(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".sce");
	bOK[1]=DeleteAKey("SCE_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteSCI(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".sci");
	bOK[1]=DeleteAKey("SCI_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteTST(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".tst");
	bOK[1]=DeleteAKey("TST_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteDEM(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".dem");
	bOK[1]=DeleteAKey("DEM_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteCOS(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".cos");
	bOK[1]=DeleteAKey("COS_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteCOSF(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".cosf");
	bOK[1]=DeleteAKey("COSF_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteBIN(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".bin");
	bOK[1]=DeleteAKey("BIN_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteSAV(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".sav");
	bOK[1]=DeleteAKey("SAV_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteGRAPH(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".graph");
	bOK[1]=DeleteAKey("GRAPH_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteGRAPHB(void)
{
	BOOL bOK[2];

	bOK[0]=DeleteAKey(".graphb");
	bOK[1]=DeleteAKey("GRAPHB_scilab_file");
	return (bOK[0] || bOK[1]);
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteAKey(char *Clef)
{
	BOOL bOK=TRUE;

	if ( SHDeleteKey(HKEY_CLASSES_ROOT,Clef) != ERROR_SUCCESS ) bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
char * GetWhereIsThisExe(void)
{
	LPSTR tail;
	char *fullfilename=NULL;
	fullfilename=(char*)MALLOC(sizeof(char)*MAX_PATH);

	GetModuleFileName(GetModuleHandle(NULL),fullfilename,MAX_PATH);

	if ((tail = strrchr (fullfilename, '\\')) != (LPSTR) NULL)
		{
		tail++;
		*tail = '\0';
		}

	return (char*)fullfilename;
}
/*-----------------------------------------------------------------------------------*/
void InitCheckSCI(HWND hDlgParent)
{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxSCI = GetDlgItem(hDlgParent, IDC_CHECKSCI );

	wsprintf(Key,"%s",".sci");
	wsprintf(GoodValue,"%s","SCI_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",10",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"SCI_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"SCI_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[3]=TestRegistryKey(KeyENG,GoodValue);
    if (!bOK[3]) bOK[3]=TestRegistryKey(KeyFR,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxSCI,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSCI,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckSCE(HWND hDlgParent)
{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxSCE = GetDlgItem(hDlgParent, IDC_CHECKSCE );

	wsprintf(Key,"%s",".sce");
	wsprintf(GoodValue,"%s","SCE_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",9",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"SCE_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"SCE_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[3]=TestRegistryKey(KeyENG,GoodValue);
	if (!bOK[3]) bOK[3]=TestRegistryKey(KeyFR,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxSCE,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSCE,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckTST(HWND hDlgParent)
{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxTST = GetDlgItem(hDlgParent, IDC_CHECKTST );

	wsprintf(Key,"%s",".tst");
	wsprintf(GoodValue,"%s","TST_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",11",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"TST_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"TST_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[3]=TestRegistryKey(KeyENG,GoodValue);
	if (!bOK[3]) bOK[3]=TestRegistryKey(KeyFR,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxTST,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxTST,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckDEM(HWND hDlgParent)
{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxDEM = GetDlgItem(hDlgParent, IDC_CHECKDEM );

	wsprintf(Key,"%s",".dem");
	wsprintf(GoodValue,"%s","DEM_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",6",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"DEM_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"DEM_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[3]=TestRegistryKey(KeyENG,GoodValue);
	if (!bOK[3]) bOK[3]=TestRegistryKey(KeyFR,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%s%s\" -P \"%%1\"",path,WSCILEX);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxDEM,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxDEM,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckCOS(HWND hDlgParent)
{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxCOS = GetDlgItem(hDlgParent, IDC_CHECKCOS );

	wsprintf(Key,"%s",".cos");
	wsprintf(GoodValue,"%s","COS_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COS_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",4",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"COS_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"COS_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(KeyENG,GoodValue);
	if (!bOK[2]) bOK[2]=TestRegistryKey(KeyFR,GoodValue);
	
	if ( bOK[0] && bOK[1] && bOK[2])
		SendMessage(hControlCheckBoxCOS,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxCOS,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckCOSF(HWND hDlgParent)
{
	BOOL bOK[4];
	char Key[PATH_MAX];
	char KeyENG[PATH_MAX];
	char KeyFR[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxCOSF = GetDlgItem(hDlgParent, IDC_CHECKCOSF );

	wsprintf(Key,"%s",".cosf");
	wsprintf(GoodValue,"%s","COSF_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COSF_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",5",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COSF_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -O \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(KeyFR,"COSF_scilab_file\\shell\\Executer avec %s\\command",VERSION);
	wsprintf(KeyENG,"COSF_scilab_file\\shell\\Run with %s\\command",VERSION);
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[3]=TestRegistryKey(KeyENG,GoodValue);
	if (!bOK[3]) bOK[3]=TestRegistryKey(KeyFR,GoodValue);
	
	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3])
		SendMessage(hControlCheckBoxCOSF,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxCOSF,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckSAV(HWND hDlgParent)
{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxSAV = GetDlgItem(hDlgParent, IDC_CHECKSAV );

	wsprintf(Key,"%s",".sav");
	wsprintf(GoodValue,"%s","SAV_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SAV_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",8",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SAV_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxSAV,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSAV,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckBIN(HWND hDlgParent)
{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxBIN = GetDlgItem(hDlgParent, IDC_CHECKBIN );

	wsprintf(Key,"%s",".bin");
	wsprintf(GoodValue,"%s","BIN_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","BIN_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",3",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","BIN_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxBIN,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxBIN,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckGRAPH(HWND hDlgParent)
{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxGRAPH = GetDlgItem(hDlgParent, IDC_CHECKGRAPH );

	wsprintf(Key,"%s",".graph");
	wsprintf(GoodValue,"%s","GRAPH_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPH_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",7",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPH_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxGRAPH,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxGRAPH,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
void InitCheckGRAPHB(HWND hDlgParent)
{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxGRAPHB = GetDlgItem(hDlgParent, IDC_CHECKGRAPHB );

	wsprintf(Key,"%s",".graphb");
	wsprintf(GoodValue,"%s","GRAPHB_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPHB_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%s%s\",7",path,WSCILEX);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPHB_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%s%s\" -X \"%%1\"",path,WSCILEX);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxGRAPHB,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxGRAPHB,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	FREE(path);
}
/*-----------------------------------------------------------------------------------*/
