/*------------------------------------------------------------------------*/
/* file: set_interp_color_vector_property.c                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the background field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_interp_color_vector_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("interp_color_vector can only be set on Polyline objects.\n");
    return -1 ;
  }

  if( ( nbCol == 3 && pPOLYLINE_FEATURE(pobj)->dim_icv == 3 ) || 
      ( nbCol == 4 && pPOLYLINE_FEATURE(pobj)->dim_icv == 4 ) )
  {
    int tmp[4] ;
    int i      ;
    double * values = getDoubleMatrixFromStack( stackPointer ) ;

    /* convert to int */
    for( i = 0 ; i < nbCol ; i++ )
    {
      tmp[i] = (int) values[i] ;
    }

    return sciSetInterpVector( pobj, nbCol, tmp ) ;
  }
  else
  {
    sciprint("Under interpolated color moden the column dimension of the color vector must match the number of points defining the line (which must be 3 or 4).\n") ;
    return -1 ;
  }
  return -1 ;

}
/*------------------------------------------------------------------------*/
