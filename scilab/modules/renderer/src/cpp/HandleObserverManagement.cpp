/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Set of functions to add observers to a graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
