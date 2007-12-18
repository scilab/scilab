/*------------------------------------------------------------------------*/
/* file: set_triangles_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the surface_color field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_triangles_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"triangles") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_FEC )
  {
    sciprint("triangles property does not exist for this handle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbCol != 5 )
  {
    sciprint("Second argument must have 5 columns.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  
  
  if ( nbRow != pFEC_FEATURE (pobj)->Ntr )
  {
    /* need to realocate */
    double * pnoeud ;

    pnoeud = createCopyDoubleVectorFromStack( stackPointer, nbRow * 5 ) ;

    if ( pnoeud == NULL )
    {
      sciprint( "Unable to allocate new triangle list, memory full.\n" ) ;
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
