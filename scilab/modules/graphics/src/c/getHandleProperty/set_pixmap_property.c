/*------------------------------------------------------------------------*/
/* file: set_pixmap_property.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the pixmap field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_pixmap_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("pixmap property undefined for this object.\n") ;
    return -1;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    sciSetPixmapMode(pobj, TRUE) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    sciSetPixmapMode(pobj, FALSE);
  }
  else
  {
    sciprint("Nothing to do (value must be 'on/off').\n") ;
    return -1 ;
  }
  return 0 ;
}
/*------------------------------------------------------------------------*/
