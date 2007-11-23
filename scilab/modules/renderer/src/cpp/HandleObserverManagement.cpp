/*------------------------------------------------------------------------*/
/* file: HandleObserverManagement.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to add observers to a graphic handle           */
/*------------------------------------------------------------------------*/

#include "HandleObserverManagement.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void attachObserver( sciPointObj * pObj, Observer * obs )
{
  pObj->pObservers = List_append( pObj->pObservers, obs ) ;
}
/*---------------------------------------------------------------------------------*/
void detachObserver( sciPointObj * pObj, Observer * obs )
{
  pObj->pObservers = List_free_item( pObj->pObservers, obs ) ;
}
/*---------------------------------------------------------------------------------*/
void notifyAllObservers( sciPointObj * pObj )
{
  DoublyLinkedList * iterator = pObj->pObservers ;
  
  while ( iterator != NULL )
  {
    Observer * obs = (Observer *)List_data(iterator) ;
    obs->update() ;
    iterator = List_next(pObj->pObservers, iterator) ;
  }
}
/*---------------------------------------------------------------------------------*/

}
