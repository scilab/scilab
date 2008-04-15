/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#include "../../../libs/GetWindowsVersion/GetWindowsVersion.h"
#else
#include <sys/utsname.h>
#endif

#include <stdio.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_getos)(char *fname,unsigned long fname_len)
{
	static int n1,m1;
	char OperatingSystem[256];
	char Release[256];
	char *output=NULL;

#ifndef _MSC_VER
	struct utsname uname_pointer;
#endif

	Rhs=Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

#ifdef _MSC_VER


	strcpy(OperatingSystem,"Windows");

	switch (GetWindowsVersion())
	{
	case OS_ERROR : default :
		strcpy(Release,"Unknow");
		break;
	case OS_WIN32_WINDOWS_NT_3_51 :
		strcpy(Release,"NT 3.51");
		break;
	case OS_WIN32_WINDOWS_NT_4_0 :
		strcpy(Release,"NT 4.0");
		break;
	case OS_WIN32_WINDOWS_95 :
		strcpy(Release,"95");
		break;
	case OS_WIN32_WINDOWS_98 :
		strcpy(Release,"98");
		break;
	case OS_WIN32_WINDOWS_Me :
		strcpy(Release,"ME");
		break;
	case OS_WIN32_WINDOWS_2000 :
		strcpy(Release,"2000");
		break;
	case OS_WIN32_WINDOWS_XP :
		strcpy(Release,"XP");
		break;
	case OS_WIN32_WINDOWS_XP_64 :
		strcpy(Release,"XP x64");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003 :
		strcpy(Release,"Server 2003");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003_R2 :
		strcpy(Release,"Server 2003 R2");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003_64 :
		strcpy(Release,"Server 2003 x64");
		break;
	case OS_WIN32_WINDOWS_VISTA :
		strcpy(Release,"Vista");
		break;
	case OS_WIN32_WINDOWS_VISTA_64 :
		strcpy(Release,"Vista x64");
		break;
	case OS_WIN32_WINDOWS_SERVER_2008 :
		strcpy(Release,"Server 2008");
		break;
	case OS_WIN32_WINDOWS_SERVER_2008_64 :
		strcpy(Release,"Server 2008 x64");
		break;

	}
#else
	uname(&uname_pointer);
	strcpy(OperatingSystem,uname_pointer.sysname);
	strcpy(Release,uname_pointer.release);
#endif

	output = strdup(OperatingSystem);
	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}
	LhsVar(1)=Rhs+1;

	if (Lhs==2)
	{
		char *output2 = strdup(Release);
		n1=1;
		CreateVarFromPtr(Rhs+ 2,STRING_DATATYPE,(m1=(int)strlen(output2), &m1),&n1,&output2);
		if (output2) {FREE(output2);output2=NULL;}
		LhsVar(2)=Rhs+2;
	}

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
