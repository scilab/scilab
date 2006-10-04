/*------------------------------------------------------------------------*/
/* file: set_auto_ticks_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the auto_ticks field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_auto_ticks_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  char ** values = getStringMatrixFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint("Incompatible type for property auto_ticks.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    sciprint( "auto_ticks property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if( nbCol == 1 )
  {
    /* only one parameter to set the value for every axes.*/
    if ( strcmp( values[0], "off" ) == 0 ) 
    {
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[0] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[1] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[2] = FALSE ;
    }
    else if ( strcmp( values[0], "on" ) == 0 )
    {
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[0] = TRUE ;
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[1] = TRUE ;
      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[2] = TRUE ;
    }
    else
    {
      sciprint("Second argument must be 'on' or 'off'") ;
      return SET_PROPERTY_ERROR ; ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( nbCol == 2 || nbCol == 3)
  {
    int i ;
    for ( i = 0; i < nbCol; i++ )
    {
      if ( strcmp(values[i],"off") == 0 )
      {
        pSUBWIN_FEATURE(pobj)->axes.auto_ticks[i] = FALSE ;
      }
      else if ( strcmp(values[i],"on") == 0 )
      {
        pSUBWIN_FEATURE(pobj)->axes.auto_ticks[i] = TRUE ;
      }
      else
      {
        sciprint("Second argument must be 'on' or 'off'.\n");
        return SET_PROPERTY_ERROR ;
      }
      return SET_PROPERTY_SUCCEED ;
    }	
  }
  else
  {
    sciprint("Number of the second argument must be taken between 1 to 3.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
