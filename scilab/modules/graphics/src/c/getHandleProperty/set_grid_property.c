/*------------------------------------------------------------------------*/
/* file: set_grid_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the grid field of                  */
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
int set_grid_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int i ;
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"grid") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"grid") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 || nbCol > 3 )
  {
    sciprint("grid must be a 1x2 or 1x3 vector.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  for (  i = 0 ; i < nbCol ; i++ )
  {
    if ( values[i] < -1 )
    {
      sciprint("Argument must be -1 (no grid) or number of color.\n");
      return SET_PROPERTY_ERROR ;
    }
    pSUBWIN_FEATURE(pobj)->grid[i] = (int) values[i] ;
  }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
