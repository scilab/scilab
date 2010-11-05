
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

#include "gw_graphics.h"
#include "stack-c.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_xrect( char *fname, unsigned long fname_len )
{
  long hdl;
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  sciPointObj * psubwin = NULL;
  sciPointObj * pFigure = NULL;
  int* tmp;
  int foreground;

  CheckRhs(1,4);

    /* Deactivated (synchronization) */
#if 0
  startGraphicDataWriting();
#endif

  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();

    /* Deactivated (synchronization) */
#if 0
  endGraphicDataWriting();
#endif

  switch( Rhs )
  {
  case 1 :
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    CheckLength(1,m1*n1,4);

    /* Deactivated (synchronization) */
#if 0
    startFigureDataWriting(pFigure);
#endif

    tmp = (int*) getGraphicObjectProperty(psubwin->UID, __GO_LINE_COLOR__, jni_int);
    foreground = *tmp;

    if (strcmp(fname,"xrect")==0)
    {
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        &foreground,NULL,FALSE,TRUE,&hdl);
    }
    else
    { /* xfrect case */
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        NULL,&foreground,TRUE,FALSE,&hdl);
    }

    /* Deactivated (synchronization) */
#if 0
    endFigureDataWriting(pFigure);
#endif

    if ( hdl < 0 )
    {
      break;
    }

    /* Deactivated (synchronization and drawing) */
#if 0
    startFigureDataReading(pFigure);
    sciDrawObjIfRequired(sciGetCurrentObj ());
    endFigureDataReading(pFigure);
#endif

    break;
  case 4 :
    {
      /* Deactivated (synchronization) */
#if 0
      startFigureDataReading(pFigure);
      foreground = sciGetForeground(psubwin);
      endFigureDataReading(pFigure);
#endif

      tmp = (int*) getGraphicObjectProperty(psubwin->UID, __GO_LINE_COLOR__, jni_int);
      foreground = *tmp;

      GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
      GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); CheckScalar(2,m2,n2);
      GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckScalar(3,m3,n3);
      GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4);

      /* Deactivated (synchronization) */
#if 0
      startFigureDataWriting(pFigure);
#endif
      if (strcmp(fname,"xrect")==0)
      {
        Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
                 &foreground,NULL,FALSE,TRUE,&hdl);
      }
      else
      {
        Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
                 NULL,&foreground,TRUE,FALSE,&hdl);
      }

      /* Deactivated (synchronization) */
#if 0
      endFigureDataWriting(pFigure);
#endif

      if ( hdl < 0 )
      {
        break;
      }

      /*
       * Deactivated (drawing)
       * To be implemented
       */
#if 0
      startFigureDataReading(pFigure);
      sciDrawObjIfRequired(sciGetCurrentObj ());
      endFigureDataReading(pFigure);
#endif
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
