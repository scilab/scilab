/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "gw_shell.h"
#include "stack-c.h"
#include "HistoryManager_c.h"
#include "MALLOC.h"
#include "cluni0.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_savehistory) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		char *filename = getFilenameScilabHistory();
		if (filename) 
		{
			writeScilabHistoryToFile(filename);
			FREE(filename);
			filename=NULL;
		}
	}
	else
	{
		#define MAXBUF	1024
		char  line[MAXBUF];
		char *Path;
		int l1, m1, n1, out_n;

		if ( GetType(1) == sci_strings )
		{
			GetRhsVar(1,"c",&m1,&n1,&l1);
			Path=cstk(l1);

			C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
			writeScilabHistoryToFile(line);
		}
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
