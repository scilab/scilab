/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/
extern int C2F(intgetos) _PARAMS((char *fname));
/*--------------------------------------------------------------------------*/
int C2F(sci_getos) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intgetos)(fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intgetos) _PARAMS((char *fname))
{
	static int n1,m1;
	char OperatinSystem[256];
	char Release[256];
	char *output=NULL;

#ifndef _MSC_VER
	struct utsname uname_pointer;
#endif

	Rhs=Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

#ifdef _MSC_VER


	sprintf(OperatinSystem,"%s","Windows");

	switch (GetWindowsVersion())
	{
	case OS_ERROR : default :
		sprintf(Release,"%s","Unknow");
		break;
	case OS_WIN32_WINDOWS_NT_3_51 :
		sprintf(Release,"%s","NT 3.51");
		break;
	case OS_WIN32_WINDOWS_NT_4_0 :
		sprintf(Release,"%s","NT 4.0");
		break;
	case OS_WIN32_WINDOWS_95 :
		sprintf(Release,"%s","95");
		break;
	case OS_WIN32_WINDOWS_98 :
		sprintf(Release,"%s","98");
		break;
	case OS_WIN32_WINDOWS_Me :
		sprintf(Release,"%s","ME");
		break;
	case OS_WIN32_WINDOWS_2000 :
		sprintf(Release,"%s","2000");
		break;
	case OS_WIN32_WINDOWS_XP :
		sprintf(Release,"%s","XP");
		break;
	case OS_WIN32_WINDOWS_XP_64 :
		sprintf(Release,"%s","XP x64");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003 :
		sprintf(Release,"%s","Server 2003");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003_R2 :
		sprintf(Release,"%s","Server 2003 R2");
		break;
	case OS_WIN32_WINDOWS_SERVER_2003_64 :
		sprintf(Release,"%s","Server 2003 x64");
		break;
	case OS_WIN32_WINDOWS_VISTA :
		sprintf(Release,"%s","Vista");
		break;
	case OS_WIN32_WINDOWS_VISTA_64 :
		sprintf(Release,"%s","Vista x64");
		break;
	case OS_WIN32_WINDOWS_LONGHORN :
		sprintf(Release,"%s","Longhorn");
		break;
	case OS_WIN32_WINDOWS_LONGHORN_64 :
		sprintf(Release,"%s","Longhorn x64");
		break;

	}
#else
	uname(&uname_pointer);
	sprintf(OperatinSystem,"%s",uname_pointer.sysname);
	sprintf(Release,"%s",uname_pointer.release);
#endif


	output=(char*)MALLOC((strlen(OperatinSystem)+1)*sizeof(char));
	sprintf(output,"%s",OperatinSystem);
	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}
	LhsVar(1)=Rhs+1;

	if (Lhs==2)
	{
		char *output2=NULL;
		output2=(char*)MALLOC((strlen(Release)+1)*sizeof(char));
		sprintf(output2,"%s",Release);
		n1=1;
		CreateVarFromPtr(Rhs+ 2,STRING_DATATYPE,(m1=(int)strlen(output2), &m1),&n1,&output2);
		if (output2) {FREE(output2);output2=NULL;}
		LhsVar(2)=Rhs+2;
	}

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
