/*------------------------------------------------------------------------*/
/* file: get_colored_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the colored field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_colored_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SEGS || pSEGS_FEATURE (pobj)->ptype == 0 )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"colored") ;
    return -1 ;
  }

  if ( pSEGS_FEATURE (pobj)->typeofchamp == 1 )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {	
    return sciReturnString( "off" ) ; 
  }

}
/*------------------------------------------------------------------------*/
