/*------------------------------------------------------------------------*/
/* file: get_text_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the text_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_text_box_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) == SCI_TEXT ) 
  {
    double size[2] ;
    sciGetUserSize( pobj, &(size[0]), &(size[1]) ) ;
    return sciReturnRowVector( size, 2 ) ;
  }
  sciprint(_("%s property does not exist for this handle.\n"),"text_box") ;
  return -1;
}
/*------------------------------------------------------------------------*/
