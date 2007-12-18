/*------------------------------------------------------------------------*/
/* file: get_auto_clear_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the auto_clear field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_auto_clear_property( sciPointObj * pobj )
{
  if (   sciGetEntityType(pobj) != SCI_SUBWIN 
      && sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("auto_clear property does not exist for this handle\n.") ;
    return -1 ;
  }

  if (!sciGetAddPlot((sciPointObj *)pobj))
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
