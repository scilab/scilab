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

#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
#include "core_math.h"
#include "matboolean.h"
#include "BOOL.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(matlog)(void)
{
	static int or = 57;
	static int non = 61;

	static int j= 0;
	static double e1, e2;

	static int i1, i2, l1, l2, m2, n2, m1, n1, op, lw, il1, il2, mn2, it1, it2, mn1, top0;

	top0 = Top;
	op = Fin;
	lw = C2F(vstk).lstk[Top] + 1;

	if (Rhs == 2)
	{
		il2 = C2F(vstk).lstk[Top - 1] + C2F(vstk).lstk[Top - 1] - 1;
		if (*istk(il2) < 0) 
		{
			il2 = iadr(*istk(il2+1));
		}
		m2 = *istk(il2 + 1);
		n2 = *istk(il2 + 2);
		it2 = *istk(il2 + 3);
		l2 = (il2 + 4) / 2 + 1;
		mn2 = m2 * n2;
		--Top;
	}

	il1 = C2F(vstk).lstk[Top - 1] + C2F(vstk).lstk[Top - 1] - 1;
	if (*istk(il1) < 0) 
	{
		il1 = iadr(*istk(il1+1));
	}
	m1 = *istk(il1 + 1);
	n1 = *istk(il1 + 2);
	it1 = *istk(il1 + 3);

	l1 = (il1 + 4) / 2 + 1;
	mn1 = m1 * n1;
	if (Fin == non) 
	{
		if (mn1 == 0) 
		{
			*istk(il1) = 1;
			*istk(il1 + 1) = 0;
			*istk(il1 + 2) = 0;
			*istk(il1 + 3) = 0;

			C2F(vstk).lstk[Top] = (il1 + 4) / 2 + 1;
			return 0;
		}
		else
		{
			*istk(il1) = 4;
			for (j = 0; j <= mn1 - 1; ++j) 
			{
				e1 = *stk(l1 + j);
				if (e1 == 0.) 
				{
					*istk(il1 + 3 + j) = 1;
				}
				else
				{
					*istk(il1 + 3 + j) = 0;
				}
			}
			C2F(vstk).lstk[Top] = (il1 + 3 + mn1) / 2 + 1;
		}
	}
	else
	{
		BOOL *matBool = NULL;
		if (mn1 == 0 || mn2 == 0) 
		{
			*istk(il1) = 1;
			*istk(il1 + 1) = 0;
			*istk(il1 + 2) = 0;
			*istk(il1 + 3) = 0;

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
			static int code_error = 60;
			Error(code_error);
			return 0;
		}

		matBool = (BOOL*)MALLOC(sizeof(BOOL) * mn1);
		if (matBool == NULL)
		{
			if (Fin == or) 
			{
				Scierror(999,_("%s: No more memory.\n"),"boolean operation '|'");
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),"boolean operation '&'");
			}
			return 0;
		}

		if (Fin == or) 
		{
			for (j = 0; j <= mn1 - 1; ++j) 
			{
				int j_m_i1 = (int)(j * i1);
				int j_m_i2 = (int)(j * i2);

				int e1tmp = (int)*stk(l1 + j_m_i1);
				int e2tmp = (int)*stk(l2 + j_m_i2);

				if (e1tmp || e2tmp) 
				{
					matBool[j] = TRUE;
				}
				else 
				{
					matBool[j] = FALSE;
				}
			}
		}
		else
		{
			for (j = 0; j <= mn1 - 1; ++j) 
			{
				int j_m_i1 = (int)(j * i1);
				int j_m_i2 = (int)(j * i2);

				int e1tmp = (int)*stk(l1 + j_m_i1);
				int e2tmp = (int)*stk(l2 + j_m_i2);

				if (e1tmp && e2tmp) 
				{
					matBool[j] = TRUE;
				}
				else
				{
					matBool[j] = FALSE;
				}
			}
		}

		for (j = 0; j <= mn1 - 1; ++j) 
		{
			*istk(il1 + 3 + j) = (int)matBool[j];
		}

		FREE(matBool);
		matBool = NULL;

		*istk(il1) = 4;
		*istk(il1 + 1) = Max(m1,m2);
		*istk(il1 + 2) = Max(n1,n2);

		C2F(vstk).lstk[Top] = (il1 + 3 + mn1) / 2 + 1;
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/


