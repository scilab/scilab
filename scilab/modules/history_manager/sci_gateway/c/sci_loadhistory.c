/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_history_manager.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "MALLOC.h"
#include "cluni0.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_loadhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0) 
	{
		char *filename = getFilenameScilabHistory();
		if (filename) 
		{
			loadScilabHistoryFromFile(filename);
			FREE(filename);
			filename=NULL;
		}
	}
	else 
	{
		#define MAXBUF	1024
		char line[MAXBUF];
		int l1 = 0, m1 = 0, n1 = 0, out_n = 0;
		char *Path = NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		Path=cstk(l1);
		C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
		loadScilabHistoryFromFile(line);
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
