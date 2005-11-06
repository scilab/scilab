/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TextWindows.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
TW textwin;
/*-----------------------------------------------------------------------------------*/
LPTW GetTextWinScilab(void)
{
	return &textwin;
}
/*-----------------------------------------------------------------------------------*/
TW InitTWStruct(void)
{
	TW TWStruct;

	TWStruct.lpr=NULL;
	TWStruct.hInstance=NULL;
	TWStruct.hPrevInstance=NULL;
	TWStruct.Title=NULL;
	TWStruct.lpmw=NULL;

	TWStruct.ScreenSize.x=0;
	TWStruct.ScreenSize.y=0;

	TWStruct.KeyBufSize=0;
	TWStruct.nCmdShow=0;
	TWStruct.hIcon=NULL;
	TWStruct.hPopMenu=NULL;
	TWStruct.hWndText=NULL;
	TWStruct.hWndParent=NULL;

	TWStruct.Origin.x=0;
	TWStruct.Origin.y=0;

	TWStruct.Size.x=0;
	TWStruct.Size.y=0;

	TWStruct.ScreenBuffer=NULL;
	TWStruct.AttrBuffer=NULL;
	TWStruct.KeyBuf=NULL;
	TWStruct.KeyBufIn=NULL;
	TWStruct.KeyBufOut=NULL;
	TWStruct.Attr=0;
	TWStruct.bFocus=FALSE;
	TWStruct.bGetCh=FALSE;
	TWStruct.bSysColors=FALSE;
	TWStruct.hbrBackground=NULL;
	wsprintf(TWStruct.fontname,"Lucida Console");
	TWStruct.fontsize=11;
	TWStruct.hfont=NULL;
	TWStruct.CharAscent=0;
	TWStruct.ButtonHeight=0;
	TWStruct.CaretHeight=0;
	TWStruct.CursorFlag=0;

	TWStruct.CursorPos.x=0;
	TWStruct.CursorPos.y=0;

	TWStruct.ClientSize.x=0;
	TWStruct.ClientSize.y=0;

	TWStruct.CharSize.x=0;
	TWStruct.CharSize.y=0;

	TWStruct.ScrollPos.x=0;
	TWStruct.ScrollPos.y=0;

	TWStruct.ScrollMax.x=0;
	TWStruct.ScrollMax.y=0;

	TWStruct.MarkBegin.x=0;
	TWStruct.MarkBegin.y=0;

	TWStruct.MarkEnd.x=0;
	TWStruct.MarkEnd.y=0;

	TWStruct.Marking=FALSE;

	return TWStruct;
}
/*-----------------------------------------------------------------------------------*/

