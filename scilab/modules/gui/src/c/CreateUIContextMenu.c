
#include "CreateUIContextMenu.h"

/**
 * CreateUIContextMenu
 * @author Vincent COUVERT
 */
sciPointObj * CreateUIContextMenu(void)
{
  int ** userData = NULL ;
  int *  udSize   = NULL ;

  sciPointObj *pobj = (sciPointObj *) NULL;
  sciUimenu *ppobj=NULL;

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
