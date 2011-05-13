/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Matthhieu Philippe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
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
 *      and -1 if an error occured.
 --------------------------------------------------------------------------*/


#ifndef __SCI_SET_PROPERTY__
#define __SCI_SET_PROPERTY__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "BOOL.h"

GRAPHICS_IMPEXP void Obj_RedrawNewAngle(sciPointObj * pSubwin, double alpha, double theta); /* SET  */

GRAPHICS_IMPEXP int sciSetEntityType (sciPointObj * pobj, sciEntityType value); /* SET */
GRAPHICS_IMPEXP int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   int m, int n); /* SET */
GRAPHICS_IMPEXP int sciInitNumColors( sciPointObj * pobj, int numcolors ) ; /* SET */
GRAPHICS_IMPEXP int sciSetNumColors ( sciPointObj * pobj, int numcolors ) ; /* SET */

GRAPHICS_IMPEXP int sciSetGoodIndex(sciPointObj * pobj, int colorindex);  /* SET */
GRAPHICS_IMPEXP void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m);  /* SET */
GRAPHICS_IMPEXP int sciUpdateBaW (sciPointObj * pobj, int flag, int value ) ; /* SET */

GRAPHICS_IMPEXP int sciInitBackground( sciPointObj * pobj, int colorindex ) ;
GRAPHICS_IMPEXP int sciSetBackground ( sciPointObj * pobj, int colorindex ) ; /* SET */

GRAPHICS_IMPEXP int sciInitForeground( sciPointObj * pobj, int colorindex ) ;
GRAPHICS_IMPEXP int sciSetForeground ( sciPointObj * pobj, int colorindex ) ; /* SET */

GRAPHICS_IMPEXP int sciInitLineWidth( sciPointObj * pobj, double linewidth ) ;
GRAPHICS_IMPEXP int sciSetLineWidth ( sciPointObj * pobj, double linewidth ) ;  /* SET */

GRAPHICS_IMPEXP int sciInitIsLine(sciPointObj * pobj, BOOL isline);
GRAPHICS_IMPEXP int sciSetIsLine (sciPointObj * pobj, BOOL isline);  /* SET */

GRAPHICS_IMPEXP int sciInitLineStyle( sciPointObj * pobj, int linestyle ) ;
GRAPHICS_IMPEXP int sciSetLineStyle ( sciPointObj * pobj, int linestyle ) ; /* SET */

GRAPHICS_IMPEXP int sciInitIsMark(sciPointObj * pobj, BOOL ismark);  /* SET */
GRAPHICS_IMPEXP int sciSetIsMark (sciPointObj * pobj, BOOL ismark);  /* SET */

GRAPHICS_IMPEXP int sciInitIsFilled(sciPointObj * pobj, BOOL isfilled);
GRAPHICS_IMPEXP int sciSetIsFilled (sciPointObj * pobj, BOOL isfilled);  /* SET */

GRAPHICS_IMPEXP int sciInitPolylineStyle( sciPointObj * pobj, int plot );
GRAPHICS_IMPEXP int sciInitArrowSize( sciPointObj * pobj, double size );

GRAPHICS_IMPEXP int sciInitMarkStyle(sciPointObj * pobj, int markstyle);
GRAPHICS_IMPEXP int sciSetMarkStyle (sciPointObj * pobj, int markstyle); /* SET */

GRAPHICS_IMPEXP int sciInitMarkSize(sciPointObj * pobj, int marksize);
GRAPHICS_IMPEXP int sciSetMarkSize (sciPointObj * pobj, int marksize); /* SET */

GRAPHICS_IMPEXP int sciInitMarkSizeUnit(sciPointObj * pobj, int marksizeunit);
GRAPHICS_IMPEXP int sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit); /* SET */

GRAPHICS_IMPEXP int sciInitMarkBackground(sciPointObj * pobj, int colorindex);
GRAPHICS_IMPEXP int sciSetMarkBackground (sciPointObj * pobj, int colorindex);  /* SET */

GRAPHICS_IMPEXP int sciInitMarkForeground(sciPointObj * pobj, int colorindex);
GRAPHICS_IMPEXP int sciSetMarkForeground (sciPointObj * pobj, int colorindex);  /* SET */

GRAPHICS_IMPEXP int sciInitBoxType( sciPointObj * pobj, EAxesBoxType type ) ;
GRAPHICS_IMPEXP int sciSetBoxType ( sciPointObj * pobj, EAxesBoxType type ) ; /* SET */

GRAPHICS_IMPEXP int sciInitIsBoxed(sciPointObj * pobj, BOOL isboxed);
GRAPHICS_IMPEXP int sciSetIsBoxed (sciPointObj * pobj, BOOL isboxed); /* SET */

GRAPHICS_IMPEXP int sciInitNbXSubTics(sciPointObj * pobj, int nbsubtics);
GRAPHICS_IMPEXP int sciSetNbXSubTics (sciPointObj * pobj, int nbsubtics); /* SET */

GRAPHICS_IMPEXP int sciInitNbYSubTics(sciPointObj * pobj, int nbsubtics);
GRAPHICS_IMPEXP int sciSetNbYSubTics (sciPointObj * pobj, int nbsubtics); /* SET */

GRAPHICS_IMPEXP int sciInitNbZSubTics(sciPointObj * pobj, int nbsubtics);
GRAPHICS_IMPEXP int sciSetNbZSubTics (sciPointObj * pobj, int nbsubtics); /* SET */

/* Text Functions */
GRAPHICS_IMPEXP int sciInitFontSize(sciPointObj * pobj, double fontSize); /* SET */
GRAPHICS_IMPEXP int sciSetFontSize (sciPointObj * pobj, double fontSize); /* SET */

GRAPHICS_IMPEXP int sciInitFontOrientation(sciPointObj * pobj, double textorientation); /* SET */
GRAPHICS_IMPEXP int sciSetFontOrientation (sciPointObj * pobj, double textorientation); /* SET */

GRAPHICS_IMPEXP int sciSetStrings( sciPointObj * pobj, const StringMatrix * pStrings ) ; /* SET */
GRAPHICS_IMPEXP int sciSetText (   sciPointObj * pobj, char ** text, int nbRow, int nbCol ) ; /* SET */

GRAPHICS_IMPEXP int sciInitFontBackground(sciPointObj * pobj, int color);
GRAPHICS_IMPEXP int sciSetFontBackground (sciPointObj * pobj, int color); /* SET */

GRAPHICS_IMPEXP int sciInitFontForeground(sciPointObj * pobj, int colorindex ) ;
GRAPHICS_IMPEXP int sciSetFontForeground (sciPointObj * pobj, int colorindex ) ; /* SET */

GRAPHICS_IMPEXP int sciInitFontStyle(sciPointObj * pobj, int iAttributes ) ;
GRAPHICS_IMPEXP int sciSetFontStyle (sciPointObj * pobj, int iAttributes ) ; /* SET */

/* Legend */
GRAPHICS_IMPEXP int sciInitLegendPlace(sciPointObj * pobj, sciLegendPlace place);
GRAPHICS_IMPEXP int sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place); /* SET */

GRAPHICS_IMPEXP int sciInitLegendPos(sciPointObj * pobj, double position[]);
GRAPHICS_IMPEXP int sciSetLegendPos (sciPointObj * pobj, double position[]); /* SET */

/* GMODE */
GRAPHICS_IMPEXP int sciInitIsClipping(sciPointObj * pobj, int value) ;
GRAPHICS_IMPEXP int sciSetIsClipping (sciPointObj * pobj, int value); /* SET */

GRAPHICS_IMPEXP int sciSetClipping (sciPointObj * pobj, double pclip[4] ); /* SET */

GRAPHICS_IMPEXP int sciInitAddPlot (sciPointObj * pobj, BOOL value);
GRAPHICS_IMPEXP int sciSetAddPlot (sciPointObj * pobj, BOOL value); /* SET */

GRAPHICS_IMPEXP int sciInitAutoScale(sciPointObj * pobj, BOOL value);
GRAPHICS_IMPEXP int sciSetAutoScale (sciPointObj * pobj, BOOL value); /* SET */

GRAPHICS_IMPEXP int sciInitZooming(sciPointObj * pobj, BOOL value);
GRAPHICS_IMPEXP int sciSetZooming (sciPointObj * pobj, BOOL value); /* SET */

GRAPHICS_IMPEXP int sciInitXorMode(sciPointObj * pobj, int value);
GRAPHICS_IMPEXP int sciSetXorMode (sciPointObj * pobj, int value); /* SET */

GRAPHICS_IMPEXP int sciInitVisibility (sciPointObj * pobj, BOOL value);
GRAPHICS_IMPEXP int sciSetVisibility (sciPointObj * pobj, BOOL value); /* SET */

GRAPHICS_IMPEXP int sciInitResize(sciPointObj * pobj, BOOL value);
GRAPHICS_IMPEXP int sciSetResize (sciPointObj * pobj, BOOL value); /* SET */

GRAPHICS_IMPEXP int sciSetDefaultValues (void); /* SET */


/* Window Functions */
GRAPHICS_IMPEXP int sciInitName(sciPointObj * pobj, char * newName); /* SET */
GRAPHICS_IMPEXP int sciSetName (sciPointObj * pobj, char * newName); /* SET */
GRAPHICS_IMPEXP int sciInitNum( sciPointObj * pobj, int value ) ;
GRAPHICS_IMPEXP int sciSetNum ( sciPointObj * pobj, int value ) ; /* SET */

GRAPHICS_IMPEXP int sciInitDimension( sciPointObj * pobj, int newWidth, int newHeight ) ;
GRAPHICS_IMPEXP int sciSetDimension(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

GRAPHICS_IMPEXP int sciInitWindowDim( sciPointObj * pobj, int newWidth, int newHeight ) ;
GRAPHICS_IMPEXP int sciSetWindowDim(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

GRAPHICS_IMPEXP int sciInitScreenPosition(sciPointObj * pobj, int pposx, int pposy);
GRAPHICS_IMPEXP int sciSetScreenPosition( sciPointObj * pobj, int pposx, int pposy); /* SET */


GRAPHICS_IMPEXP void sciSelectFirstSubwin( sciPointObj * parentFigure ) ; /* SET */

GRAPHICS_IMPEXP int sciInitSelectedSubWin (sciPointObj * psubwinobj);
GRAPHICS_IMPEXP int sciSetSelectedSubWin (sciPointObj * psubwinobj); /* SET */

GRAPHICS_IMPEXP int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin); /* SET */

GRAPHICS_IMPEXP int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol); /* SET */

GRAPHICS_IMPEXP int sciSetdrawmode(BOOL mode); /* SET */
GRAPHICS_IMPEXP int sciSwitchWindow(int winnum); /* SET */

GRAPHICS_IMPEXP int sciInitUsedWindow( int winNum ) ; /* SET */
GRAPHICS_IMPEXP int sciSetUsedWindow( int winNum ) ; /* SET */

GRAPHICS_IMPEXP int sciSetInterpVector(sciPointObj * pobj, int size, int * value);

GRAPHICS_IMPEXP int sciInitAutoRotation( sciPointObj * pObj, BOOL value ) ;
GRAPHICS_IMPEXP int sciSetAutoRotation ( sciPointObj * pObj, BOOL value ) ;

GRAPHICS_IMPEXP int sciInitAutoPosition( sciPointObj * pObj, BOOL value ) ;
GRAPHICS_IMPEXP int sciSetAutoPosition ( sciPointObj * pObj, BOOL value ) ;

GRAPHICS_IMPEXP int sciInitAutoSize( sciPointObj * pObj, BOOL autoSize ) ;
GRAPHICS_IMPEXP int sciSetAutoSize(  sciPointObj * pObj, BOOL autoSize ) ;

GRAPHICS_IMPEXP int sciInitAlignment( sciPointObj * pObj, sciTextAlignment align ) ;
GRAPHICS_IMPEXP int sciSetAlignment(  sciPointObj * pObj, sciTextAlignment align ) ;

GRAPHICS_IMPEXP int sciInitUserSize( sciPointObj * pObj, double width, double height ) ;
GRAPHICS_IMPEXP int sciSetUserSize(  sciPointObj * pObj, double width, double height ) ;

GRAPHICS_IMPEXP int sciInitCenterPos( sciPointObj * pObj, BOOL newCP ) ;
GRAPHICS_IMPEXP int sciSetCenterPos(  sciPointObj * pObj, BOOL newCP ) ;

GRAPHICS_IMPEXP int sciInitIs3d( sciPointObj * pObj, BOOL is3d ) ;
GRAPHICS_IMPEXP int sciSetIs3d(  sciPointObj * pObj, BOOL is3d ) ;

GRAPHICS_IMPEXP int sciInitSelectedObject( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP int sciSetSelectObject(    sciPointObj * pObj ) ;

GRAPHICS_IMPEXP int sciInitHiddenColor( sciPointObj * pObj, int newColor ) ;
GRAPHICS_IMPEXP int sciSetHiddenColor(  sciPointObj * pObj, int newColor ) ;

GRAPHICS_IMPEXP int sciInitHiddenAxisColor( sciPointObj * pObj, int newColor ) ;
GRAPHICS_IMPEXP int sciSetHiddenAxisColor( sciPointObj * pObj, int newColor ) ;

GRAPHICS_IMPEXP int setSubWinAngles( sciPointObj *psubwin, double theta, double alpha );
GRAPHICS_IMPEXP int sciInitdrawmode( BOOL mode );

GRAPHICS_IMPEXP int sciInitGraphicsStyle( sciPointObj * pobj, BOOL value );

GRAPHICS_IMPEXP int sciInitGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; 
GRAPHICS_IMPEXP int sciSetGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; /* SET */

GRAPHICS_IMPEXP int sciSetViewport(  sciPointObj * pObj, const int viewport[4] ) ; /* SET */

GRAPHICS_IMPEXP int sciInitInfoMessage(sciPointObj * pObj, const char * newMessage);
GRAPHICS_IMPEXP int sciSetInfoMessage( sciPointObj * pObj, const char * newMessage ) ; /* SET */

GRAPHICS_IMPEXP int sciInitEventHandler( sciPointObj * pObj, char * name ) ;
GRAPHICS_IMPEXP int sciSetEventHandler(  sciPointObj * pObj, char * name ) ;

GRAPHICS_IMPEXP int sciInitIsEventHandlerEnable( sciPointObj * pObj, BOOL enable ) ;
GRAPHICS_IMPEXP int sciSetIsEventHandlerEnable(  sciPointObj * pObj, BOOL enable ) ;

GRAPHICS_IMPEXP int sciSetDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* SET */
GRAPHICS_IMPEXP int sciSetRealDataBounds(sciPointObj * pObj, const double bounds[6]); /* SET */

GRAPHICS_IMPEXP int sciInitViewingAngles( sciPointObj * pObj, double alpha, double theta) ;
GRAPHICS_IMPEXP int sciSetViewingAngles( sciPointObj * pObj, double alpha, double theta); /* SET */

GRAPHICS_IMPEXP int setInfoMessageWithRotationAngles(sciPointObj * pFigure, double alpha, double theta);

GRAPHICS_IMPEXP int sciInitPixmapMode(sciPointObj * pObj, BOOL onOrOff);
GRAPHICS_IMPEXP int sciSetPixmapMode(sciPointObj * pObj, BOOL onOrOff); /* SET */

GRAPHICS_IMPEXP int sciInitTextPos( sciPointObj * pObj, double posX, double posY, double posZ);
GRAPHICS_IMPEXP int sciSetTextPos( sciPointObj * pObj, double posX, double posY, double posZ);

GRAPHICS_IMPEXP int sciInitLogFlags(sciPointObj * pObj, char logFlags[3]);
GRAPHICS_IMPEXP int sciSetLogFlags(sciPointObj * pObj, char logFlags[3]) ; /* SET */

GRAPHICS_IMPEXP int sciInitAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ);
GRAPHICS_IMPEXP int sciSetAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ); /* SET */

GRAPHICS_IMPEXP int sciSetZoomBox(sciPointObj * pObj, const double zoomBox[6]); /* SET */

GRAPHICS_IMPEXP int sciInitImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);
GRAPHICS_IMPEXP int sciSetImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);

GRAPHICS_IMPEXP int sciInitUseNurbs(sciPointObj * pObj, BOOL useNurbs);
GRAPHICS_IMPEXP int sciSetUseNurbs(sciPointObj * pObj, BOOL useNurbs); /* SET */

GRAPHICS_IMPEXP int sciInitIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics);
GRAPHICS_IMPEXP int sciSetIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics); /* SET */

GRAPHICS_IMPEXP int sciInitColorRange(sciPointObj * pObj, int subset[2]); 
GRAPHICS_IMPEXP int sciSetColorRange(sciPointObj * pObj, int subset[2]); /* SET */

GRAPHICS_IMPEXP int sciInitOutsideColors(sciPointObj * pObj, int colors[2]);
GRAPHICS_IMPEXP int sciSetOutsideColors(sciPointObj * pObj, int colors[2]); /* SET */

GRAPHICS_IMPEXP int sciInitZBounds(sciPointObj * pObj, double bounds[2]);
GRAPHICS_IMPEXP int sciSetZBounds(sciPointObj * pObj, double bounds[2]); /* SET */

GRAPHICS_IMPEXP int sciInitGridFront(sciPointObj * pObj, BOOL gridFront);
GRAPHICS_IMPEXP int sciSetGridFront(sciPointObj * pObj, BOOL gridFront); /* SET */

GRAPHICS_IMPEXP int sciInitLegendLocation(sciPointObj * pObj, sciLegendPlace location);
GRAPHICS_IMPEXP int sciSetLegendLocation(sciPointObj * pObj, sciLegendPlace location); /* SET */

GRAPHICS_IMPEXP int sciInitAntialiasingQuality(sciPointObj * pObj, int quality);
GRAPHICS_IMPEXP int sciSetAntialiasingQuality(sciPointObj * pObj, int quality); /* SET */

GRAPHICS_IMPEXP BOOL sciCheckColorIndex(sciPointObj * pObj, int colorIndex);

/*---------------------------------------------------------------------------*/
/* return 0 if the string contains any % character, 1 if a %d has been found */
/* and -1 otherwise. */
GRAPHICS_IMPEXP int checkPercent( char * string ) ;
/*---------------------------------------------------------------------------*/

#endif /* __SCI_SET_PROPERTY__ */
