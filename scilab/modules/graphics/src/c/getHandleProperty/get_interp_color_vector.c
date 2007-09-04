/*------------------------------------------------------------------------*/
/* file: get_interp_color_vector_property.c                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the interp_color_vector field of */
/*        a handle.                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*-----------------------------------------------------------------------------------*/
int get_interp_color_vector_property( sciPointObj * pobj )
{
  int * interpVector = NULL ;
  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("interp_color_vector property does not exist for this handle.\n") ;
    return -1 ;
  }
  interpVector = sciGetInterpVector( pobj ) ;
  if( interpVector != NULL )
  {
    return sciReturnRowVectorFromInt( interpVector, pPOLYLINE_FEATURE(pobj)->n1 ) ;
  }
  else
  {
    return sciReturnEmptyMatrix() ;
  }
}
/*-----------------------------------------------------------------------------------*/
