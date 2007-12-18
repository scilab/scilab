/*------------------------------------------------------------------------*/
/* file: get_rotation_style_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the rotation_style field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_rotation_style_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("rotation_style property undefined for this object") ;
    return -1;
  }
  if ( pFIGURE_FEATURE(pobj)->rotstyle == 0 )
  {
    return sciReturnString( "unary" ) ;
  }
  else
  {
    return sciReturnString( "multiple" ) ;
  }
}
/*------------------------------------------------------------------------*/
