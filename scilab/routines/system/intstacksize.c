/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intstacksize.h"
#ifdef _MSC_VER
	#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
	#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
extern integer C2F(scimem)(integer *n, integer *ptr);
extern integer C2F(adjuststacksize)();
extern integer C2F(getstackinfo)();
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
#define MIN_STACKSIZE 180000
#define KERNEL_MEMORY_ZONE  134217728  /* 2^27 0x80000000 */
/*-----------------------------------------------------------------------------------*/
int C2F(intstacksize) __PARAMS((char *fname,unsigned long fname_len))
/**
/* stacksize - set scilab stack size 

* Calling Sequence 

* stacksize(n)   
* stacksize('max')   
* sz=stacksize()   

* Parameters 

* n : integer, the required stack size given in number of double precision words 
* 'max' : try to allocate the maximum of memory
* sz : 2-vector [total used] 

*/
{
	static int l1,n1,m1;
	integer ptr;
	int *paramoutINT=NULL;

	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		int total=0;
		int used=0;
		paramoutINT=(int*)MALLOC(sizeof(int)*2);

		C2F(getstackinfo)(&total,&used);
		paramoutINT[0]=total;
		paramoutINT[1]=used;

		n1=1;m1=2;
		CreateVarFromPtr(1, "i", &n1, &m1, &paramoutINT);

		LhsVar(1) = 1;
		C2F(putlhsvar)();	

		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			GetRhsVar(1,"i",&m1,&n1,&l1);

			if ( (m1 == 1) && (n1 == 1) )
			{
				int MEMSTACKSIZE=*istk(l1);

				if ( (MEMSTACKSIZE>=MIN_STACKSIZE) && (MEMSTACKSIZE<=KERNEL_MEMORY_ZONE) )
				{
					int currentstacksize=0;
					int used=0;

					C2F(getstackinfo)(&currentstacksize,&used);

					if (MEMSTACKSIZE != currentstacksize)
					{
						C2F(scimem)(&MEMSTACKSIZE,&ptr);
						C2F(adjuststacksize)(&MEMSTACKSIZE,&ptr);
					}
					else
					{
						/* same stacksize nothing to do */
					}
				}
				else
				{
					Scierror(999,"%s: %d not in [%d,%d].",fname,MEMSTACKSIZE,MIN_STACKSIZE,KERNEL_MEMORY_ZONE);
					return 0;
				}
			}
			else
			{
				Scierror(204,"%s: Argument 1: wrong type argument expecting a scalar or 'max'.",fname);
				return 0;
			}
		}
		else
		if ( GetType(1) == sci_strings )
		{
			char *param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param=cstk(l1);

			if ( strcmp("max",param) == 0 )
			{
				int currentstacksize=0;
				int used=0;

				/* max allocation function to do */

				C2F(getstackinfo)(&currentstacksize,&used);
				sciprint("%s('max') fixed to %d.\n",fname,currentstacksize);
			}
			else
			{
				Scierror(204,"%s: Argument 1: wrong type argument expecting a scalar or 'max'.",fname);
				return 0;
			}
		}
		else
		{
			Scierror(204,"%s: Argument 1: wrong type argument expecting a scalar or 'max'.",fname);
			return 0;
		}
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
