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
/* file: set_cdata_mapping_property.c                                     */
/* desc : function to modify in Scilab the cdata_mapping field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_cdata_mapping_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  sciSurface * ppSurf = NULL ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"cdata_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SURFACE || pSURFACE_FEATURE(pobj)->typeof3d != SCI_FAC3D )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"cdata_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  ppSurf = pSURFACE_FEATURE ( pobj ) ;

  if ( isStringParamEqual( stackPointer, "scaled" ) )
  {
    if( ppSurf->cdatamapping != 0 )
    { /* not already scaled */
      LinearScaling2Colormap(pobj);
      ppSurf->cdatamapping = 0;
    }
  } 
  else if ( isStringParamEqual( stackPointer, "direct" ) )
  {
    if(pSURFACE_FEATURE (pobj)->cdatamapping != 1)
    { 
      /* not already direct */
      int nc = ppSurf->nc ;

      FREE( ppSurf->color ) ;
      ppSurf->color = NULL ;

      if( nc > 0 )
      {
        if ((ppSurf->color = MALLOC (nc * sizeof (double))) == NULL)
        {
			sciprint(_("%s: No more memory.\n"),"set_cdata_mapping_property");
          return SET_PROPERTY_ERROR ;
        }
      }

      doubleArrayCopy( ppSurf->color, ppSurf->zcol, nc ) ;


      ppSurf->cdatamapping = 1 ;
    }
  }
  else
  {
    sciprint(_("Wrong value for argument: '%s' or '%s' expected.\n"),"scaled","direct");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;


}
/*------------------------------------------------------------------------*/
