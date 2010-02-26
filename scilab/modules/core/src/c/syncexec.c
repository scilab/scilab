/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Serge STEER
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "syncexec.h"
#include "callinterf.h"
#include "stack-c.h"
#include "core_math.h"
#include "parse.h"
#include "stack-def.h" /* C2F(basbrk) */
#include "Scierror.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
#define Pt (C2F(recu).pt)
/*--------------------------------------------------------------------------*/
extern int C2F(bexec)();
/*--------------------------------------------------------------------------*/
int syncexec(char *str, int *ns, int *ierr, int *seq, long int str_len)
{
	

	/* Local variables */
	static int zero = 0;
	static int one = 1;
	int Pts, Tops;
	static int k;
	static int ir;

	static int *Ids  = C2F(recu).ids-nsiz-1;
	static int *Rstk = C2F(recu).rstk-1;
	static int *Lstk = C2F(vstk).lstk-1;
	static int *Infstk  = C2F(vstk).infstk-1;

	Pt = Max(Pt,0);Pts=Pt;
	Top = Max(Top,0);Tops=Top;
	C2F(bexec)(str, ns, ierr, (*ns));
	if (*ierr != 0) {
		goto L9998;
	}
	/* + */
	if (Eptover(1)) {
		goto L9998;
	}
	Ids[1 + Pt * nsiz] = Lhs;
	Ids[2 + Pt * nsiz] = Rhs;
	Ids[3 + Pt * nsiz] = C2F(com).sym;
	Ids[4 + Pt * nsiz] =  C2F(basbrk).interruptible;
	C2F(basbrk).interruptible = *seq == 0;

	Rstk[Pt] = 1002;
	++C2F(recu).niv;
	C2F(com).fun = 0;
	C2F(recu).icall = 5;

	/* code translated from callinterf.h */
L60:

	C2F(parse)();

	if (Err > 0) {
	  goto L9999;
	}

	if (C2F(com).fun == 99) {
		C2F(com).fun = 0;
		goto L200;
	}

	if ( isRecursionCallToFunction() )
	{
		int gw = getRecursionGatewayToCall();
		if (gw == END_OVERLOAD)
		{
			goto L96;
		} 
		else if (gw == ERROR_GW_ID)
		{
			goto L89;
		}
		else
		{
			k = gw;
		}
		goto L95;
	}


L89:
	if (Top < Rhs) {
		int lierr = 22;
		Error(lierr);
		goto L9999;
	}
	if (Top - Rhs + Lhs + 1 >= Bot) {
		int lierr = 18;
		Error(lierr);
		goto L9999;
	}
	goto L91;

L90:
	if (Err > 0) {
		goto L9999;
	}
L91:
	k = C2F(com).fun;
	C2F(com).fun = 0;
	if (k == C2F(recu).krec) {
		int lierr = 22;
		C2F(recu).krec = -1;
		Error(lierr);
		goto L9999;
	}
	C2F(recu).krec = -1;
	if (k == 0) {
		goto L60;
	}
L95:
	if (! C2F(allowptr)(&k)) {
		C2F(ref2val)();
	}
	C2F(recu).krec = k;
	C2F(callinterf)(&k);
	/*      if (k.eq.krec) krec=99999 */
	C2F(recu).krec = -1;
	if (C2F(com).fun >= 0) {
		if (Top - Lhs + 1 > 0) {
			C2F(iset)(&Rhs, &zero, &Infstk[Top - Lhs+1], &one);
		}
		goto L90;
	}
	/*     called interface ask for a scilab function to perform the function (fun=-1) */
	/*     the function name is given in ids(1,pt+1) */
	C2F(ref2val)();
	C2F(com).fun = 0;
	C2F(funs)(&Ids[1 + (Pt + 1) * nsiz]);
	if (Err > 0) {
		goto L9999;
	}
	if (C2F(com).fun > 0) {
		goto L91;
	}
	if (Fin == 0) {
		int lierr = 246;
		Error(lierr);
		if (Err > 0) {
			goto L9999;
		}
		goto L90;
	}
	++Pt;
	Fin = Lstk[C2F(com).fin];
	Rstk[Pt] = 910;
	C2F(recu).icall = 5;
	C2F(com).fun = 0;
	/*     *call*  macro */
	goto L60;
L96:
	--Pt;
	goto L90;
	/* End of callinterf.h code */

L200:
	Lhs = Ids[1 + Pt * nsiz];
	Rhs = Ids[2 + Pt * nsiz];
	C2F(com).sym = Ids[3 + Pt * nsiz];
	C2F(basbrk).interruptible = Ids[4 + Pt * nsiz];
	--Pt;
	--Top;
	/* + */
	--C2F(recu).niv;
	*ierr = 0;
	C2F(recu).icall = 0;
	Fin = 3;
	return 0;
L9998:
	*ierr = 1;
	C2F(basbrk).interruptible = Ids[4 + Pt * nsiz];
	Pt=Pts;Top=Tops;
	return 0;
L9999:
	/* Err == 9999999 arises if abort has been used to terminate the callback execution */
	if (Err != 9999999) *ierr = 1;
	--Top;
	--C2F(recu).niv;
	/*
	** OVVERRIDE WHAT PARSE AS PUT INTO THIS
	** F. FLAG !!!!
	** We want to be interruptible after having an error
	** _VERY_ CRAPPY CODE
	*/
	//C2F(basbrk).interruptible = Ids[4 + Pt * nsiz];
	C2F(basbrk).interruptible = TRUE;
	Pt=Pts;Top=Tops;
	C2F(recu).icall = 0;
	return 0;
} /* syncexec */
/*--------------------------------------------------------------------------*/

