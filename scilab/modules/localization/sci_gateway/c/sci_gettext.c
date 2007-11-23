/**
 * INRIA 2007
 * @author Allan CORNET - initial
 * @author Sylvestre Ledru <sylvestre.ledru@inria.fr> - Rewrite using gettext
 */
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_gettext) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
		{
			static int l1,n1,m1;

			char *param1=NULL;
			char *TranslatedString=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			param1=cstk(l1);

			/* We always have something from this functions because gettext
			 * is returning the same string if it cannot find it */
			TranslatedString=gettext(param1);

			n1=1;
			CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString), &m1),&n1,&TranslatedString);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
			return 0;
		}
	else
		{
			Scierror(999,"invalid first parameter. See help gettext.\n");
			return 0;
		}
}
/*--------------------------------------------------------------------------*/
