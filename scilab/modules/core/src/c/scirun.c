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
/* Code automatically translated from Fortran to C */
/*--------------------------------------------------------------------------*/
#include "scirun.h"
#include "callinterf.h"
#include "stack-c.h"
#include "Scierror.h"
#include "recursionFunction.h"
#include "exitCodeValue.h"
/*--------------------------------------------------------------------------*/
extern int C2F(parse)(void);
extern int C2F(isbyref)(int *);
/*--------------------------------------------------------------------------*/
static void str_copy_buf(register char *a, register char *b, long int la, long int lb);

/**
 * This function should be completely rewritten... (F2C effect)
 * It is not understable for someone normal
 *
 */
int C2F(scirun)(char *startupCode, long int startupCode_len)
{
  static int k = 0;
  /* stack recovery is now handled in errmgr */
  /* int ireftop=0;*/

  /* set instruction to execute at start-up */
  /* cha1 is comming from stack-def.h */
  str_copy_buf(C2F(cha1).buf, startupCode, bsiz, startupCode_len);

  C2F(iop).rio = -1;
  C2F(iop).lpt[0] = 1;
  C2F(iop).lpt[5] = 0;
  C2F(recu).pt = 0;

  C2F(recu).icall = 0;
  C2F(recu).krec = 99999;

  /* call instructions parser */

 L60:
  C2F(parse)();
  /* @TODO : What is 99 */
  if (C2F(com).fun == 99)
    {
      C2F(com).fun = 0;
      return 0;
    }
  if (Err > 0)
    {
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }

  if ( isRecursionCallToFunction() )
    {
      int gw = getRecursionGatewayToCall();
      if (gw == END_OVERLOAD)
        {
          --C2F(recu).pt;
          goto L90;
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
  if (Top < Rhs)
    {
      SciError(22);
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }
  if (Top - Rhs + Lhs + 1 >= Bot)
    {
      SciError(18);
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }
  /* stack recovery is now handled in errmgr */
  /*ireftop=Top - Max(0,Rhs);*/
  goto L91;

 L90:
  if (Err > 0)
    {
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }
 L91:
  k = C2F(com).fun;
  C2F(com).fun = 0;
  if (k == C2F(recu).krec)
    {
      C2F(recu).krec = -1;
      SciError(22);
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }
  C2F(recu).krec = -1;
  if (k == 0) goto L60;

 L95:
  if (! C2F(allowptr)(&k))
    {
      C2F(ref2val)();
    }
  C2F(recu).krec = k;
  C2F(callinterf)(&k);
  C2F(recu).krec = -1;
  if (C2F(com).fun == -999) /* exit detected */
    {
      return getExitCodeValue();
    }
  if (C2F(com).fun >= 0)
    {
      if (Top - Lhs + 1 > 0)
        {
          int cx0=0;
          int cx1=0;
          C2F(iset)(&Rhs, &cx0, &C2F(vstk).infstk[Top - Lhs], &cx1);
        }
      if (C2F(recu).paus > 0)
        {
          goto L91;
        }
      if (C2F(errgst).err1 > 0)
        {
          /*stack recovery is now handled in errmgr */
          /*Top = ireftop;*/
        }
      goto L90;
    }

  /*
    called interface ask for a scilab function to perform the function (fun=-1)
    the function name is given in ids(1,pt+1)
    call ref2val removed here because if forces overloading function to
    be called by value
    call ref2val
  */
  C2F(com).fun = 0;
  C2F(funs)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6]);
  if (Err > 0)
    {
      if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
      goto L60;
    }
  if (C2F(com).fun > 0)
    {
      if (C2F(isbyref)(&C2F(com).fun) == 0)
        {
          C2F(ref2val)();
        }
      goto L91;
    }

  if (Fin == 0)
    {
      SciError(246);
      if (Err > 0)
        {
          if (C2F(recu).niv > 0 && C2F(recu).paus > 0) C2F(com).fun = 0;
          goto L60;
        }
      goto L90;
    }
  ++C2F(recu).pt;
  Fin = C2F(vstk).lstk[Fin - 1];
  C2F(recu).rstk[C2F(recu).pt - 1] = 910;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /* *call*  macro */
  goto L60;

  return 0;
}
/*--------------------------------------------------------------------------*/
static void str_copy_buf(register char *a, register char *b, long int la, long int lb)
{
  if (lb>la) strncpy(a,b,la);
  else
    {
      int i=0;
      strncpy(a,b,lb);
      for (i=lb;i<la;i++) a[i]= ' ';
    }
}
/*--------------------------------------------------------------------------*/
