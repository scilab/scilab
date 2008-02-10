/*------------------------------------------------------------------------*/
/* file: get_tics_color_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tics_color field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_tics_color_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) == SCI_AXES)
  {
    return sciReturnDouble( sciGetForegroundToDisplay(pobj) ) ;
  }
  else if (sciGetEntityType (pobj) == SCI_SUBWIN)
  {
    sciprint("Warning: tics_color use is deprecated and no more taken into account, use foreground property to edit Axes color.\n");
    return sciReturnDouble( pSUBWIN_FEATURE(pobj)->axes.ticscolor ) ;
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"tics_color");
    return -1;
  }

}
/*------------------------------------------------------------------------*/
