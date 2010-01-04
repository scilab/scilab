/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: get_axes_reverse_property.c                                      */
/* desc : function to retrieve in Scilab the axes_reverse field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_axes_reverse_property( sciPointObj * pobj )
{

  char * axes_reverse[3]  = { NULL, NULL, NULL } ;
  int i ;
  int status = -1 ;

  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_reverse") ;
    return -1 ;
  }

  for ( i = 0 ; i < 3 ; i++ )
  {
    axes_reverse[i] = MALLOC( 4 * sizeof(char) ) ;
    if ( axes_reverse[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( axes_reverse[j] ) ;
				Scierror(999, _("%s: No more memory.\n"),"get_axes_reverse_property");
        return -1 ;
      }
    }
    if ( pSUBWIN_FEATURE (pobj)->axes.reverse[i] )
    {
      strcpy( axes_reverse[i], "on" ) ;
    }
    else
    {
      strcpy( axes_reverse[i], "off" ) ;
    }
  }

  status = sciReturnRowStringVector( axes_reverse, 3 ) ;

  for ( i = 0 ; i < 3 ; i++ )
  {
    FREE( axes_reverse[i] ) ;
  }

  return status ;
}
/*------------------------------------------------------------------------*/
