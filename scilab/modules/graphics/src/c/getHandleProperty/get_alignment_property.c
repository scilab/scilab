/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_alignment_property.c                                         */
/* desc : function to retrieve in Scilab the alignment field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
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
      Scierror(999, _("Wrong value for '%s' property.\n"),"alignment");
      return -1 ;
    }
  }
  Scierror(999, _("'%s' property does not exist for this handle.\n"),"alignment");
  return -1 ;

}
/*------------------------------------------------------------------------*/
