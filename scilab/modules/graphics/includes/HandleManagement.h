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

#include "ObjectStructure.h"
#include "BOOL.h"

/* Handle functions */
void sciSetHandle (sciPointObj * pobj, long value);
int sciAddNewHandle (sciPointObj * pobj);
int sciDelHandle (sciPointObj * pobj);
long sciGetHandle (sciPointObj * pobj);
sciPointObj *sciGetPointerFromHandle (long handle);
sciPointObj *sciGetPointerFromJavaIndex (int javaIndex);

sciRelationShip *sciGetRelationship (sciPointObj * pobj); /* HANDLE MANAGEMENT */
int sciSetParent (sciPointObj * pson, sciPointObj * pparent); /* HANDLE MANAGEMENT */
sciPointObj *sciGetParent (sciPointObj * pobj); /* HANDLE MANAGEMENT */
BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent); /* HANDLE MANAGEMENT */
BOOL sciAddThisToItsParentLastPos(sciPointObj * pthis, sciPointObj * parent); /* HANDLE MANAGEMENT */
BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent); /* HANDLE MANAGEMENT */
BOOL sciDelSonFromItsParent(sciSons * son, sciPointObj * parent); /* HANDLE MANAGEMENT */
sciSons *sciGetSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
sciSons *sciGetLastSons (sciPointObj * pobj); /* HANDLE MANAGEMENT */
sciSons * sciGetNextAccessibleSon( sciSons * son ) ; /* HANDLE MANAGEMENT */
sciSons * sciGetFirstAccessibleSon( sciPointObj * pObj ) ; /* HANDLE MANAGEMENT */
sciSons * sciFindSon( sciPointObj * searchedObj, sciPointObj * parentObj ) ;
int sciRelocateHandles( unsigned long handles[], int nbHandles, unsigned long newParentHandle ) ;
BOOL sciCanBeSonOf( sciPointObj * son, sciPointObj * parent ) ;
int swapHandles( unsigned long firstHdl, unsigned long secondHdl ) ;

/**
 * Check if a handle is still valid
 */
BOOL isHandleValid(long handle);

sciPointObj * sciGetIndexedSon(sciPointObj * pobj, int lindex);

/**
 * Generate a new handle index for an object.
 */
long generateNewHandle(sciPointObj * pObj);

#endif /* __SCI_HANDLE_MANAGEMENT__ */
