/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to add and remove drawing observers from a graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

