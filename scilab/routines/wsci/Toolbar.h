/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __TOOLBARWIN__
#define __TOOLBARWIN__
/*-----------------------------------------------------------------------------------*/ 

#include <windows.h>

#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"
#include "wgnuplib.h"
#include "resource.h"
#include "../graphics/scigraphic.h"
#include "../graphics/Graphics.h"

#include "wgraph.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

void SetDefaultShowToolBar(BOOL valShowToolBar);
void HideGraphToolBar(struct BCG * ScilabGC);
void ShowGraphToolBar(struct BCG * ScilabGC);
void CreateGraphToolBar(struct BCG * ScilabGC) ;
void RefreshGraphToolBar(struct BCG * ScilabGC) ;
void ModifyEntityPickerToolbar(struct BCG * ScilabGC,BOOL Pressed);

void ShowToolBar(LPTW lptw);
void HideToolBar(LPTW lptw);
void ToolBarOnOff(LPTW lptw);
void CreateMyTooltip (HWND hwnd,char ToolTipString[30]);
int HideToolBarWin32(int WinNum);

int GetStateToolBarWin32(int WinNum);
int ToolBarWin32(int WinNum,char *onoff);

void EnableToolBar(LPTW lptw);
void DisableToolBar(LPTW lptw);
void CreateButtonToolBar(struct BCG * ScilabGC,char *Name,HMENU CallbackID,int ID_ICON,char *ToolTipString,DWORD Style);
#endif /* __TOOLBARWIN__ */
/*-----------------------------------------------------------------------------------*/ 
