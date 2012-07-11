/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
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

#ifndef __HANDLE_MANAGEMENT_H__
#define __HANDLE_MANAGEMENT_H__

#include "dynlib_graphic_objects.h"

/* Handle functions */
/*
GRAPHICS_IMPEXP void sciSetHandle (sciPointObj * pobj, long value);
GRAPHICS_IMPEXP int sciAddNewHandle (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciDelHandle (sciPointObj * pobj);
GRAPHICS_IMPEXP sciPointObj *sciGetPointerFromHandle (long handle);

GRAPHICS_IMPEXP sciRelationShip *sciGetRelationship (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciSetParent (sciPointObj * pson, sciPointObj * pparent);
GRAPHICS_IMPEXP sciPointObj *sciGetParent (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent);
GRAPHICS_IMPEXP BOOL sciAddThisToItsParentLastPos(sciPointObj * pthis, sciPointObj * parent);
GRAPHICS_IMPEXP BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent);
GRAPHICS_IMPEXP BOOL sciDelSonFromItsParent(sciSons * son, sciPointObj * parent);
GRAPHICS_IMPEXP sciSons *sciGetSons (sciPointObj * pobj);
GRAPHICS_IMPEXP sciSons *sciGetLastSons (sciPointObj * pobj);
GRAPHICS_IMPEXP sciSons * sciGetNextAccessibleSon( sciSons * son ) ;

GRAPHICS_IMPEXP sciSons * sciFindSon( sciPointObj * searchedObj, sciPointObj * parentObj ) ;
*/

/**
 * Check if a handle is still valid
 */
/*
GRAPHICS_IMPEXP BOOL isHandleValid(long handle);

GRAPHICS_IMPEXP sciPointObj * sciGetIndexedSon(sciPointObj * pobj, int lindex);
*/
/**
 * Generate a new handle index for an object.
 */
/*
GRAPHICS_IMPEXP long generateNewHandle(sciPointObj * pObj);
*/

GRAPHIC_OBJECTS_IMPEXP void *sciGetPointerFromHandle(long handle);

/**
 * @brief Get the handle for an UID.
 *
 * This should also allocate a new handle on demand. UID can be safely
 * deallocated after this call.
 *
 * @param any uid
 * @return a handle mapped to the uid
 */
GRAPHIC_OBJECTS_IMPEXP long getHandle(char const* UID);

/**
 * @brief Get the uid of an handle.
 *
 * @param a valid handle
 * @return a reference to an existing uid or null if invalid.
 */
GRAPHIC_OBJECTS_IMPEXP char const* getObjectFromHandle(long handle);

// DEPRECATED functions : Should be removed
GRAPHIC_OBJECTS_IMPEXP long sciGetHandle(void* pobj);
GRAPHIC_OBJECTS_IMPEXP void* sciGetParent(void* pobj);
GRAPHIC_OBJECTS_IMPEXP void* sciGetSons(void* pobj);
GRAPHIC_OBJECTS_IMPEXP void* sciGetCurrentFigure(void);
GRAPHIC_OBJECTS_IMPEXP void* sciGetCurrentSubWin(void);
GRAPHIC_OBJECTS_IMPEXP void* sciGetCurrentObj(void);
GRAPHIC_OBJECTS_IMPEXP void sciSetCurrentObj(void* pobj);

#endif /* !__HANDLE_MANAGEMENT_H__ */
