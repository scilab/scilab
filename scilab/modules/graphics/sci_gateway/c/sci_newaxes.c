/*------------------------------------------------------------------------*/
/* file: sci_newaxes.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_newaxes.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_newaxes( char * fname,unsigned long fname_len )
{ 
  int minrhs = 0,maxrhs = 0,minlhs=0,maxlhs=1;
  sciPointObj *masousfen;
  int outindex,numrow   = 1,numcol   = 1;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  if ((masousfen = ConstructSubWin (sciGetCurrentFigure())) != NULL)
  {
    sciSetCurrentObj(masousfen);
    sciSetSelectedSubWin(masousfen);
    CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
    *hstk(outindex) = sciGetHandle(masousfen);


    LhsVar(1)=1;
  }
  else {
    Scierror(999,"%s: running out of memory\n",fname);
  }
  return 0;

}
/*-----------------------------------------------------------------------------------*/
