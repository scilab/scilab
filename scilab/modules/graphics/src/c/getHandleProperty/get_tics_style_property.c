/*------------------------------------------------------------------------*/
/* file: get_tics_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tics_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_tics_style_property( sciPointObj * pobj )
{

  return sciReturnChar( pAXES_FEATURE (pobj)->tics ) ;

}
/*------------------------------------------------------------------------*/
