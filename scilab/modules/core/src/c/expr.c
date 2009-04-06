/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "expr.h"
#include "ifexpr.h"
#include "msgs.h"
#include "stack-def.h"
#include "stack-c.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/ 
static int inc = 1;
static int checkvalue = 4095;
static int code = 30;
static int val = 0;
/*--------------------------------------------------------------------------*/ 
extern int C2F(compil)(int *, int *, int *, int *, int *);
extern int C2F(getsym)(void);
extern int C2F(istrue)(int *);
extern int C2F(eptover)(int *, int *);
/*--------------------------------------------------------------------------*/ 
#define  et      58 /* @TODO does 'et' is 'and' in english ? */
#define  equal   50
#define  less    59
#define  great   60
#define  not     61
#define  eol     99
#define  colon   44
#define  blank   40
#define  plus    45
#define  minus   46
#define  ou      57 /* @TODO does 'ou' is 'or' in english ? */
/*--------------------------------------------------------------------------*/ 
int C2F(expr)(void)
{
	static int eye[6] = { 672014862,673720360,673720360,673720360, 673720360,673720360 };
	int i = 0, j = 0;
	int r = 0 , s = 0, ir = 0, op = 0, ls = 0, sign = 0;
	int temp = 0;
	int kount = 0;

	if (C2F(iop).ddt == 4) { }

	r = C2F(recu).rstk[C2F(recu).pt - 1];
	if (r == 204) goto L85;

	ir = r / 100;

	if (ir != 1) goto L1;
	
	switch (r - 100) 
	{
		case 1:  goto L5;
		case 2:  goto L6;
		case 3:  goto L25;
		case 4:  goto L26;
		case 5:  goto L61;
		case 6:  goto L73;
		case 7:  goto L74;
		case 8:  goto L82;
		case 9:  goto L83;
		case 10:  goto L86;
		case 11:  goto L87;
		case 12:  goto L102;
		case 13:  goto L104;
		case 14:  goto L102;
		case 15:  goto L23;
	}

L1:
	if (C2F(com).sym >= ou && C2F(com).sym <= great) 
	{
		int code_error = 40;
		Error(code_error);
		return 0;
	}
L2:
	kount = 1;
	if (C2F(com).sym == not) goto L70;
	
	if (C2F(com).sym == colon) C2F(putid)(C2F(com).syn, eye);

L3:
	s = 1;
L4:
	if (C2F(com).sym == minus) s = -s;
	
	if (C2F(com).sym == plus || C2F(com).sym == minus) 
	{
		C2F(getsym)();
		goto L4;
	}
	sign = plus;
	if (s < 0) sign = minus;
	
	if (C2F(eptover)(&inc, &checkvalue)) return 0;
	
	C2F(recu).pstk[C2F(recu).pt - 1] = sign + (kount << 8);
	C2F(recu).rstk[C2F(recu).pt - 1] = 101;
	C2F(recu).icall = 2;
	/* *call* term */
	return 0;
L5:
	sign = C2F(recu).pstk[C2F(recu).pt - 1] % 256;
	kount = C2F(recu).pstk[C2F(recu).pt - 1] / 256;
	--C2F(recu).pt;
	if (sign != minus) goto L10;
	
	Rhs = 1;
	++C2F(recu).pt;
	C2F(recu).pstk[C2F(recu).pt - 1] = kount;
	Fin = minus;
	C2F(recu).rstk[C2F(recu).pt - 1] = 102;
	C2F(recu).icall = 4;
	/* *call* allops(minus) */
	return 0;
L6:
	kount = C2F(recu).pstk[C2F(recu).pt - 1];
	--C2F(recu).pt;
L10:
	if (C2F(com).sym == plus || C2F(com).sym == minus) goto L20;
	if (C2F(recu).rstk[C2F(recu).pt - 1] == 113) goto L104;
	if (C2F(recu).rstk[C2F(recu).pt - 1] == 112) goto L102;
	if (C2F(com).sym == eol) goto L50;
	if (C2F(com).sym >= ou || C2F(com).sym == equal) goto L70;
	goto L50;
L20:
	if (C2F(recu).rstk[C2F(recu).pt - 1] != 301) goto L21;
	/* blank or tab is delimiter inside angle brackets */
	ls = C2F(iop).lpt[2] - 2;
	if ( (i = C2F(iop).lin[ls - 1], abs(i)) == blank && (j = C2F(iop).lin[C2F(iop).lpt[2] - 1], abs(j)) != blank) goto L50;
		
L21:
	op = C2F(com).sym;
	C2F(getsym)();
	/* next lines added to handle sequence of + and - operators, S. Steer */
	/* 03/2005 (Matlab compatibility). Here i implemented a lazy way */
	/* without calling unary + or unary - operator */
L22:
	if (C2F(com).sym == plus) 
	{
		/* 1++2 or 1-+2 */
		C2F(getsym)();
		goto L22;
	}
	if (C2F(com).sym == minus) 
	{
		/* 1+-2 or 1--2 */
		if (op == minus) op = plus;
		else op = minus;
			
		C2F(getsym)();
		goto L22;
	}
	++C2F(recu).pt;
	C2F(recu).pstk[C2F(recu).pt - 1] = op + (kount << 8);
	if (C2F(com).sym != not) goto L24;
		
	C2F(recu).rstk[C2F(recu).pt - 1] = 115;
	/* *call* lfact */
	goto L85;

L23:
	goto L25;

L24:
	C2F(recu).rstk[C2F(recu).pt - 1] = 103;
	C2F(recu).icall = 2;
	/* *call* term */
	return 0;
L25:
	op = C2F(recu).pstk[C2F(recu).pt - 1] % 256;
	kount = C2F(recu).pstk[C2F(recu).pt - 1] / 256;
	Rhs = 2;
	C2F(recu).pstk[C2F(recu).pt - 1] = kount;
	C2F(recu).rstk[C2F(recu).pt - 1] = 104;
	Fin = op;
	C2F(recu).icall = 4;
	/* *call* allops(op) */
	return 0;
L26:
	kount = C2F(recu).pstk[C2F(recu).pt - 1];
	--C2F(recu).pt;
	goto L10;
L50:
	if (C2F(com).sym != colon) goto L60;

	C2F(getsym)();
	++kount;
	goto L3;
L60:
	if (kount > 3) 
	{
		int code_error = 33;
		Error(code_error);
		if (Err > 0) return 0;
	}
	Rhs = kount;
	if (kount <= 1) return 0;
	++C2F(recu).pt;
	C2F(recu).rstk[C2F(recu).pt - 1] = 105;
	Fin = colon;
	C2F(recu).icall = 4;
	/* *call* allops(colon) */
	return 0;
L61:
	--C2F(recu).pt;
	r = C2F(recu).rstk[C2F(recu).pt - 1];
	ir = r / 100;
	if (ir != 1) return 0;

	switch (r - 100) 
	{
		case 1:  goto L5;
		case 2:  goto L6;
		case 3:  goto L25;
		case 4:  goto L26;
		case 5:  goto L61;
		case 6:  goto L73;
		case 7:  goto L74;
		case 8:  goto L82;
		case 9:  goto L83;
		case 10:  goto L86;
		case 11:  goto L87;
		case 12:  goto L102;
		case 13:  goto L104;
		case 14:  goto L102;
	}

	/* in-line lexpr */
L70:
	if (C2F(iop).ddt == 4) {}
L72:
	if (C2F(eptover)(&inc, &checkvalue)) return 0;
	C2F(recu).ids[C2F(recu).pt * 6 - 6] = 0;
	C2F(recu).ids[C2F(recu).pt * 6 - 5] = C2F(errgst).err1;
	if (C2F(com).sym == ou) 
	{
		/* call getsym */
		C2F(recu).pstk[C2F(recu).pt - 1] = ou;
		if (C2F(ifexpr)()) 
		{
			/* checking for possible logical 'if expression' */
			/* evaluation shortcircuit */
			if (C2F(com).comp[0] != 0) 
			{
				if (C2F(compil)(&code, &inc, &val, &val, &val)) 
				{
					if (Err > 0) return 0;
					C2F(recu).ids[C2F(recu).pt * 6 - 6] = C2F(com).comp[0];
				}
			} 
			else 
			{
				temp = (i = *istk(iadr(C2F(vstk).lstk[Top - 1])), abs(i));
				if ( (temp != 8) && (C2F(istrue)(&val)) )
				{
					/* first term is true there is no use to evaluate the other */
					C2F(recu).ids[C2F(recu).pt * 6 - 6] = 1;
					/* err1 <>0 sets interpretation without evaluation */
					/* use special value to be able to distinguish from */
					/* recovered errors */
					C2F(errgst).err1 = 9191919;
				}
			}
		}
	}
	else C2F(recu).pstk[C2F(recu).pt - 1] = 0;
	C2F(recu).pstk[C2F(recu).pt - 1] += kount << 8;
	C2F(recu).rstk[C2F(recu).pt - 1] = 106;
	/* *call* lterm */
	goto L80;
L73:
	op = C2F(recu).pstk[C2F(recu).pt - 1] % 256;
	kount = C2F(recu).pstk[C2F(recu).pt - 1] / 256;
	--C2F(recu).pt;
	if (op == 0) goto L75;
	
	if (C2F(com).comp[0] == 0 && C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6] == 1) 
	{
		/* term has not been evaluated */
		if ((i = -C2F(errgst).errct, abs(i)) / 100000 == 0) 
		{
			C2F(errgst).err1 = C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5];
		} 
		else if (C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5] != 0) 
		{
			C2F(errgst).err1 = C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5];
		} 
		else 
		{
			if (C2F(errgst).err1 == 9191919) C2F(errgst).err1 = 0;
		}
		if (C2F(errgst).err1 > 0) return 0;
		goto L75;
	}
	C2F(recu).icall = 4;
	Fin = ou;
	Rhs = 2;
	++C2F(recu).pt;
	C2F(recu).rstk[C2F(recu).pt - 1] = 107;
	C2F(recu).pstk[C2F(recu).pt - 1] = kount;
	/* *call* allops(ou) */
	return 0;
L74:
	kount = C2F(recu).pstk[C2F(recu).pt - 1];
	if (C2F(com).comp[0] != 0 && C2F(recu).ids[C2F(recu).pt * 6 - 6] != 0) 
	{
		i = C2F(recu).ids[C2F(recu).pt * 6 - 6] - 1;
		if ( (C2F(compil)(&code, &val, &i, &val, &val)) && (Err > 0) ) return 0;
	}
	--C2F(recu).pt;
L75:
	if (C2F(com).sym == ou) goto L72;
	goto L50;
	/* in-line lterm */
L80:
	if (C2F(iop).ddt == 4) { }
L81:
	if (C2F(eptover)(&inc, &checkvalue)) return 0;
	
	C2F(recu).ids[C2F(recu).pt * 6 - 6] = 0;
	C2F(recu).ids[C2F(recu).pt * 6 - 5] = C2F(errgst).err1;
	if (C2F(com).sym == et) 
	{
		C2F(recu).pstk[C2F(recu).pt - 1] = et;
		C2F(recu).ids[C2F(recu).pt * 6 - 6] = 0;
		if (C2F(ifexpr)()) 
		{
			/* if expression evaluation, checking for possible */
			/* logical expression evaluation shortcircuit */
			if (C2F(com).comp[0] != 0) 
			{
				if (C2F(compil)(&code, &val, &val, &val, &val)) 
				{
					if (Err > 0) return 0;
					C2F(recu).ids[C2F(recu).pt * 6 - 6] = C2F(com).comp[0];
				}
			} 
			else 
			{
				temp = (i = *istk(iadr(C2F(vstk).lstk[Top - 1])), abs(i));
				if ( (temp != 8) && (! C2F(istrue)(&val)) )
				{
					/* first term is false there is no use to evaluate the other */
					C2F(recu).ids[C2F(recu).pt * 6 - 6] = 1;
					/* err1 <>0 sets interpretation without evaluation */
					C2F(errgst).err1 = 9191919;
				}
			}
		}
	} 
	else 
	{
		C2F(recu).pstk[C2F(recu).pt - 1] = 0;
	}
	C2F(recu).pstk[C2F(recu).pt - 1] += kount << 8;
	C2F(recu).rstk[C2F(recu).pt - 1] = 108;
	/* *call* lfact */
	goto L85;
L82:
	op = C2F(recu).pstk[C2F(recu).pt - 1] % 256;
	kount = C2F(recu).pstk[C2F(recu).pt - 1] / 256;
	--C2F(recu).pt;
	if (op == 0) goto L84;
	
	if (C2F(com).comp[0] == 0 && C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6] == 1) 
	{
		/* term has not been evaluated */
		if ((i = -C2F(errgst).errct, abs(i)) / 100000 == 0) 
		{
			C2F(errgst).err1 = C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5];
		} 
		else if (C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5] != 0) 
		{
			/* error detected before if expression evaluation (should not occur ?) */
			C2F(errgst).err1 = C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 5];
		} 
		else 
		{
			if (C2F(errgst).err1 == 9191919) C2F(errgst).err1 = 0;
		}
		if (C2F(errgst).err1 > 0) return 0;
		goto L84;
	}
	C2F(recu).icall = 4;
	Fin = et;
	Rhs = 2;
	++C2F(recu).pt;
	C2F(recu).pstk[C2F(recu).pt - 1] = kount;
	C2F(recu).rstk[C2F(recu).pt - 1] = 109;
	/* *call* allops(et) */
	return 0;
L83:
	kount = C2F(recu).pstk[C2F(recu).pt - 1];
	if (C2F(com).comp[0] != 0 && C2F(recu).ids[C2F(recu).pt * 6 - 6] != 0) 
	{
		i = C2F(recu).ids[C2F(recu).pt * 6 - 6] - 1;
		if ( (C2F(compil)(&code, &val, &i, &val, &val)) && (Err > 0) ) return 0;
	}
	--C2F(recu).pt;
L84:
	if (C2F(com).sym != et) goto L73;
	goto L81;

	/* in-line lfact */
L85:
	if (C2F(iop).ddt == 4) { }
	if (C2F(eptover)(&inc, &checkvalue)) return 0;
		
	C2F(recu).pstk[C2F(recu).pt - 1] = 0;
	if (C2F(com).sym == not && C2F(com).char1 != equal) 
	{
		C2F(recu).pstk[C2F(recu).pt - 1] = not;
		C2F(getsym)();
	}
	C2F(recu).pstk[C2F(recu).pt - 1] += kount << 8;
	C2F(recu).rstk[C2F(recu).pt - 1] = 110;
	/* *call* lprim */
	goto L100;
L86:
	op = C2F(recu).pstk[C2F(recu).pt - 1] % 256;
	kount = C2F(recu).pstk[C2F(recu).pt - 1] / 256;
	--C2F(recu).pt;
	if (op == 0) goto L82;
	Fin = op;
	Rhs = 1;
	++C2F(recu).pt;
	C2F(recu).pstk[C2F(recu).pt - 1] = kount;
	C2F(recu).rstk[C2F(recu).pt - 1] = 111;
	C2F(recu).icall = 4;
	/* *call* allops(not) */
	return 0;
L87:
	kount = C2F(recu).pstk[C2F(recu).pt - 1];
	--C2F(recu).pt;
	/* next two lines to handle a+~b and a*~b,... */
	if (C2F(recu).rstk[C2F(recu).pt - 1] == 115) goto L23;
	if (C2F(recu).rstk[C2F(recu).pt - 1] == 204) return 0;
	goto L82;
	/* in-line lprim */
L100:
	if (C2F(iop).ddt == 4) { }
	if (C2F(recu).pstk[C2F(recu).pt - 1] % 256 != 0) goto L101;
	if (C2F(com).sym != et && C2F(com).sym != ou) goto L103;
	C2F(getsym)();
	/* modif SS */
	if (C2F(eptover)(&val, &checkvalue)) return 0;
L101:
	++C2F(recu).pt;
	C2F(recu).rstk[C2F(recu).pt - 1] = 112;
	C2F(recu).icall = 1;
	/* *call* expr */
	goto L2;
L102:
	--C2F(recu).pt;
	if ((C2F(com).sym != equal && C2F(com).sym < less) || C2F(com).sym == eol) goto L86;
L103:
	op = C2F(com).sym;
	C2F(getsym)();
	if (op == equal && C2F(com).sym != equal) 
	{
		int code_message = 7;
		C2F(msgs)(&code_message, &val);
	}
	
	if (C2F(com).sym == equal || C2F(com).sym == great) 
	{
		if (op != equal) op += C2F(com).sym;
		C2F(getsym)();
		if (op == not + equal) op = less + great;
	}
	++C2F(recu).pt;
	C2F(recu).rstk[C2F(recu).pt - 1] = 113;
	C2F(recu).pstk[C2F(recu).pt - 1] = op;
	C2F(recu).icall = 1;
	/* *call* expr */
	goto L2;
L104:
	Fin = C2F(recu).pstk[C2F(recu).pt - 1];
	Rhs = 2;
	C2F(recu).rstk[C2F(recu).pt - 1] = 114;
	C2F(recu).icall = 4;
	/* *call* allops(fin) */
	return 0;
}
/*--------------------------------------------------------------------------*/ 


