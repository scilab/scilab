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
/* file: set_sub_tics_property.c                                          */
/* desc : function to modify in Scilab the sub_tics field of              */
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
int set_sub_tics_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "sub_tics");
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) == SCI_AXES)
  {
    pAXES_FEATURE(pobj)->subint= (int) getDoubleFromStack( stackPointer ) ;
  }
  else if ( sciGetEntityType(pobj) == SCI_SUBWIN ) 
  {
    int i ;
    double * values = getDoubleMatrixFromStack( stackPointer ) ;
    sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
    if ( (nbCol != 3 ) && (nbCol != 2) )
    {
      Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "sub_tics", 2, 3);
      return  SET_PROPERTY_ERROR ;
    }
    ppSubWin->flagNax = TRUE;
    for ( i = 0; i < nbCol ; i++ )
    {
      int  nbTicks ;

      nbTicks = (int) values[i] ;
      if( nbTicks >= 0 )
      {
        ppSubWin->axes.nbsubtics[i] = nbTicks ;
      } 
      else
      {
        ppSubWin->axes.nbsubtics[i] = 0 ;
      }
    }
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
