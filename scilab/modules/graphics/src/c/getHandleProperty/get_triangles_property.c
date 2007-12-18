/*------------------------------------------------------------------------*/
/* file: get_triangles_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the triangles field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_triangles_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_FEC )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"triangles") ;
    return -1;
  }
  
  return sciReturnMatrix( pFEC_FEATURE (pobj)->pnoeud, pFEC_FEATURE (pobj)->Ntr, 5 ) ;

}
/*------------------------------------------------------------------------*/
