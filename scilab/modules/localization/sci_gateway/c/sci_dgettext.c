/**
 * INRIA 2007
 * @author Sylvestre LEDRU
 */
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_dgettext) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
		{
			if ( (GetType(2) == sci_strings) )
				{
					static int l1,n1,m1;
					static int l2,n2,m2;

					char *domainname=NULL;
					char *msgid=NULL;
					char *TranslatedString=NULL;

					GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
					domainname=cstk(l1);
					
					GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
					msgid=cstk(l2);
					/* We always have something from this functions because dgettext
					 * is returning the same string if it cannot find it */
					TranslatedString=dgettext(domainname, msgid);

					n1=1;
					CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString), &m1),&n1,&TranslatedString);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
			else
				{
					Scierror(999,"%s: Wrong type for second input argument: String expected.\n",fname);
					return 0;
				}
		} else
			{
				Scierror(999,"%s: Wrong type for first input argument: String expected.\n",fname);
				return 0;
			}
		
}
/*--------------------------------------------------------------------------*/
