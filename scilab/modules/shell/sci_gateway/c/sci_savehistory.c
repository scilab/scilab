/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "history.h"
#include "gw_core.h"
#include "gw_shell.h"
#include "stack-c.h"
#include "inffic.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_savehistory) _PARAMS((char *fname,unsigned long fname_len))
{
	char  line[MAXBUF];
	char *Path;
	int l1, m1, n1, out_n;

	Rhs=Max(Rhs,0);
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		Path=get_sci_data_strings(HISTORY_ID);
		C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
		write_history (line);
	}
	else
	{
		if ( GetType(1) == 1 ) 
		{
			GetRhsVar(1,"i",&m1,&n1,&l1);
			savehistoryafterncommands(*istk(l1));
		}
		else if ( GetType(1) == 10 )
		{
			GetRhsVar(1,"c",&m1,&n1,&l1);
			Path=cstk(l1);

			C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
			write_history (line);
		}
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
