/*------------------------------------------------------------------------*/
/* file: get_format_n_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the format_n field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_format_n_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"format_n");
    return -1;
  }
  return sciReturnString( pAXES_FEATURE (pobj)->format ) ;
}
/*------------------------------------------------------------------------*/
