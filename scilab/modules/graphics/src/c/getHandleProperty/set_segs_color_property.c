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
/* file: set_segs_color_property.c                                        */
/* desc : function to modify in Scilab the segs_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_segs_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int nbSegs = 0 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "segs_color");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SEGS || pSEGS_FEATURE(pobj)->ptype != 0 )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"segs_color");
    return SET_PROPERTY_ERROR ;
  }

  nbSegs = pSEGS_FEATURE(pobj)->Nbr1 / 2 ;

  if ( nbRow * nbCol == 1 )
  {
    int i ;
    int value = (int) getDoubleFromStack( stackPointer ) ;
    pSEGS_FEATURE (pobj)->iflag = 0 ;
    for ( i = 0 ; i < nbSegs ; i++ )
    {
      pSEGS_FEATURE (pobj)->pstyle[i] = value ;
    }
  }
  else if ( nbRow * nbCol == nbSegs )
  {
    pSEGS_FEATURE (pobj)->iflag = 1 ;
    copyDoubleVectorToIntFromStack( stackPointer, pSEGS_FEATURE (pobj)->pstyle, nbSegs ) ;
  }
  else
  {
    Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "segs_color", 1, nbSegs);
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
