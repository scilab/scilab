/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.c                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#include "get_ticks_utils.h"
#include "returnPropertyList.h"

/*--------------------------------------------------------------------------------*/
int buildTListForTicks( const double * locations, char * labels[], int nbTics )
{
  char * variable_tlist[] = {"ticks","locations","labels"};

  returnedList * tList = createReturnedList( 2, variable_tlist ) ;

  addColVectorToReturnedList( tList, locations, nbTics ) ;
  addStringColVectorToReturnedList( tList, labels, nbTics ) ;

  destroyReturnedList( tList ) ;

  return 0;
}
/*--------------------------------------------------------------------------------*/
