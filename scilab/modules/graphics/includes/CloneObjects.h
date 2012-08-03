/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
 *    This file contains all functions used to CLONE an object, it means make
 *    a copy of an object under the same subwin.
 --------------------------------------------------------------------------*/



#ifndef __SCI_CLONE__
#define __SCI_CLONE__

#include "dynlib_graphics.h"

/*--------------------------------------------------------------------------*/
/**
 * copy the ContouredObject properties of a source object to a destination object
 * The previous (non-MVC) implementation always returned 0, even if it was specified
 * that it could return -1 in the return comment.
 * @param sourceIdentifier the source object's MVC identifier
 * @param destIdentifier the destination object's MVC identifier
 * @return always 0, the copy is considered to be always correct
 */
GRAPHICS_IMPEXP int cloneGraphicContext(char* sourceIdentifier, char* destIdentifier);
/*--------------------------------------------------------------------------*/
/**
 * copy the fontContext of an object to another.
 * @param sourceIdentifier the source object's MVC identifier
 * @param destIdentifier the destination object's MVC identifier
 * @return always 0, the copy is considered to be always correct
 */
GRAPHICS_IMPEXP int cloneFontContext(char* sourceIdentifier, char* destIdentifier);
/*--------------------------------------------------------------------------*/
/**
 * Clones a polyline
 * @param sourceIdentifier the source polyline's identifier
 * @return the identifier of the cloned polyline.
 */
GRAPHICS_IMPEXP char* clonePolyline(char* sourceIdentifier);

#endif /* __SCI_CLONE__ */
