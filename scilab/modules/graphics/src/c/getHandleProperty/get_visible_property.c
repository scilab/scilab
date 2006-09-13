/*------------------------------------------------------------------------*/
/* file: get_visible_property                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the visible field of a handle    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/

int get_visible_property( sciPointObj * pobj )
{
  int numRow   = 0 ;
  int numCol   = 0 ;
  int outIndex = 0 ;

  if ( sciGetVisibility( pobj ) )
  {
    numRow   = 1;
    numCol   = 2;
    CreateVar(Rhs+1,"c",&numRow,&numCol,&outIndex);
    strncpy(cstk(outIndex),"on", numRow*numCol);
  }
  else
  {
    numRow   = 1;
    numCol   = 3;
    CreateVar(Rhs+1,"c",&numRow,&numCol,&outIndex);
    strncpy(cstk(outIndex),"off", numRow*numCol);
  }

  return 0 ;
}

/*------------------------------------------------------------------------*/
