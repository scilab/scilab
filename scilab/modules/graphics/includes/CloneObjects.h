/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
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
 *    This file contains all functions used to CLONE an object, it means make
 *    a copy of an object under the same subwin.
 --------------------------------------------------------------------------*/



#ifndef __SCI_CLONE__
#define __SCI_CLONE__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"

GRAPHICS_IMPEXP sciPointObj *CloneText (sciPointObj * pthis); /* CLONE */
GRAPHICS_IMPEXP sciPointObj *sciCloneObj (sciPointObj * pobj); /* CLONE */

GRAPHICS_IMPEXP sciPointObj *CloneRectangle (sciPointObj * pthis);
GRAPHICS_IMPEXP sciPointObj *ClonePolyline (sciPointObj * pthis);
GRAPHICS_IMPEXP sciPointObj *CloneArc (sciPointObj * pthis);

GRAPHICS_IMPEXP sciPointObj * sciCopyObj (sciPointObj * pobj, sciPointObj * psubwinparenttarget ); /* CLONE */

/*--------------------------------------------------------------------------*/
/**
 * copy the graphicontext of an object to another
 * @param pObjSource the object from which the GC is taken
 * @param pObjDest the object in which the GC is paste
 * @return 0 if the copy is ok, -1 otherwise.
 */
GRAPHICS_IMPEXP int cloneGraphicContext( sciPointObj * pObjSource, sciPointObj * pObjDest ) ;
/*--------------------------------------------------------------------------*/
/**
 * copy the fontContext of an object to another.
 * @param pObjSource the object from which the FC is taken
 * @param pObjDest the object in which the FC is paste
 * @return 0 if the copy is ok, -1 otherwise.
 */
GRAPHICS_IMPEXP int cloneFontContext( sciPointObj * pObjSource, sciPointObj * pObjDest ) ;
/*--------------------------------------------------------------------------*/
/**
 * copy the user data of an object to an other.
 * @return 0 if the copy is ok, -1 otherwise.
 */
int cloneUserData( sciPointObj * pObjSource, sciPointObj * pObjDest ) ;
/*--------------------------------------------------------------------------*/

#endif /* __SCI_CLONE__ */
