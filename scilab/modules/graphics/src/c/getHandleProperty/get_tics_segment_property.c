/*------------------------------------------------------------------------*/
/* file: get_tics_segment_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tics_segment field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_tics_segment_property( sciPointObj * pobj )
{

  if ( sciGetEntityType( pobj ) != SCI_AXES )
  {
    sciprint("tics_segment property does not exist for this handle\n.") ;
    return -1 ;
  }

  if ( pAXES_FEATURE(pobj)->seg == 1 )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {	
    return sciReturnString( "off" ) ;
  }

}
/*------------------------------------------------------------------------*/
