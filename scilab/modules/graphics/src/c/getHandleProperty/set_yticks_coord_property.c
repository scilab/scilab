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
/* file: set_ytics_coord_property.c                                       */
/* desc : function to modify in Scilab the ytics_coord field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <math.h>
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
/* @TODO: remove stackPointer, nbRow, nbCol which are used */
int set_ytics_coord_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int N = 0;
  double * vector = NULL;
  char c_format[5];

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "ytics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"ytics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "ytics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE(pobj)->ny == 1 && nbCol != 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "ytics_coord");
    return SET_PROPERTY_ERROR ;
  }

  if (  pAXES_FEATURE(pobj)->ny != 1 && nbCol == 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: At least %d elements expected.\n"), "ytics_coord", 2);
    return SET_PROPERTY_ERROR ;
  }

  /* what follows remains here as it was */

  FREE(pAXES_FEATURE(pobj)->vy); pAXES_FEATURE(pobj)->vy = NULL;

  pAXES_FEATURE(pobj)->vy = createCopyDoubleVectorFromStack( stackPointer, nbCol ) ;


  ComputeXIntervals( pobj, pAXES_FEATURE(pobj)->tics, &vector, &N, 0 ) ;
  ComputeC_format( pobj, c_format ) ;

  if( pAXES_FEATURE(pobj)->str != NULL )
  {
    destroyStringArray( pAXES_FEATURE(pobj)->str, pAXES_FEATURE(pobj)->nb_tics_labels ) ;
  }

  pAXES_FEATURE (pobj)->nb_tics_labels = N;
  pAXES_FEATURE(pobj)->str = copyFormatedArray( vector, N, c_format, 256 ) ;


  FREE( vector ) ;

  if ( pAXES_FEATURE(pobj)->str == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"set_ytics_coord_property");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
