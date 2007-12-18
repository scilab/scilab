/*------------------------------------------------------------------------*/
/* file: get_tight_limits_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tight_limits field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_tight_limits_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    sciprint(_("%s property does not exist for this handle.\n"),"tight_limits") ;
    return -1 ;
  }

  if ( pSUBWIN_FEATURE (pobj)->tight_limits )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }

}
/*------------------------------------------------------------------------*/
