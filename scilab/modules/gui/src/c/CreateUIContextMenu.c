/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateUIContextMenu.h"
#include "BuildDrawingObserver.h" /* createDrawingObserver */
#include "ContextMenu.h" /* createContextMenu */
sciPointObj * CreateUIContextMenu(void)
{
  sciPointObj *pobj = (sciPointObj *) NULL;
  sciUicontextmenu *ppobj=NULL;

  if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
    {	
      return (sciPointObj *) NULL;
    }

  sciSetEntityType (pobj, SCI_UICONTEXTMENU);

  if ((pobj->pfeatures = MALLOC ((sizeof (sciUicontextmenu)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  ppobj=pUICONTEXTMENU_FEATURE (pobj);

  /* add the handle in the handle list */
  if ( sciAddNewHandle(pobj) == -1 )
    {
      FREE( pobj->pfeatures ) ;
      FREE( pobj ) ;
      return NULL ;
    }

  /* no sons for now */
  sciInitSelectedSons( pobj ) ;
      
  sciGetRelationship(pobj)->psons        = NULL ;
  sciGetRelationship(pobj)->plastsons    = NULL ;
  sciGetRelationship(pobj)->pSelectedSon = NULL ;
      
  sciInitVisibility( pobj, TRUE ) ;
      
  pobj->pObservers = DoublyLinkedList_new() ;
  createDrawingObserver( pobj ) ;
      
  pobj->pDrawer = NULL ;
 
  createContextMenu((sciPointObj *) pobj);
      
  return (sciPointObj *) pobj;
}
/*----------------------------------------------------------------------------*/
