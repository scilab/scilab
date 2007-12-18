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
#include "localization.h"

/*--------------------------------------------------------------------------*/
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
    int index_  =  0 ;
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
      children[index_] = sciGetHandle( curSon->pointobj ) ;
      index_++ ;
      curSon = sciGetNextAccessibleSon( curSon ) ;
    }
    status = sciReturnColHandleVector( children, nbChildren ) ;

    FREE( children ) ;

    return status ;
  }
}
/*--------------------------------------------------------------------------*/
