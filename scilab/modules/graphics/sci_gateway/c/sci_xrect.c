
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
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
/* file: sci_xrect.c                                                      */
/* desc : interface for xrect routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xrect.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_xrect( char *fname, unsigned long fname_len )
{
  long hdl;
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  sciPointObj * psubwin = NULL;
  sciPointObj * pFigure = NULL;

  CheckRhs(1,4);

  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();
  endGraphicDataWriting();

  switch( Rhs )
  {
  case 1 :
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    CheckLength(1,m1*n1,4);
    startFigureDataWriting(pFigure);
    if (strcmp(fname,"xrect")==0)
    {
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        &foreground,NULL,FALSE,TRUE,&hdl);
    }
    else
    { /* xfrect case */
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        NULL,&foreground,TRUE,FALSE,&hdl);
    }
    endFigureDataWriting(pFigure);

    if ( hdl < 0 )
    {
      break;
    }

    startFigureDataReading(pFigure);
    sciDrawObjIfRequired(sciGetCurrentObj ());
    endFigureDataReading(pFigure);

    break;
  case 4 :
    {
      int foreground = 0;
      startFigureDataReading(pFigure);
      foreground = sciGetForeground(psubwin);
      endFigureDataReading(pFigure);
      GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
      GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); CheckScalar(2,m2,n2);
      GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckScalar(3,m3,n3);
      GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4);
      startFigureDataWriting(pFigure);
      if (strcmp(fname,"xrect")==0)
      {
        Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
                 &foreground,NULL,FALSE,TRUE,&hdl) ;
      }
      else
      {
        Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
                 NULL,&foreground,TRUE,FALSE,&hdl);
      }
      endFigureDataWriting(pFigure);

      if ( hdl < 0 )
      {
        break;
      }
      startFigureDataReading(pFigure);
      sciDrawObjIfRequired(sciGetCurrentObj ());
      endFigureDataReading(pFigure);
    }
    break;
  default :
    Scierror(999,_("%s: Wrong number of input argument(s): %d or %d expected.\n"),fname, 1,4);
    break ;
  }

  if ( hdl > 0 )
  {
    LhsVar(1)=0;
		C2F(putlhsvar)();
  }
  return 0;
} 

/*--------------------------------------------------------------------------*/
