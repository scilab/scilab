/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: ObjectSelection.c                                                */
/* desc : Set of functions to select children of an object                */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "ObjectSelection.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "HandleManagement.h"

/*--------------------------------------------------------------------------*/
void sciInitSelectedSons( sciPointObj * pObj )
{
    abort();
// ???
//  sciGetRelationship(pObj)->pSelectedSon = DoublyLinkedList_new() ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciGetFirstSelectedSon( sciPointObj * pObj )
{
    abort();
//  return (sciPointObj *) List_data(((TypedSonsList*)List_data(sciGetRelationship(pObj)->pSelectedSon))->typedSons ) ;
    return NULL;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciGetFirstTypedSelectedSon( sciPointObj * pObj, sciEntityType objType )
{
    abort();
    return NULL;
// ???
//return (sciPointObj *) List_data( sciGetTypedSelectedSons( pObj, objType ) ) ;
}
/*--------------------------------------------------------------------------*/
DoublyLinkedList * sciGetTypedSelectedSons( sciPointObj * pObj, sciEntityType objType )
{
    abort();
    return NULL;
//???
#if 0
  TypedSonsList * curList = sciGetTypedList( pObj, objType ) ;
  if ( curList == NULL )
  {
    return NULL ;
  }
  return curList->typedSons ;
#endif
}
/*--------------------------------------------------------------------------*/
int sciRemoveSelectedSon( sciPointObj * pParent, sciPointObj * pObj )
{
    abort();
//???
#if 0
  TypedSonsList * curList = sciGetTypedList( pParent, sciGetEntityType(pObj) ) ;
  curList->typedSons = List_free_item( curList->typedSons, pObj ) ;
#endif
  return 0 ;
}
/*--------------------------------------------------------------------------*/
void sciUnselectSons( sciPointObj * pParent )
{
    abort();
//???
//  List_free_full( sciGetRelationship(pParent)->pSelectedSon, deleteTypedSonList ) ;
//  sciGetRelationship(pParent)->pSelectedSon = DoublyLinkedList_new() ;
}
/*--------------------------------------------------------------------------*/
void sciUnselectTypedSons( sciPointObj * pParent, sciEntityType sonsType )
{
    abort();
// ???
#if 0
  /* first search if there are already objects with the specified type */
  TypedSonsList * typedList = sciGetTypedList( pParent, sonsType ) ;

  if ( typedList == NULL ) { return ; } /* no item with this type */

  /* remove the input for this type in the main list */
  sciGetRelationship(pParent)->pSelectedSon = List_free_item( sciGetRelationship(pParent)->pSelectedSon, typedList ) ;

  /* free the input and the list of object with this type */
  deleteTypedSonList( typedList ) ;
#endif
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsSelected( sciPointObj * pObj )
{
    abort();
    return FALSE;
// ???
//  DoublyLinkedList * curList = sciGetTypedSelectedSons( sciGetParent(pObj), sciGetEntityType(pObj) ) ;
//  return ( curList != NULL && List_find( curList, pObj ) != NULL ) ;
}
/*--------------------------------------------------------------------------*/
TypedSonsList * sciGetTypedList( sciPointObj * pObj, sciEntityType objType )
{
    abort();
    return NULL;
// ???
#if 0
  TypedSonsList refType = { objType, NULL } ; /* just for comparison with type */
  DoublyLinkedList * foundList = List_find_full( sciGetRelationship(pObj)->pSelectedSon, &refType, hasSameType ) ;
  if ( foundList == NULL ) { return NULL ; }
  return (TypedSonsList *) List_data( foundList ) ;
#endif
}
/*--------------------------------------------------------------------------*/
TypedSonsList * newTypedSonList( sciEntityType type, DoublyLinkedList * typedSons )
{
  TypedSonsList * newList = MALLOC(sizeof(TypedSonsList)) ;
  newList->sonType   = type ;
  newList->typedSons = typedSons ;

  return newList ;
}
/*--------------------------------------------------------------------------*/
void deleteTypedSonList( void * typedSonsList )
{
  if ( ((TypedSonsList *)typedSonsList)->typedSons != NULL )
  {
    List_free( ((TypedSonsList *)typedSonsList)->typedSons ) ;
  }
  FREE(typedSonsList) ;
}
/*--------------------------------------------------------------------------*/
