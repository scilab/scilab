/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __WINDOWS_MAIN_H__
#define __WINDOWS_MAIN_H__

#include <Windows.h>

/**
* Main for scilab with GUI
* @param [in] current instance of the application.
* @param [in] previous instance of the application
* @param [in] command line for the application
* @param [in] Specifies how the window is to be shown
* @return 0
*/
int WINAPI Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow);

#endif /* __WINDOWS_MAIN_H__ */
/*--------------------------------------------------------------------------*/ 
