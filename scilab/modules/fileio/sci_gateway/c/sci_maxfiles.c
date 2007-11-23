/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "filesmanagement.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
#define MAX_FILES 100
/*--------------------------------------------------------------------------*/
static int DoOneLhs(void);
/*--------------------------------------------------------------------------*/
/**
* maxfiles sets the soft limit for the number of scilab's files allowed to open simultaneously.
* Minimum 20
* Maximum 100
* Default 20
*/
int C2F(sci_maxfiles) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		DoOneLhs();
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				int NewMaxFiles=0;
				NewMaxFiles = (unsigned long) *stk(l1);
				if (NewMaxFiles > GetMaximumFileOpenedInScilab() )
				{
					if (NewMaxFiles > MAX_FILES)
					{
						sciprint(_("Warning : %d > Maximum of files (%d).\n"),NewMaxFiles,MAX_FILES);
						DoOneLhs();
					}
					else
					{
						if ( ExtendScilabFilesList(NewMaxFiles) )
						{
							DoOneLhs();
						}
						else
						{
							Scierror(999,_("Problem to extend the limit of scilab's files opened.\n"),NewMaxFiles);
							return 0;
						}
					}
				}
				else
				{
					sciprint(_("Warning : only extend the limit for the number of scilab's files opened simultaneously.\n"));
					DoOneLhs();
				}
			}
			else
			{
				Scierror(999,_("invalid numbers of parameters.\n"));
				return 0;
			}
		}
		else
		{
			Scierror(999,_("invalid parameter type.\n"));
			return 0;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int DoOneLhs(void)
{
	static int n1,m1;
	int *paramoutINT=NULL;
	paramoutINT=(int*)MALLOC(sizeof(int));

	*paramoutINT= GetMaximumFileOpenedInScilab();

	n1=1;m1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
