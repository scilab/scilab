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
 *    This file contains all functions used to BUILD new objects : 
 - break the binding between the deleted object and its parent in the 
 existing hierarchy
 - freeing memory
 --------------------------------------------------------------------------*/

#ifndef __SCI_DESTROY__
#define __SCI_DESTROY__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "machine.h" /* C2F */


GRAPHICS_IMPEXP int destroyGraphicsSons(sciPointObj * pthis);
GRAPHICS_IMPEXP int destroyGraphicHierarchy(sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP void AllGraphWinDelete( void ) ;

#define SciGerase() { if( C2F(scigerase)() !=0 ) { Scierror(999,"%s :Requested figure cannot be created\n",fname) ; return 0 ; } }
GRAPHICS_IMPEXP int C2F(scigerase)( void ) ;

GRAPHICS_IMPEXP int DestroyFigure (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroySubWin (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int deallocateText( sciPointObj * pthis ) ;
GRAPHICS_IMPEXP int DestroyText (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyLegend (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int deallocatePolyline (sciPointObj * pthis);
GRAPHICS_IMPEXP int DestroyPolyline (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyArc (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyRectangle (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroySurface (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyGrayplot (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyAxes (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyFec (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroySegs (sciPointObj * pthis); /* DESTROY */
GRAPHICS_IMPEXP int DestroyCompound (sciPointObj * pthis);

GRAPHICS_IMPEXP int sciUnCompound (sciPointObj * pobj); /* DESTROY */

GRAPHICS_IMPEXP int sciDelGraphicObj (sciPointObj * pthis); /* DESTROY */

GRAPHICS_IMPEXP int DestroyLabel (sciPointObj * pthis); /* DESTROY */


/* free the user_data */
GRAPHICS_IMPEXP void clearUserData( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP void sciDeleteWindow( int winNum ) ;

GRAPHICS_IMPEXP int updateMerge( sciPointObj * pSubwin ) ;

GRAPHICS_IMPEXP int sciStandardDestroyOperations( sciPointObj * pThis ) ;

GRAPHICS_IMPEXP void destroyGraphicPointer(void * pointer);

GRAPHICS_IMPEXP void destroyGraphicStringArray(char ** strArray, int nbStrings);

/**
 * Destroy the relationShip used by an object
 */
GRAPHICS_IMPEXP void destroyRelationShip(sciPointObj * pObj);

#endif /* __SCI_DESTROY__ */
