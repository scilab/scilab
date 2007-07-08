/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_shell.h"
#include "machine.h"
#include "stack-c.h"
#include "history.h"
#include "inffic.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_loadhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	char  line[MAXBUF];
	char  *Path = NULL;
	int l1 = 0, m1 = 0, n1 = 0, out_n = 0;

	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0) Path=get_sci_data_strings(HISTORY_ID);
	else 
	{
		GetRhsVar(1,"c",&m1,&n1,&l1);
		Path=cstk(l1);
	}

	C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);

	read_history (line);

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
