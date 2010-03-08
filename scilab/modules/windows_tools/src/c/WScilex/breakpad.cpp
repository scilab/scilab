/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <exception_handler.h>
#include "breakpad.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../../libs/google_breakpad/exception_handler.lib")
/*--------------------------------------------------------------------------*/
using namespace google_breakpad;
/*--------------------------------------------------------------------------*/
static ExceptionHandler* handler = NULL;
/*--------------------------------------------------------------------------*/
bool MinidumpCallback(const wchar_t* dump_path,
					  const wchar_t* minidump_id,
					  void* context,
					  EXCEPTION_POINTERS* exinfo,
					  MDRawAssertionInfo* assertion,
					  bool succeeded)
{
	MessageBox(NULL, L"breakpad : exception detected", L"", MB_ICONWARNING);
	return succeeded;
}
/*--------------------------------------------------------------------------*/
void initializeBreakPad(void)
{
	static ExceptionHandler* handler = NULL;
	handler = new ExceptionHandler(L".",
		NULL,
		MinidumpCallback,
		NULL,
		ExceptionHandler::HANDLER_ALL,
		MiniDumpNormal,
		NULL,
		NULL);
}
/*--------------------------------------------------------------------------*/
