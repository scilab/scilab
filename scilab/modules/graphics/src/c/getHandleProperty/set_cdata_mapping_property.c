/*------------------------------------------------------------------------*/
/* file: set_cdata_mapping_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
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
int set_cdata_mapping_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
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
			sciprint(_("%s: No more memory\n"),"set_cdata_mapping_property");
          return SET_PROPERTY_ERROR ;
        }
      }

      doubleArrayCopy( ppSurf->color, ppSurf->zcol, nc ) ;


      ppSurf->cdatamapping = 1 ;
    }
  }
  else
  {
    sciprint("cdata_mapping value must be 'scaled' or 'direct'.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;


}
/*------------------------------------------------------------------------*/
