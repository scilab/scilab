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
/* file: get_tics_labels_property.c                                       */
/* desc : function to retrieve in Scilab the tics_labels field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
int get_tics_labels_property( sciPointObj * pobj )
{

  if ( sciGetEntityType (pobj) != SCI_AXES )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_labels") ;
    return -1 ;
  }

  if ( pAXES_FEATURE(pobj)->str == NULL )
  {
    int status = -1 ;

    /* tics_labels is allocatred here */
    StringMatrix * tics_labels = computeDefaultTicsLabels( pobj ) ; /* actually it is vector */

    if ( tics_labels == NULL )
    {
      Scierror(999, _("%s: No more memory.\n"), "get_tics_labels_property") ;
      return -1 ;
    }

    status = sciReturnRowStringVector( getStrMatData( tics_labels ), pAXES_FEATURE (pobj)->nb_tics_labels ) ;

    deleteMatrix( tics_labels ) ;

    return status ;
  }
  else
  {
    /* str has been previously set once */
    return sciReturnRowStringVector( pAXES_FEATURE(pobj)->str, pAXES_FEATURE (pobj)->nb_tics_labels ) ;
  }
}
/*------------------------------------------------------------------------*/
