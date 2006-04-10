/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intgetdate.h"

#include <time.h>
#include <locale.h>
#include <stdio.h>

#if WIN32
#include <sys/types.h> 
#include <sys/timeb.h>
#else
#include <sys/time.h> 
#endif
/*-----------------------------------------------------------------------------------*/
extern void C2F(convertdate)();
extern void C2F(scigetdate)();
/*-----------------------------------------------------------------------------------*/
int C2F(intgetdate) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	int i=0;
	int *DATEMATRIX=NULL;
	Rhs=Max(Rhs,0);
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	DATEMATRIX=(int *)MALLOC( (10)*sizeof(int) );
	for (i=0;i<10;i++) DATEMATRIX[i]=0;

	if (Rhs == 0)
	{
		time_t dt;
		int ierr=0;
		C2F(scigetdate)(&dt,&ierr);
		if (ierr)
		{
			Scierror(999,"Impossible to get the date...");
			return 0;
		}
		else
		{
			C2F(convertdate)(&dt,DATEMATRIX);
			m1=1;
			n1=10;
			CreateVarFromPtr(Rhs+1, "i", &m1, &n1 ,&DATEMATRIX);
		}
		
	}
	else /* Rhs == 1 */
	{
		if (GetType(1) == sci_strings)
		{
			char *Param1=NULL;
      GetRhsVar(1,"c",&m1,&n1,&l1);
			Param1=cstk(l1);

			if (strcmp("s",Param1)==0)
			{
				time_t dt;
				int ierr=0;
				C2F(scigetdate)(&dt,&ierr);
				DATEMATRIX[0]=dt;
				m1=1;
				n1=1;
				CreateVarFromPtr(Rhs+1, "i", &m1, &n1 ,&DATEMATRIX);
			}
			else
			{
				Scierror(999,"Parameter must be a \"s\" or a integer.");
				return 0;
			}
		}
		else
		{
			if ( GetType(1) == sci_matrix )
			{
				GetRhsVar(1,"i",&m1,&n1,&l1);
				if ( (m1 == 1) && (n1 == 1) )
				{
					int x=0;
					x=*istk(l1);
					C2F(convertdate)(&x,DATEMATRIX);
					m1=1;
					n1=10;
					CreateVarFromPtr(Rhs+1, "i", &m1, &n1 ,&DATEMATRIX);
				}
				else
				{
					Scierror(999,"Parameter must be a integer.");
					return 0;
				}
			}
			else
			{
				Scierror(999,"Parameter must be a \"s\" or a integer.");
				return 0;
			}
		}
	}

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	if (DATEMATRIX) {FREE(DATEMATRIX);DATEMATRIX=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
