/*------------------------------------------------------------------------*/
/* file: get_sub_tics_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the sub_tics or sub_ticks field  */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_sub_tics_property( sciPointObj * pobj )
{

  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */

  if ( sciGetEntityType (pobj) == SCI_AXES )
  {
    return sciReturnDouble( pAXES_FEATURE(pobj)->subint ) ;
  }
  else if ( sciGetEntityType (pobj) == SCI_SUBWIN )
  {
    double sub_ticks[3] ;
    int i ;
    for ( i = 0 ; i < 3 ; i++ )
    {
      sub_ticks[i] = pSUBWIN_FEATURE (pobj)->axes.nbsubtics[i];
    }
    if ( sciGetIs3d( pobj ) )
    {
      return sciReturnRowVector( sub_ticks, 3 ) ;
    }
    else
    {
      return sciReturnRowVector( sub_ticks, 2 ) ;
    }
  }
  else
  {
    sciprint("sub_ticks property does not exist for this handle.\n") ;
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
