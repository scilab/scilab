/* Scilab wgnuplib.c */
/* from GNUPLOT - win/wgnuplib.c */
/*
 * Copyright (C) 1992   Russell Lang
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 *
 * AUTHORS
 *   Russell Lang
 * 
 * Modified for Scilab 
 *   Jean-Philippe Chancelier 
 *
 */
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
  #include <windowsx.h>*/
#include <ctype.h>
/*#include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"

HINSTANCE hdllInstance;
LPSTR szParentClass = "wscilab_parent";
LPSTR szTextClass = "wscilab_text";
LPSTR szPauseClass = "wscilab_pause";
LPSTR szGraphClass = "wscilab_graph";
LPSTR szParentGraphClass = "wscilab_parent_graph";

#ifdef __DLL__
int WINAPI 
DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
  hdllInstance = hInstance;
  return 1;
}

EXPORT int WINAPI 
WEP (int nParam)
{
  return 1;
}
#else
/** used in winmain **/
void 
NoDll (HINSTANCE hInstance)
{
  hdllInstance = hInstance;
}
#endif /* __DLL__ */

/********************************************
 * checks compatibility between executable 
 * and dll
 ********************************************/

EXPORT BOOL CheckWScilabVersion (LPSTR str)
{
  char mess[256];
  LPSTR version;
  version = WSCILABVERSION;
  if (lstrcmp (str, version))
    {
      wsprintf (mess, "Incorrect DLL version\nExpected version   %s\nThis is version   %s", str, version);
      MessageBox (NULL, mess, "WSCILAB.DLL", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
      return TRUE;
    }
  return FALSE;			/* Correct version */
}

/********************************************
 * checks compatibility between executable 
 * and dll
 ********************************************/

void *
LocalAllocPtr (UINT flags, UINT size)
{
  HLOCAL hlocal;
  hlocal = LocalAlloc (flags, size + 1);
  return (char *) LocalLock (hlocal);
}

void 
LocalFreePtr (void *ptr)
{
  HLOCAL hlocal;
  hlocal = LocalHandle (ptr);
  LocalUnlock (hlocal);
  LocalFree (hlocal);
  return;
}


/********************************************
 * ascii to int convertion 
 * Used to read the *.ini files 
 * returns:
 *  A pointer to character past int if successful,
 *  otherwise NULL on failure.
 *  convert int is stored at pval.
 ********************************************/

LPSTR 
GetLInt (LPSTR str, long int *pval)
{
  int loc;
  LPSTR loc1;
  loc1 = GetInt (str, &loc);
  *pval = (long int) loc;
  return (loc1);
}

LPSTR 
GetInt (LPSTR str, LPINT pval)
{
  int val = 0;
  BOOL negative = FALSE;
  BOOL success = FALSE;
  int ch;
  if (!str)
    return NULL;
  while ((ch = *str) != 0 && isspace (ch))
    str++;
  if (ch == '-')
    {
      negative = TRUE;
      str++;
    }
  while ((ch = *str) != 0 && isdigit (ch))
    {
      success = TRUE;
      val = val * 10 + (ch - '0');
      str++;
    }
  if (success)
    {
      if (negative)
	val = -val;
      *pval = val;
      return str;
    }
  return NULL;
}
