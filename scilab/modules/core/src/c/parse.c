/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1984-2010 - INRIA - Serge STEER
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------
 * Scilab parsing function
 *
 * Code automatically translated from Fortran to C
 *------------------------------------------------------------------ */
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
#include "parse.h"
#include "basout.h"
#include "core_math.h"
#include "cvstr.h"
#include "msgs.h"
#include "scilabmode.h"
#include "stack-def.h" /* C2F(basbrk) */
#include "Scierror.h"
#include "do_error_number.h"
#include "prompt.h"
#include "storeCommand.h"
#include "msgs.h"
#include "eqid.h"
/*--------------------------------------------------------------------------*/
#undef Lstk
#undef Infstk
/*--------------------------------------------------------------------------*/

/* Table of constant values */

static int c__1 = 1;
static int c__0 = 0;
#define comma  52
#define lparen  41
#define rparen  42
#define left  54
#define right  55
#define less  59
#define great  60
#define dot  51
#define name  1
#define num  0
#define cmt  2
#define insert  2
#define extrac  3
#define blank  40
#define semi  43
#define slash  48

#define equal  50
#define eol  99

#define Pt (C2F(recu).pt)
/*--------------------------------------------------------------------------*/
extern int C2F(clause)(void);
extern int C2F(fact)(void);
extern int C2F(expr)(void);
extern int C2F(terme)(void);
extern int C2F(stackp)(int *,int *);
extern int C2F(macro)(void);
extern int C2F(getsym)(void);

extern int C2F(bexec)(char *,int *,int *);

extern int C2F(findequal)(int *);
extern int C2F(print)(int *,int *,int *);
extern int C2F(createref1)(int *);
extern int C2F(command)(int *,int *);
extern int C2F(getlin)(int *,int *);
extern int C2F(mrknmd)(void);
extern int C2F(mkindx)(int *,int *);
extern int C2F(whatln)(int *,int *, int *,int *,int *,int *);
extern int C2F(prompt)(int *,int *);
extern int C2F(seteol)(void);
extern int C2F(run)(void);
extern int C2F(name2var)(int *);
extern int C2F(compil)(int *,int *,int *,int *,int *);
extern int C2F(ptover)(int *,int *); /* see src/fortran/ptover.f */
extern int C2F(eptover)(int *,int *);/* see src/fortran/eptover.f */
/*--------------------------------------------------------------------------*/
void handle_onprompt(int *n);
/*--------------------------------------------------------------------------*/
static int Compil(int code,int * val1,int val2,int val3,int val4)
{
  return C2F(compil)(&code, val1, &val2, &val3, &val4);
}
/*--------------------------------------------------------------------------*/
int Eptover(int n)
{
  int c;
  return C2F(eptover)(&n,(c=psiz,&c));
}
/*--------------------------------------------------------------------------*/
int Ptover(int n)
{
  int c;
  return C2F(ptover)(&n,(c=psiz,&c));
}
/*--------------------------------------------------------------------------*/
int C2F(parse)(void)
{
  /* Initialized data */
  static int ans[6] = { 672929546,673720360,673720360,673720360,
                        673720360,673720360 };
  static int varargout[6] = { 169544223,504893467,673720349,673720360,
                              673720360,673720360 };
  /* static int catch[6] = {203229708,673720337,673720360,673720360, 673720360,673720360 };*/

  static int *Ids     = C2F(recu).ids - nsiz - 1;
  static int *Rstk    = C2F(recu).rstk - 1;
  static int *Pstk    = C2F(recu).pstk - 1;
  static int *Lstk    = C2F(vstk).lstk - 1;
  static int *Lin     = C2F(iop).lin - 1;
  static int *Lct     = C2F(iop).lct - 1;
  static int *Lpt     = C2F(iop).lpt - 1;

  /* System generated locals */
  int i__2, i__3;

  /* Local variables */
  static int iesc, ndel, ifin, ibpt;
  static int iret;
  static int topk;
  static int lpts, next;
  static int ierr;
  static int k, p, r;
  static int imode, schar;
  static int itime, where_;
  static int found;
  static int excnt;
  static int l1;
  static int id[6], lb, io, ir;
  static int dotsep;
  static int icount;
  static int nentry, lastindpos;
  static int job, nlc, pts;
  static char tmp[80];

  /* Used to manage space between prompts */
  static int returnFromCallbackExec = FALSE;

  /* Retrieve the current Scilab Mode */
  /*  scilabMode sciMode=getScilabMode();*/

  itime = 10000;
 L1:
  r = 0;
  if (Pt > 0) {
    r = Rstk[Pt];
  }
  if (C2F(iop).ddt == 4) {
    sprintf(tmp," TOP    pt:%d rstk(pt):%d icall: %d niv: %d err:%d",
            Pt,Rstk[Pt], C2F(recu).icall, C2F(recu).niv,Err);
    C2F(basout)(&io, &C2F(iop).wte,tmp, (long)strlen(tmp));
  }

  if (C2F(recu).icall == 5) {
    goto L88;
  }
  if (Pt > 0) {
    goto L86;
  }
  if (Err > 0) {
    goto L98;
  }
  /*     initialization */
  /* ------------------- */
 L5:
  C2F(com).sym = eol;
  job = 0;
  if (C2F(iop).rio == -1) {
    job = -1;
  }
  Top = 0;
  Fin = 0;
  C2F(recu).macr = 0;
  C2F(recu).paus = 0;
  C2F(recu).icall = 0;
  C2F(iop).rio = C2F(iop).rte;
  Lct[3] = 0;
  Lct[4] = 2;
  Lpt[1] = 1;
  if (job == -1) {
    goto L13;
  }

 L10:
  if (C2F(com).sym != eol) {
    goto L15;
  }
  if (C2F(com).comp[0] == 0) {
    goto L12;
  }
  if (Lin[Lpt[4]+1] == eol) {
    goto L88;
  }

  /*     get a new line */
  /* ------------------- */
 L12:
  if (Lct[4] <= -10) {
    Lct[4] = -Lct[4] - 11;
  } else {
    if (Lct[4] / 2 % 2 == 1) {
      i__2 = Lct[4] / 4;
      /* Manage space between two prompts */
      if (!returnFromCallbackExec)
        {
          /* Space added only if Scilab does not return form a callback execution */
          C2F(prompt)(&i__2, &iesc);
        }
      else
        {
          /* Reset the flag indicating a callback has just been executed */
          returnFromCallbackExec = FALSE;
        }
      if (iesc == 1) {
        /* interrupted line acquisition (mode=7) */
        iret = 3;
        goto L96;
      }
      Lct[1] = 0;
      if (C2F(recu).paus == 0 &&
          C2F(iop).rio == C2F(iop).rte &&
          C2F(recu).macr ==  0) {
        if (Pt != 0) {
          Msgs(30,0);
          Pt = 0;
        }
        if (Top != 0) {
          Msgs(31,0);
          Top = 0;
        }
      }
    }
  }
 L13:
  //C2F(tksynchro)(&C2F(recu).paus);

  C2F(getlin)(&job, &c__1);

  ClearTemporaryPrompt();
  //C2F(tksynchro)(&c_n1);


  if (Fin == -3) {
    /*     interrupted line acquisition */
    iret = 2;
    goto L96;
  } else if (Fin == -1) {
    /*     Continuation line handling when scilab is called as a routine */
    C2F(com).fun = 99;
    return 0;
  }
  job = 0;
  Err = 0;

  if (Pt != 0) {
    goto L15;
  }
 L14:
  handle_onprompt(&where_);
  if (Err > 0) {
    goto L98;
  }
  switch ((int)where_) {
  case 1:  goto L85;
  case 2:  goto L88;
  }
  /*     Beginning of a new statement, clause expression or command */
  /* ------------------------------------------------------------ */
 L15:
  if (ismenu() == 1 && C2F(basbrk).interruptible) {
    iret = 1;
    goto L96;
  }

  r = 0;
  if (Pt > 0) {
    r = Rstk[Pt];
  }
  if (C2F(iop).ddt == 4) {
    sprintf(tmp," parse  pt:%d rstk(pt):%d top: %d niv: %d err:%d",
            Pt,r,Top, C2F(recu).niv,Err);
    C2F(basout)(&io, &C2F(iop).wte,tmp, (long)strlen(tmp));
  }

  excnt = 0;
  if (! C2F(basbrk).iflag || C2F(com).comp[0] != 0 ) {
    goto L18;
  }
  /*     Handling of pauses */
 L16:
  if (Eptover(1)) {
    goto L98;
  }
  Pstk[Pt] = C2F(iop).rio;
  Ids[2 + Pt * nsiz] = Top;
  C2F(iop).rio = C2F(iop).rte;
  Rstk[Pt] = 701;
  C2F(basbrk).iflag = FALSE;
  Fin = 2;
  if (Lct[4] <= -10) {
    Fin = -1;
    Lct[4] = -Lct[4] - 11;
  }
  /*     *call* macro */
  goto L88;
  /*     pauses termination */
 L17:
  C2F(iop).rio = Pstk[Pt];
  Top = Ids[2 + Pt * nsiz];
  --Pt;
  goto L15;
 L18:
  Lhs = 1;
  excnt = 0;
  C2F(putid)(id, ans);
  /*     preserve current character position */
  lpts = Lpt[4] - 1;
  pts = Pt;

  C2F(getsym)();
  if (C2F(com).sym == right || C2F(com).sym == rparen || C2F(com).sym == less ||
      C2F(com).sym == great) {
    SciError(1);
    goto L98;
  } else if (C2F(com).sym == semi || C2F(com).sym == comma || C2F(com).sym == eol) {
    goto L77;
  } else if (C2F(com).sym == cmt) {
    C2F(parsecomment)();
    goto L77;
  } else if (C2F(com).sym == name) {
    lpts = Lpt[3] - 1;
    /* try to avoid the command call whenever it is possible */
    if (C2F(com).char1 == equal) {
      goto L25;
    }
    /*         if (char1 .eq. lparen) then */
    /*           one can get this case with "if ( ) then" */
    /*         endif */
    if (Lpt[4] >= 2) {
      if (Lin[Lpt[4] - 2] == blank) {
        goto L20;
      }
    }
    if (C2F(com).char1 == dot) {
      /* name.x */
      schar = C2F(com).char1;
      goto L30;
    }
    goto L20;
  } else if (C2F(com).sym == left) {
    /* is there an explicit affectation */
    lpts = Lpt[3] - 1;
    Lpt[4] = lpts;
    Lpt[3] = lpts;
    C2F(com).char1 = blank;
    C2F(findequal)(&found);
    if (Err > 0) {
      goto L98;
    }
    if (found) {
      /* multiple lhs */
      goto L40;
    } else {
      /* no ==> implicit lhs */
      goto L50;
    }
  } else {
    /* not lhs defined */
    /* set ans for lhs */
    if (Eptover(1)) {
      goto L98;
    }
    C2F(putid)(&Ids[1 + Pt * nsiz], ans);
    Lhs = 1;
    Pstk[Pt] = 0;
    /* go to rhs analysis */
    goto L60;
  }

  /*     lhs begins with name */
  /* ------------------------- */
  /*     check if it is a simple command like clear,... */
 L20:
  C2F(command)(C2F(com).syn, &c__0);
  if (Err > 0) {
    goto L98;
  }
  if (Fin == 0) {
    goto L21;
  }
  if (C2F(com).fun == 99) {
    return 0;
  }
  if (C2F(com).fun != 0) {
    goto L93;
  }
  if (Fin < 0) {
    goto L80;
  }
  if (Fin == 2) {
    goto L88;
  }
  if (Fin == 3) {
    goto L16;
  }
  if (Fin == 4) {
    goto L5;
  }
  if (Fin > 0) {
    goto L77;
  }
  /*     name is not a command */
 L21:
  Rhs = 0;
  Fin = -5;
  /* IL y a p avec fin=-5 (on ne trouve pas les macros parce que l'on ne */
  /* veut pas que les macros sans arg soient vues comme des commandes */
  /* mais pourquoi pas il suffirait de dire que pour visualiser une macro */
  /* il faut faire disp() */
  C2F(com).fun = 0;
  C2F(funs)(C2F(com).syn);
  if (Fin > 0) {
    /*        name is a builtin name */
    if (C2F(com).char1 == equal) {
      /*           fun=expr is not allowed */
      C2F(putid)(&Ids[1 + (Pt + 1) * nsiz], C2F(com).syn);
      SciError(25);
      goto L98;
    }
    /*        skip lhs analysis */
    if (Eptover(1)) {
      goto L98;
    }
    C2F(putid)(&Ids[1 + Pt * nsiz], id);
    Lhs = 1;
    Pstk[Pt] = 0;
    /* go to rhs analysis */
    goto L60;
  }

  /*     peek one character ahead */
  if (C2F(com).char1 == semi || C2F(com).char1 == comma || C2F(com).char1 == eol) {
    C2F(putid)(id, C2F(com).syn);
  }
  if (C2F(com).char1 == lparen) {
    schar = C2F(com).char1;
    goto L30;
  }
  /*     instruction is just "name", skip lhs analysis */
  /*     record name as lhs */
  if (Eptover(1)) {
    goto L98;
  }
  C2F(putid)(&Ids[1 + Pt * nsiz], id);
  Lhs = 1;
  Pstk[Pt] = 0;
  /*     go to rhs analysis */
  goto L60;

  /*     name = expr  or name == expr syntax */
  /* ---------------------------------------- */
 L25:
  C2F(putid)(id, C2F(com).syn);
  lpts = Lpt[2];
  C2F(getsym)();
  if (C2F(com).char1 == equal) {
    /* name == expr syntax ==> implicit lhs */
    goto L50;
  } else {
    /* lhs found */
    /* record it */
    if (Eptover(1)) {
      goto L98;
    }
    C2F(putid)(&Ids[1 + Pt * nsiz], id);
    Lhs = 1;
    Pstk[Pt] = 0;
    C2F(getsym)();
    /* go to rhs analysis */
    goto L60;
  }

  /*     lhs is name(...) or name.x... */
  /* ----------------------------------- */
 L30:
  Lpt[5] = Lpt[4];
  C2F(putid)(id, C2F(com).syn);

  /*     looking for equal to check if it is really an lhs */
  C2F(findequal)(&found);
  if (Err > 0) {
    goto L98;
  }
  if (! found) {
    goto L50;
  }
  /*     It is really a lhs (insertion syntax) */
 L32:
  Lpt[4] = Lpt[5];
  C2F(com).char1 = schar;

  /* 35     call parseindexlist(excnt) */
  /*     if(err.gt.0) goto 98 */
  if (Compil(21, &c__0, 0, 0, 0)) {
    if (Err > 0) {
      return 0;
    }
  }
  /*     begin the index lists */
  icount = 0;
  C2F(getsym)();

 L33:
  /*     begin a new index list (.,..) or .name */
  ++icount;

  dotsep = C2F(com).sym == dot;
  C2F(getsym)();
  if (dotsep) {
    /* --> new index list is .name */
    if (C2F(com).sym != name) {
      SciError(21);
      if (Err > 0) {
        return 0;
      }
    }

    if (C2F(com).comp[0] != 0) {
      if (Compil(23, C2F(com).syn, 0, 0, 0)) {
        if (Err > 0) {
          return 0;
        }
      }
    } else {
      C2F(name2var)(C2F(com).syn);
    }
    C2F(getsym)();
    /*         icount=icount+1 */
    if (C2F(com).sym == dot) {
      goto L33;
    }
    dotsep = FALSE;
    excnt = 1;
    goto L36;
  }
  /*     --> new index list is (.,..) */
 L34:
  /*     add a new index in index list (i,...) */
  ++excnt;
  if (Eptover(1)) {
    goto L98;
  }
  C2F(putid)(&Ids[1 + Pt * nsiz], id);
  Pstk[Pt] = excnt + icount * 1000;
  Rstk[Pt] = 702;
  /*     *call* expr */
  goto L81;
 L35:
  C2F(putid)(id, &Ids[1 + Pt * nsiz]);
  icount = Pstk[Pt] / 1000;
  excnt = Pstk[Pt] - icount * 1000;
  --Pt;

  if (C2F(com).sym == comma) {
    /* current syntax is (i,j,..) */
    C2F(getsym)();
    goto L34;
  }

  if (C2F(com).sym == rparen) {
    /* end of the current index list */
    C2F(getsym)();
  } else {
    SciError(3);
    if (Err > 0) {
      goto L98;
    }
  }

 L36:
  if (C2F(com).sym == lparen || C2F(com).sym == dot) {
    /* begining of a a new index list */
    /* first memorize the previous one */
    if (excnt > 1) {
      /* previously analysed syntax is (i,j,..)( */
      if (C2F(com).comp[0] == 0) {
        /* form  list with individual indexes i,j,.. */
        C2F(mkindx)(&c__0, &excnt);
        if (Err > 0) {
          return 0;
        }
      } else {
        if (Compil(19, &c__0, excnt, 0, 0)) {
          if (Err > 0) {
            return 0;
          }
        }
      }
      excnt = 1;
    }
    /* open a new index list */
    excnt = 0;
    /*         icount=icount+1 */
    goto L33;
  }
  /*     end of all the index lists */
  if (icount > 1) {
    /* form  list with individual indexes */
    if (C2F(com).comp[0] == 0) {
      C2F(mkindx)(&icount, &excnt);
      if (Err > 0) {
        return 0;
      }
    } else {
      if (Compil(19, &icount, excnt, 0, 0)) {
        if (Err > 0) {
          return 0;
        }
      }
    }
    excnt = 1;
  }
  /*     end of code for recursive index */
  if (Rstk[Pt] == 711) {
    goto L42;
  }

  if (C2F(com).sym == equal) {
    /* name(...) = expr syntax */
    /* record name as lhs */
    if (Eptover(1)) {
      goto L98;
    }
    C2F(putid)(&Ids[1 + Pt * nsiz], id);
    Pstk[Pt] = excnt;
    Lhs = 1;
    C2F(getsym)();
    /* go to rhs analysis */
    goto L60;
  } else {
    /* this should never happen. this case has been detected above */
    /* when lookin for equal sign */
    /* name(...) syntax ==> really an rhs */
    if (Eptover(1)) {
      goto L98;
    }
    C2F(putid)(&Ids[1 + Pt * nsiz], ans);
    Lhs = 1;
    Pstk[Pt] = 0;
    goto L60;
  }

  /*     multiple lhs [a,...]= or [a(..),..] */
  /* ---------------------------------------- */
 L40:
  Lpt[4] = lpts;
  Lpt[3] = lpts;
  C2F(com).char1 = blank;
  Lhs = 0;
  C2F(getsym)();
  /*     start lhs arguments list */
  C2F(getsym)();
 L41:
  /*     begin analysis of a new lhs argument */
  if (C2F(com).sym != name) {
    SciError(274);
    goto L98;
  }
  if (C2F(eqid)(C2F(com).syn, varargout)) {
    SciError(275);
    goto L98;
  }
  C2F(putid)(id, C2F(com).syn);
  excnt = 0;
  next = Lin[Lpt[4] - 2];
  if (next == blank || C2F(com).char1 == comma || C2F(com).char1 == right) {
    /* argument followed by a blank, a comma or a ] ==> it is a simple name */
    C2F(getsym)();
    goto L44;
  } else if (C2F(com).char1 != lparen && C2F(com).char1 != dot) {
    /* invalid lhs */
    SciError(273);
    goto L98;
  }
  /*     lhs argument is name(..) or name.xx */
  if (Eptover(1)) {
    goto L98;
  }
  Rstk[Pt] = 711;
  Lpt[5] = Lpt[4];
  schar = C2F(com).char1;
  goto L32;
  /*     *parse* index */
 L42:
  --Pt;
  goto L44;
 L44:
  /*     record current lhs arg */
  if (Eptover(1)) {
    goto L98;
  }
  C2F(putid)(&Ids[1 + Pt * nsiz], id);
  ++Lhs;
  Pstk[Pt] = excnt;
  Rstk[Pt] = 0;
  /*     end analysis of a current lhs arg */
  if (C2F(com).sym == right) {
    goto L46;
  }
  if (C2F(com).sym == comma) {
    C2F(getsym)();
  }
  /*     loop on lhs args */
  goto L41;

 L46:
  C2F(getsym)();
  if (C2F(com).sym == equal && C2F(com).char1 != equal) {
    /*        really found a lhs go to  the rhs analysis part */
    C2F(getsym)();
    goto L60;
  }
  /*     lhs revealed to be an rhs */
  /* ------------------------------------- */
 L50:
  /*     no equal symbol */
  goto L51;
 L51:
  /*     lhs is in fact an  rhs */
  /*     1 - reinititialise the parser at the instruction beginning */
  Pt = pts;
  Lpt[4] = lpts;
  Lpt[3] = lpts;
  C2F(com).char1 = blank;
  /* L52: */
  C2F(getsym)();
  /*      if(sym.eq.comma.or.sym.eq.semi) goto 52 */
  /*     2 - make "ans" the lhs */
  if (Eptover(1)) {
    goto L98;
  }
  C2F(putid)(&Ids[1 + Pt * nsiz], ans);
  Pstk[Pt] = 0;
  Lhs = 1;
  /*     3 - go to the rhs analysis part */
  goto L60;

  /*     lhs finished, start rhs */
  /* ---------------------------- */
 L60:
  Rstk[Pt] = 703;
  /*     *call* expr */
  goto L81;
 L65:
  if (Rstk[Pt - Lhs] == 313) {
    /*     store  new variable as "named" at the top of the stack */
    if (C2F(com).sym == rparen || C2F(com).sym == comma) {
      C2F(mrknmd)();
      if (Err > 0) {
        goto L98;
      }
      goto L83;
    } else {
      SciError(40);
      goto L98;
    }
  }
  if (C2F(com).sym == semi || C2F(com).sym == comma || C2F(com).sym == eol|| C2F(com).sym == cmt) {
    goto L70;
  }
  if (Rstk[Pt - Lhs] == 808) {/* syntax error while in try */
    /* set back the standard error handling ++++*/
    Pt=Pt-Lhs;
    C2F(errgst).errct = Ids[2 + Pt * nsiz];
    C2F(errgst).errpt = Ids[5 + Pt * nsiz];
    /* forgot the catch error */
    C2F(errgst).err1 = Ids[3 + Pt * nsiz];
    C2F(errgst).err2 = Ids[4 + Pt * nsiz];
    C2F(com).comp[0] = 0;
    Lpt[2] = Lpt[3]+1;
    SciError(276);
    Pt = Pt - 1;
    goto L98;
  } else {
    Lpt[2] = Lpt[3]+1;
    SciError(276);
    goto L98;
  }

  /*     store results */
  /* ------------------- */
 L70:
  Lhs = Max(Lhs,1);
  if (Compil(29, &(Lhs), C2F(com).sym, 0, 0)) {
    if (Err > 0) {
      return 0;
    }
    Pt -= Lhs;
    Lhs = 0;
    goto L77;
  }
  ndel = 0;
 L71:
  Rhs = Pstk[Pt];
  lastindpos = Top - Lhs - ndel;
  if (C2F(errgst).err1 != 0) {
    goto L76;
  }
  if (Rhs == 0) {
    /* goto simple affectation */
    C2F(stackp)(&Ids[1 + Pt * nsiz], &c__0);
    if (Err > 0) {
      goto L98;
    }
    if (C2F(errgst).err1 > 0) {
      if (C2F(errgst).err1 != 13||Rstk[Pt]!=502) --Pt;
      --Lhs;
      goto L98;
    }
    /* topk points on the newly saved variable */
    topk = Fin;
    /* go to print */
    goto L73;
  }
  /*     partial variable affectation (insertion) */
  if (lastindpos + 1 != Top) {
    /* create reference variables to get index1,...,indexn, value at */
    /* the top of the stack in this order */
    /* create reference variables pointing to the  indices */
    i__2 = Rhs;
    for (ir = 1; ir <= i__2; ++ir) {
      i__3 = lastindpos - Rhs + ir;
      C2F(createref1)(&i__3);
    }
    /* create reference variable pointing to the value */
    i__2 = Top - Rhs;
    C2F(createref1)(&i__2);
    /* remind to remove the original indices */
    ndel += Rhs;
  }
  lastindpos -= Rhs;
  /*     put a reference to the lhs variable */
  Fin = -3;
  C2F(stackg)(&Ids[1 + Pt * nsiz]);
  if (Err > 0) {
    goto L98;
  }
  /*     perform insertion operation */
  /*     index1,...,indexn, value ==> updated lhs value (or pointer to) */
  if (Eptover(1)) {
    goto L98;
  }
  Pstk[Pt] = Lhs;
  C2F(putid)(&Ids[1 + Pt * nsiz], &Ids[1 + (Pt - 1) * nsiz]);
  Ids[1 + (Pt - 1) * nsiz] = ndel;
  Ids[2 + (Pt - 1) * nsiz] = lastindpos;
  Rstk[Pt] = 704;
  Rhs += 2;
  Lhs = 1;
  Fin = insert;
  /*     *call* allops(insert) */
  goto L91;
 L72:
  Lhs = Pstk[Pt];
  ndel = Ids[1 + (Pt - 1) * nsiz];
  lastindpos = Ids[2 + (Pt - 1) * nsiz];
  C2F(putid)(&Ids[1 + (Pt - 1) * nsiz], &Ids[1 + Pt * nsiz]);
  --Pt;
  /*     store the updated value */
  C2F(stackp)(&Ids[1 + Pt * nsiz], &c__0);
  if (Err > 0) {
    goto L98;
  }
  if (C2F(errgst).err1 > 0) {
    --Pt;
    --Lhs;
    goto L98;
  }
  /*     topk points on the newly saved variable */
  topk = Fin;
  /*     remove variable containing the value if required */
  if (lastindpos != Top) {
    --Top;
  }
 L73:
  /*     print if required */
  /* ---------------------- */
  if (Lct[4] < 0 || Fin == 0) {
    goto L76;
  }
  if (! ((C2F(com).sym != semi && Lct[3] == 0) || (C2F(com).sym == semi &&
                                                   Lct[3] == 1))) {
    goto L76;
  }
 L74:
  C2F(print)(&Ids[1 + Pt * nsiz], &topk, &C2F(iop).wte);
  if (Err > 0) {
    goto L98;
  }
  if (topk == 0) {
    goto L76;
  }
  /*     overloaded display, call a macro */
  if (Eptover(1)) {
    goto L98;
  }
  Rstk[Pt] = 708;
  Pstk[Pt] = C2F(com).sym;
  Ids[1 + Pt * nsiz] = C2F(com).sym;
  Ids[2 + Pt * nsiz] = ndel;
  Ids[3 + Pt * nsiz] = Lhs;
  if (C2F(com).fun == 0) {
    goto L88;
  }
  goto L85;
 L75:
  C2F(com).sym = Pstk[Pt];
  ndel = Ids[2 + Pt * nsiz];
  Lhs =  Ids[3 + Pt * nsiz];
  --Pt;
  goto L74;

 L76:
  --Pt;
  --Lhs;
  if (Lhs > 0) {
    goto L71;
  }
  Top -= ndel;

  /*     finish statement */
  /* --------------------- */
 L77:
  Fin = 0;
  p = 0;
  r = 0;
  if (Pt > 0) p = Pstk[Pt];
  if (Pt > 0) r = Rstk[Pt];
  if (C2F(iop).ddt == 4) {
    sprintf(tmp," finish  pt:%d rstk(pt):%d  pstk(pt):%d lpt(1): %d niv: %d macr:%d, paus:%d",
            Pt,r,p, Lpt[1],C2F(recu).niv,C2F(recu).macr,C2F(recu).paus);
    C2F(basout)(&io, &C2F(iop).wte,tmp, (long)strlen(tmp));
  }
  if (C2F(errgst).err1 != 0) {
    /* a catched error has occured */
    if (r == 808) {
      /* in try instructions */
      goto L80;
    } else if (Ids[1 + (Pt - 1) * nsiz] != 0) {
      /* execution is explicitly required to be stopped */
      if (r == 502 && Rstk[Pt - 1] == 903) {
        /* in an execstr(...,'errcatch') instruction */
        goto L88;
      } else if (r == 502 && Rstk[Pt - 1] == 909) {
        /* in an exec(function,'errcatch') instruction */
        goto L88;
      } else if (r == 503 && Rstk[Pt - 1] == 902) {
        /* in an exec(file,'errcatch') instruction */
        goto L88;
      }
    }
    if (C2F(errgst).err2 == 0) {
      C2F(errgst).err2 = C2F(errgst).err1;
    }
    if (C2F(errgst).errcatch > 0) {
      /* running under errcatch(num,....) */
      C2F(errgst).err1 = 0;
      if (Pt<C2F(errgst).errpt) C2F(errgst).errcatch = 0;
    }

    imode = (i__2 = C2F(errgst).errct / 100000, abs(i__2));
    if (imode - (imode / 8 << 3) == 2) {
      C2F(basbrk).iflag = TRUE;
      goto L16;
    }
  }
  C2F(errgst).toperr = Top;
  /*     fin instruction */
  if (C2F(com).sym != eol) {
    if (C2F(com).sym == cmt){
      C2F(parsecomment)();}
    else
      goto L15;
  }

  /*     gestion des points d'arrets dynamiques */
  if (C2F(dbg).nmacs != 0) { /* there are breakpoints set */
    int kfin=C2F(dbg).wmac-1; /*the stack index of the current function*/
    /*  first test if the function has breakpoints   */
    int kmac;
    int curline;
    for (kmac=0;kmac<C2F(dbg).nmacs;kmac++) { /* loop on table of functions containing breakpoints */
      /* does the name of the current funtion fit the registered name*/
      if (C2F(eqid)(&(C2F(vstk).idstk[kfin * nsiz]), &(C2F(dbg).macnms[kmac * nsiz]))) {/* yes */
        /* test if there is a registered breakpoint at the current line*/
        i__2 = Lpt[2] - 1;
        C2F(whatln)(&Lpt[1], &i__2, &Lpt[6], &nlc, &l1, &ifin);
        i__2 = C2F(dbg).lgptrs[kmac+1] - 1;
        curline = Lct[8] - nlc - 1;
        for (ibpt = C2F(dbg).lgptrs[kmac]; ibpt <= i__2; ++ibpt) {
          //sciprint("la Lct[8]-nlc =%d, bptlg=%d\n",Lct[8] - nlc,C2F(dbg).bptlg[ibpt - 1]);
          if (curline == C2F(dbg).bptlg[ibpt - 1]) { /* yes */
            /* display a message */
            C2F(cvname)(&C2F(dbg).macnms[kmac * nsiz], tmp, &c__1, nlgh);
            sprintf(C2F(cha1).buf,"%s %5d",tmp, curline);
            Msgs(32, 0);
            /* raise the interruption flag */
            C2F(basbrk).iflag = TRUE;
            goto L79;
          }
        }
        break;
      }
    }
  }
 L79:

  if (C2F(com).comp[0] != 0) {
    C2F(seteol)();
  }
  /*     EOL */
  if (r / 100 != 8) {
    goto L10;
  }
  /*     end of an instruction or a clause */
  if (C2F(com).comp[0] != 0) {
    k = Lpt[6];
    if (Lin[k - 1] == eol && Lin[k] == eol) {
      SciError(47);
      goto L98;
    }
  }
  if (Lpt[4] == Lpt[6]) {
    /*         call getlin(1,0) */
    goto L13;
  } else {
    ++Lpt[4];
    C2F(getsym)();
  }
  goto L15;

  /*     simulate recursion */
  /* ----------------------- */
 L80:
  C2F(recu).icall = 0;
  C2F(clause)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  if (Pt <= 0) {
    goto L15;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L99;

 L81:
  C2F(recu).icall = 0;
  C2F(expr)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L99;

 L82:
  C2F(recu).icall = 0;
  C2F(terme)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L99;

 L83:
  C2F(recu).icall = 0;
  C2F(fact)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L99;

 L85:
  C2F(recu).icall = 0;
  if (C2F(com).fun == C2F(recu).krec) {
    if (C2F(com).fun > 0) {
      SciError(22);
      goto L98;
    }
  }
  if (C2F(errgst).err1 != 0) {
    if (Rstk[Pt] / 100 == 9) {
      if (Rstk[Pt] >= 901 && Rstk[Pt] <= 909) {
        /*              *call* matfns */
        return 0;
      } else {
        --Pt;
        goto L86;
      }
    } else {
      goto L86;
    }
  }
  /*     compilation matfns: <100*fun rhs lhs fin> */
  if (Compil( C2F(com).fun * 100, &(Rhs), Lhs, Fin, 0)) {
    if (Err > 0) {
      goto L98;
    }
    goto L86;
  } else {
    /*     *call* matfns */
    return 0;
  }
 L86:
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L98;

 L88:
  C2F(recu).icall = 0;
  C2F(macro)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L10;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  case 10:  goto L89;
  }
  goto L99;
 L89:
  C2F(com).fun = 99;
  return 0;

 L90:
  C2F(recu).icall = 0;
  C2F(run)();
  if (Err > 0) {
    goto L98;
  }
  if (C2F(com).fun == 99) {
    return 0;
  }
  /*     last label is used to handle return from abort */
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  case 10:  goto L5;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  case 10:  goto L89;
  }
  goto L99;

 L91:
  C2F(recu).icall = 0;
  C2F(allops)();
  if (Err > 0) {
    goto L98;
  }
  switch ((int)C2F(recu).icall) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L15;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  r = Rstk[Pt] / 100;
  switch ((int)r) {
  case 1:  goto L81;
  case 2:  goto L82;
  case 3:  goto L83;
  case 4:  goto L91;
  case 5:  goto L88;
  case 6:  goto L90;
  case 7:  goto L92;
  case 8:  goto L80;
  case 9:  goto L85;
  }
  goto L98;

 L92:
  switch ((int)(Rstk[Pt] - 700)) {
  case 1:  goto L17;
  case 2:  goto L35;
  case 3:  goto L65;
  case 4:  goto L72;
  case 5:  goto L65;
  case 6:  goto L97;
  case 7:  goto L94;
  case 8:  goto L75;
  case 9:  goto L99;
  case 10:  goto L14;
  }
  goto L99;

 L93:
  /*     command like function and macro call */
  /*     store ans as lhs */
  if (Eptover(1)) {
    goto L98;
  }
  C2F(putid)(&Ids[1 + Pt * nsiz], ans);
  Pstk[Pt] = 0;

  Rstk[Pt] = 707;
  if (C2F(com).fun > 0) {
    /*        *call* matfns */
    goto L85;
  } else {
    if (C2F(com).comp[0] != 0) {
      ++Rhs;
      Fin = extrac;
      /*           *call* allops(extrac) */
      goto L91;
    } else {
      Fin = Lstk[C2F(com).fin];
      /*           *call* macro */
      goto L88;
    }
  }
 L94:
  /*     go to store code */
  goto L70;

 L96:
  /*     asynchronous events handling */
  C2F(basbrk).interruptible = C2F(getmen)(C2F(cha1).buf, &lb, &nentry) == 0;
  C2F(bexec)(C2F(cha1).buf, &lb, &ierr);
  if (ierr != 0) {
    goto L15;
  }
  if (Eptover(1)) {
    goto L98;
  }
  Pstk[Pt] = Top;
  Rstk[Pt] = 706;
  Ids[1 + Pt * nsiz] = iret;
  /*     *call* macro */
  goto L88;
 L97:
  /* Rstk[Pt] == 706 indicates we return from a callback execution */
  if (Rstk[Pt] == 706)
    {
      returnFromCallbackExec = TRUE;
    }

  Top = Pstk[Pt] - 1;
  C2F(basbrk).interruptible = TRUE;
  iret = Ids[1 + Pt * nsiz];
  --Pt;
  if (iret == 1) {
    goto L15;
  } else if (iret == 2) {
    if (Lpt[6] == Lpt[1]) {
      job = 0;
    } else {
      /* go ahead with interrupted continuation line */
      job = 3;
    }
    C2F(com).sym = eol;
    goto L13;
  } else if (iret == 3) {
    job = 0;
    goto L12;
  } else if (iret == 4) {
    return 0;
  }
 L98:
  C2F(basbrk).interruptible = TRUE;
  /*     error recovery */
  /* ------------------- */
  imode = abs(C2F(errgst).errct) / 100000;
  imode -= imode / 8 << 3;
  if (imode == 3) {
    C2F(com).fun = 99;
    return 0;
  }

  /*     error in an external (niv), during compilation (comp) or in a pause */
  if (Pt != 0) {
    if (Rstk[Pt] == 503 && C2F(iop).rio == C2F(iop).rte) {
      C2F(com).comp[0] = 0;
      goto L12;
    }
    if (C2F(errgst).err1 != 0 && Rstk[Pt] == 502) { /* catched error while compiling */
      goto L88;
    }
    if (C2F(errgst).err1 != 0 && Rstk[Pt] == 808) { /* catched error in a try */
      goto L80;
    }
  }

  if (C2F(recu).niv > 0) { /*     error in an external */
    C2F(com).fun = 99;
    return 0;
  }
  else if (C2F(recu).paus>0) {/*     error in a  pause */
    C2F(com).comp[0] = 0;
    goto L5;
  }
  else if (Err>0) {
    Pt=0;
    C2F(errgst).errct=-1;
    C2F(errgst).errpt=0;
    C2F(com).comp[0] = 0;
    goto L5;
  }
  else
    {
      C2F(com).comp[0] = 0;
      goto L5;
    }

 L99:
  SciError(22);
  goto L1;

}
/*--------------------------------------------------------------------------*/
/**
 * checks if an implicit execution is required on the prompt
 * @param where_ returned indicator
 * where_ = 0 : no implicit execution is required
 * where_ = 1 : implicit execution is a primitive
 * where_ = 2 : implicit execution is a Scilab function
 */
void handle_onprompt(int *where_)
{
  /* Initialized data */
  /* onprompt[6] internal scilab code for %onprompt variable see C2F(cvname) */
  static int onprompt[6] = {420943928,420878363,673720349,673720360,673720360,673720360 };

  static int *Rstk = C2F(recu).rstk-1;
  static int *Pstk = C2F(recu).pstk-1;
  static int *Lstk = C2F(vstk).lstk-1;

  *where_ = 0;
  if (Pt > 0)
    {
      /* back from %onprompt */
      C2F(errgst).errct = Pstk[Pt];
      --Pt;
      C2F(errgst).err2 = 0;
      Top = 0;
      Fin = 0;
      C2F(com).fun = 0;
    }
  else
    {
      /* on prompt implicit execution */
      C2F(com).fun = 0;
      C2F(funs)(onprompt);
      if (Err > 0) return;
      if (C2F(com).fun <= 0 && Fin == 0) return;
      /* %onprompt function exists */
      Rhs = 0;
      Lhs = 1;
      ++Pt;
      Pstk[Pt] = C2F(errgst).errct;
      Rstk[Pt] = 710;
      /* set error catch with mode continue */
      C2F(errgst).errct = -100001;
      C2F(errgst).errpt = 1;
      if (C2F(com).fun > 0)
        {
          /* %onprompt is a primitive *call* matfns */
          *where_ = 1;
        }
      else
        {
          /* %onprompt is a Scilab function *call*  macro */
          C2F(com).fin = Lstk[Fin];
          *where_ = 2;
        }
    }
}
/*--------------------------------------------------------------------------*/
void C2F(parsecomment)(void)
{

  static int *Lstk    = C2F(vstk).lstk-1;
  static int *Lin     = C2F(iop).lin-1;
  static int *Lpt     = C2F(iop).lpt-1;
  static int *Comp    = C2F(com).comp-1;


  static int l, ll, lkp, l0,c1=1;
  /* look for eol */
  l0=Lpt[4]-1;
  if( (Lin[l0]==slash) &&(Lin[l0-1]==slash)&(Lin[l0+1]==eol)) l0=l0+1;

  l=l0;
  while (Lin[l]!=eol) l++;
  ll = l - l0;
  if (Comp[1] == 0)
    {
      /* ignore all characters up to the end */
    }
  else
    {
      /* compilation [30 number-of-char chars-vector] */
      lkp = C2F(com).comp[0];
      Err = (lkp + 2 + ll) / 2 + 1 - Lstk[Bot];
      if (Err > 0)
        {
          SciError(17);
          return ;
        }
      *istk(lkp) = 31;
      *istk(lkp+1) = ll;
      C2F(icopy)(&ll, &(Lin[l0]), &c1, istk(lkp+2), &c1);
      Comp[1] = lkp + 2 + ll;
    }
  Lpt[4] = l;
  C2F(com).char1 = eol;
  C2F(com).sym = eol;
  return ;
}
/*--------------------------------------------------------------------------*/

