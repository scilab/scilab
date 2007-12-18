/*------------------------------------------------------------------------*/
/* file: get_rotation_angles_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the rotation_angles field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_rotation_angles_property( sciPointObj * pobj )
{
  /* DJ.A 2003 */
  double angles[2] ;
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"rotation_angle") ;
    return -1 ;
  }

  angles[0] = pSUBWIN_FEATURE(pobj)->alpha ;
  angles[1] = pSUBWIN_FEATURE(pobj)->theta ;

  return sciReturnRowVector( angles, 2 ) ;


}
/*------------------------------------------------------------------------*/
