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
/* file: get_segs_color_property.c                                        */
/* desc : function to retrieve in Scilab the segs_color field             */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_segs_color_property( sciPointObj * pobj )
{
  double * colors = NULL ;
  int nbSegs = 0 ;
  int i ;
  int status = -1 ;
  if ( sciGetEntityType( pobj ) != SCI_SEGS || pSEGS_FEATURE(pobj)->ptype != 0 )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"segs_color") ;
    return -1 ;
  }

  /* convert from int array to double one. */
  nbSegs = pSEGS_FEATURE(pobj)->Nbr1 / 2 ;
  colors = MALLOC( nbSegs * sizeof(double) ) ;
  if ( colors == NULL )
  {
	  Scierror(999, _("%s: No more memory.\n"),"get_segs_color_property");
	  return -1 ;
  }

  for ( i = 0 ; i  < nbSegs ; i++ )
  {
    colors[i] = pSEGS_FEATURE (pobj)->pstyle[i] ;
  }

  status = sciReturnRowVector( colors, pSEGS_FEATURE(pobj)->Nbr1 / 2 ) ;

  FREE( colors ) ;

  return status ;

}
/*------------------------------------------------------------------------*/
