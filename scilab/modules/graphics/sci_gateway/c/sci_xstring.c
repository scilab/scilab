/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xtring.c                                                     */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "sciCall.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xstring( char *fname, unsigned long fname_len )
{
  double rect[4];
  double x,y,angle=0.0;
  int m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str = NULL;
  char **sendStr = NULL;
  int sendm3, sendn3;
  long hdlstr;
  int nbElement=0, i;
  BOOL isboxed = FALSE;

  CheckRhs(3,5);

  GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Str);
  if ( m3*n3 == 0 )
	{
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	}


  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);

  if(m1*n1 == 1 || m2*n2 == 1)
  {
    nbElement = m1*n1 * m2*n2;
  }
  else if(m1*n1 == m2*n2)
  {
    nbElement = m1*n1;
  }
  else
  {
    Scierror(999,_("%s: Incompatible input arguments #%d and #%d: Same element number expected.\n"), fname, 1, 2);
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
  }

  if(nbElement == 0)
	{
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	}
  
  if (Rhs >= 4)
  {
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4);
    if(m4*n4 != 1 && m4*n4 != nbElement)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d or %d elements expected.\n"), fname, 4, 1, nbElement);
		  LhsVar(1)=0;
		  C2F(putlhsvar)();
		  return 0;
    }
  }
  if (Rhs >= 5)
  { 
    GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m5,&n5,&l5);
    if(m5*n5 != 1 && m5*n5 != nbElement)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d or %d elements expected.\n"), fname, 5, 1, nbElement);
		  LhsVar(1)=0;
		  C2F(putlhsvar)();
		  return 0;
    }
  }


  x = *stk(l1);
  y = *stk(l2);
  sendStr = Str;
  sendm3 = m3;
  sendn3 = n3;
  if (Rhs >= 4) angle = DEG2RAD(*stk(l4));
  if (Rhs >= 5) isboxed = (*stk(l5) != 0);

  if(nbElement == 1)
  {
    Objstring (sendStr,sendm3,sendn3,x,y,&angle,rect,TRUE,NULL,&hdlstr,FALSE,NULL,NULL,isboxed && (angle==0),TRUE,FALSE, ALIGN_LEFT ) ;
  }
  else
  {
    for(i=0;i<nbElement;i++)
    {
      if(m1*n1 == nbElement) x = *(stk(l1)+i);
      if(m2*n2 == nbElement) y = *(stk(l2)+i);
      if(m3*n3 == nbElement)
      {
        sendStr = Str+i;
        sendm3 = sendn3 = 1;
      }
      if(Rhs >= 4 && m4*n4 == nbElement) angle = DEG2RAD(*(stk(l4)+i));
      if(Rhs >= 5 && m5*n5 == nbElement) isboxed = (*stk(l5) != 0);

      Objstring (sendStr,sendm3,sendn3,x,y,&angle,rect,TRUE,NULL,&hdlstr,FALSE,NULL,NULL,isboxed && (angle==0),TRUE,FALSE, ALIGN_LEFT ) ;
    }
    ConstructCompoundSeq(nbElement);
  }

  /* we must free Str memory */ 
  
  freeArrayOfString(Str,m3*n3);

  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
} 
/*--------------------------------------------------------------------------*/
