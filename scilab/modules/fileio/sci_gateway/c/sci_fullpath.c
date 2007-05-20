/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef _MSC_VER
#include <sys/param.h>
#endif
#include <stdlib.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
#ifndef _MAX_PATH
#define _MAX_PATH   260
#endif
#ifndef _MSC_VER
#define _fullpath(a,r,l)        realpath(r,a)
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_fullpath) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs=Max(Rhs,0);
	CheckRhs(1,1) ;
	CheckLhs(0,1) ;

	if (GetType(1) == sci_strings)
	{
		static int l1,n1,m1;
		char *relPath=NULL;
		char fullpath[_MAX_PATH*4];
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		relPath=cstk(l1);

		if( _fullpath( fullpath, relPath, _MAX_PATH*4 ) != NULL )
		{
			char *Output=NULL;
			Output=(char*)MALLOC((strlen(fullpath)+1)*sizeof(char));
			strcpy(Output,fullpath);

			n1=1;
			CreateVarFromPtr( Rhs+1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
			LhsVar(1) = Rhs+1;
			C2F(putlhsvar)();	

			if (Output) {FREE(Output);Output=NULL;}
		}
		else
		{
			Scierror(999,"Invalid path.\n");
		}
	}
	else
	{
		Scierror(999,"Invalid parameter , it must be a path (string).\n");
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/

