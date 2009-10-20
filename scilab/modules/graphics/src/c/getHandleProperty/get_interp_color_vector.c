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
/* file: get_interp_color_vector_property.c                               */
/* desc : function to retrieve in Scilab the interp_color_vector field of */
/*        a handle.                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int get_interp_color_vector_property( sciPointObj * pobj )
{
  int * interpVector = NULL ;
  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"interp_color_vector") ;
    return -1 ;
  }
  interpVector = sciGetInterpVector( pobj ) ;
  if( interpVector != NULL )
  {
    return sciReturnRowVectorFromInt( interpVector, pPOLYLINE_FEATURE(pobj)->n1 ) ;
  }
  else
  {
    return sciReturnEmptyMatrix() ;
  }
}
/*--------------------------------------------------------------------------*/
