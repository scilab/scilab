/*------------------------------------------------------------------------*/
/* file: get_text_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the text field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "StringMatrix.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_text_property( sciPointObj * pobj )
{
  int nbRow = 0 ;
  int nbCol = 0 ;
  /* get the size of the text matrix */
  sciGetTextSize( pobj , &nbRow, &nbCol ) ;

  if ( nbRow < 0 || nbCol < 0 )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"text") ;
    return -1;
  }

  return sciReturnStringMatrix( getStrMatData( sciGetText( pobj ) ), nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
