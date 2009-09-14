/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "stackinfo.h"
#include "getmaxMALLOC.h"
#include "scimem.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(adjustgstacksize)();
/*--------------------------------------------------------------------------*/
#define MIN_GSTACKSIZE 11000
/*--------------------------------------------------------------------------*/
/* gstacksize - set scilab global stack size

* Calling Sequence

* gstacksize(n)
* gstacksize('max')
* gstacksize('min')
* sz=gstacksize()

* Parameters

* n : integer, the required stack size given in number of double precision words
* 'max' : try to allocate the maximum of memory
* 'max' : allocate the minimum of memory
* sz : 2-vector [total used]
*/
/*--------------------------------------------------------------------------*/
int C2F(sci_gstacksize)(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;

	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		int *paramoutINT=NULL;
		int total=0;
		int used=0;
		paramoutINT=(int*)MALLOC(sizeof(int)*2);

		C2F(getgstackinfo)(&total,&used);
		paramoutINT[0]=total;
		paramoutINT[1]=used;

		n1=1;m1=2;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);

		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();

		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

			if ( (m1 == 1) && (n1 == 1) )
			{
				unsigned long MEMGSTACKSIZE = (unsigned long) *stk(l1);
				/* add 1 for alignment problems */
				if ( is_a_valid_size_for_scilab_stack(MEMGSTACKSIZE + 1) )
				{
					if ( (MEMGSTACKSIZE>=MIN_GSTACKSIZE) && ((unsigned long)MEMGSTACKSIZE<=get_max_memory_for_scilab_stack()) )
					{
						int currentgstacksize=0;
						int used=0;

						C2F(getgstackinfo)(&currentgstacksize,&used);

						if (MEMGSTACKSIZE != (unsigned long)currentgstacksize)
						{
							int ptr=0;
							int l=0;

							C2F(scigmem)((int *)&MEMGSTACKSIZE,&ptr);
							l = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1];

							if (ptr) 
							{
								LhsVar(1) = 0;
								C2F(putlhsvar)();

								C2F(adjustgstacksize)(&MEMGSTACKSIZE,&ptr,&l);
								return 0;
							}
							else
							{
								Scierror(999,_("%s: Cannot alloc more memory.\nTry gstacksize('max').\n"),fname);
								return 0;
							}
						}
						else
						{
							/* same gstacksize nothing to do */
						}

						LhsVar(1) = 0;
						C2F(putlhsvar)();
						return 0;
					}
					else
					{
						Scierror(1504,_("%s: Out of bounds value. Not in [%lu,%lu].\n"),fname,MIN_GSTACKSIZE,get_max_memory_for_scilab_stack()-1);
						return 0;
					}
				}
			}
			else
			{
				Scierror(204,_("%s: Wrong type for input argument #%d: Scalar, '%s' or '%s' expected.\n"),fname,1,"min","max");
				return 0;
			}
		}
		else
		{
			if ( GetType(1) == sci_strings )
			{
				char *param=NULL;

				GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
				param=cstk(l1);

				if ( strcmp("max",param) == 0 )
				{
					int ptr=0;
					int memgstacktotal=0;
					int memgstackused=0;

					int memstacktotal=0;
					int memstackused=0;

					int newmaxgstack=0;
					int l;

					unsigned long memmaxavailablebyscilab=get_max_memory_for_scilab_stack();
					unsigned long maxmemfree=(GetLargestFreeMemoryRegion())/sizeof(double);

					C2F(getgstackinfo)(&memgstacktotal,&memgstackused);
					C2F(getstackinfo)(&memstacktotal,&memstackused);

					newmaxgstack = maxmemfree-memgstackused-memstackused;

					if ( newmaxgstack > (int) memmaxavailablebyscilab )
					{
						newmaxgstack = memmaxavailablebyscilab;
					}
					if (newmaxgstack < MIN_GSTACKSIZE)
					{
						newmaxgstack = MIN_GSTACKSIZE;
					}

					l = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1];

					C2F(scigmem)(&newmaxgstack,&ptr);
					if (ptr)
					{
						LhsVar(1) = 0;
						C2F(putlhsvar)();

						C2F(adjustgstacksize)(&newmaxgstack,&ptr,&l);
						return 0;
					}

					LhsVar(1) = 0;
					C2F(putlhsvar)();
					return 0;
				}
				else if ( strcmp("min",param) == 0 )
				{
					int ptr=0;
					int memgstacktotal=0;
					int memgstackused=0;
					int newmingstack=0;
					int l;

					C2F(getgstackinfo)(&memgstacktotal,&memgstackused);

					if (memgstackused<MIN_GSTACKSIZE)
					{
						newmingstack = MIN_GSTACKSIZE;
					}
					else
					{
						/* Add 3000 security for the stack */
						newmingstack = memgstackused+3000;
					}

					l = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1];

					C2F(scigmem)(&newmingstack,&ptr);
					if (ptr)
					{
						LhsVar(1) = 0;
						C2F(putlhsvar)();

						C2F(adjustgstacksize)(&newmingstack,&ptr,&l);
						return 0;
					}
					LhsVar(1) = 0;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					Scierror(204,_("%s: Wrong type for input argument #%d: Scalar, '%s' or '%s' expected.\n"),fname,1, "min","max");
					return 0;
				}

			}
			else
			{
				Scierror(204,_("%s: Wrong type for argument #%d: Scalar, '%s' or '%s' expected.\n"),fname,1,"min","max");
				return 0;
			}
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
