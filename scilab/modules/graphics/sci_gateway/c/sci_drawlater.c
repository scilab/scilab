/*------------------------------------------------------------------------*/
/* file: sci_drawlater.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for drawlater routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_drawlater.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "ObjectStructure.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_drawlater( char * fname, unsigned long fname_len )
{
  sciPointObj *pfigure = NULL;

  CheckRhs(0,0);
  CheckLhs(0,1); 


  if (Rhs <= 0) {
    pfigure = sciGetCurrentFigure ();
    pFIGURE_FEATURE(pfigure)->auto_redraw = FALSE;

    LhsVar(1) = 0;
    return 0;
  }

  LhsVar(1) = 0;
  return 0;

}
/*-----------------------------------------------------------------------------------*/
