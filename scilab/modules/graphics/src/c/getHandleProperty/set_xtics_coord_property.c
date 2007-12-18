/*------------------------------------------------------------------------*/
/* file: set_xtics_coord_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the xtics_coord field of           */
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
int set_xtics_coord_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  int N = 0;
  double * vector = NULL;
  char c_format[5];

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"xtics_coord") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint("xtics_coord does not exist for this handle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 )
  {
    sciprint(_("%s: Wrong type for second input argument: row vector expected.\n"), "set_xtics_coord_property") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->nx == 1 && nbCol != 1 )
  {
    sciprint(_("%s: Wrong type for second input argument: scalar expected.\n"), "set_xtics_coord_property") ;
    return SET_PROPERTY_ERROR ;
  }

  if (  pAXES_FEATURE(pobj)->nx != 1 && nbCol == 1 )
  {
    sciprint(_("%s: Wrong type for second input argument: vector expected.\n"), "set_xtics_coord_property") ;
    return SET_PROPERTY_ERROR ;
  }

  /* what follows remains here as it was */

  pAXES_FEATURE(pobj)->nx = nbCol ;

  FREE(pAXES_FEATURE(pobj)->vx); pAXES_FEATURE(pobj)->vx = NULL;

  pAXES_FEATURE(pobj)->vx = createCopyDoubleVectorFromStack( stackPointer, nbCol ) ;


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
