/*------------------------------------------------------------------------*/
/* file: set_segs_color_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the segs_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_segs_color_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int nbSegs = 0 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"segs_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SEGS || pSEGS_FEATURE(pobj)->ptype != 0 )
  {
    sciprint("segs_color_property does not exist for this handle.\n") ;
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
    sciprint("segs color has a wrong size (%d), expecting 1 or (%d)", nbRow * nbCol , nbSegs );
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
