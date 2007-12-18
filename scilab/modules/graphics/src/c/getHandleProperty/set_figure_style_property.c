/*------------------------------------------------------------------------*/
/* file: set_figure_style_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the figure_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "DestroyObjects.h"
#include "BuildObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "old" ) )
  {
    sciprint("Old graphic mode is no longer available. Please refer to the set help page.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  else if ( isStringParamEqual( stackPointer, "new" ) )
  {
    /* graphic mode must be new */
    /* nothing to do */
    return SET_PROPERTY_UNCHANGED ;
  }
  else
  {
    sciprint("Figure style must be 'old' or 'new'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
