/*------------------------------------------------------------------------*/
/* file: sci_unglue.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for unglue routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_unglue.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "ObjectStructure.h"
#include "GetProperty.h"
#include "DestroyObjects.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_unglue(char *fname,unsigned long fname_len)
{ 
  integer m1,n1,l1; 
  unsigned long hdl;
  int numrow, numcol, outindex, i;
  sciPointObj *pobj;  
  sciSons *psonstmp;

  CheckRhs(1,1);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
  hdl = (unsigned long)*hstk(l1);
  pobj = sciGetPointerFromHandle(hdl);
  if (pobj == NULL)
  {
    Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
    return 0;
  }
  if (sciGetEntityType (pobj) == SCI_AGREG)
  {
    psonstmp = sciGetLastSons (pobj);
    i = 0;
    psonstmp = sciGetSons((sciPointObj *) pobj);
    while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
    {
      psonstmp = psonstmp->pnext;
      i++;
    }
    numrow   = i;
    numcol   = 1;
    CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
    psonstmp = sciGetSons((sciPointObj *) pobj);
    i = 0;
    while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
    {
      hstk(outindex)[i] = sciGetHandle((sciPointObj *)psonstmp->pointobj);
      psonstmp = psonstmp->pnext;/* psonstmp   is pointer to one son */
      i++;
    }
    LhsVar(1) = Rhs+1;
    sciUnCompound ((sciPointObj *)pobj);
  }
  else
  {
    Scierror(999,_("%s: Object must be a Compound.\n"),fname);
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
