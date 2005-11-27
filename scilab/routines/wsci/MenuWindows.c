/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MenuWindows.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */

/*-----------------------------------------------------------------------------------*/
#define MENUNAME "wscilabE.mnu"
#define GRAPHMENUNAME "wgscilabE.mnu"
/*-----------------------------------------------------------------------------------*/
MW menuwin;
/*-----------------------------------------------------------------------------------*/
static LPSTR szMenuName;
static LPSTR szGraphMenuName;
/*-----------------------------------------------------------------------------------*/
LPMW GetMenuWinScilab(void)
{
	return &menuwin;
}
/*-----------------------------------------------------------------------------------*/
MW InitMWStruct(void)
{
	MW MWStruct;

	MWStruct.szMenuName=NULL;
	MWStruct.hMenu=NULL;
	MWStruct.macro=NULL;
	MWStruct.macrobuf=NULL;
	MWStruct.nCountMenu=0;
	MWStruct.lpProcInput=NULL;
	MWStruct.szPrompt=NULL;
	MWStruct.szAnswer=NULL;
	MWStruct.nChar=0;
	MWStruct.nButton=0;
	MWStruct.hButton[BUTTONMAX-1]=NULL;
	MWStruct.hButtonID[BUTTONMAX-1];
	MWStruct.lpfnMenuButtonProc=NULL;
	MWStruct.lpfnButtonProc[BUTTONMAX-1]=NULL;
	MWStruct.IsAIcon[BUTTONMAX-1]=FALSE;
	MWStruct.PositionX[BUTTONMAX-1]=0;
	MWStruct.ShowToolBar=FALSE;
	MWStruct.LockToolBar=FALSE;
	MWStruct.CodeLanguage=0;
	
	return MWStruct;
}
/*-----------------------------------------------------------------------------------*/
LPSTR GetszMenuName(void)
{
	return szMenuName;
}
/*-----------------------------------------------------------------------------------*/
LPSTR GetszGraphMenuName(void)
{
	return szGraphMenuName;
}
/*-----------------------------------------------------------------------------------*/
void InitszMenuName(char *ScilabDirectory)
{
	szMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (MENUNAME) + 10);
	wsprintf(szMenuName,"%s\\bin\\%s",ScilabDirectory,MENUNAME);
}
/*-----------------------------------------------------------------------------------*/
void InitszGraphMenuName(char *ScilabDirectory)
{
	szGraphMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (GRAPHMENUNAME) + 10);
	wsprintf(szGraphMenuName,"%s\\bin\\%s",ScilabDirectory, GRAPHMENUNAME);
}
/*-----------------------------------------------------------------------------------*/
