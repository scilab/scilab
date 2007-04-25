/*------------------------------------------------------------------------*/
/* file: sci_xend.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xend routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xend.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "stack-c.h"
#include "ObjectStructure.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "graphicSession.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"

/*-----------------------------------------------------------------------------------*/
int sci_xend(char *fname,unsigned long fname_len)
{
  integer v;
  double dv;
  sciPointObj * figure = NULL;
  struct BCG *Xgc;

  SciWin();

  CheckRhs(-1,0);

  closeGraphicSession() ;   /* we DO draw now into the file/memory (depending on the driver type) */
  figure = sciGetCurrentFigure();
  sciDrawObj(figure);
  C2F(dr1)("xend","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,2L);

  /* figure already defined above */
  Xgc = pFIGURE_FEATURE(figure)->pScilabXgc;
  DestroyAllGraphicsSons (figure);
  DestroyFigure (figure); figure = (sciPointObj *) NULL;
  Xgc->mafigure = (sciPointObj *) NULL;

  LhsVar(1)=0;
  return 0;
}


/*-----------------------------------------------------------------------------------*/
