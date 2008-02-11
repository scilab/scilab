/*------------------------------------------------------------------------*/
/* file: sci_mpopup.c                                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : interface for sci_mpopup routine                                */
/*       (temporary function waiting for uicontextmenu function)          */
/*------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h> 
/*--------------------------------------------------------------------------*/
#include "sci_mpopup.h"
#include "MALLOC.h" /* MALLOC */
#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "sciprint.h"
#include "ContextMenu.h"
#include "returnProperty.h"
#include "getPropertyAssignedValue.h"
/*--------------------------------------------------------------------------*/
int sci_mpopup(char *fname,unsigned long fname_len)
{
  int nbRow = 0,  nbCol = 0, menuAdr = 0;

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &menuAdr);
    }
  else
    {
      Scierror(999,_("%s: Wrong type for first input argument: String vector expected.\n"),fname);
      return FALSE;
    }

  createContextMenu(getStringMatrixFromStack(menuAdr), nbRow*nbCol);

  LhsVar(1)=0;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
