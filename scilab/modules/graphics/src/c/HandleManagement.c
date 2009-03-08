
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------
 *    Graphic library 
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used in graphics handle management.
 --------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "HandleManagement.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "WindowList.h"
#include "DoublyLinkedList.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "BuildDrawingObserver.h"
#include "SetJavaProperty.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Scierror.h"
#include "SciHandleTab.h"

static int sciSwapObjects( sciPointObj * firstObject, sciPointObj * secondObject );
static int sciRelocateObject( sciPointObj * movedObj, sciPointObj * newParent );
static sciPointObj * getPointerFromJavaIndex(sciPointObj * pObj, int javaIndex);
static sciPointObj * getPointerFromChildrenJavaIndex(sciPointObj * pObj, int javaIndex);

/*********************************** Handle ******************************************/

/**sciSetHandle
 * Sets the handle to this object used only by sciAddNewHandle !!!
 */
void sciSetHandle (sciPointObj * pobj, long value)
{
  sciGetRelationship(pobj)->handleIndex = value ; /** put the new index handle */
}


/**sciAddNewHandle
 * Returns a generated handle for this object, and put the handle and the object in the handle table
 */
int sciAddNewHandle (sciPointObj * pobj)
{
	/* For now we use the value given by the pointer (Fabrice Leray). */
	long objectHandle = generateNewHandle(pobj);
	sciSetHandle(pobj, objectHandle);
  if (sciAddHandleObjectMapping(objectHandle, pobj))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}




/**sciDelHandle
 * Removes this pointed handle from the handle table
 */
int sciDelHandle(sciPointObj * pobj)
{
	if (sciRemoveObjectMapping(pobj) == NULL)
	{
		/* The object is not in the handle tab */
		return -1;
	}
	else
	{
		return 0;
	}
}


/**sciGetHandle
 * Returns the handle 
 */
long sciGetHandle (sciPointObj * pobj)
{
  return (pobj == NULL ? 0 : (sciGetRelationship(pobj))->handleIndex) ;
}



/**sciGetPointFromHandle
 * Returns the object pointer form the handle argument
 */
sciPointObj * sciGetPointerFromHandle(long handle)
{
		return sciGetObjectFromHandle(handle);
}

/**sciGetPointerFromJavaIndex
 * Returns the object pointer form a Java UIElementMapper index
 */
sciPointObj * sciGetPointerFromJavaIndex (int javaIndex)
{
	int nbFigure = sciGetNbFigure();
	int * ids = NULL;
	int i;

	if (nbFigure == 0)
	{
		/* No figures, nothing to search */
		return NULL;
	}

	ids = MALLOC(nbFigure * sizeof(int));
	if (ids == NULL)
	{
		return NULL;
	}

	/* Get the id of all the figures */
	sciGetFiguresId(ids);

	for (i = 0; i < nbFigure; i++)
	{
		sciPointObj * found = getPointerFromJavaIndex(getFigureFromIndex(ids[i]), javaIndex);
		if (found != NULL)
		{
			FREE(ids);
			return found;
		}
	}

	FREE(ids);

	return NULL;
}

static sciPointObj * getPointerFromJavaIndex(sciPointObj * pObj, int javaIndex)
{
	/* For now Uicontrol and Uimenus can only be sons of a figure */
	/* So it is only necessary to look only for figure children. */
	switch (sciGetEntityType(pObj))
	{
	case SCI_UICONTROL:
		if (pUICONTROL_FEATURE(pObj)->hashMapIndex == javaIndex)
		{
			return pObj;
		}
		/* look inside the children */
		return getPointerFromChildrenJavaIndex(pObj, javaIndex);
		break;
	case SCI_UIMENU:
		if (pUIMENU_FEATURE(pObj)->hashMapIndex == javaIndex)
		{
			return pObj;
		}
		/* look inside the children */
		return getPointerFromChildrenJavaIndex(pObj, javaIndex);
		break;
	case SCI_FIGURE:
		return getPointerFromChildrenJavaIndex(pObj, javaIndex);
		break;
	}

	/* The object is not a one which can contained java objects */
	return NULL;
}

static sciPointObj * getPointerFromChildrenJavaIndex(sciPointObj * pObj, int javaIndex)
{
	sciSons * children = sciGetSons(pObj);
	while (children != NULL)
	{
		sciPointObj * found = getPointerFromJavaIndex(children->pointobj, javaIndex);
		if (found != NULL)
		{
			return found;
		}
		children = children->pnext;
	}

	/* No pointer found from the children */
	return NULL;
}

/************************************************ End Handle *************************************************/




/**sciGetRelationship
 * Returns the structure of the relationship. Do not use this in the Consturctor Functions !
 */
sciRelationShip *
sciGetRelationship (sciPointObj * pobj)
{
	return pobj->relationShip;
}


/**sciSetParent
 * Sets the parent to this object (that have to be the son). 
 * The parent's FIGURE has to be NULL
 * pson est l'objet courant et *pparent est le parent a lui associer
 */
int sciSetParent (sciPointObj * pson, sciPointObj * pparent)
{
  sciGetRelationship(pson)->pparent = pparent ;
  return 0;
}


/**sciGetParent
 * Returns the pointer to the parent object
 */
sciPointObj * sciGetParent (sciPointObj * pobj)
{
  return sciGetRelationship(pobj)->pparent ;
}


/****************************************** SONS ******************************************/


/**sciAddThisToItsParent
 * Sets this object to its parent. 
 * The list is pointed from the newer to the older.
 */
BOOL
sciAddThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
  sciSons * OneSon = NULL ;
  
  /* Special case for text, it needs to be registered with its parent subwin */
  if (sciGetEntityType(pthis) == SCI_TEXT)
  {
    if (sciGetParent(pthis) != NULL)
    {
      sciJavaRemoveTextToDraw(pthis, sciGetParentSubwin(pthis));
    }
    sciJavaAddTextToDraw(pthis, sciGetParentSubwin(pparent));
  }

  if ( sciSetParent(pthis, pparent) == -1 )
  {
    return FALSE ;
  }

  if ( pparent == NULL )
  {
    /* nothing more to do */
    return TRUE ;
  }

  /* check if there are already sons */
  if ( sciGetRelationship (pparent)->psons != NULL )
  {			
    /* Il existe au moins un fils d'affecte */
    /* on cree la nouvelle variable */
    if ( (OneSon = MALLOC(sizeof(sciSons))) == NULL ) { return FALSE ; }
    OneSon->pnext = sciGetRelationship (pparent)->psons ;
    OneSon->pprev = NULL;
    sciGetRelationship(pparent)->psons->pprev = OneSon ;
  }
  else
  {
    /* Frist son to be added! */
    if ( (OneSon = MALLOC( sizeof(sciSons) )) == NULL ) {return FALSE ; }
    OneSon->pnext = NULL ;
    OneSon->pprev = NULL ;
    sciGetRelationship(pparent)->plastsons = OneSon ;
  }
  OneSon->pointobj = pthis ;
  sciGetRelationship(pparent)->psons = OneSon ;

  return TRUE;
}

/**
 * Same as sciAddThisToItsParent, but add the son at the end of the sons list.
 * Should be used only in special cases.
 */
BOOL sciAddThisToItsParentLastPos(sciPointObj * pthis, sciPointObj * parent)
{
  sciSons * newSon = NULL ;
  
  /* Special case for text, it needs to be registered with its parent subwin */
  if (sciGetEntityType(pthis) == SCI_TEXT)
  {
    if (sciGetParent(pthis) != NULL)
    {
      sciJavaRemoveTextToDraw(pthis, sciGetParentSubwin(pthis));
    }
    sciJavaAddTextToDraw(pthis, sciGetParentSubwin(parent));
  }

  if ( sciSetParent(pthis, parent) == -1 )
  {
    return FALSE ;
  }

  if ( parent == NULL )
  {
    /* nothing more to do */
    return TRUE ;
  }

  newSon = MALLOC(sizeof(sciSons));
  if (newSon == NULL)
  {
    return FALSE;
  }

  if (sciGetSons(parent) == NULL)
  {
    /* first son to be added */
    newSon->pnext = NULL;
    newSon->pprev = NULL;

    /* update psons and last sons */
    sciGetRelationship(parent)->psons = newSon;
    sciGetRelationship(parent)->plastsons = newSon;
  }
  else
  {
    /* There is alredy a son */
    newSon->pnext = NULL;
    newSon->pprev = sciGetRelationship(parent)->plastsons;

    /* update psons and last sons */
    sciGetRelationship(parent)->plastsons->pnext = newSon;
    sciGetRelationship(parent)->plastsons = newSon;
  }
  newSon->pointobj = pthis;

  return TRUE;
}


/**sciDelThisToItsParent
 * deletes this son object to its parent, 
 * free the son structure, but not the son object structure 
 * (for which its parent is set to null) , 
 * that have to be free manually or packed to another parent
 */
BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
  sciSons *OneSon     = NULL ;
  sciSons *OneSonprev = NULL ;

  /* Special case for text, it needs to be registered with its parent subwin */
  if (sciGetEntityType(pthis) == SCI_TEXT)
  {
    sciJavaRemoveTextToDraw(pthis, sciGetParentSubwin(pthis));
  }

  if ( pparent == NULL )
  {
    /* nothing to do */
    return TRUE ;
  }

  /* search for the object to remove */
  OneSon = (sciGetRelationship (pparent)->psons);
  OneSonprev = OneSon;
  while ( (OneSon != NULL) &&  (OneSon->pointobj != pthis) )
  {
    OneSonprev = OneSon ; 
    OneSon = OneSon->pnext;
  }

  return sciDelSonFromItsParent(OneSon, pparent);
}

/**
 * Remove a son of an object from the sons list.
 * @param son son to remove, must be in the parent children list.
 *            param is no longer usable when the function returns.
 */
BOOL sciDelSonFromItsParent(sciSons * son, sciPointObj * parent)
{
  int tmp = 0;

  /* dans quel cas de figure somme nous ? */
  if ( son == NULL )
  {
    tmp++ ;
  }
  else 
  {
    if ( son->pprev == NULL ) { tmp += 2 ; }
    if ( son->pnext == NULL ) { tmp += 4 ; }
  }

  switch(tmp)
  {
    case 0:/* ok<-OneSon->ok     */
      (son->pnext)->pprev = (son->pprev);
      (son->pprev)->pnext = (son->pnext);
      FREE(son);
      return TRUE;
      break;
    case 2:/* ok<-OneSon->NULL   */
      (sciGetRelationship (parent)->psons) = son->pnext;
      (sciGetRelationship (parent)->psons)->pprev = NULL ;
      FREE(son);
      return TRUE;
      break;
    case 4:/* NULL<-OneSon->ok   */
      sciGetRelationship (parent)->plastsons = son->pprev;
      (sciGetRelationship (parent)->plastsons)->pnext = NULL;
      FREE(son);
      return TRUE;
      break;
    case 6:/* NULL<-OneSon->NULL */
      sciGetRelationship (parent)->plastsons = NULL;
      sciGetRelationship (parent)->psons = NULL;
      FREE(son);
      return TRUE;
      break;
    case 1:/* OneSon == NULL     */
    case 3:
    case 5:
    case 7:
    default :
      return FALSE;
      break;
    }
}


/**sciGetSons
 * Returns the pointer to the table of all sons objects. 
 * There is no SetSons, because a new Son calls sciAddThisToItsParent() it self
 */
sciSons * sciGetSons (sciPointObj * pobj)
{
  return (sciSons *) (sciGetRelationship (pobj)->psons);
}


/**sciGetLastSons
 * Returns the pointer to the last son (in fact the first created and drawn). 
 * There is no SetSons, because a new Son calls sciAddThisToItsParent() it self
 */
sciSons * sciGetLastSons (sciPointObj * pobj)
{
  return (sciSons *)sciGetRelationship (pobj)->plastsons ;
}

/*--------------------------------------------------------------------------*/
/**
 * return the next son in the sons list which is accessible, ie which can be seen in the
 * Scilab console. If there is not any more accessible one, then return NULL.
 */
sciSons * sciGetNextAccessibleSon( sciSons * son )
{
  sciSons * nextSon = son->pnext ;
  while ( nextSon != NULL && !(sciGetIsAccessibleChild( nextSon->pointobj ) ) )
  {
    nextSon = nextSon->pnext ;
  }
  return nextSon ;
}
/*--------------------------------------------------------------------------*/
/**
 * return the first accessible son of an object or NULL if there is not any.
 */
sciSons * sciGetFirstAccessibleSon( sciPointObj * pObj )
{
  sciSons * firstSon = sciGetSons( pObj ) ;
  while( firstSon != NULL && !(sciGetIsAccessibleChild( firstSon->pointobj ) ) )
  {
    firstSon = firstSon->pnext ;
  }
  return firstSon ;
}
/*--------------------------------------------------------------------------*/
/**
 * return the first son of parentObj which points on searchObj.
 * @param[in] searchObj Object we are looking for in the children list.
 * @param[in] parentObj Object containing the list of children.
 * @return The son of the object parent pointing on searchObj, or NULL if there is not any.
 */
sciSons * sciFindSon( sciPointObj * searchedObj, sciPointObj * parentObj )
{
  sciSons * curSon = sciGetSons( parentObj ) ;
  while ( curSon != NULL && curSon->pointobj != searchedObj )
  {
    curSon = curSon->pnext ;
  }
  return curSon ;
}
/*--------------------------------------------------------------------------*/

/************************************ END SONS **************************************/
/*--------------------------------------------------------------------------*/
/**
 * move a graphic object from its position in the hierarchy to an other place.
 * Note that we don't check wether the moved object is compatible with its new parent.
 * @param[in/out] movedObj  graphical object which is moved.
 * @param[in/out] newParent graphical object under which the movedObj will be placed.
 * @return        0 if everything was achieved, -1 otherwise.
 */
static int sciRelocateObject( sciPointObj * movedObj, sciPointObj * newParent )
{
  sciPointObj * oldParent = sciGetParent( movedObj ) ;
  
  if ( oldParent == newParent )
  {
    /* nothing to do */
    return 0 ;
  }
  
  sciDelThisToItsParent( movedObj, oldParent ) ;
  sciAddThisToItsParent( movedObj, newParent ) ;

  /* if an axis has been moved, its previous parent, a figure might haven't son any more.*/
  if ( sciGetEntityType( movedObj ) == SCI_SUBWIN && sciGetIsSelected( movedObj ) )
  {
    sciSelectFirstSubwin( oldParent ) ;

		if (createFirstSubwin(oldParent) == NULL)
		{
			return -1;
		}
  }

  // redraw the object and its children
  forceHierarchyRedraw(movedObj);

  return 0 ;
}
/*--------------------------------------------------------------------------*/
/**
 * check if an object can be the son of an other.
 * In Scilab :
 * - figures are the root of scilab hierarchy and can not be sons of any other object.
 * - axes can only be under a figure object
 * - any other object can be gathered under a subwindow or a compound.
 * @param[in] son object we try to move under parent
 * @return TRUE if son can be moved under parent, FALSE otherwise.
 */
BOOL sciCanBeSonOf( sciPointObj * son, sciPointObj * parent )
{
  sciEntityType parentType ;

  if ( parent == NULL || son == NULL )
  {
    return FALSE ;
  }
  
  parentType = sciGetEntityType( parent ) ;
  switch ( sciGetEntityType( son ) )
  {
  case SCI_FIGURE:
		/* figure are the top objects of the hierarchy */
    return FALSE ;
  case SCI_SUBWIN:
    /* axes can only have figure parents */
    return ( parentType == SCI_FIGURE ) ;
  default:
    return ( parentType == SCI_SUBWIN || parentType == SCI_AGREG ) ;
  }
}
/*--------------------------------------------------------------------------*/
/**
 * move a list of handle from anywhere in the handles hierarchy under another handle.
 * Can be used for example to move an object from a window to another.
 * @param[in] handles Indexes of the handles to relocate.
 * @parem[in] nbHandles Size of the handle array.
 * @param[in] newParentHandle handle of which the handles will be sons.
 * @return    0 if everithing was executed fine, -1 otherwise.
 */
int sciRelocateHandles( unsigned long handles[], int nbHandles, unsigned long newParentHandle )
{
  sciPointObj ** movedObjs = NULL ; /* array of moved objects */
  sciPointObj *  parentObj = sciGetPointerFromHandle( newParentHandle ) ;
  int i ;
  int nbFigure = 0 ;
  DoublyLinkedList * modifiedFiguresList = DoublyLinkedList_new() ; /* list of modified figures. */
                                                              /* Only the needed figures are redrawn at the end of the function. */
  
  /* check parent */
  if ( parentObj == NULL )
  {
    Scierror( 999,_("The parent handle is not or no more valid.\n")) ;
    return -1 ;
  }

  /* we copy the pointer on the objects in this array */
  movedObjs = MALLOC( nbHandles * sizeof(sciPointObj *) ) ;
  if ( movedObjs == NULL )
  {
    Scierror(999,_("%s: No more memory.\n"), "sciRelocateHandles");
    return -1 ;
  }

  /* check handles and copy their object in an array */
  /* It is better to do a first loop, just to test the validity of the handles. */
  /* As a result, we won't need to stop in the middle of the changes. */
  for ( i = 0 ; i < nbHandles ; i++ )
  {
    movedObjs[i] = sciGetPointerFromHandle( handles[i] ) ;
    /* check handle validity */
    if ( movedObjs[i] == NULL )
    {
       Scierror(999,_("Handle number %d is not or no more valid.\n"), i + 1  ) ;
       FREE( movedObjs ) ;
       return -1 ;
    }
    /* check that handles can be inserted under the parent */
    if ( !sciCanBeSonOf( movedObjs[i], parentObj ) )
    {
      Scierror(999,_("Handle number %d is not compatible with the parent handle.\n"), i + 1 ) ;
      FREE( movedObjs ) ;
      return -1 ;
    }
  }

  /* allocate the array with as much space as number of figures. */
  nbFigure = sciGetNbFigure() ;

  /* now move each object */
  for ( i = 0 ; i < nbHandles ; i++ )
  {
    /* both the current and future (wich might be the same) figure of the object */
    /* are modified. */
    sciPointObj * currentFig = sciGetParentFigure( movedObjs[i] ) ;
    sciPointObj * futureFig  = sciGetParentFigure( parentObj    ) ;

    if ( List_find( modifiedFiguresList, currentFig ) == NULL )
    {
      /* the figure was not already considered modified */
      modifiedFiguresList = List_push( modifiedFiguresList, currentFig ) ;
    }

    if ( List_find( modifiedFiguresList, futureFig ) == NULL )
    {
      /* the figure was not already considered modified */
      modifiedFiguresList = List_push( modifiedFiguresList, futureFig ) ;
    }

    if ( sciRelocateObject( movedObjs[i], parentObj ) != 0 )
    {
      Scierror(999,_("Error relocating handle %d."), i  ) ;
    }
  }
  
  FREE( movedObjs ) ;
 
  


  /* redraw the modified figures */
  while( !List_is_empty( modifiedFiguresList ) )
  {
    sciPointObj * modifiedFig  = NULL ;
    modifiedFiguresList = List_pop( modifiedFiguresList, (void *)&modifiedFig ) ;
    notifyObservers( modifiedFig ) ;
  }
  
  List_free( modifiedFiguresList ) ;
  
  return 0 ;
}
/*--------------------------------------------------------------------------*/
/**
 * Change the position of two graphic objects in the scilab hierarchy.
 * The two objects should be compatible with their new parents.
 * @return 0 if the swap was successful, -1 otherwise.
 */
static int sciSwapObjects( sciPointObj * firstObject, sciPointObj * secondObject )
{
  sciSons * firstSon  = NULL ; /* the two sons entitities pointing on the objects */
  sciSons * secondSon = NULL ;
  sciPointObj * firstParent  = sciGetParent( firstObject  ) ; /* current parent of the first object */
  sciPointObj * secondParent = sciGetParent( secondObject ) ;

  /* check compatibility of the objects with new parents */
  if ( !sciCanBeSonOf( firstObject, sciGetParent( secondObject ) ) )
  {
    Scierror(999,_("Handle #%d is not compatible with its new parent.\n"),1) ;
    return -1 ;
  }
  
  if ( !sciCanBeSonOf( secondObject, sciGetParent( firstObject ) ) )
  {
    Scierror(999,_("Handle #%d is not compatible with its new parent.\n"),2) ;
    return -1 ;
  }

  firstSon  = sciFindSon( firstObject , firstParent  ) ;
  secondSon = sciFindSon( secondObject, secondParent ) ;
  
  /* check if the sons are correct. This should always be true. */
  if ( firstSon == NULL )
  {
    Scierror( 999,_("Object #%d is not correctly placed in the hierarchy.\n"),1) ;
    return -1 ;
  }
  if ( secondSon == NULL )
  {
    Scierror( 999,_("Object #%d is not correctly placed in the hierarchy.\n"),2) ;
    return -1 ;
  }

  firstSon->pointobj  = secondObject ;
  secondSon->pointobj = firstObject  ;

  /* change their parents */
  sciSetParent( firstObject , secondParent ) ;
  sciSetParent( secondObject, firstParent  ) ;
  
  /* Special case for texts we need to register them with the new parent */
  if (sciGetEntityType(firstObject) == SCI_TEXT)
  {
    sciJavaRemoveTextToDraw(firstObject, sciGetParentSubwin(firstParent));
    sciJavaAddTextToDraw(firstObject, sciGetParentSubwin(secondParent));
  }

  if (sciGetEntityType(secondObject) == SCI_TEXT)
  {
    sciJavaRemoveTextToDraw(secondObject, sciGetParentSubwin(secondParent));
    sciJavaAddTextToDraw(secondObject, sciGetParentSubwin(firstParent));
  }

  forceHierarchyRedraw(firstObject);
  forceHierarchyRedraw(secondObject);

  return 0 ;

}
/*--------------------------------------------------------------------------*/
/**
 * Swap the position of the two handles in the hierarchy.
 * Both handles should be compatible with their new places.
 * @param[in] firstHandle  Index of an handle.
 * @param[in] secondHandle Index of the other handle.
 * @return 0 if swapping is successful, -1 otherwise.
 */
int swapHandles( unsigned long firstHdl, unsigned long secondHdl )
{
  sciPointObj * firstObject  = sciGetPointerFromHandle( firstHdl  )  ;
  sciPointObj * secondObject = sciGetPointerFromHandle( secondHdl )  ;
  sciPointObj * firstParentFig  = NULL ;
  sciPointObj * secondParentFig = NULL ;
  
  if ( firstObject == NULL )
  {
    Scierror( 999,_("Handle #%d is not or no more valid.\n"),1);
    return -1 ;
  }

  if ( secondObject == NULL )
  {
     Scierror( 999,_("Handle #%d is not or no more valid.\n"),2);
    return -1 ;
  }

  if ( sciSwapObjects( firstObject, secondObject ) != 0 )
  {
    return -1 ;
  }

  firstParentFig  = sciGetParentFigure( firstObject ) ;
  secondParentFig = sciGetParentFigure( secondObject ) ;

  /* redraw the parent figures */
  if ( firstParentFig == secondParentFig )
  {
    /* we need only to redraw once since it is the same figure */
    notifyObservers( firstParentFig ) ;
  }
  else
  {
    notifyObservers( firstParentFig )  ;
    notifyObservers( secondParentFig ) ;
  }
  
  return 0 ;

}
/*--------------------------------------------------------------------------*/
BOOL isHandleValid(long handle)
{
  return (sciGetPointerFromHandle(handle) != NULL);
}
/*--------------------------------------------------------------------------*/
/**
 * Get a son of an object knowing its position within the children.
 * @return a pointer on the object or NULL if the index is out of bounds
 */
sciPointObj * sciGetIndexedSon(sciPointObj * pobj, int position)
{
  sciSons * curSon = sciGetSons( pobj ) ;
  int curIndex = 0;
  while (curSon != NULL && curIndex < position)
  {
    curSon = curSon->pnext ;
    curIndex++;
  }
  if (curSon == NULL)
  {
    return NULL;
  }
  return curSon->pointobj ;
}
/*--------------------------------------------------------------------------*/
long generateNewHandle(sciPointObj * pObj)
{
	static long handleValue = 0;
	/* Start with 1, 0 stands for invalid index */
	handleValue++;

	return handleValue;
}
/*--------------------------------------------------------------------------*/
