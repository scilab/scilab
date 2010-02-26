/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

#ifndef __SCI_HANDLE_MANAGEMENT__
#define __SCI_HANDLE_MANAGEMENT__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "BOOL.h"

/* Handle functions */
GRAPHICS_IMPEXP void sciSetHandle (sciPointObj * pobj, long value);
GRAPHICS_IMPEXP int sciAddNewHandle (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciDelHandle (sciPointObj * pobj);
GRAPHICS_IMPEXP long sciGetHandle (sciPointObj * pobj);
GRAPHICS_IMPEXP sciPointObj *sciGetPointerFromHandle (long handle);
GRAPHICS_IMPEXP sciPointObj *sciGetPointerFromJavaIndex (int javaIndex);

GRAPHICS_IMPEXP sciRelationShip *sciGetRelationship (sciPointObj * pobj); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP int sciSetParent (sciPointObj * pson, sciPointObj * pparent); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciPointObj *sciGetParent (sciPointObj * pobj); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP BOOL sciAddThisToItsParentLastPos(sciPointObj * pthis, sciPointObj * parent); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP BOOL sciDelSonFromItsParent(sciSons * son, sciPointObj * parent); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciSons *sciGetSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciSons *sciGetLastSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciSons * sciGetNextAccessibleSon( sciSons * son ) ; /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciSons * sciGetFirstAccessibleSon( sciPointObj * pObj ) ; /* HANDLE MANAGEMENT */
GRAPHICS_IMPEXP sciSons * sciFindSon( sciPointObj * searchedObj, sciPointObj * parentObj ) ;
GRAPHICS_IMPEXP int sciRelocateHandles( unsigned long handles[], int nbHandles, unsigned long newParentHandle ) ;
GRAPHICS_IMPEXP BOOL sciCanBeSonOf( sciPointObj * son, sciPointObj * parent ) ;
GRAPHICS_IMPEXP int swapHandles( unsigned long firstHdl, unsigned long secondHdl ) ;

/**
 * Check if a handle is still valid
 */
GRAPHICS_IMPEXP BOOL isHandleValid(long handle);

GRAPHICS_IMPEXP sciPointObj * sciGetIndexedSon(sciPointObj * pobj, int lindex);

/**
 * Generate a new handle index for an object.
 */
GRAPHICS_IMPEXP long generateNewHandle(sciPointObj * pObj);

#endif /* __SCI_HANDLE_MANAGEMENT__ */
