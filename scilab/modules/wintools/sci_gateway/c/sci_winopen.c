/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "cluni0.h"
#include "gw_wintools.h"
/* Open Web Browser*/
/*-----------------------------------------------------------------------------------*/
int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l))
{
	#ifndef _MSC_VER
	#define FILENAME_MAX 4096 
	#endif
	
	char filename[FILENAME_MAX];
	int m1,n1,l1;
	int out_n;
	long int lout;
	#ifdef _MSC_VER
	HINSTANCE error=NULL;
	#endif
	CheckRhs(1,1);
  
	/*  checking variable file */
	GetRhsVar(1,"c",&m1,&n1,&l1);
	/*** first call to get the size **/
	lout=FILENAME_MAX;
	C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);

	#ifdef _MSC_VER
	error = ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
	if ( error<= (HINSTANCE)32)
	{
		Scierror(999,"Couldn't Open this document\r\n");
		return 0;
	}
	else LhsVar(1)=0;
	#else
		LhsVar(1)=0;
	#endif

	C2F(putlhsvar)();
return 0;
}
/*-----------------------------------------------------------------------------------*/
