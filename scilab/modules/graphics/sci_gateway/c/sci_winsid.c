/*------------------------------------------------------------------------*/
/* file: sci_winsid.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for winsid routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_winsid.h"
#include "MALLOC.h"
#include "WindowList.h"
#include "sciprint.h"
#include "returnProperty.h"

/*-----------------------------------------------------------------------------------*/
int sci_winsid(char *fname,unsigned long fname_len)
{
  int one = 1;
  int * ids = NULL;
  int nbFigure = sciGetNbFigure();
  CheckRhs(-1,0);

  ids = MALLOC(nbFigure * sizeof(int));
  if (ids == NULL)
  {
    sciprint("Error in function winsid, unable to perform operations, memory full.\n");
    return 0;
  }

  sciGetFiguresId(ids);

  return sciReturnRowVectorFromInt(ids, nbFigure);
}
/*-----------------------------------------------------------------------------------*/
