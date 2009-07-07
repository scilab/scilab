
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "double.h"

/*--------------------------------------------------------------------------*/
static int colon = 44;
static int quote = 53;
static int less = 59;
static int equal = 50;
static int ou = 57;
static int et = 58;
static int non = 61;
static int star = 47;
static int dstar = 62;
static int slash = 48;
static int bslash = 49;
static int dot = 51;
/*--------------------------------------------------------------------------*/
static int ChooseOtherOperation(int op);
int C2F(matops)(void);
/*--------------------------------------------------------------------------*/
extern int C2F(matchsgn)();
extern int C2F(matsubst)();
extern int C2F(matcc)();
extern int C2F( matrc)();
extern int C2F(mattr)();
extern int C2F(matadd)();
extern int C2F(matcmp)();
extern int C2F(matlog)();
extern int C2F(mattrc)();
extern int C2F(vecmul)();
extern int C2F(matpow)();
extern int C2F(matins1)();
extern int C2F(matins2)();
extern int C2F(matext1)();
extern int C2F(matext2)();
extern int C2F(vecldiv)();
extern int C2F(vecimpl)();
extern int C2F(matldiv)();
extern int C2F(vecrdiv)();
extern int C2F(matrdiv)();
extern int C2F(matmult)();
extern int C2F(matxpow)();
/*--------------------------------------------------------------------------*/
int C2F(matops)(void)
{
    static int op;

    op = Fin;

	/* operations binaires et ternaires */

    C2F(com).fun = 0;

	/*  cconc   insert extrac rconc */
    switch ((int)op)
	{
		case 1:
			{
				/* concatenation [a b] */
				C2F(matrc)();
				return 0;
			}
		case 2:
			{
				/* insertion */
				if (Rhs == 3) {
					C2F(matins1)();
				} else if (Rhs == 4) {
					C2F(matins2)();
				} else {
					Fin = -Fin;
				}
				return 0;
			}
		case 3:
			{
				/* extraction a(i) and a(i,j) */
				if (Rhs == 2) {
					C2F(matext1)();
				} else if (Rhs == 3) {
					C2F(matext2)();
				} else {
					Fin = -Fin;
				}
				return 0;
			}
		case 4:
			{
				/* concatenation [a;b] */
				C2F(matcc)();
				return 0;
			}
    }

	/* :  +  -  * /  \  =  ' */
    switch ((int)(op + 1 - colon))
	{
		case 1:
		{
			/*  : */
			C2F(vecimpl)();
			return 0;
		}
	case 2:
		{
			/* addition */
			C2F(matadd)();
			return 0;
		}
	case 3:
		{
			/* substraction */
			if (Rhs == 1)
			{
				/* .  unary minus */
				C2F(matchsgn)();
			} else
			{
				C2F(matsubst)();
			}
			return 0;
		}
	case 4:
		{
			/*  multiplication */
			matmult();
			return 0;
		}
	case 5:
		{
			/* division a droite */
			//C2F(matrdiv)();
			matrdiv();
			return 0;
		}
	case 6:
		{
			/* \ */
			//C2F(matldiv)();
			matldiv();
			return 0;
		}
	case 7:
		{
			/* == <= >= ~= */

			C2F(matcmp)();
			return 0;

		}
	case 8:
	case 9:
		{
			ChooseOtherOperation(op);
		}
	case 10:
		{
			/* ' */
			C2F(mattrc)();
			return 0;
		}
    }

	ChooseOtherOperation(op);
	return 0;

}
/*--------------------------------------------------------------------------*/
int ChooseOtherOperation(int op)
{
	if (op == dstar)
	{
		/*  ^ */
		C2F(matpow)();
		return 0;
	}
	if (op == quote + dot)
	{
		/* .' */
		C2F(mattr)();
		return 0;
	}
	if (op == dstar + dot)
	{
		/* .^ */
		C2F(matxpow)();
		return 0;
	}
	if (op >= dot * 3 + star)
	{
		/* .*. ./. .\. */
		/* kronecker */
		Fin = op - dot * 3 - star + 19;
		C2F(com).fun = 6;
		Rhs = 2;
		return 0;
	}
	if (op >= (dot << 1) + star)
	{
		/* *. /. \. */
		Fin = -Fin;
		return 0;

	}
	if (op >= less + equal)
	{
		/* == <= >= ~= */
		C2F(matcmp)();
		return 0;
	}
	if (op == dot + star)
	{
		/* .* */
		C2F(vecmul)();
		return 0;
	}
	if (op == dot + slash)
	{
		/*  ./ */
		C2F(vecrdiv)();
		return 0;
	}
	if (op == dot + bslash)
	{
		/* .\ */
		C2F(vecldiv)();
		return 0;
	}
	if (op == et || op == ou || op == non)
	{
		C2F(matlog)();
		return 0;
	}
	if (op >= less)
	{
		/* == <= >= ~= */
		C2F(matcmp)();
		return 0;
	}
	Fin = -Fin;
	return 0;
}
/*--------------------------------------------------------------------------*/
