/* Allan CORNET INRIA 2004 */
#ifndef __CLIPBOARD__

#define __CLIPBOARD__

#include <Windows.h>
#include "wcommon.h"

extern char ScilexWindowName[MAX_PATH];

void CreateThreadPaste(char *Text);
BOOL IsReadyForAnewLign(void);
void SetReadyOrNotForAnewLign(BOOL Ready);
DWORD WINAPI SendInputText(LPVOID lpParam );
void PasteFunction(LPTW lptw,BOOL special);
BOOL IsEmptyClipboard(LPTW lptw);
void TextCopyClip(LPTW lptw);
void CleanClipboard(LPTW lptw);
HANDLE GetHandleThreadPaste(void);
BOOL GetThreadPasteRunning(void);
void SetThreadPasteRunning(BOOL Running);
#endif /*__CLIPBOARD__*/