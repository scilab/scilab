/*------------------------------------------------------------------------*/
/* file: get_axes_visible_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the axes_visible field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_axes_visible_property( sciPointObj * pobj )
{

  char * axes_visible[3]  = { NULL, NULL, NULL } ;
  int i ;
  int status = -1 ;

  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"axes_visible") ;
    return -1 ;
  }

  for ( i = 0 ; i < 3 ; i++ )
  {
    axes_visible[i] = MALLOC( 4 * sizeof(char) ) ;
    if ( axes_visible[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( axes_visible[j] ) ;
        sciprint("Not enough memory to display vector.\n") ;
        return -1 ;
      }
    }
    if ( pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] )
    {
      strcpy( axes_visible[i], "on" ) ;
    }
    else
    {
      strcpy( axes_visible[i], "off" ) ;
    }
  }

  status = sciReturnRowStringVector( axes_visible, 3 ) ;

  for ( i = 0 ; i < 3 ; i++ )
  {
    FREE( axes_visible[i] ) ;
  }

  return status ;
}
/*------------------------------------------------------------------------*/
