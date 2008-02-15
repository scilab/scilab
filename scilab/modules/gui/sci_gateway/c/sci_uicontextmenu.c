/*------------------------------------------------------------------------*/
/* file: sci_uicontextmenu.c                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : interface for sci_uicontextmenu routine                         */
/*------------------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h> 
/*--------------------------------------------------------------------------*/
#include "sci_uicontextmenu.h"
#include "CreateUIContextMenu.h"
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
/*--------------------------------------------------------------------------*/
int sci_uicontextmenu( char *fname,unsigned long fname_len )
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  unsigned long GraphicHandle = 0;

  CheckRhs(0,0);
  CheckLhs(0,1);

  /* Create a new context menu */
  GraphicHandle=sciGetHandle(CreateUIContextMenu());

  /* Create return variable */
  nbRow = 1;
  nbCol = 1;
  CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *hstk(stkAdr) = GraphicHandle;

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
