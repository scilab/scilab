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
/* file: ObjectSelection.h                                                */
/* desc : Set of functions to select children of an object                */
/*------------------------------------------------------------------------*/

#ifndef _OBJECT_SELECTION_H_
#define _OBJECT_SELECTION_H_

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "BOOL.h"
#include "DoublyLinkedList.h" 

/**
 * The set of selected sons is lade as follow:
 * Every selected sons is put in a list with every element
 * of its type.
 * Then the lists are put in a main list wich regroup every
 * list with objects with the same type.
 * Moreover each element of the main contains also the type of objects wich will be stored in.
 * Consequentely, the selected sons are stored in a list of typedSonsList.
 */
typedef struct  
{
  sciEntityType      sonType   ; /* type of the sons in the list */
  DoublyLinkedList * typedSons ; /* list of all selected sons with type sonType */
} TypedSonsList ;

/**
 * Add an object in the set of selected son of the parent.
 * Be careful, pObj should be a son of pParent.
 */
GRAPHICS_IMPEXP int sciAddSelectedSon( sciPointObj * pParent, sciPointObj * pObj ) ;

/**
 * Add an object in the set of selected son of the parent.
 * If an other object with the same type is already inserted,
 * it will be destroyed to let the place for this one.
 */
GRAPHICS_IMPEXP int sciAddUniqueSelectedSon( sciPointObj * pParent, sciPointObj * pObj ) ;

/**
 * Initialize the set of sons
 */
GRAPHICS_IMPEXP void sciInitSelectedSons( sciPointObj * pObj ) ;

/**
 * Return the first son found in the set of selected sons.
 */
GRAPHICS_IMPEXP sciPointObj * sciGetFirstSelectedSon( sciPointObj * pObj ) ;

/**
 * Return the first son with a certain type found in the set of selected sons
 */
GRAPHICS_IMPEXP sciPointObj * sciGetFirstTypedSelectedSon( sciPointObj * pObj, sciEntityType objType ) ;

/**
 * Get the list of selected sons with a certain type.
 * This is a list of sciPointObj *.
 * Be careful, the value is the direct pointer on the data.
 */
GRAPHICS_IMPEXP DoublyLinkedList * sciGetTypedSelectedSons( sciPointObj * pObj, sciEntityType objType ) ;

/**
 * Remove a selected object from the set.
 */
GRAPHICS_IMPEXP int sciRemoveSelectedSon( sciPointObj * pParent, sciPointObj * pObj ) ;

/**
 * Free the set of sons
 */
GRAPHICS_IMPEXP void sciUnselectSons( sciPointObj * pParent ) ;

/**
 * Remove all selected sons from a certain type
 */
GRAPHICS_IMPEXP void sciUnselectTypedSons( sciPointObj * pParent, sciEntityType sonsType ) ;

/**
 * To know if an object is selected
 */
GRAPHICS_IMPEXP BOOL sciGetIsSelected( sciPointObj * pObj ) ;

/*------------------------------------------------------------------------*/
/* Internal use */
GRAPHICS_IMPEXP TypedSonsList * sciGetTypedList( sciPointObj * pObj, sciEntityType objType ) ;

GRAPHICS_IMPEXP TypedSonsList * newTypedSonList( sciEntityType type, DoublyLinkedList * typedSons ) ;

/**
 * Void * to be used directly by the list object
 */
GRAPHICS_IMPEXP void deleteTypedSonList( void * typedSonsList ) ;

/*------------------------------------------------------------------------*/

#endif /* _OBJECT_SELECTION_H_ */
