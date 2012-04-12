
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

#include "HandleManagement.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "WindowList.h"
#include "ObjectSelection.h"
#include "BuildDrawingObserver.h"
#include "SetJavaProperty.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Scierror.h"
#include "SciHandleTab.h"

static int sciRelocateObject( sciPointObj * movedObj, sciPointObj * newParent );

/*********************************** Handle ******************************************/

/**sciSetHandle
 * Sets the handle to this object used only by sciAddNewHandle !!!
 */
void sciSetHandle (sciPointObj * pobj, long value)
{
  pobj->handleIndex = value ; /** put the new index handle */
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
    //return (pobj == NULL ? 0 : (sciGetRelationship(pobj))->handleIndex) ;
    return pobj->handleIndex;
}



/**sciGetPointFromHandle
 * Returns the object pointer form the handle argument
 */
sciPointObj * sciGetPointerFromHandle(long handle)
{
		return sciGetObjectFromHandle(handle);
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
