/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#endif

#include "machine.h"
#include "stack-c.h"
#include "core_math.h"
#include "matboolean.h"
/*--------------------------------------------------------------------------*/
#define ISTK ((integer *)&C2F(stack))
/*--------------------------------------------------------------------------*/
int C2F(matlog)(void)
{
    static integer ou = 57;
    static integer non = 61;

    static integer j=0;
    static double e1, e2;

    static integer i1, i2, l1, l2, m2, n2, m1, n1, op, lw, il1, il2, mn2, it1, it2, mn1, top0;

    top0 = Top;
    op = Fin;
    lw = C2F(vstk).lstk[Top] + 1;

    if (Rhs == 2)
    {
		il2 = C2F(vstk).lstk[Top - 1] + C2F(vstk).lstk[Top - 1] - 1;
		if (ISTK[il2 - 1] < 0) 
		{
	    	il2 = ISTK[il2] + ISTK[il2] - 1;
		}
		m2 = ISTK[il2];
		n2 = ISTK[il2 + 1];
		it2 = ISTK[il2 + 2];
		l2 = (il2 + 4) / 2 + 1;
		mn2 = m2 * n2;
		--Top;
    }

    il1 = C2F(vstk).lstk[Top - 1] + C2F(vstk).lstk[Top - 1] - 1;
    if (ISTK[il1 - 1] < 0) 
   	{
		il1 = ISTK[il1] + ISTK[il1] - 1;
    }
    m1 = ISTK[il1];
    n1 = ISTK[il1 + 1];
    it1 = ISTK[il1 + 2];

    l1 = (il1 + 4) / 2 + 1;
    mn1 = m1 * n1;
    if (Fin == non) 
   	{
		if (mn1 == 0) 
		{
			ISTK[il1 - 1] = 1;
			ISTK[il1] = 0;
			ISTK[il1 + 1] = 0;
			ISTK[il1 + 2] = 0;

			C2F(vstk).lstk[Top] = (il1 + 4) / 2 + 1;
			return 0;
		}
		else
		{
			ISTK[il1 - 1] = 4;
			for (j = 0; j <= mn1 - 1; ++j) 
			{
				e1 = C2F(stack).Stk[l1 + j - 1];
				if (e1 == 0.) 
				{
		    		ISTK[il1 + 3 + j - 1] = 1;
				}
				else
				{
		    		ISTK[il1 + 3 + j - 1] = 0;
				}
			}
			C2F(vstk).lstk[Top] = (il1 + 3 + mn1) / 2 + 1;
		}
	}
	else
	{
		if (mn1 == 0 || mn2 == 0) 
		{
			ISTK[il1 - 1] = 1;
			ISTK[il1] = 0;
			ISTK[il1 + 1] = 0;
			ISTK[il1 + 2] = 0;

			C2F(vstk).lstk[Top] = (il1 + 4) / 2 + 1;
			return 0;
		}
		if (mn1 == 1) 
		{
			i1 = 0;
			mn1 = mn2;
		}
		else 
		{
			i1 = 1;
		}
		if (mn2 == 1) 
		{
			i2 = 0;
			mn2 = mn1;
		}
		else 
		{
		    i2 = 1;
		}
		if (mn1 != mn2) 
		{
			static integer code_error = 60;
			Error(code_error);
			return 0;
		}
		if (Fin == ou) 
		{
	        for (j = 0; j <= mn1 - 1; ++j) 
			{
				e1 = C2F(stack).Stk[l1 + j * i1 - 1];
				e2 = C2F(stack).Stk[l2 + j * i2 - 1];
				if (e1 != 0. || e2 != 0.) 
				{
		    		ISTK[il1 + 3 + j - 1] = 1;
				}
				else 
				{
		    		ISTK[il1 + 3 + j - 1] = 0;
				}
			}
		}
		else
		{
			for (j = 0; j <= mn1 - 1; ++j) 
			{
				e1 = C2F(stack).Stk[l1 + j * i1 - 1];
				e2 = C2F(stack).Stk[l2 + j * i2 - 1];
				if (e1 != 0. && e2 != 0.) 
				{
					ISTK[il1 + 3 + j - 1] = 1;
				}
				else
				{
		    		ISTK[il1 + 3 + j - 1] = 0;
				}
			}
		}
		ISTK[il1 - 1] = 4;
		ISTK[il1] = Max(m1,m2);
		ISTK[il1 + 1] = Max(n1,n2);

		C2F(vstk).lstk[Top] = (il1 + 3 + mn1) / 2 + 1;
		return 0;
	}
return 0;
}
#undef ISTK
/*--------------------------------------------------------------------------*/


