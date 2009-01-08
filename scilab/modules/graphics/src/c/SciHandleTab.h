/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SCI_HANDLE_TAB_H_
#define _SCI_HANDLE_TAB_H_

#include "hashtable.h"
#include "ObjectStructure.h"

/*--------------------------------------------------------------------------*/ 
/**
 * Hide the real type of hashtable
 */
typedef struct hashtable SciHandleTab;



/*--------------------------------------------------------------------------*/
/**
 * @return a new empty table of handle.
 */
SciHandleTab * createHandleTab(void);

/**
 * Destroy an instance of handle tab.
 */
void destroyHandleTab(SciHandleTab * tab);

/**
 * Add mapping between an handle and a graphic object in a tab.
 * @return TRUE if the adding has been succesfully completed, false otherwise.
 */
BOOL addHandleObjectMapping(SciHandleTab * tab, long handle, sciPointObj * pObj);

/**
 * Remove a handle and its mapping in a tab.
 * @return removed Object
 */
sciPointObj * removeHandleObjectMapping(SciHandleTab * tab, long handle);

/**
 * Remove a handle and its mapping in a tab, knowing the object.
 * @return the object itself or NULL if it has not been found in the hashtable.
 */
sciPointObj * removeObjectMapping(SciHandleTab * tab, sciPointObj * pObj);

/**
 * Search in the handle tab the object corresponding to the handle.
 * @return the object corresponding to the handle or NULL if the object is not found.
 */
sciPointObj * getObjectFromHandle(SciHandleTab * tab, long handle);
/*--------------------------------------------------------------------------*/
/**
 * Create a new hashTable of get functions
 */
SciHandleTab * getScilabHandleTab(void) ;

/**
 * destroy the handle tab used by Scilab
 */
void destroyScilabHandleTab(void) ;

/**
 * Add mapping in the Scilab HandleTab between an handle and a graphic object in a tab.
 * @return TRUE if the adding has been succesfully completed, false otherwise.
 */
BOOL sciAddHandleObjectMapping(long handle, sciPointObj * pObj);

/**
 * Remove a handle from the Scilab HandleTab and its mapping in a tab.
 * @return the object removed or NULL if no object was found with this key.
 */
sciPointObj * sciRemoveHandleObjectMapping(long handle);

/**
 * Remove a handle from the Scilab HandleTab and its mapping in a tab, knowing the object.
 * @return the object itself or NULL if it has not been found in the hashtable.
 */
sciPointObj * sciRemoveObjectMapping(sciPointObj * pObj);

/**
 * Search in the Scilab handle tab the object corresponding to the handle.
 * @return the object corresponding to the handle or NULL if the object is not found.
 */
sciPointObj * sciGetObjectFromHandle(long handle);

/*--------------------------------------------------------------------------*/


#endif /* _SCI_HANDLE_TAB_H_ */

