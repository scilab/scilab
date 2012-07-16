/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Matthhieu Philippe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
 *    -This file contains all functions used to SET the properties of graphics
 *    objects.
 *    - there are two kinds of functions : the sciInit*** which only set an object
 *      property and sciSet*** which do the same work but check before if it is
 *      necessary to do so.
 *    - The returning status of each functions is as follow :
 *      0 for correct execution, 1 if no change was made
 *      and -1 if an error occurred.
 --------------------------------------------------------------------------*/


#ifndef __SCI_SET_PROPERTY__
#define __SCI_SET_PROPERTY__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "BOOL.h"

GRAPHICS_IMPEXP int sciSetLineWidth ( char* pobj, double linewidth ) ;  /* SET */

GRAPHICS_IMPEXP int sciSetLineStyle ( char* pobj, int linestyle ) ; /* SET */

GRAPHICS_IMPEXP int sciSetMarkSize (char* pobj, int marksize); /* SET */

/* Text Functions */
GRAPHICS_IMPEXP int sciSetText (   char* pobj, char ** text, int nbRow, int nbCol ) ; /* SET */

/* GMODE */
GRAPHICS_IMPEXP int sciSetDefaultValues (void); /* SET */


/* Window Functions */
GRAPHICS_IMPEXP int sciSetSelectedSubWin (char* psubwinobj); /* SET */

GRAPHICS_IMPEXP int sciSetPoint(char* pthis, double *tab, int *numrow, int *numcol); /* SET */

GRAPHICS_IMPEXP int sciSetInterpVector(char* pobj, int size, int * value);

GRAPHICS_IMPEXP int sciInitAutoRotation( char* pObj, BOOL value ) ;
GRAPHICS_IMPEXP int sciSetAutoRotation ( char* pObj, BOOL value ) ;

GRAPHICS_IMPEXP int sciInitAutoPosition( char* pObj, BOOL value ) ;
GRAPHICS_IMPEXP int sciSetAutoPosition ( char* pObj, BOOL value ) ;

GRAPHICS_IMPEXP int sciInitAutoSize( char* pObj, BOOL autoSize ) ;
GRAPHICS_IMPEXP int sciSetAutoSize(  char* pObj, BOOL autoSize ) ;

GRAPHICS_IMPEXP int sciInitAlignment( char* pObj, sciTextAlignment align ) ;
GRAPHICS_IMPEXP int sciSetAlignment(  char* pObj, sciTextAlignment align ) ;

GRAPHICS_IMPEXP int sciInitUserSize( char* pObj, double width, double height ) ;
GRAPHICS_IMPEXP int sciSetUserSize(  char* pObj, double width, double height ) ;

GRAPHICS_IMPEXP int sciInitCenterPos( char* pObj, BOOL newCP ) ;
GRAPHICS_IMPEXP int sciSetCenterPos(  char* pObj, BOOL newCP ) ;

GRAPHICS_IMPEXP int sciSetSelectObject(    char* pObj ) ;

GRAPHICS_IMPEXP int sciInitGraphicsStyle( char* pobj, BOOL value );

GRAPHICS_IMPEXP int sciInitEventHandler( char* pObj, char * name ) ;
GRAPHICS_IMPEXP int sciSetEventHandler(  char* pObj, char * name ) ;

GRAPHICS_IMPEXP int sciInitIsEventHandlerEnable( char* pObj, BOOL enable ) ;
GRAPHICS_IMPEXP int sciSetIsEventHandlerEnable(  char* pObj, BOOL enable ) ;

GRAPHICS_IMPEXP int sciSetDataBounds( char* pObj, double bounds[6] ) ; /* SET */
GRAPHICS_IMPEXP int sciSetRealDataBounds(char* pObj, const double bounds[6]); /* SET */

GRAPHICS_IMPEXP int sciInitPixmapMode(char* pObj, BOOL onOrOff);
GRAPHICS_IMPEXP int sciSetPixmapMode(char* pObj, BOOL onOrOff); /* SET */

GRAPHICS_IMPEXP int sciInitTextPos( char* pObj, double posX, double posY, double posZ);
GRAPHICS_IMPEXP int sciSetTextPos( char* pObj, double posX, double posY, double posZ);

GRAPHICS_IMPEXP int sciInitAutoTicks(char* pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ);
GRAPHICS_IMPEXP int sciSetAutoTicks(char* pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ); /* SET */

GRAPHICS_IMPEXP int sciSetZoomBox(char* pObj, const double zoomBox[6]); /* SET */

GRAPHICS_IMPEXP int sciInitImmediateDrawingMode(char* pObj, BOOL autoRedraw);
GRAPHICS_IMPEXP int sciSetImmediateDrawingMode(char* pObj, BOOL autoRedraw);

GRAPHICS_IMPEXP BOOL sciCheckColorIndex(char* pObj, int colorIndex);

/*---------------------------------------------------------------------------*/
/* return 0 if the string contains any % character, 1 if a %d has been found */
/* and -1 otherwise. */
GRAPHICS_IMPEXP int checkPercent( char * string ) ;
/*---------------------------------------------------------------------------*/

#endif /* __SCI_SET_PROPERTY__ */
