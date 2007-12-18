/*------------------------------------------------------------------------*/
/* file: get_figures_id_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figures_id field of a handle */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
#include "WindowList.h"

/*------------------------------------------------------------------------*/
int get_figures_id_property( sciPointObj * pobj )
{
  int   nbFig  = 0    ;
  int * ids    = NULL ;
  int   status = -1   ;
  

  nbFig = sciGetNbFigure() ; /* get the number of opened windows */
  
  ids = MALLOC( nbFig * sizeof(int) ) ;
  if ( ids == NULL )
  { 
    sciprint("Error returning figures_id vector, memory full.\n") ;
    return -1 ;
  }

  sciGetFiguresId( ids ) ;

  status = sciReturnRowIntVector( ids, nbFig ) ;

  FREE( ids ) ;

  return status ;

}
/*------------------------------------------------------------------------*/
