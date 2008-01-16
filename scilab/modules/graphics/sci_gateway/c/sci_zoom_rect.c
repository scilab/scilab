/*------------------------------------------------------------------------*/
/* file: sci_zoom_rect.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
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

/*--------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname,unsigned long fname_len)
{
  int m,n;
  int stackPointer;
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  if (Rhs <= 0) 
  {
    zoom();
  }
  else
  {
    double * rectData = NULL;
    sciPointObj * curFigure = NULL;
    sciPointObj * curSubWin = NULL;


    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&stackPointer); 
    CheckLength(1,4,m*n);
    /*sciZoom2D(sciGetCurrentSubWin(), getDoubleMatrixFromStack(stackPointer));*/
    rectData = getDoubleMatrixFromStack(stackPointer);
    
    curFigure = sciGetCurrentFigure();
    curSubWin = sciGetCurrentSubWin();

    startFigureDataWriting(curFigure);
    sciZoomRect(curSubWin, (int) rectData[0], (int) rectData[1],
                (int) rectData[2], (int) rectData[3]);
    endFigureDataWriting(curFigure);

    sciDrawObj(curSubWin);
  }

  LhsVar(1)=0; 
  return 0;
} 
/*--------------------------------------------------------------------------*/
