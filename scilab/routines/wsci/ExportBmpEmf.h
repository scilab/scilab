/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __EXPORTBMPEMF__
#define __EXPORTBMPEMF__
/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include "wgnuplib.h"
#include "resource.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "../version.h"
#include "../os_specific/win_mem_alloc.h"
#include "wresource.h"
#include "../graphics/bcg.h"
/*-----------------------------------------------------------------------------------*/
BOOL HdcToBmpFile(HDC hdc, char *pszflname,POINT DeviceSize);
BOOL HwndToBmpFile(HWND hwnd, char *pszflname);
void ExportBMP(struct BCG *ScilabGC,char *pszflname);
void ExportEMF(struct BCG *ScilabGC,char *pszflname);
/*-----------------------------------------------------------------------------------*/
#endif /* __EXPORTBMPEMF__ */
