/*------------------------------------------------------------------------*/
/* file: get_arrow_size_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the arrow_size field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_arrow_size_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SEGS )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"arrow_size") ;
    return -1 ;
  }
  return sciReturnDouble( sciGetArrowSize(pobj) ) ;
}
/*------------------------------------------------------------------------*/
