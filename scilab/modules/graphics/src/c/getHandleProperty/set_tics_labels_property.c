/*------------------------------------------------------------------------*/
/* file: set_tics_labels_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the tics_labels field of           */
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

/*------------------------------------------------------------------------*/
int set_tics_labels_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"tics_labels") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"tics_labels") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1)
  {
    sciprint("Second argument must be a row vector.\n");
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->nb_tics_labels > nbCol )
  {
    sciprint("Value must have at least %d elements",pAXES_FEATURE(pobj)->nb_tics_labels) ;
    return SET_PROPERTY_ERROR ;
  }

  if(pAXES_FEATURE(pobj)->str != NULL)
  {
    destroyStringArray( pAXES_FEATURE(pobj)->str, pAXES_FEATURE(pobj)->nb_tics_labels ) ;
  }

  pAXES_FEATURE(pobj)->str = createCopyStringMatrixFromStack( stackPointer, nbCol ) ;
  pAXES_FEATURE(pobj)->nb_tics_labels = nbCol ; /* could be increased to support xy_type switching (i.e. xy_type='v' -> xy_type='r') */

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
