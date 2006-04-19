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

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
extern void C2F(convertdate)();
extern void C2F(scigetdate)();
extern int *InversionMatrixInt(int W,int L,int *Matrix);
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
				DATEMATRIX[0]=(int)dt;
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
				int i=0;
				int k=0;
				int l=0;
				double *param=NULL;
			
				int *DATEARRAY=NULL;
				int *DATEARRAYtmp=NULL;

				GetRhsVar(1,"d",&m1,&n1,&l1);
				param=stk(l1);

				l=10*m1*n1;
				DATEARRAY=(int *)MALLOC( (l)*sizeof(int) );
				for (k=0;k<l;k++) DATEARRAY[k]=0;
			
				for(i=0;i<m1*n1;i++)
				{
					int j=0;
					int paramtemp=(int)param[i];
					double millisecondes=param[i]-paramtemp;
					C2F(convertdate)(&paramtemp,DATEMATRIX);
					for (j=0;j<10;j++)
					{
						DATEARRAY[(i*10)+j]=DATEMATRIX[j];
					}
					if (millisecondes>0)
					{
						if (millisecondes>0.999) millisecondes=0.999;
						DATEARRAY[(i*10)+9]=(int)(millisecondes*1000);
					}
				}

				m1=l/10;
				n1=10;
				DATEARRAYtmp=DATEARRAY;
				DATEARRAY=InversionMatrixInt(n1,m1,DATEARRAY);
				CreateVarFromPtr(Rhs+1, "i", &m1, &n1 ,&DATEARRAY);

				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();
				if (DATEMATRIX) {FREE(DATEMATRIX);DATEMATRIX=NULL;}
				if (DATEARRAY) {FREE(DATEARRAY);DATEARRAY=NULL;}
				if (DATEARRAYtmp) {FREE(DATEARRAYtmp);DATEARRAYtmp=NULL;}
				return 0;

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
