/*------------------------------------------------------------------------*/
/* file: get_data_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the data field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_data_property( sciPointObj * pobj )
{
  int nbRow  =  0 ;
  int nbCol  =  0 ;
  int status = -1 ;
  /* Warning the following function allocate data */
  double * data = sciGetPoint ( pobj, &nbRow, &nbCol ) ;
  if ( data  == NULL )
  {
    sciprint( "No data found for this handle.\n" ) ;
    return -1;
  }

  status = sciReturnMatrix( data, nbRow, nbCol ) ;

  FREE( data ) ;

  return status ;
}
/*------------------------------------------------------------------------*/
