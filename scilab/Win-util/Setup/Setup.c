#include <windows.h>
#include <shlwapi.h>
#include <string.h>
#include "../../routines/version.h"
#include "resource.h"

#pragma comment(lib, "shlwapi.lib")
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
BOOL Test1Registry(char *extension,char *KeyToCompare)
{
	BOOL bOK=TRUE;
	HKEY key;
	DWORD Length=MAX_PATH;
	char Clef[MAX_PATH];
	
	wsprintf(Clef,"%s",extension);
	if ( RegOpenKeyEx(HKEY_CLASSES_ROOT, Clef, 0, KEY_QUERY_VALUE , &key) == ERROR_SUCCESS )
	{
		char Line[MAX_PATH];
		if ( RegQueryValueEx(key, "", NULL, NULL, (LPBYTE)&Line, &Length) != ERROR_SUCCESS )
		{
			bOK=FALSE;
		}
		else
		{
			if ( strcmp(Line,KeyToCompare) != 0 ) bOK=FALSE;
		}
		RegCloseKey(key);
		
	}
	else bOK=FALSE;
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL Test2Registry(char *KeyToCompare,char *path)
{
	BOOL bOK=TRUE;
	HKEY key;
	DWORD Length=MAX_PATH;
	char Clef[MAX_PATH];
	
	wsprintf(Clef,"%s\\%s",KeyToCompare,"DefaultIcon");
	if ( RegOpenKeyEx(HKEY_CLASSES_ROOT, Clef, 0, KEY_QUERY_VALUE , &key) == ERROR_SUCCESS )
	{
		char Line[MAX_PATH];
		if ( RegQueryValueEx(key,"", NULL, NULL, (LPBYTE)&Line, &Length) != ERROR_SUCCESS )
		{
			bOK=FALSE;
		}
		else
		{
			char FullPath[MAX_PATH];
			char PathTemp[MAX_PATH];
			LPSTR tail;
			if ((tail = strrchr (Line, '\\')) != (LPSTR) NULL)
			{
				tail++;
				*tail = '\0';
			}
			if (Line[0] == '"') wsprintf(PathTemp,"%s",&Line[1]);
			else wsprintf(PathTemp,"%s",Line);

			wsprintf(FullPath,"%sbin\\",path);
			
			if ( strcmp(PathTemp,FullPath) != 0 ) bOK=FALSE;
			
		}

		RegCloseKey(key);
	}
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL VerifyRegistry(char *path)
{
	BOOL bOK=TRUE;
	
	if ( !Test1Registry(".sce","SCE_scilab_file") || !Test2Registry("SCE_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".sci","SCI_scilab_file") || !Test2Registry("SCI_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".tst","TST_scilab_file") || !Test2Registry("TST_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".dem","DEM_scilab_file") || !Test2Registry("DEM_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".cos","COS_scilab_file") || !Test2Registry("COS_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".cosf","COSF_scilab_file") || !Test2Registry("COSF_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".sav","SAV_scilab_file") || !Test2Registry("SAV_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".bin","BIN_scilab_file") || !Test2Registry("BIN_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".graph","GRAPH_scilab_file") || !Test2Registry("GRAPH_scilab_file",path) ) bOK=FALSE;
	if ( !Test1Registry(".graphb","GRAPHB_scilab_file") || !Test2Registry("GRAPHB_scilab_file",path) ) bOK=FALSE;

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

	//wsprintf(Line,"\"%sbin\\Wscilex.exe\" -P \"%%1\"",path);
	//if (bOK) bOK=UpdateAKey("COSF_scilab_file\\shell\\print\\command",Line);

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
void InitCheckBox(HWND hwnd,char *extension,char *KeyToCompare)
{
	char *path=GetWhereIsThisExe();

	if ( Test1Registry(extension,KeyToCompare) && Test2Registry(KeyToCompare,path) )
		SendMessage(hwnd,(UINT) BM_SETCHECK,(WPARAM) BST_CHECKED,0);
	else
		SendMessage(hwnd,(UINT) BM_SETCHECK,(WPARAM) BST_UNCHECKED,0);

	free(path);
}
/*-----------------------------------------------------------------------------------*/
typedef int (*fptr)(void);
/*-----------------------------------------------------------------------------------*/
void UpdateFromCheckBox(HWND hwnd,fptr Update,fptr Delete)
{
	LRESULT lResult;

	lResult = SendMessage(  hwnd ,
						    (UINT) BM_GETCHECK, 
							(WPARAM) 0,
							(LPARAM) 0); 

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
BOOL CALLBACK InstallDialogProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam)
{
	BOOL bOK=FALSE;

	switch (Message)
	{
		case WM_INITDIALOG:
		{
			HWND hControl[10];
			
			hControl[0]= GetDlgItem( hwnd, IDC_CHECKSCI );
			hControl[1]= GetDlgItem( hwnd, IDC_CHECKSCE );
			hControl[2]= GetDlgItem( hwnd, IDC_CHECKTST );
			hControl[3]= GetDlgItem( hwnd, IDC_CHECKDEM );
			hControl[4]= GetDlgItem( hwnd, IDC_CHECKCOS );
			hControl[5]= GetDlgItem( hwnd, IDC_CHECKCOSF );
			hControl[6]= GetDlgItem( hwnd, IDC_CHECKBIN );
			hControl[7]= GetDlgItem( hwnd, IDC_CHECKSAV );
			hControl[8]= GetDlgItem( hwnd, IDC_CHECKGRAPH );
			hControl[9]= GetDlgItem( hwnd, IDC_CHECKGRAPHB );

			InitCheckBox(hControl[0],".sci","SCI_scilab_file");
			InitCheckBox(hControl[1],".sce","SCE_scilab_file");
			InitCheckBox(hControl[2],".tst","TST_scilab_file");
			InitCheckBox(hControl[3],".dem","DEM_scilab_file");
			InitCheckBox(hControl[4],".cos","COS_scilab_file");
			InitCheckBox(hControl[5],".cosf","COSF_scilab_file");
			InitCheckBox(hControl[6],".sav","SAV_scilab_file");
			InitCheckBox(hControl[7],".bin","BIN_scilab_file");
			InitCheckBox(hControl[8],".graph","GRAPH_scilab_file");
			InitCheckBox(hControl[9],".graphb","GRAPHB_scilab_file");
			
		}
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					{
						HWND hControl[10];
											
						hControl[0]= GetDlgItem( hwnd, IDC_CHECKSCI );
						hControl[1]= GetDlgItem( hwnd, IDC_CHECKSCE );
						hControl[2]= GetDlgItem( hwnd, IDC_CHECKTST );
						hControl[3]= GetDlgItem( hwnd, IDC_CHECKDEM );
						hControl[4]= GetDlgItem( hwnd, IDC_CHECKCOS );
						hControl[5]= GetDlgItem( hwnd, IDC_CHECKCOSF );
						hControl[6]= GetDlgItem( hwnd, IDC_CHECKBIN );
						hControl[7]= GetDlgItem( hwnd, IDC_CHECKSAV );
						hControl[8]= GetDlgItem( hwnd, IDC_CHECKGRAPH );
						hControl[9]= GetDlgItem( hwnd, IDC_CHECKGRAPHB );
					
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

    					EndDialog(hwnd, IDOK);
					}
				break;
				case IDCANCEL:
    				EndDialog(hwnd, IDCANCEL);
				break;
			}
		break;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
  int iOK=0;
  
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

	/*Test1Registry(".sce","SCE_scilab_file") && Test2Registry("SCE_scilab_file",Path)
	Test1Registry(".sci","SCI_scilab_file") && Test2Registry("SCI_scilab_file",Path)
	Test1Registry(".tst","TST_scilab_file") && Test2Registry("TST_scilab_file",Path)
	Test1Registry(".dem","DEM_scilab_file") && Test2Registry("DEM_scilab_file",Path)
	Test1Registry(".cos","COS_scilab_file") && Test2Registry("COS_scilab_file",Path)
	Test1Registry(".cosf","COSF_scilab_file") && Test2Registry("COSF_scilab_file",Path)
	Test1Registry(".sav","SAV_scilab_file") && Test2Registry("SAV_scilab_file",Path)
	Test1Registry(".bin","BIN_scilab_file")&& Test2Registry("BIN_scilab_file",Path)
	Test1Registry(".graph","GRAPH_scilab_file") && Test2Registry("GRAPH_scilab_file",Path)
	Test1Registry(".graphb","GRAPHB_scilab_file") && Test2Registry("GRAPHB_scilab_file",Path)*/

	DialogBox(hInstance,(LPCSTR)(IDD_INSTALL),NULL,(DLGPROC)InstallDialogProc);
		
		}
		else
		{
			MessageBox(NULL,"Please reinstall scilab","Error",MB_ICONWARNING|MB_OK);
			iOK=-1;
		}

	  free(Path);

	  
  }
  return iOK;

	
	
}
/*-----------------------------------------------------------------------------------*/