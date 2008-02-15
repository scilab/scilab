/*------------------------------------------------------------------------*/
/* file: sci_uiwait.c                                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : interface for sci_uiwait routine                                */
/*------------------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h> 
/*--------------------------------------------------------------------------*/
#include "sci_uiwait.h"
#include "MALLOC.h" /* MALLOC */
#include "ObjectStructure.h"
#include "BuildObjects.h"
#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "localization.h"
#include "Scierror.h"
#include "ContextMenu.h"
/*--------------------------------------------------------------------------*/
int sci_uiwait( char *fname,unsigned long fname_len )
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  char * result = NULL;

  long hdl = 0;

  sciPointObj *pObj = NULL;

  CheckRhs(1,1);
  CheckLhs(0,1);

  if (VarType(1) == sci_handles)
    {
      GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
      if (nbRow * nbCol !=1)
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single handle expected.\n"), "uiwait");
          return FALSE;
        }
      hdl = (unsigned long)*hstk(stkAdr);
      pObj = sciGetPointerFromHandle(hdl);

      if (sciGetEntityType(pObj) == SCI_UICONTEXTMENU)
        {
          result = uiWaitContextMenu(pObj);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: uicontextmenu expected.\n"), "uiwait");
          return FALSE;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single handle expected.\n"), "uiwait");
      return FALSE;
    }
 
  /* Create return variable */
  nbRow = (int)strlen(result);
  nbCol = 1;
  CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
  strcpy(cstk(stkAdr), result);

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
