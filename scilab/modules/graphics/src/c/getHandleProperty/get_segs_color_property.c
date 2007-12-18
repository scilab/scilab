/*------------------------------------------------------------------------*/
/* file: get_segs_color_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the segs_color field             */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_segs_color_property( sciPointObj * pobj )
{
  double * colors = NULL ;
  int nbSegs = 0 ;
  int i ;
  int status = -1 ;
  if ( sciGetEntityType( pobj ) != SCI_SEGS || pSEGS_FEATURE(pobj)->ptype != 0 )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"segs_color") ;
    return -1 ;
  }

  /* convert from int array to double one. */
  nbSegs = pSEGS_FEATURE(pobj)->Nbr1 / 2 ;
  colors = MALLOC( nbSegs * sizeof(double) ) ;
  if ( colors == NULL )
  {
    sciprint("Error returing segs color, memory full.\n") ;
    return -1 ;
  }

  for ( i = 0 ; i  < nbSegs ; i++ )
  {
    colors[i] = pSEGS_FEATURE (pobj)->pstyle[i] ;
  }

  status = sciReturnRowVector( colors, pSEGS_FEATURE(pobj)->Nbr1 / 2 ) ;

  FREE( colors ) ;

  return status ;

}
/*------------------------------------------------------------------------*/
