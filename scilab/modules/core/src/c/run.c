/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Serge STEER
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------
 * Execution of a compiled macro (byte code)
 * byte code is a sequence of tags each of them containing the data relative
 * to a particular basic operation
 *
 * Code automatically translated from Fortran to C
 *------------------------------------------------------------------ */

#include <string.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <stdlib.h>
#endif

#include <time.h>
#include "sciquit.h"
#include "stack-c.h"
#include "run.h"
#include "basout.h"
#include "parse.h"
#include "localization.h"
#include "core_math.h"
#include "scilabmode.h"
#include "stack-def.h" /* C2F(basbrk) */
#include "storeCommand.h"
#include "do_error_number.h"
#include "Scierror.h"
#include "msgs.h"
#undef Lstk
#undef Infstk


/* Table of constant values */

static int c__1 = 1;
static int c__0 = 0;

#define insert  2
#define extrac  3
#define semi  43
#define equal  50
#define iselect 3
#define Pt (C2F(recu).pt)
extern int C2F(stackp)(int *,int *);
extern int C2F(eqid)(int *,int *);
extern int C2F(bexec)(char *,int *,int *);
extern int C2F(print)(int *,int *,int *);
extern int C2F(createref1)(int *);
extern int C2F(command)(int *,int *);
extern int C2F(mkindx)(int *,int *);
extern int C2F(whatln)(int *,int *, int *,int *,int *,int *);
extern int C2F(prompt)(int *,int *);
extern int C2F(seteol)(void);
extern int C2F(name2var)(int *);
extern int C2F(getendian)(void);
extern int C2F(nextj)(int *, int *);
extern int C2F(isafunptr)(int *, int *,int *,int *);
extern int C2F(varfunptr)(int *, int *,int *);
extern int C2F(defmat)(void);
extern int C2F(clunit)(int *, char *, int *);
extern int C2F(istrue)(int *);

int Istrue(int n)
{

  return C2F(istrue)(&n);
}



int C2F(run)(void)
{
  /* Initialized data */
  /* Fortran common data equivalence */
  static int    *Ids  = C2F(recu).ids-nsiz-1;
  static int    *Rstk = C2F(recu).rstk-1;
  static int    *Pstk = C2F(recu).pstk-1;
  static int    *Lstk = C2F(vstk).lstk-1;
  static int    *Lin  = C2F(iop).lin-1;
  static int    *Lpt  = C2F(iop).lpt-1;
  static int  *Infstk = C2F(vstk).infstk-1;
  static int    *Lct = C2F(iop).lct - 1;

  static double equiv_4[1];
#define x (equiv_4)
#define ix ((int *)equiv_4)

  /* Local variables */
  static int ifin, iesc, ibpt, tref, ifun;
  static int ierr, ndel;
  static int j, k, m, n, p, r, t;
  static int lname, imode;
  static int l0;
  static int id[6], lc, kc, nc, lb, li, il, io, ip;
  static int ok;
  static int ir, lr, op;
  static int mm1;
  static int nn1;
  static int nentry, lastindpos;
  static int lcc, kid, nlr;
  int i2;
  static char tmp[80]; /*string for sending debug messages*/
  tref = 0;

  /* set debug trace mode on  */
  if (C2F(iop).ddt == 4) {
    sprintf(tmp," run pt:%d rstk(pt):%d",Pt,Rstk[Pt]);
    C2F(basout)(&io, &C2F(iop).wte,tmp, (long)strlen(tmp));
  }

  l0 = 0;
  nc = 0;

  if (Ptover(0)) {
    return 0;
  }

  r = Rstk[Pt];
  ir = r / 100;
  if (ir != 6) {
    goto L1;
  }
  switch ((int)(r - 600)) {
  case 1:  goto L33;
  case 2:  goto L66;
  case 3:  goto L82;
  case 4:  goto L92;
  case 5:  goto L58;
  case 6:  goto L116;
  case 7:  goto L250;
  case 8:  /*Rstk[Pt]=1101;*/ goto L254;
  case 9:  /*Rstk[Pt]=1101;*/ goto L240;

  }


 L1: /*  Start execution of a "compiled" function  */
  tref = clock();
  C2F(errgst).toperr = Top;
  k = Lpt[1] - (13+nsiz);
  lc = Lin[k + 7];

 L10: /* Current opcode finished handle error, interruptions,...*/
  if (Err > 0)  return 0;

  if (C2F(basbrk).iflag) {
    C2F(basbrk).iflag = FALSE;
    goto L91;
  }
  if (C2F(errgst).err1 != 0 ) {
    if ((C2F(errgst).errpt >0) && (Pt >= C2F(errgst).errpt) && (Rstk[C2F(errgst).errpt]==618)) {
      /* error under try catch */
      for (p=Pt;p>=C2F(errgst).errpt;p--) {
	if (Rstk[p]<=502 && Rstk[p]>=501){
	  k = Lpt[1] - (13+nsiz);
	  Lpt[1] = Lin[k+1];
	  Lpt[2] = Lin[k+2];
	  Lpt[3] = Lin[k+3];
	  Lpt[4] = Lin[k+4];
	  Lpt[6] = k;
	  C2F(recu).macr--;
	  if (Rstk[p-1]==909) Top--; /* execed function*/
	}
	/* may it will be necessary to take care of for loop variables */
      }
      Pt = C2F(errgst).errpt;
      goto L271;
    }
   /* errcatch in exec(function,'errcatch')
     * or catched error in an external
     * or errcatch in execstr('foo()','errcatch') */
    if (C2F(errgst).errcatch == 0) goto L999;
    /* error under errcatch(....,'continue') */

	/* @TODO : replace 903 909 1001 1002 by a #define ... */
    if (Rstk[Pt - 1] == 903 || Rstk[Pt - 1] == 909 || Rstk[Pt] == 1001 || Rstk[Pt] == 1002)  return 0;
  }
  if (lc - l0 == nc) { /* is current opcodes block (if, for, .. structure) finished ?*/
    /* yes */
    r = Rstk[Pt] - 610;
    switch (r) {
    case 1:  goto L46;
    case 2:  goto L47;
    case 3:  goto L52;
    case 4:  goto L56;
    case 5:  goto L57;
    case 6:  goto L61;
    case 8:  goto L271;
    case 9:  goto L272;
    }
  }


 L11:   /*  next opcode */
  op = *istk(lc);
  /*  label 49 retains to be able issue a compatibility error message */
  switch ((int)op) { /* step to corresponding part*/
  case 1:  goto L20;
  case 2:  goto L25;
  case 3:  goto L40;
  case 4:  goto L42;
  case 5:  goto L30;
  case 6:  goto L41;
  case 7:  goto L45;
  case 8:  goto L49;
  case 9:  goto L49;
  case 10:  goto L55;
  case 11:  goto L270;/* try */
  case 12:  goto L90;
  case 13:  goto L95;
  case 14:  goto L100;
  case 15:  goto L105;
  case 16:  goto L110;
  case 17:  goto L120;
  case 18:  goto L130;
  case 19:  goto L140;
  case 20:  goto L150;
  case 21:  goto L160;
  case 22:  goto L170;
  case 23:  goto L180;
  case 24:  goto L190;
  case 25:  goto L200;
  case 26:  goto L210;
  case 27:  goto L220;
  case 28:  goto L97;
  case 29:  goto L230;
  case 30:  goto L260;
  case 31:  goto L261;

  }
  if (op >= 100) {
    /* ------------- primitive call (matfn's) -------------- */
    goto L80;
  }

  if (op == 99) {
    /* ------------- return -------------------------------- */
    /* check if "return" occured in a for loop */
    p = Pt + 1;
  L12:
    --p;
    if (Rstk[p] == 612) {
      /* yes, remove the for loop variable */
      --Top;
      goto L12;
    } else if (Rstk[p] != 501) {
      goto L12;
    }
    Fin = 2;
    goto L998;
  }

  if (op <= 0) {
    /* ------------- nop ---------------------------------- */
    lc += *istk(1 + lc);
    goto L11;
  }

  SciError(60);
  return 0;

 L20: /* stackp, retplaced by assign */
  /*     retained for 2.7 and earlier versions compatibility */
  C2F(stackp)(istk(1 + lc), &c__0);
  /*     store info if printing is required see code 22 */
  C2F(putid)(id, istk(1 + lc));
  kid = Fin;
  lc += 7;
  goto L10;

 L25: /* stackg */
  Fin = *istk(7 + lc);
  ifin = Fin;
  Rhs = *istk(8 + lc);
  lname = lc + 1;
 L26:
  C2F(stackg)(istk(lname));
  if (Err > 0||C2F(errgst).err1 > 0) {
    lc += 9;
    goto L10;
  }
  if (Fin != 0) {/* variable exists */
    goto L28;
  }
  C2F(funs)(istk(1 + lc)); /* check if it is a function */
  if (Err > 0||C2F(errgst).err1 > 0) {
    lc += 9;
    goto L10;
  }
  if (C2F(com).fun != -2) {
    C2F(putid)(&Ids[1 +(Pt + 1) * nsiz ], istk(1 + lc));
    if (C2F(com).fun == 0) {
      /* the search variable is neither a regular variable nor a function in a librar */
      /* it may be a simple variable in a lib */

      C2F(stackg)(istk(lname));
      if (Err > 0||C2F(errgst).err1 > 0) {
	lc += 9;
	goto L10;
      }
      if (Fin==0) {
        SciError(4);
        if (Err > 0||C2F(errgst).err1 > 0) {
          lc += 9;
          goto L10;
        }
      }

    } else {
      /* referenced name was function at compile time it is now a
       * primitive. Modify the code for further use */
      if (ifin != -4 && ifin != 0) {
	/* function call */
	/* change current  opcode to nop */
	*istk(lc) = 0;
	*istk(1 + lc) = 9;
	lc += 9;
	/* change the following opcode to matfn opcode */
	op = C2F(com).fun * 100;
	*istk(lc) = op;
	*istk(1 + lc) = *istk(2 + lc) - 1;
	*istk(2 + lc) = *istk(3 + lc);
	*istk(3 + lc) = Fin;
	goto L80;
      } else {
	/* only reference to a function */
	/* stackg opcode replaced by varfun opcode */
	*istk(lc) = 27;
	*istk(1 + lc) = C2F(com).fun;
	*istk(2 + lc) = Fin;
	C2F(putid)(istk(3 + lc), &Ids[1 + (Pt + 1) * nsiz]);
	goto L10;
      }
    }
    lc += 9;
    goto L10;
  }
  Fin = *istk(7 + lc);
  goto L26;
 L28:
  if (Rhs == 0 && ((*istk(7 + lc) == -2)||(*istk(7 + lc) == -1)) && Fin == -1) {
    lc += 9;

    if (*istk(7 + lc-9) == -2) {

      /* instruction reduced to <name> with name not a function, replace */
      /* next two op code by a single store */
      /* skip extract op-code <5 3 1 1> */
      if (*istk(lc) != 5 || *istk(1 + lc) != 3) {
	strcpy(C2F(cha1).buf,_("Unexpected opcode, please report into the Scilab bug tracker."));
	SciError(9999);
	return 0;
      }
      lc += 4;
    }
    /* skip assignment op_code <29 43 ans 0> */
    if (*istk(lc) != 29) {
      strcpy(C2F(cha1).buf,_("Unexpected opcode, please report into the Scilab bug tracker."));
      SciError(9999);
      return 0;
    }
    lc += 10;
    /* store */
    Rhs = 1;
    C2F(ref2val)();
    C2F(stackp)(istk(lname), &c__0);
    if (Err > 0 ||C2F(errgst).err1 > 0) {
      goto L10;
    }
    goto L10;
  }
  lc += 9;
  if (Fin > 0) {
    goto L65;
  }
  goto L10;

  /*     allops */
 L30:
  Fin = *istk(1 + lc);
  Rhs = *istk(2 + lc);
  Lhs = *istk(3 + lc);
  lc += 4;
  if (Fin == extrac) {
    C2F(isafunptr)(&Top, id, &ifun, &ifin);
    if (ifun != 0) {
      --Top;
      --Rhs;
      C2F(com).fun = ifun;
      Fin = ifin;
      C2F(adjustrhs)();
      goto L81;
    }
  }
  if (Fin == extrac || Fin == insert) {
    C2F(adjustrhs)();
  }
  ++Pt;
  Rstk[Pt] = 601;
  Ids[1 + Pt * nsiz] = tref;
  Ids[3 + Pt * nsiz] = l0;
  Ids[4 + Pt * nsiz] = nc;
  C2F(recu).icall = 4;
  /*     pstk(pt) is used by allops to get the name of output variable (insertion) */
  Pstk[Pt] = lc;
  /*     *call* allops */
  return 0;
 L33:
  tref = Ids[1 + Pt * nsiz];
  l0 = Ids[3 + Pt *  nsiz];
  nc = Ids[4 + Pt *  nsiz];
  lc = Pstk[Pt];
  --Pt;
  goto L70;

  /*     string */
 L40:
  n = *istk(1 + lc);
  if (C2F(errgst).err1 <= 0) {
    ++Top;
    if (C2F(cresmat)("run", &Top, &c__1, &c__1, &n, 3L)) {
      C2F(getsimat)("run", &Top, &Top, &mm1, &nn1, &c__1, &
		    c__1, &lr, &nlr, 3L);
      C2F(icopy)(&n, istk(2 + lc), &c__1, istk(lr), &c__1);
    }
  }
  lc = lc + n + 2;
  goto L10;

  /*     num */
 L41:
  if (C2F(errgst).err1 <= 0) {
    if (C2F(getendian)() == 1) {
      ix[0] = *istk(1 + lc);
      ix[1] = *istk(2 + lc);
    } else {
      ix[1] = *istk(1 + lc);
      ix[0] = *istk(2 + lc);
    }
    ++Top;
    if (C2F(cremat)("run", &Top, &c__0, &c__1, &c__1, &lr, &lcc, 3L)) {
      *stk(lr) = *x;
    }
  }
  lc += 3;
  goto L10;

 L42:
  C2F(defmat)();
  ++lc;
  goto L10;

  /*     for */
 L45:
  nc = *istk(1 + lc);
  lc += 2;
  l0 = lc;
  if (Ptover(1)) {
    lc += nc;
    lc = lc + nsiz + *istk(lc);
    goto L10;
  }
  Rstk[Pt] = 611;
  Ids[1 + Pt * nsiz] = l0;
  Ids[2 + Pt * nsiz] = nc;
  goto L10;

 L46:
  nc = *istk(lc);
  l0 = lc + 7;
  if  (C2F(errgst).errcatch>=1 &&C2F(errgst).err1 > 0) {
    /*an error occured in the loop variable expression evaluation, in 'continue' mode
      skip all the for codes*/
    lc = l0;
    goto L48;
  }
  Rstk[Pt] = 612;
  Pstk[Pt] = 0;
  Ids[1 + Pt * nsiz] = l0;
  Ids[2 + Pt * nsiz] = Lct[8];
  Ids[3 + Pt * nsiz] = Top;
  Ids[4 + Pt * nsiz] = C2F(errgst).toperr;
  C2F(errgst).toperr = Top;
 L47:
  lc = l0;
  if (Top != Ids[3 + Pt * nsiz]) {
    SciError(115);
    goto L48;
  }
  C2F(nextj)(istk(1 + l0 - 7), &Pstk[Pt]);
  if (Pstk[Pt] != 0) {
    Lct[8] = Ids[2 + Pt * nsiz];
    if (ismenu() == 1 && C2F(basbrk).interruptible) goto L115;
    goto L10;
  }
  /*     fin for */
 L48:
  lc += nc;
  C2F(errgst).toperr = Ids[4 + Pt * nsiz];
  --Pt;
  goto L70;

  /*     Very old if - while (removed) */
 L49:
  if (*istk(1 + lc) < 0) {
    goto L55;
  }
 L52:
  strcpy(C2F(cha1).buf, _("Functions compiled with very old versions are no more handled."));
  SciError(997);
  return 0;

  /*     "select- case"  or  "if elseif else end" */
 L55:
  if (Ptover(1)) {
    lc += (i2 = *istk(1 + lc), abs(i2));
    goto L10;
  }
  Pstk[Pt] = lc;
  Ids[3 + Pt * nsiz] = C2F(errgst).toperr;

 L551:
  if (*istk(1 + lc) > 0) {
    /*    first expression */
    nc = *istk(3 + lc);
    Rstk[Pt] = 614;
    lc += 4;
    l0 = lc;
    Ids[1 + Pt * nsiz] = l0;
    Ids[2 + Pt * nsiz] = nc;
    goto L10;
  } else {
    lc += 4;
  }

  /*     expri */
 L56:
  if  (C2F(errgst).errcatch>=1 && C2F(errgst).err1 > 0 ) {
    /*an error occured in the first expression evaluation, in 'continue' mode
      skip all the control structure codes*/
    goto L62;
  }

  if (*istk(Pstk[Pt]) == 10) {
    /*     copy first expression */
    i2 = Top + 1;
    if (! C2F(vcopyobj)("run", &Top, &i2,3L)) {
      return 0;
    }
    ++Top;
  }

  nc = *istk(lc);
  Rstk[Pt] = 615;
  ++lc;
  l0 = lc;
  Ids[1 + Pt * nsiz] = l0;
  Ids[2 + Pt * nsiz] = nc;
  goto L10;

  /*     instructions i */
 L57:
  if  (C2F(errgst).errcatch>=1 && C2F(errgst).err1 > 0 ) {
    /*an error occured in the first expression evaluation, in 'continue' mode
      skip all the control structure codes*/
    goto L62;
  }
  if (nc == 0) {
    /* if nc=0 the instruction correspond to the else */
    ok = TRUE;
    if (*istk(Pstk[Pt]) == 10) {
      --Top;
    }
    goto L59;
  } else if (*istk(Pstk[Pt]) != 10) {
    ok = Istrue(1);
    if (Err > 0 || C2F(errgst).err1 > 0) {
      goto L10;
    }
    goto L59;
  }
  ++Pt;
  Fin = equal;
  Rhs = 2;
  Lhs = 1;
  Rstk[Pt] = 605;
  C2F(recu).icall = 4;
  Pstk[Pt] = lc;
  Ids[1 + Pt * nsiz] = tref;
  /*     *call* allops(equal) */
  return 0;

 L58:
  if  (C2F(errgst).errcatch>=1 && C2F(errgst).err1 > 0 ) {
    /*an error occured in the first expression evaluation, in 'continue' mode
      skip all the control structure codes*/
    goto L62;
  }

  lc = Pstk[Pt];
  tref = Ids[1 + Pt * nsiz];
  --Pt;
  ok = Istrue(1);
  if (Err > 0 || C2F(errgst).err1 > 0) {
    goto L10;
  }
 L59:
  nc = *istk(lc);
  C2F(errgst).toperr = Top;
  if (ok) {
    ++lc;
    if (*istk(Pstk[Pt]) == 10) {
      --Top;
    }
    l0 = lc;
    Ids[1 + Pt * nsiz] = l0;
    Ids[2 + Pt * nsiz] = nc;
    Rstk[Pt] = 616;
    if (ismenu() == 1 && C2F(basbrk).interruptible) goto L115;
    goto L10;
  } else {
    if (*istk(Pstk[Pt]) == 9) {
      goto L62;
    }
    lc = lc + nc + 1;
    goto L56;
  }

 L61:
  /*     fin if while select/case */
  l0 = Pstk[Pt];
  if (*istk(Pstk[Pt]) == 9) {
    lc = l0 + 4;
    goto L56;
  }
 L62:
  l0 = Pstk[Pt];
  lc = l0 + (i2 = *istk(1 + l0), abs(i2));
  C2F(errgst).toperr = Ids[3 + Pt * nsiz];
  --Pt;
  goto L70;

  /*     macro */
 L65:
  i2 = *istk(2 + lc) - 1;
  Rhs = Max(i2,0);
  C2F(adjustrhs)();
  Lhs = *istk(3 + lc);

  lc += 4;

  if (Ptover(1)) {
    goto L10;
  }
  Rstk[Pt] = 602;
  Pstk[Pt] = lc;
  /*  Ids[1 + Pt * nsiz] = C2F(dbg).wmac; *//*moved into macro.f*/
  Ids[2 + Pt * nsiz] = tref;
  Ids[3 + Pt * nsiz] = l0;
  Ids[4 + Pt * nsiz] = nc;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /*     *call* macro */
  return 0;
 L66:
  lc = Pstk[Pt];
  /*C2F(dbg).wmac = Ids[1 + Pt * nsiz];*//*moved into macro.f*/
  tref = Ids[2 + Pt * nsiz];
  l0 = Ids[3 + Pt * nsiz];
  nc = Ids[4 + Pt * nsiz];
  --Pt;
  goto L70;

 L70:
  /* re entering run to continue macro evaluation */
  if (ismenu() == 1 && C2F(basbrk).interruptible) goto L115;

 L71:
  /* reset proper values for l0 and nc if a control structure had been escaped*/
  r = Rstk[Pt] - 610;
  switch ((int)r) {
  case 1:
    l0 = Ids[1 + Pt * nsiz];
    nc = Ids[2 + Pt * nsiz];
    goto L10;
  case 2: /* back to a for */
    j = Pstk[Pt];
    l0 = Ids[1 + Pt * nsiz];
    nc = *istk(l0 - 7);
    goto L10;
  case 3: /* back to an if or a while */
    li = Ids[1 + Pt * nsiz];
    kc = Ids[2 + Pt * nsiz];
    nc = *istk(2 + li);
    l0 = li + 5;
    if (kc == 0) {
      goto L10;
    }
    l0 += nc;
    nc = *istk(3 + li);
    if (kc == 1) {
      goto L10;
    }
    l0 += nc;
    nc = *istk(4 + li);
    goto L10;
  case 4:
  case 5:
  case 6:
  /*    back to a select case   */
    l0 = Ids[1 + Pt * nsiz];
    nc = Ids[2 + Pt * nsiz];
    goto L10;
  case 8: /*back to a try*/
    l0 = Ids[1 + Pt * nsiz];
    nc = *istk(l0 - 2);
    goto L10;
  case 9:  /*back to a catch*/
    l0 = Ids[1 + Pt * nsiz];
    nc = *istk(l0 - 1);
    l0 = l0 + *istk(l0 - 2);
    goto L10;
  default :
    goto L10;
  }

 L80:
  C2F(com).fun = op / 100;
  Rhs = *istk(1 + lc);
  C2F(adjustrhs)();
  Lhs = *istk(2 + lc);
  Fin = *istk(3 + lc);
  lc += 4;

 L81:
  ++Pt;
  Rstk[Pt] = 603;
  Pstk[Pt] = lc;
  C2F(recu).icall = 9;
  Ids[2 + Pt * nsiz] = 0;
  Ids[3 + Pt * nsiz] = tref;
  Ids[4 + Pt * nsiz] = l0;
  Ids[5 + Pt * nsiz] = nc;
  /*     *call* matfns */
  return 0;
 L82:
  /*     warning if builtin is "resume" control is passed to macro and not here */
  lc = Pstk[Pt];
  tref = Ids[3 + Pt * nsiz];
  l0 =   Ids[4 + Pt * nsiz];
  nc =   Ids[5 + Pt * nsiz];
  --Pt;
  goto L70;

  /*     pause */
 L90:
  ++lc;
 L91:
  if (Ptover(1)) {
    goto L10;
  }
  Pstk[Pt] = C2F(iop).rio;
  C2F(iop).rio = C2F(iop).rte;
  Fin = 2;
  if (Lct[4] <= -10) {
    Fin = -1;
	Lct[4] = -Lct[4] - 11;
  }
  Ids[1 + Pt * nsiz] = lc;
  Ids[2 + Pt * nsiz] = Top;
  Ids[3 + Pt * nsiz] = tref;
  Ids[4 + Pt * nsiz] = l0;
  Ids[5 + Pt * nsiz] = nc;
  Rstk[Pt] = 604;
  C2F(recu).icall = 5;
  /*     *call* macro */
  return 0;
 L92:
  lc =   Ids[1 + Pt * nsiz];
  Top =  Ids[2 + Pt * nsiz];
  tref = Ids[3 + Pt * nsiz];
  l0 =   Ids[4 + Pt * nsiz];
  nc =   Ids[5 + Pt * nsiz];
  C2F(iop).rio = Pstk[Pt];
  --Pt;
  goto L70;

  /*     break */
 L95:
  p = Pt + 1;
 L96:
  --p;
  if (p == 0) {
    ++lc;
    goto L10;
  }
  if (Rstk[p] == 612) {
    /*     break in a for */
    l0 = Ids[1 + p * nsiz];
    lc = l0 + *istk(1 + l0 - 8);
    Pt = p - 1;
    --Top;
    goto L70;
  } else if (Rstk[p] == 616 && *istk(1 + Pstk[p] - 1) == 9)
    {
      /*     break in a while */
      l0 = Pstk[p];
      lc = l0 + (i2 = *istk(1 + l0), abs(i2));
      Pt = p - 1;
      goto L70;
    } else if (Rstk[p] == 501 || Rstk[p] == 502 ||
	       Rstk[p] == 503) {
      /*     going outside a function an exec (break ignored) */
      ++lc;
      goto L10;
    } else {
      goto L96;
    }
  /*     continue */
 L97:
  p = Pt + 1;
 L98:
  --p;
  if (p == 0) {
    ++lc;
    goto L10;
  }
  if (Rstk[p] == 612) {
    /*     continue in a  for */
    l0 = Ids[1 + p * nsiz];
    /* nc is required for the end of loop */
    lc = l0 - 7;
    nc = *istk(lc);
    Pt = p;
    goto L47;
  } else if (Rstk[p] == 616 && *istk(1 + Pstk[p] - 1) == 9)
    {
      /*     continue in a while */
      l0 = Pstk[p];
      lc = l0;
      nc = *istk(lc);
      Pt = p;
      goto L551;
    } else {
      goto L98;
    }
  /*     abort */
 L100:
  ++Pt;
 L101:
  --Pt;
  if (Pt == 0) {
    goto L102;
  }
  if (Rstk[Pt] / 100 == 5) {
    k = Lpt[1] - (13 + nsiz);
    Lpt[1] = Lin[1 + k];
    Lpt[2] = Lin[2 + k];
    Lpt[3] = Lin[3 + k];
    Lpt[4] = Lin[4 + k];
	Lct[4] = Lin[6 + k ];
    Lpt[6] = k;
    if (Rstk[Pt] <= 502) {
      if (Pt>1) {
	if (Rstk[Pt-1] != 903 && Rstk[Pt-1] != 909 && Rstk[Pt-1] != 706)
	  Bot = Lin[5 + k];}
      else
	Bot = Lin[5 + k];
    }
    else if (Rstk[Pt] == 503) {
      if (C2F(iop).rio == C2F(iop).rte) {
	    /* abort in a pause mode */
	C2F(iop).rio = Pstk[Pt-1];
	C2F(recu).paus--;
	Bot = Lin[5 + k];}
      else {
	int mode[3];
	int lunit = -C2F(iop).rio;
	/*  abort in an exec*/
	mode[0]=0;
	C2F(clunit)(&lunit,C2F(cha1).buf,mode);
	C2F(iop).rio = Pstk[Pt-1];
      }
    }
  }
  goto L101;
 L102:
  C2F(recu).icall = 10;
  Top = 0;
  C2F(com).comp[0] = 0;
  if (C2F(recu).niv > 1) {
    Err = 9999999;
  }
  return 0;

 L105:
  /*     eol */
  /*     la gestion de la recuperation des erreurs devrait plutot se trouver */
  /*     a la fin de l'instruction (mais il n'y a pas actuellement d'indicateur */
  /*     de fin d'instruction dans les macros */
  if (C2F(errgst).err1 != 0) { 
    if (C2F(errgst).err2 == 0) {
      C2F(errgst).err2 = C2F(errgst).err1;
    }
    if (C2F(errgst).errcatch > 0) {
      /* running under errcatch(num,....) */
      if (Rstk[Pt] != 614 && Rstk[Pt] != 615 && Rstk[Pt] != 605) C2F(errgst).err1 = 0;
      if (Pt<C2F(errgst).errpt) {
	C2F(errgst).errcatch = 0;
      }
    }
    imode = (i2 = C2F(errgst).errct / 100000, abs(i2));
    if (imode - (imode / 8 << 3) == 2) {
      C2F(basbrk).iflag = TRUE;
    }
  }

  /*     gestion des points d'arrets dynamiques */
  if (C2F(dbg).nmacs != 0) { /* there are breakpoints set */
    int kfin=C2F(dbg).wmac-1; /*the stack index of the current function*/
    /*  first test if the function has breakpoints   */
    int kmac;
    for (kmac=0;kmac<C2F(dbg).nmacs;kmac++) { /* loop on table of functions containing breakpoints */
      /* does the name of the current funtion fit the registered name*/
      if (C2F(eqid)(&(C2F(vstk).idstk[kfin * nsiz]), &(C2F(dbg).macnms[kmac * nsiz]))) {/* yes */
	/* test if there is a registered breakpoint at the current line*/
	i2 = C2F(dbg).lgptrs[kmac+1] - 1;
	for (ibpt = C2F(dbg).lgptrs[kmac]; ibpt <= i2; ++ibpt) {
	  if (Lct[8] == C2F(dbg).bptlg[ibpt - 1]) { /* yes */
	    /* display a message */
	    C2F(cvname)(&C2F(dbg).macnms[kmac * nsiz], tmp, &c__1, 24L);
	    sprintf(C2F(cha1).buf,"%s %5d",tmp, Lct[8]);
	    Msgs(32, 0);
	    /* raise the interruption flag */
	    C2F(basbrk).iflag = TRUE;
	    goto L107;
	  }
	}
	break;
      }
    }
  }
 L107:

  if (Lct[4] / 2 % 2 == 1) {
    i2 = Lct[4] / 4;
    C2F(prompt)(&i2, &iesc);
  }
  ++Lct[8];
  ++lc;
  if (ismenu() == 1 && C2F(basbrk).interruptible) goto L115;

  goto L10;

  /* set line number.
   *
   *   Au debut de chaque expression liee a un then et a la fin de
   *   chaque clause, le compilateur (compcl) inscrit la valeur de la
   *   ligne. ceci permet de mettre rapidement a jour le compteur de
   *   ligne sans avoir a analyser la suite des codes operatoires */

 L110:
  Lct[8] = *istk(1 + lc);
  lc += 2;
  goto L10;

  /* gestion des evements asynchrones "interpretes" */
 L115:
  C2F(basbrk).interruptible = C2F(getmen)(C2F(cha1).buf, &lb, &nentry) == 0;
  C2F(bexec)(C2F(cha1).buf, &lb, &ierr);
  if (ierr != 0) {
    goto L10;
  }
  ++Pt;
  Ids[1 + Pt * nsiz] = lc;
  Ids[2 + Pt * nsiz] = l0;
  Ids[3 + Pt * nsiz] = nc;
  Ids[4 + Pt * nsiz] = tref;
  Rstk[Pt] = 606;
  C2F(recu).icall = 5;
  /*     *call* macro */
  return 0;
 L116:
  C2F(basbrk).interruptible = TRUE;
  lc = Ids[1 + Pt * nsiz];
  l0 = Ids[2 + Pt * nsiz];
  nc = Ids[3 + Pt * nsiz];
  tref = Ids[4 + Pt * nsiz];
  --Top;
  --Pt;
  goto L71;
    /*  r = Rstk[Pt] - 610;
  switch ((int)r) {
  case 1:  goto L74;
  case 2:  goto L71;
  case 3:  goto L72;
  case 4:  goto L73;
  case 5:  goto L73;
  case 6:  goto L73;
  }
  goto L10;*/

  /*     quit */

 L120:
  if (C2F(recu).paus!=0) {
    /*   quit in a pause: decrease recursion level up to the pause one (Rstk[Pt] == 503) */
    Pt = Pt + 1;
  L121:  
    Pt = Pt - 1;
    /*  suppress loop variables if any */
    if (Rstk[Pt]==802 || Rstk[Pt]==612 || 
	(Rstk[Pt]==805 && Ids[1 + Pt * nsiz]==iselect) ||  
	(Rstk[Pt]==616 && Pstk[Pt] ==10)) Top--;
    if (Rstk[Pt] != 503) goto L121;
    /* recall macro to terminate the pause level */
    C2F(com).fun=0;
    return 0;
  }
  else
    C2F(com).fun = 99;
  return 0;

  /*     named variable */

 L130:
  Infstk[Top] = 1;
  C2F(putid)(&C2F(vstk).idstk[Top * nsiz - nsiz], istk(1 + lc));
  lc += 7;
  goto L10;

  /*     form recursive extraction list */

 L140:
  m = *istk(2 + lc);
  if (Rstk[Pt] == 617) {
    /* runtime arg count (list extraction) */
    m += Pstk[Pt];
    Pstk[Pt] = 0;
  }
  C2F(mkindx)(istk(1 + lc), &m);
  lc += 3;
  goto L10;

  /*     exit */

 L150:
  ++lc;
  if (C2F(recu).niv > 0) {
    sciquit();
    exit(0); /* stop */
  }
  C2F(com).fun = 99;
  goto L10;

  /*     begrhs - for run time rhs value computation */
  /*              syntax like: l=list(...); a(l(:)) */

 L160:
  ++lc;
  ++Pt;
  Rstk[Pt] = 617;
  Pstk[Pt] = 0;
  goto L10;

  /*     printmode */

 L170:
  /*     print stored variable */
  if (Lct[4] >= 0 && *istk(1 + lc) != semi && kid != 0) {
    C2F(print)(id, &kid, &C2F(iop).wte);
  }
  lc += 2;
  goto L10;
 L180:
  /*     name2var */
  C2F(name2var)(istk(1 + lc));
  lc += 7;
  goto L10;

 L190:
  /*     deffnull */
  ++lc;
  ++Top;
  C2F(objvide)(" ", &Top, 1L);
  goto L10;

 L200:
  /*     profile */
  ++*istk(1 + lc);
  t = clock();
  *istk(2 + lc) = *istk(2 + lc) + t - tref;
  tref = t;
  lc += 3;
  goto L10;

 L210:
  /*     character string vector */
  if (C2F(errgst).err1 <= 0) {
    n = *istk(1 + lc) * *istk(2 + lc);
    nc = *istk(lc + 4 + n) - 1;
    ++Top;
    il = Lstk[Top] + Lstk[Top] - 1;
    i2 = il + 5 + n + nc;
    Err = i2 / 2 + 1 - Lstk[Bot];
    if (Err > 0 || C2F(errgst).err1 > 0) {
      SciError(17);
      lc = lc + 5 + n + nc;
      goto L10;
    }
    i2 = n + 5 + nc;
    C2F(icopy)(&i2, istk(lc), &c__1, istk(il), &c__1);
    *istk(il) = 10;
    i2 = il + 5 + n + nc;
    Lstk[1 + Top] = i2 / 2 + 1;
  }
  lc = lc + 5 + n + nc;
  goto L10;
 L220:
  /*     varfun */
  C2F(varfunptr)(istk(3 + lc), istk(1 + lc), istk(2 + lc));
  lc += 9;
  goto L10;
 L230:
  /*     affectation */
  Lhs = *istk(1 + lc);
  ip = *istk(2 + lc);
  li = lc + 3;
  lc = li + Lhs * 7;
  /*     following code is an adaptation of corresponding code in parse.f */
  ndel = 0;
 L231:
  Rhs = *istk(6 + li);
  lastindpos = Top - Lhs - ndel;
  if (C2F(errgst).err1 != 0) {
    goto L253;
  }
  if (Rhs == 0) {
    /* goto simple affectation */
    C2F(stackp)(istk(li), &c__0);
    if (Err > 0 || C2F(errgst).err1 > 0) {
      goto L10;
    }
    if (C2F(errgst).err1 > 0) {
      goto L253;
    }
    /* fin points on the newly saved variable */
    if (!(Lct[4] >= 0 && ip != semi && Fin != 0)) goto L253;
    ifin=Fin;
  L232:
    C2F(print)(istk(li), &ifin, &C2F(iop).wte);
    if (Rstk[Pt]!=1101) goto L253;
    ++Pt;
    Pstk[Pt] = li;
    Ids[1 + Pt * nsiz] = ndel;
    Ids[2 + Pt * nsiz] = lastindpos;
    Ids[3 + Pt * nsiz] = tref;
    Ids[4 + Pt * nsiz] = l0;
    Ids[5 + Pt * nsiz] = Lhs;
    Ids[6 + Pt * nsiz] = nc;
    Rstk[Pt]=609;
    return 0;
  L240:
    li = Pstk[Pt];
    ip = *istk(li-1);
    ndel =       Ids[1 + Pt * nsiz];
    lastindpos = Ids[2 + Pt * nsiz];
    tref =       Ids[3 + Pt * nsiz];
    l0 =         Ids[4 + Pt * nsiz];
    Lhs =        Ids[5 + Pt * nsiz];
    nc =         Ids[6 + Pt * nsiz];
    --Pt;
    /*goto L253;*/
    goto L232;

  }

  /*     take rhs (number of indices) computed at runtime into account */
  C2F(adjustrhs)();
  /*     partial variable affectation (insertion) */
  if (lastindpos + 1 != Top) {
    /* create reference variables to get index1,...,indexn, value at */
    /* the top of the stack in this order */
    /* create reference variables pointing to the  indices */
    for (ir = 1; ir <= Rhs; ++ir) {
      i2 = lastindpos - Rhs + ir;
      C2F(createref1)(&i2);
    }
    /* create reference variable pointing to the value */
    i2 = Top - Rhs;
    C2F(createref1)(&i2);
    /* remind to remove the original indices */
    ndel += Rhs;
  }
  lastindpos -= Rhs;
  /*     put a reference to the lhs variable */
  Fin = -3;
  C2F(stackg)(istk(li));
  if (Err > 0 || C2F(errgst).err1 > 0) {
    goto L10;
  }
  /*     perform insertion operation */
  /*     index1,...,indexn, value ==> updated lhs value (or pointer to) */
  if (Eptover(1)) {
    return 0;
  }
  /*     pstk(pt) is used by allops to get the name of output variable */
  Pstk[Pt] = li;
  Ids[1 + Pt * nsiz] = ndel;
  Ids[2 + Pt * nsiz] = lastindpos;
  Ids[3 + Pt * nsiz] = tref;
  Ids[4 + Pt * nsiz] = l0;
  Ids[5 + Pt * nsiz] = Lhs;
  Ids[6 + Pt * nsiz] = nc;
  Rstk[Pt] = 607;
  Rhs += 2;
  Lhs = 1;
  C2F(recu).icall = 4;
  Fin = insert;
  /*     *call* allops(insert) */
  return 0;
 L250:
  li = Pstk[Pt];
  ip = *istk(li-1);
  ndel =       Ids[1 + Pt * nsiz];
  lastindpos = Ids[2 + Pt * nsiz];
  tref =       Ids[3 + Pt * nsiz];
  l0 =         Ids[4 + Pt * nsiz];
  Lhs =        Ids[5 + Pt * nsiz];
  nc =         Ids[6 + Pt * nsiz];
  --Pt;
  /*     store the updated value */
  C2F(stackp)(istk(li), &c__0);

  if (Err > 0 || C2F(errgst).err1 > 0) {
    goto L10;
  }
  if (C2F(errgst).err1 > 0) {
    goto L253;
  }
  /*     fin points on the newly saved variable */
  if (!(Lct[4] >= 0 && ip != semi && Fin != 0))  goto L252;
  ifin=Fin;
 L251:
  C2F(print)(istk(li), &ifin, &C2F(iop).wte);
  if (Rstk[Pt]!=1101) goto L252;
  ++Pt;
  Pstk[Pt] = li;
  Ids[1 + Pt * nsiz] = ndel;
  Ids[2 + Pt * nsiz] = lastindpos;
  Ids[3 + Pt * nsiz] = tref;
  Ids[4 + Pt * nsiz] = l0;
  Ids[5 + Pt * nsiz] = Lhs;
  Ids[6 + Pt * nsiz] = nc;
  Rstk[Pt]=608;
  return 0;
 L254:
  li = Pstk[Pt];
  ip = *istk(li-1);
  ndel =       Ids[1 + Pt * nsiz];
  lastindpos = Ids[2 + Pt * nsiz];
  tref =       Ids[3 + Pt * nsiz];
  l0 =         Ids[4 + Pt * nsiz];
  Lhs =        Ids[5 + Pt * nsiz];
  nc =         Ids[6 + Pt * nsiz];
  --Pt;
  goto L251;

 L252:
  /*     remove variable containing the value if required */
  if (lastindpos != Top)   --Top;

 L253:
  li += 7;
  --Lhs;
  if (Lhs > 0) {
    goto L231;
  }
  Top -= ndel;
  lc = li;
  goto L10;

  /*     logical expression shortcircuit */
 L260:
  if (*istk(1 + lc) == 1) {
    /* | case */
    if (C2F(gettype)(&Top) != sci_ints && Istrue(0)) {
      lc += *istk(2 + lc);
    }
  } else {
    /* & case */
    if (C2F(gettype)(&Top) != sci_ints && ! Istrue(0)) {
      lc += *istk(2 + lc);
    }
  }
  lc += 3;
  goto L10;
 /*     comment */
 L261:

  lc += 2+*istk(1 + lc);
  goto L10;

 /*     try catch */
 L270:
  nc = *istk(1 + lc);
  lc += 3;
  l0 = lc;
  if (Ptover(1)) {
    lc += nc;
    lc += nsiz + *istk(lc);
    goto L10;
  }
  Rstk[Pt] = 618;
  Ids[1 + Pt * nsiz] = l0;
  /* preserve current error modes */
  Ids[2 + Pt * nsiz] = C2F(errgst).errct;
  Ids[3 + Pt * nsiz] = C2F(errgst).err2;
  Ids[4 + Pt * nsiz] = C2F(errgst).err1;
  Ids[5 + Pt * nsiz] = C2F(errgst).errpt;
  Ids[6 + Pt * nsiz] = (Lct[4]+100)+10000*C2F(com).sym;
  /* set error recovery mode without message*/
  C2F(errgst).errct = -(900000+1);
  C2F(errgst).errpt = Pt;
  Pstk[Pt] = Top;
  goto L10;
 L271:
  /* try op-codes finished*/
  l0 = Ids[1 + Pt * nsiz];
  /*check if an error occured*/
  ok = Max(C2F(errgst).err2,C2F(errgst).err1)<=0;
  /* restore preserved error modes */
  C2F(errgst).errct = Ids[2 + Pt * nsiz];
  C2F(errgst).err2  = Ids[3 + Pt * nsiz];
  C2F(errgst).err1  = Ids[4 + Pt * nsiz];
  C2F(errgst).errpt = Ids[5 + Pt * nsiz];
  C2F(com).sym      = Ids[6 + Pt * nsiz]/10000;
  Lct[4]            = Ids[6 + Pt * nsiz]-10000*C2F(com).sym - 100;
  if (ok) {
    /* no error occured in the try part*/
    nc = *istk(l0-1);
    lc += nc; /*skip catch  instructions*/
    /* finish try catch context and continue*/
    --Pt;
    goto L70;
  }
  /*an error occured in the try part*/
  lc = l0+*istk(l0-2);/*skip remaining try instruction*/
  nc = *istk(l0-1);
  /*execute catch instructions (next op-codes)*/
  l0 = lc;
  Rstk[Pt] = 619;
  goto L10;
 L272:
  /* catch op-codes finished*/
  /* close "try catch" context and continue*/
  --Pt;
  goto L70;


 L998:
  Lhs = 0;
 L999:
  /*remove context down to current running macro */
  if (Rstk[Pt] != 501) {
    --Pt;
    goto L999;
  }
  C2F(com).fun = 0;
  return 0;
#undef ix
#undef x
}



int C2F(adjustrhs)(void)
{
  /* to adjust rhs in the case it is only fixed at run time example
  *  l=list(....); foo(a,l(2:3)).  the parser supposes that the rhs
  *  for foo is 2. at run time it is really 3. See begrhs.
  * Copyright INRIA
  * Author S. Steer
  */
  static int    *Ids  = C2F(recu).ids-nsiz-1;
  static int    *Rstk = C2F(recu).rstk-1;
  static int    *Pstk = C2F(recu).pstk-1;

  if (Rstk[Pt] == 617) {
    Rhs += Pstk[Pt];
    --Pt;
  } else if (Rstk[Pt] == 501) {
    /* retained for 2.4.1 compatiblity */
    Rhs += Ids[5 + Pt * nsiz];
    Ids[5 + Pt * nsiz] = 0;
  }
  return 0;
}
