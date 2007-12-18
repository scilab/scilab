/*------------------------------------------------------------------------*/
/* file: set_tics_direction_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the tics_direction field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_tics_direction_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"tics_direction") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint("tics_direction property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE (pobj)->ny == 1 )
  { 
    if( isStringParamEqual( stackPointer, "top" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'u' ;
    }
    else if ( isStringParamEqual( stackPointer, "bottom" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'd' ;
    }
    else
    {
      sciprint("Second argument must be 'top' or 'bottom'.\n") ;
      return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_SUCCEED ;
  } 
  else
  {
    if( isStringParamEqual( stackPointer, "right" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'r' ;
    }
    else if ( isStringParamEqual( stackPointer, "left" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'l' ;
    }
    else
    {
      sciprint("Second argument must be 'right' or 'left'.\n") ;
      return -1 ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
