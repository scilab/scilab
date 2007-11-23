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
#include "GetProperty.h"
#include "DrawObjects.h"
#include "graphicSession.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"

/*--------------------------------------------------------------------------*/
int sci_xend(char *fname,unsigned long fname_len)
{
  sciPointObj * figure = NULL;


  CheckRhs(-1,0);

  closeGraphicSession() ;   /* we DO draw now into the file/memory (depending on the driver type) */
  figure = sciGetCurrentFigure();
  sciDrawObj(figure);

  /* figure already defined above */
  destroyGraphicHierarchy(figure);

  LhsVar(1)=0;
  return 0;
}


/*--------------------------------------------------------------------------*/
