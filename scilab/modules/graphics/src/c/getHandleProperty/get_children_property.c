/*------------------------------------------------------------------------*/
/* file: get_children_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the children field of a          */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "sciprint.h"

/*-----------------------------------------------------------------------------------*/
int get_children_property( sciPointObj * pobj )
{
  sciSons * curSon = NULL ;
  int nbChildren = sciGetNbAccessibleChildren( pobj ) ;

  if ( nbChildren == 0 )
  {
    return sciReturnEmptyMatrix() ;
  }
  else
  {
    int index  =  0 ;
    int status = -1 ;
    long * children = NULL ;
    
    children = MALLOC( nbChildren * sizeof(long) ) ;
    if ( children == NULL )
    {
      sciprint("Error returning children vector, memory full.\n") ;
      return -1 ;
    }

    curSon = sciGetFirstAccessibleSon( pobj ) ;
    while ( curSon != NULL && curSon->pointobj != NULL )
    {
      children[index] = sciGetHandle( curSon->pointobj ) ;
      index++ ;
      curSon = sciGetNextAccessibleSon( curSon ) ;
    }
    status = sciReturnRowHandleVector( children, nbChildren ) ;

    FREE( children ) ;

    return status ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
