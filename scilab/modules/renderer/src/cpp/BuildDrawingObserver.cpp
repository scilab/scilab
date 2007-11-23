/*------------------------------------------------------------------------*/
/* file: BuildDrawingObserver.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions to add and remove drawing observers from a graphic    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "BuildDrawingObserver.h"
#include "HandleDrawingObserver.h"
#include "HandleObserverManagement.h"

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void createDrawingObserver( sciPointObj * pObj )
{
  HandleDrawingObserver * obs = new HandleDrawingObserver( pObj ) ;
  attachObserver( pObj, obs ) ;

}
/*---------------------------------------------------------------------------------*/
void deleteObservers( sciPointObj * pObj )
{
  DoublyLinkedList * iterator = pObj->pObservers ;
  
  while ( iterator != NULL )
  {
    delete ((Observer *)List_data(iterator)) ;
    iterator = List_next(pObj->pObservers, iterator) ;
  }

  List_free( pObj->pObservers ) ;
  pObj->pObservers = NULL ;
}
/*---------------------------------------------------------------------------------*/
void notifyObservers( sciPointObj * pObj )
{
  notifyAllObservers( pObj ) ;
}
/*---------------------------------------------------------------------------------*/

