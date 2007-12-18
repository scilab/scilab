/*------------------------------------------------------------------------*/
/* file: set_axes_visible_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the grid field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_axes_visible_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  char ** values = getStringMatrixFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_visible") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint( "axes_visible property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if( nbCol == 1 )
  {

    if ( strcmp( values[0], "off") == 0 )
    {
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[0] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[1] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[2] = FALSE ;
    }
    else if ( strcmp( values[0], "on") == 0 )
    {
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[0] = TRUE ;
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[1] = TRUE ;
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[2] = TRUE ;
    }
    else
    {
      sciprint("Second argument must be 'on' or 'off'.\n") ;
      return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( nbCol == 2 || nbCol == 3 )
  {
    int i ;
    for ( i = 0; i < nbCol ; i++ )
    {
      if ( strcmp( values[i], "off" ) == 0)
      {
        pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] = FALSE ;
      }
      else if ( strcmp( values[i], "on" ) == 0 )
      {
        pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] = TRUE ;
      }
      else
      {
        sciprint("Second argument must be 'on' or 'off'.\n") ;
        return SET_PROPERTY_ERROR ;
      }
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("Number of the second argument must be taken between 1 to 3.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
