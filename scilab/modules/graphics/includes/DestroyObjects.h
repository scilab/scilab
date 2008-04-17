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

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_DESTROY__
#define __SCI_DESTROY__


int destroyGraphicsSons(sciPointObj * pthis);
extern int destroyGraphicHierarchy(sciPointObj * pthis); /* DESTROY */
void AllGraphWinDelete( void ) ;

#define SciGerase() { if( C2F(scigerase)() !=0 ) { Scierror(999,"%s :Requested figure cannot be created\n",fname) ; return 0 ; } }
int C2F(scigerase)( void ) ;

extern int DestroyFigure (sciPointObj * pthis); /* DESTROY */
extern int DestroyStatusBar (sciPointObj * pthis); /* DESTROY */
extern int DestroySubWin (sciPointObj * pthis); /* DESTROY */
extern int DestroyScrollV (sciPointObj * pthis); /* DESTROY */
extern int DestroyScrollH (sciPointObj * pthis); /* DESTROY */
extern int deallocateText( sciPointObj * pthis ) ;
extern int DestroyText (sciPointObj * pthis); /* DESTROY */
extern int DestroyTitle (sciPointObj * pthis); /* DESTROY */
extern int DestroyLegend (sciPointObj * pthis); /* DESTROY */
int deallocatePolyline (sciPointObj * pthis);
extern int DestroyPolyline (sciPointObj * pthis); /* DESTROY */
extern int DestroyArc (sciPointObj * pthis); /* DESTROY */
extern int DestroyRectangle (sciPointObj * pthis); /* DESTROY */
extern int DestroySurface (sciPointObj * pthis); /* DESTROY */
extern int DestroyGrayplot (sciPointObj * pthis); /* DESTROY */
extern int DestroyAxes (sciPointObj * pthis); /* DESTROY */
extern int DestroyFec (sciPointObj * pthis); /* DESTROY */
extern int DestroySegs (sciPointObj * pthis); /* DESTROY */
extern int DestroyCompound (sciPointObj * pthis);

extern void DeleteObjs(int win); /* DESTROY */
extern int sciUnCompound (sciPointObj * pobj); /* DESTROY */

extern int sciDelGraphicObj (sciPointObj * pthis); /* DESTROY */

extern int DestroyLabel (sciPointObj * pthis); /* DESTROY */
int sciDestroyConsole(     sciPointObj * pthis ) ; /* DESTROY */
int sciDestroyFrame(       sciPointObj * pThis ) ;
int sciDestroyWindow(      sciPointObj * pThis ) ;
int sciDestroyWindowFrame( sciPointObj * pThis ) ;
int sciDestroyScreen(      sciPointObj * pThis ) ;

/* ADDED */

extern void delete_sgwin_entities(int win_num);

/* free the user_data */
extern void clearUserData( sciPointObj * pObj ) ;

extern void sciDeleteWindow( int winNum ) ;

extern int updateMerge( sciPointObj * pSubwin ) ;

void CleanPlots(char *unused, integer *winnumber, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4) ;

int sciStandardDestroyOperations( sciPointObj * pThis ) ;

void destroyGraphicPointer(void * pointer);

void destroyGraphicStringArray(char ** strArray, int nbStrings);

#endif /* __SCI_DESTROY__ */
