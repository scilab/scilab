/*-----------------------------------------------------------------------------------*/
/* CORNET Allan INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include <shlwapi.h>
#include <string.h>
#include "../../routines/version.h"
#include "resource.h"
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "shlwapi.lib")
/*-----------------------------------------------------------------------------------*/
typedef int (*fptr)(void);
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK FilesAssociationDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
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
#define PATH_MAX 512
/*-----------------------------------------------------------------------------------*/
char * GetWhereIsThisExe(void)
{
	LPSTR tail;
	char *fullfilename=NULL;
	fullfilename=(char*)malloc(sizeof(char)*MAX_PATH);

	GetModuleFileName(GetModuleHandle(NULL),fullfilename,MAX_PATH);

	if ((tail = strrchr (fullfilename, '\\')) != (LPSTR) NULL)
    {
      tail++;
      *tail = '\0';
    }
  	
	return (char*)fullfilename;
}
/*-----------------------------------------------------------------------------------*/
BOOL FileExist(char *filename)
{
     BOOL retour=FALSE;	
	
     WIN32_FIND_DATA FindFileData;
     HANDLE handle = FindFirstFile (filename, &FindFileData);
     if (handle != INVALID_HANDLE_VALUE)
     {
         FindClose (handle);
         retour=TRUE;
     }
     else retour=FALSE;

     return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL TestScilabFiles(char *path)
{
	char filename[MAX_PATH];
	BOOL bOK=TRUE;

	wsprintf(filename,"%s%s\\%s",path,"bin","LibScilab.dll");
	if (!FileExist(filename)) bOK=FALSE;

	wsprintf(filename,"%s%s\\%s",path,"bin","wscilex.exe");
	if (!FileExist(filename)) bOK=FALSE;

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
		RegCloseKey(key);
	}
	else bOK=FALSE;
  	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteAKey(char *Clef)
{
	BOOL bOK=TRUE;

	if ( SHDeleteKey(HKEY_CLASSES_ROOT,Clef) != ERROR_SUCCESS ) bOK=FALSE;
 	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateRegistry(void)
{
	BOOL bOK[10];
	BOOL bOKAY=TRUE;
	int i=0;

	bOK[0]=UpdateSCE();
	bOK[1]=UpdateSCI();
	bOK[2]=UpdateTST();
	bOK[3]=UpdateDEM();
	bOK[4]=UpdateCOS();
	bOK[5]=UpdateCOSF();
	bOK[6]=UpdateSAV();
	bOK[7]=UpdateBIN();
	bOK[8]=UpdateGRAPH();
	bOK[9]=UpdateGRAPHB();

	for(i=0;i<10;i++)
	{
		if ( ! bOK[i] ) bOKAY=FALSE;
	}

	return bOKAY;
}
/*-----------------------------------------------------------------------------------*/
BOOL DeleteRegistry(void)
{
	BOOL bOK[10];
	BOOL bOKAY=TRUE;
	int i=0;

	bOK[0]=DeleteSCE();
	bOK[1]=DeleteSCI();
	bOK[2]=DeleteTST();
	bOK[3]=DeleteDEM();
	bOK[4]=DeleteCOS();
	bOK[5]=DeleteCOSF();
	bOK[6]=DeleteSAV();
	bOK[7]=DeleteBIN();
	bOK[8]=DeleteGRAPH();
	bOK[9]=DeleteGRAPHB();

	for(i=0;i<10;i++)
	{
		if ( ! bOK[i] ) bOKAY=FALSE;
	}

	return bOKAY;
}
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK InstallDialogProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	switch (wMsg)
		{
		case WM_INITDIALOG:    
			{
			int CodeLanguage=0;

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
			}
		return TRUE;

		case WM_COMMAND:
			{
			switch (LOWORD (wParam))
				{
				case IDOK:
					{
					HWND hControl[10];
					hControl[0] = GetDlgItem(hDlg, IDC_CHECKSCI);
					hControl[1] = GetDlgItem(hDlg, IDC_CHECKSCE);
					hControl[2] = GetDlgItem(hDlg, IDC_CHECKTST);
					hControl[3] = GetDlgItem(hDlg, IDC_CHECKDEM);
					hControl[4] = GetDlgItem(hDlg, IDC_CHECKCOS);
					hControl[5] = GetDlgItem(hDlg, IDC_CHECKCOSF);
					hControl[6] = GetDlgItem(hDlg, IDC_CHECKBIN);
					hControl[7] = GetDlgItem(hDlg, IDC_CHECKSAV);
					hControl[8] = GetDlgItem(hDlg, IDC_CHECKGRAPH);
					hControl[9] = GetDlgItem(hDlg, IDC_CHECKGRAPHB);

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

					RegDisablePredefinedCache();

					EndDialog(hDlg, IDOK);
					return TRUE;
					}
				break;

				case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
					break;

				case WM_CLOSE :
					{
					EndDialog (hDlg, LOWORD (wParam));
					return TRUE;
					}
				break;
				}
			}
		}

	return FALSE;
	}

/*-----------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
  INT_PTR iOK=0;
  
  if ( strcmp(lpCmdLine,"-uninstall")==0 )
  {
	  iOK=DeleteRegistry();
  }
  else if ( strcmp(lpCmdLine,"-install")==0 )
  {
	  iOK=DialogBox(hInstance,(LPCSTR)(IDD_INSTALL),NULL,(DLGPROC)InstallDialogProc);
  }
  else
  {
	/* Interactive Interface */
	char *Path=NULL;
	Path=GetWhereIsThisExe();

	if ( TestScilabFiles(Path) )
	{
		DialogBox(hInstance,(LPCSTR)(IDD_INSTALL),NULL,(DLGPROC)InstallDialogProc);
	}
	else
	{
		MessageBox(NULL,"Please reinstall scilab","Error",MB_ICONWARNING|MB_OK);
		iOK=-1;
	}

	free(Path);
  
  }
  return (int)iOK;
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

	bOK=UpdateAKey(".sce","SCE_scilab_file");
	if (bOK) bOK=UpdateAKey("SCE_scilab_file","SCE_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",9",path);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -O \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\shell\\Execution\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -P \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCE_scilab_file\\shell\\print\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateSCI(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".sci","SCI_scilab_file");
	if (bOK) bOK=UpdateAKey("SCI_scilab_file","SCI_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",10",path);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -O \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\shell\\Execution\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -P \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SCI_scilab_file\\shell\\print\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateTST(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".tst","TST_scilab_file");
	if (bOK) bOK=UpdateAKey("TST_scilab_file","TST_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",11",path);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -O \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\shell\\Execution\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -P \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("TST_scilab_file\\shell\\print\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateDEM(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".dem","DEM_scilab_file");
	if (bOK) bOK=UpdateAKey("DEM_scilab_file","DEM_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",6",path);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -O \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\shell\\Execution\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -P \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("DEM_scilab_file\\shell\\print\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateCOS(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".cos","COS_scilab_file");
	if (bOK) bOK=UpdateAKey("COS_scilab_file","COS_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",4",path);
	if (bOK) bOK=UpdateAKey("COS_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("COS_scilab_file\\shell\\open\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateCOSF(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".cosf","COSF_scilab_file");
	if (bOK) bOK=UpdateAKey("COSF_scilab_file","COSF_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",5",path);
	if (bOK) bOK=UpdateAKey("COSF_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -O \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("COSF_scilab_file\\shell\\open\\command",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("COSF_scilab_file\\shell\\Execution\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateSAV(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".sav","SAV_scilab_file");
	if (bOK) bOK=UpdateAKey("SAV_scilab_file","SAV_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",8",path);
	if (bOK) bOK=UpdateAKey("SAV_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("SAV_scilab_file\\shell\\open\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateBIN(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".bin","BIN_scilab_file");
	if (bOK) bOK=UpdateAKey("BIN_scilab_file","BIN_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",3",path);
	if (bOK) bOK=UpdateAKey("BIN_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("BIN_scilab_file\\shell\\open\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateGRAPH(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".graph","GRAPH_scilab_file");
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file","GRAPH_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",7",path);
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("GRAPH_scilab_file\\shell\\open\\command",Line);

	free(path);
	return bOK;
	}
/*-----------------------------------------------------------------------------------*/
BOOL UpdateGRAPHB(void)
	{
	BOOL bOK=TRUE;
	char Line[512];
	char *path=GetWhereIsThisExe();

	bOK=UpdateAKey(".graphb","GRAPHB_scilab_file");
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file","GRAPHB_scilab_file");

	wsprintf(Line,"\"%sbin\\Wscilex.exe\",7",path);
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file\\DefaultIcon",Line);

	wsprintf(Line,"\"%sbin\\Wscilex.exe\" -X \"%%1\"",path);
	if (bOK) bOK=UpdateAKey("GRAPHB_scilab_file\\shell\\open\\command",Line);

	free(path);
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
void InitCheckSCI(HWND hDlgParent)
	{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxSCI = GetDlgItem(hDlgParent, IDC_CHECKSCI );

	wsprintf(Key,"%s",".sci");
	wsprintf(GoodValue,"%s","SCI_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",10",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -O \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\shell\\Execution\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[3]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCI_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -P \"%%1\"",path);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxSCI,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSCI,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
void InitCheckSCE(HWND hDlgParent)
	{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxSCE = GetDlgItem(hDlgParent, IDC_CHECKSCE );

	wsprintf(Key,"%s",".sce");
	wsprintf(GoodValue,"%s","SCE_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",9",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -O \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\shell\\Execution\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[3]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SCE_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -P \"%%1\"",path);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxSCE,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSCE,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
void InitCheckTST(HWND hDlgParent)
	{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxTST = GetDlgItem(hDlgParent, IDC_CHECKTST );

	wsprintf(Key,"%s",".tst");
	wsprintf(GoodValue,"%s","TST_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",11",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -O \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\shell\\Execution\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[3]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","TST_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -P \"%%1\"",path);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxTST,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxTST,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
void InitCheckDEM(HWND hDlgParent)
	{
	BOOL bOK[5];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxDEM = GetDlgItem(hDlgParent, IDC_CHECKDEM );

	wsprintf(Key,"%s",".dem");
	wsprintf(GoodValue,"%s","DEM_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",6",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -O \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\shell\\Execution\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[3]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","DEM_scilab_file\\shell\\print\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -P \"%%1\"",path);
	bOK[4]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3] && bOK[4])
		SendMessage(hControlCheckBoxDEM,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxDEM,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
void InitCheckCOS(HWND hDlgParent)
	{
	BOOL bOK[3];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxCOS = GetDlgItem(hDlgParent, IDC_CHECKCOS );

	wsprintf(Key,"%s",".cos");
	wsprintf(GoodValue,"%s","COS_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COS_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",4",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COS_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2])
		SendMessage(hControlCheckBoxCOS,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxCOS,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
void InitCheckCOSF(HWND hDlgParent)
	{
	BOOL bOK[4];
	char Key[PATH_MAX];
	char GoodValue[PATH_MAX];
	char *path=GetWhereIsThisExe();
	HWND hControlCheckBoxCOSF = GetDlgItem(hDlgParent, IDC_CHECKCOSF );

	wsprintf(Key,"%s",".cosf");
	wsprintf(GoodValue,"%s","COSF_scilab_file");
	bOK[0]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COSF_scilab_file\\DefaultIcon");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",5",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COSF_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -O \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","COSF_scilab_file\\shell\\Execution\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[3]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] && bOK[3])
		SendMessage(hControlCheckBoxCOSF,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxCOSF,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
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
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",8",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","SAV_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxSAV,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxSAV,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
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
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",3",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","BIN_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxBIN,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxBIN,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
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
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",7",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPH_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxGRAPH,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxGRAPH,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
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
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\",7",path);
	bOK[1]=TestRegistryKey(Key,GoodValue);

	wsprintf(Key,"%s","GRAPHB_scilab_file\\shell\\open\\command");
	wsprintf(GoodValue,"\"%sbin\\wscilex.exe\" -X \"%%1\"",path);
	bOK[2]=TestRegistryKey(Key,GoodValue);

	if ( bOK[0] && bOK[1] && bOK[2] )
		SendMessage(hControlCheckBoxGRAPHB,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hControlCheckBoxGRAPHB,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
	}
/*-----------------------------------------------------------------------------------*/
