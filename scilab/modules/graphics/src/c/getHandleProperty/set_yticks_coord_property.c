/*------------------------------------------------------------------------*/
/* file: set_ytics_coord_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the ytics_coord field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "BasicAlgos.h"
#include "Format.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int set_ytics_coord_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  int N = 0;
  double * vector = NULL;
  char c_format[5];

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint("ytics_coord does not exist for this handle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 )
  {
    sciprint("Second argument must be a row vector.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->ny == 1 && nbCol != 1 )
  {
    sciprint("Second argument must be a scalar.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if (  pAXES_FEATURE(pobj)->ny != 1 && nbCol == 1 )
  {
    sciprint("Second argument  must be a vector.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  /* what follows remains here as it was */

  FREE(pAXES_FEATURE(pobj)->vy); pAXES_FEATURE(pobj)->vy = NULL;

  pAXES_FEATURE(pobj)->vy = createCopyDoubleVectorFromStack( stackPointer, nbCol ) ;


  ComputeXIntervals( pobj, pAXES_FEATURE(pobj)->tics, &vector, &N, 0 ) ;
  ComputeC_format( pobj, c_format ) ;

  if( pAXES_FEATURE(pobj)->str != NULL )
  {
    destroyStringArray( pAXES_FEATURE(pobj)->str, pAXES_FEATURE(pobj)->nb_tics_labels ) ;
  }

  pAXES_FEATURE (pobj)->nb_tics_labels = N;
  pAXES_FEATURE(pobj)->str = copyFormatedArray( vector, N, c_format, 256 ) ;


  FREE( vector ) ;

  if ( pAXES_FEATURE(pobj)->str == NULL )
  {
    sciprint( "error allocating vector.\n");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
