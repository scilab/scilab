/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xsetech.c                                                    */
/* desc : interface for xsetech routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xsetech.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "PloEch.h"

/*--------------------------------------------------------------------------*/
int sci_xsetech(char* fname,unsigned long fname_len)
{ 
  double * wrect   = NULL ; 
  double * frect   = NULL ;
  double * arect   = NULL ;
  char   * logflag = NULL ;
  static char logflag_def[]="nn";
  /** optional names must be stored in alphabetical order in opts **/
  static rhs_opts opts[]= { 
    {-1,"arect","d",0,0,0},
    {-1,"frect","d",0,0,0},
    {-1,"logflag","c",0,0,0},
    {-1,"wrect","d",0,0,0},
    {-1,NULL,NULL,0,0}
  };
  int minrhs = 1,maxrhs = 0,minlhs=0,maxlhs=1,nopt;

  nopt = NumOpt();

  if ( nopt == 0) 
  {
    /** compatibility with old version **/

    int m1,n1,l1,m2,n2,l2,m3,n3,l3;
    minrhs = 1,maxrhs = 3,minlhs=0,maxlhs=1;
    CheckRhs(minrhs,maxrhs) ;
    CheckLhs(minlhs,maxlhs) ;

    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    CheckDims(1,m1,n1,1,4);
    wrect = stk(l1);

    if (Rhs >= 2) {
			GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
			CheckDims(2,m2,n2,1,4);
			frect=stk(l2);
		} 
    if (Rhs >= 3) {
			GetRhsVar(3,STRING_DATATYPE, &m3, &n3, &l3);
			CheckLength(3,m3,2);
			logflag = cstk(l3);
		}
		else {
      logflag = logflag_def ; /* compatibility with old version */
		}
  }
  else 
  {
    CheckRhs(minrhs,maxrhs+nopt) ;
    CheckLhs(minlhs,maxlhs) ;

    if ( get_optionals(fname,opts) == 0) 
	{ 
		/* error */
		return 0; 
	}

    if ( opts[0].position != -1 )
    {
      arect = stk(opts[0].l);	CheckLength(opts[0].position,opts[0].m*opts[0].n,4);
    }
    if ( opts[1].position != -1 )
    {
      frect = stk(opts[1].l);	CheckLength(opts[1].position,opts[1].m*opts[1].n,4);
    } 
    if ( opts[2].position != -1 )
    {
      logflag = cstk(opts[2].l);CheckLength(opts[2].position,opts[2].m*opts[2].n,2);
    } 
    else
    {
      logflag = logflag_def ;
    }
    if ( opts[3].position != -1 )
    {
      wrect = stk(opts[3].l);
			CheckLength(opts[3].position,opts[3].m*opts[3].n,4);
    } 
  }
  setscale2d(wrect,arect,frect,logflag);
  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;

}
/*--------------------------------------------------------------------------*/
