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
/* file: set_triangles_property.c                                         */
/* desc : function to modify in Scilab the surface_color field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int set_triangles_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "triangles");
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_FEC )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"triangles") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbCol != 5 )
  {
    Scierror(999, _("Wrong size for '%s' property: Must have %d columns.\n"), "triangles", 5);
    return SET_PROPERTY_ERROR ;
  }

  
  
  if ( nbRow != pFEC_FEATURE (pobj)->Ntr )
  {
    /* need to realocate */
    double * pnoeud ;

    pnoeud = createCopyDoubleVectorFromStack( stackPointer, nbRow * 5 ) ;

    if ( pnoeud == NULL )
    {
      Scierror(999, _("%s: No more memory.\n"),"set_triangles_property");
      return SET_PROPERTY_ERROR ;
    }

    /* allocation ok we can change the pnoeud */
    FREE( pFEC_FEATURE(pobj)->pnoeud ) ;
    
    pFEC_FEATURE(pobj)->pnoeud = pnoeud;

  }
  else
  {
    copyDoubleVectorFromStack( stackPointer, pFEC_FEATURE(pobj)->pnoeud, nbRow * 5 ) ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
