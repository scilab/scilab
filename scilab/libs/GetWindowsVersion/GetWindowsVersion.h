/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __GETWINDOWSVERSION_H__
#define __GETWINDOWSVERSION_H__
/*-----------------------------------------------------------------------------------*/
#ifdef EXPORT_GETWINDOWSVERSION_DLL
#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllexport)
#else
#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllimport)
#endif

#define OS_ERROR							-1
#define OS_WIN32_WINDOWS_NT_3_51			0
#define OS_WIN32_WINDOWS_NT_4_0				1
#define OS_WIN32_WINDOWS_95					2
#define OS_WIN32_WINDOWS_98					3
#define OS_WIN32_WINDOWS_Me					4
#define OS_WIN32_WINDOWS_2000				5
#define OS_WIN32_WINDOWS_XP					6
#define OS_WIN32_WINDOWS_XP_64				7
#define OS_WIN32_WINDOWS_SERVER_2003		8
#define OS_WIN32_WINDOWS_SERVER_2003_R2		9
#define OS_WIN32_WINDOWS_SERVER_2003_64		10
#define OS_WIN32_WINDOWS_VISTA				11
#define OS_WIN32_WINDOWS_VISTA_64			12
#define OS_WIN32_WINDOWS_SERVER_2008		13
#define OS_WIN32_WINDOWS_SERVER_2008_64		14
#define OS_WIN32_WINDOWS_SEVEN				15
#define OS_WIN32_WINDOWS_SEVEN_64			16
#define OS_WIN32_WINDOWS_SEVEN_SERVER		17
#define OS_WIN32_WINDOWS_SEVEN_SERVER_64	18
#define OS_WIN32_WINDOWS_EIGHT				19
#define OS_WIN32_WINDOWS_EIGHT_64			20
#define OS_WIN32_WINDOWS_EIGHT_SERVER		21
#define OS_WIN32_WINDOWS_EIGHT_SERVER_64	22
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_GETWINDOWSVERSION_DLL int GetWindowsVersion(void);
IMPORT_EXPORT_GETWINDOWSVERSION_DLL BOOL IsWow64(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __GETWINDOWSVERSION_H__ */
