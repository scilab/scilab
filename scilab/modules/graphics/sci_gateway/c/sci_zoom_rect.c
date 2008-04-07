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
/* file: sci_zoom_rect.c                                                  */
/* desc : interface for zoom_rect routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_zoom_rect.h"
#include "stack-c.h"
#include "getPropertyAssignedValue.h"
#include "PloEch.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "axesScale.h"
#include "DrawingBridge.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "Interaction.h"

/*--------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname,unsigned long fname_len)
{
  int m,n;
  int stackPointer;
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  if (Rhs <= 0) 
  {
    sciZoomRect();
  }
  else
  {
    double * rectData = NULL;
    sciPointObj * curFigure = NULL;
    sciPointObj * curSubWin = NULL;


    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&stackPointer); 
    CheckLength(1,4,m*n);

    curFigure = sciGetCurrentFigure();
    curSubWin = sciGetCurrentSubWin();

    startFigureDataWriting(curFigure);
    sciZoom2D(sciGetCurrentSubWin(), getDoubleMatrixFromStack(stackPointer));
    endFigureDataWriting(curFigure);

    sciDrawObj(curSubWin);
  }

  LhsVar(1)=0; 
  return 0;
} 
/*--------------------------------------------------------------------------*/
