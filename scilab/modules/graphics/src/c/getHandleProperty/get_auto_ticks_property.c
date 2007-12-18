/*------------------------------------------------------------------------*/
/* file: get_auto_ticks_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the auto_ticks field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_auto_ticks_property( sciPointObj * pobj )
{

  char * auto_ticks[3]  = { NULL, NULL, NULL } ;
  int i ;
  int status = -1 ;

  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"auto_ticks") ;
    return -1 ;
  }

  for ( i = 0 ; i < 3 ; i++ )
  {
    auto_ticks[i] = MALLOC( 4 * sizeof(char) ) ;
    if ( auto_ticks[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( auto_ticks[j] ) ;
        sciprint("Not enough memory to display ticks.\n") ;
        return -1 ;
      }
    }
    if ( pSUBWIN_FEATURE (pobj)->axes.auto_ticks[i] )
    {
      strcpy( auto_ticks[i], "on" ) ;
    }
    else
    {
      strcpy( auto_ticks[i], "off" ) ;
    }
  }

  status = sciReturnRowStringVector( auto_ticks, 3 ) ;

  for ( i = 0 ; i < 3 ; i++ )
  {
    FREE( auto_ticks[i] ) ;
  }

  return status ;
    
}
/*------------------------------------------------------------------------*/
