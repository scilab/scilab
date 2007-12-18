/*------------------------------------------------------------------------*/
/* file: get_alignment_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the alignment field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_alignment_property( sciPointObj * pobj )
{
  if ( sciGetEntityType( pobj ) == SCI_TEXT )
  {
    switch ( sciGetAlignment( pobj ) )
    {
    case ALIGN_LEFT :
      return sciReturnString("left") ;
      break;
    case ALIGN_RIGHT:
      return sciReturnString("right") ;
      break ;
    case ALIGN_CENTER:
      return sciReturnString("center");
      break ;
    default:
      sciprint("error accessing alignment property.\n");
      return -1 ;
    }
  }
  sciprint(_("%s property does not exist for this handle.\n"),"alignment");
  return -1 ;

}
/*------------------------------------------------------------------------*/
