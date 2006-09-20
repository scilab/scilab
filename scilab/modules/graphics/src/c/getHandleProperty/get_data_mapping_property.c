/*------------------------------------------------------------------------*/
/* file: get_data_mapping_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the data_mapping field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_data_mapping_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_GRAYPLOT)
  {
    sciprint("data_mapping property does not exist for this handle.\n");
    return -1;
  }

  return sciReturnString( pGRAYPLOT_FEATURE(pobj)->datamapping ) ;

}
/*------------------------------------------------------------------------*/
