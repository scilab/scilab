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
/* file: set_xtics_coord_property.c                                       */
/* desc : function to modify in Scilab the xtics_coord field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "BasicAlgos.h"
#include "Format.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int set_xtics_coord_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int N = 0;
  double * vector = NULL;
  char c_format[5];

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "xtics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"xtics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "xtics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->nx == 1 && nbCol != 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "xtics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if (  pAXES_FEATURE(pobj)->nx != 1 && nbCol == 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: At least %d elements expected.\n"), "xtics_coord", 2);
    return SET_PROPERTY_ERROR ;
  }

  /* what follows remains here as it was */

  pAXES_FEATURE(pobj)->nx = nbCol ;

  FREE(pAXES_FEATURE(pobj)->vx); pAXES_FEATURE(pobj)->vx = NULL;

  pAXES_FEATURE(pobj)->vx = createCopyDoubleVectorFromStack( stackPointer, nbCol ) ;


  if (ComputeXIntervals( pobj, pAXES_FEATURE(pobj)->tics, &vector, &N, 0 ) != 0)
	{
		/* Somthing wrong happened */
		FREE( vector ) ;
		return -1;
	}

  if (ComputeC_format( pobj, c_format ) != 0)
	{
		/* Somthing wrong happened */
		FREE( vector ) ;
		return -1;
	}

  if( pAXES_FEATURE(pobj)->str != NULL )
  {
    destroyStringArray( pAXES_FEATURE(pobj)->str, pAXES_FEATURE(pobj)->nb_tics_labels ) ;
  }

  pAXES_FEATURE (pobj)->nb_tics_labels = N;
  pAXES_FEATURE(pobj)->str = copyFormatedArray( vector, N, c_format, 256 ) ;


  FREE( vector ) ;

  if ( pAXES_FEATURE(pobj)->str == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"set_xtics_coord_property");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
