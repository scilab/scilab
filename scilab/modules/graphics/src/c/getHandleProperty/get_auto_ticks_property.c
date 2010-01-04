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
/* file: get_auto_ticks_property.c                                        */
/* desc : function to retrieve in Scilab the auto_ticks field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_auto_ticks_property( sciPointObj * pobj )
{

  char * auto_ticks[3]  = { NULL, NULL, NULL } ;
  int i ;
  int status = -1 ;

  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"auto_ticks") ;
    return -1 ;
  }

  for ( i = 0 ; i < 3 ; i++ )
  {
    auto_ticks[i] = MALLOC( 4 * sizeof(char) ) ;
    if ( auto_ticks[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( auto_ticks[j] ) ;
				Scierror(999, _("%s: No more memory.\n"),"get_auto_ticks_property");
        return -1 ;
      }
    }
    if ( pSUBWIN_FEATURE (pobj)->axes.auto_ticks[i] )
    {
      strcpy( auto_ticks[i], "on" ) ;
    }
    else
    {
      strcpy( auto_ticks[i], "off" ) ;
    }
  }

  status = sciReturnRowStringVector( auto_ticks, 3 ) ;

  for ( i = 0 ; i < 3 ; i++ )
  {
    FREE( auto_ticks[i] ) ;
  }

  return status ;
    
}
/*------------------------------------------------------------------------*/
