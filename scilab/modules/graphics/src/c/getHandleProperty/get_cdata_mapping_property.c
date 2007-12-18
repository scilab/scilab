/*------------------------------------------------------------------------*/
/* file: get_cdata_mapping_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the cdata_mapping field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_cdata_mapping_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_SURFACE  )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"cdata_mapping");
    return -1;
  }
  if ( pSURFACE_FEATURE (pobj)->typeof3d != SCI_FAC3D )
  {
    sciprint("cdata_mapping property only exists for Fac3d surfaces.\n");
    return -1;
  }

  if( pSURFACE_FEATURE(pobj)->cdatamapping == 0 )
  {
    /* scaled mode */
    return sciReturnString( "scaled" ) ;
  }
  else if( pSURFACE_FEATURE (pobj)->cdatamapping == 1 )
  {
    /* direct mode */
    return sciReturnString( "direct" ) ;
  }

  return -1 ;

}
/*------------------------------------------------------------------------*/
