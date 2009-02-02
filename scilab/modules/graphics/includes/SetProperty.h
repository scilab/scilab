/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Matthhieu Philippe
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
 *    -This file contains all functions used to SET the properties of graphics
 *    objects.
 *    - there are two kinds of functions : the sciInit*** which only set an object
 *      property and sciSet*** which do the same work but check before if it is
 *      necessary to do so.
 *    - The returning status of each functions is as follow :
 *      0 for correct execution, 1 if no change was made
 *      and -1 if an error occured.
 --------------------------------------------------------------------------*/


#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_SET_PROPERTY__
#define __SCI_SET_PROPERTY__

#include "BOOL.h"

void Obj_RedrawNewAngle(sciPointObj * pSubwin, double alpha, double theta); /* SET  */

int sciSetEntityType (sciPointObj * pobj, sciEntityType value); /* SET */
int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   int m, int n); /* SET */
int sciInitNumColors( sciPointObj * pobj, int numcolors ) ; /* SET */
int sciSetNumColors ( sciPointObj * pobj, int numcolors ) ; /* SET */

int sciSetGoodIndex(sciPointObj * pobj, int colorindex);  /* SET */
void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m);  /* SET */
int sciUpdateBaW (sciPointObj * pobj, int flag, int value ) ; /* SET */

int sciInitBackground( sciPointObj * pobj, int colorindex ) ;
int sciSetBackground ( sciPointObj * pobj, int colorindex ) ; /* SET */

int sciInitForeground( sciPointObj * pobj, int colorindex ) ;
int sciSetForeground ( sciPointObj * pobj, int colorindex ) ; /* SET */

int sciInitLineWidth( sciPointObj * pobj, int linewidth ) ;
int sciSetLineWidth ( sciPointObj * pobj, int linewidth ) ;  /* SET */

int sciInitIsLine(sciPointObj * pobj, BOOL isline);
int sciSetIsLine (sciPointObj * pobj, BOOL isline);  /* SET */

int sciInitLineStyle( sciPointObj * pobj, int linestyle ) ;
int sciSetLineStyle ( sciPointObj * pobj, int linestyle ) ; /* SET */

int sciInitIsMark(sciPointObj * pobj, BOOL ismark);  /* SET */
int sciSetIsMark (sciPointObj * pobj, BOOL ismark);  /* SET */

int sciInitIsFilled(sciPointObj * pobj, BOOL isfilled);
int sciSetIsFilled (sciPointObj * pobj, BOOL isfilled);  /* SET */

int sciInitPolylineStyle( sciPointObj * pobj, int plot );
int sciInitArrowSize( sciPointObj * pobj, double size );

int sciInitMarkStyle(sciPointObj * pobj, int markstyle);
int sciSetMarkStyle (sciPointObj * pobj, int markstyle); /* SET */

int sciInitMarkSize(sciPointObj * pobj, int marksize);
int sciSetMarkSize (sciPointObj * pobj, int marksize); /* SET */

int sciInitMarkSizeUnit(sciPointObj * pobj, int marksizeunit);
int sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit); /* SET */

int sciInitMarkBackground(sciPointObj * pobj, int colorindex);
int sciSetMarkBackground (sciPointObj * pobj, int colorindex);  /* SET */

int sciInitMarkForeground(sciPointObj * pobj, int colorindex);
int sciSetMarkForeground (sciPointObj * pobj, int colorindex);  /* SET */

int sciInitBoxType( sciPointObj * pobj, EAxesBoxType type ) ;
int sciSetBoxType ( sciPointObj * pobj, EAxesBoxType type ) ; /* SET */

int sciInitIsBoxed(sciPointObj * pobj, BOOL isboxed);
int sciSetIsBoxed (sciPointObj * pobj, BOOL isboxed); /* SET */

/* Text Functions */
int sciInitFontSize(sciPointObj * pobj, double fontSize); /* SET */
int sciSetFontSize (sciPointObj * pobj, double fontSize); /* SET */

int sciInitFontOrientation(sciPointObj * pobj, double textorientation); /* SET */
int sciSetFontOrientation (sciPointObj * pobj, double textorientation); /* SET */

int sciSetStrings( sciPointObj * pobj, const StringMatrix * pStrings ) ; /* SET */
int sciSetText (   sciPointObj * pobj, char ** text, int nbRow, int nbCol ) ; /* SET */

int sciInitFontBackground(sciPointObj * pobj, int color);
int sciSetFontBackground (sciPointObj * pobj, int color); /* SET */

int sciInitFontForeground(sciPointObj * pobj, int colorindex ) ;
int sciSetFontForeground (sciPointObj * pobj, int colorindex ) ; /* SET */

int sciInitFontStyle(sciPointObj * pobj, int iAttributes ) ;
int sciSetFontStyle (sciPointObj * pobj, int iAttributes ) ; /* SET */

/* Legend */
int sciInitLegendPlace(sciPointObj * pobj, sciLegendPlace place);
int sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place); /* SET */

int sciInitLegendPos(sciPointObj * pobj, double position[]);
int sciSetLegendPos (sciPointObj * pobj, double position[]); /* SET */

/* GMODE */
int sciInitIsClipping(sciPointObj * pobj, int value) ;
int sciSetIsClipping (sciPointObj * pobj, int value); /* SET */

int sciSetClipping (sciPointObj * pobj, double pclip[4] ); /* SET */

int sciInitAddPlot (sciPointObj * pobj, BOOL value);
int sciSetAddPlot (sciPointObj * pobj, BOOL value); /* SET */

int sciInitAutoScale(sciPointObj * pobj, BOOL value);
int sciSetAutoScale (sciPointObj * pobj, BOOL value); /* SET */

int sciInitZooming(sciPointObj * pobj, BOOL value);
int sciSetZooming (sciPointObj * pobj, BOOL value); /* SET */

int sciInitXorMode(sciPointObj * pobj, int value);
int sciSetXorMode (sciPointObj * pobj, int value); /* SET */

int sciInitVisibility (sciPointObj * pobj, BOOL value);
int sciSetVisibility (sciPointObj * pobj, BOOL value); /* SET */

int sciInitResize(sciPointObj * pobj, BOOL value);
int sciSetResize (sciPointObj * pobj, BOOL value); /* SET */

int sciSetDefaultValues (void); /* SET */


/* Window Functions */
int sciInitName(sciPointObj * pobj, char * newName); /* SET */
int sciSetName (sciPointObj * pobj, char * newName); /* SET */
int sciInitNum( sciPointObj * pobj, int value ) ;
int sciSetNum ( sciPointObj * pobj, int value ) ; /* SET */

int sciInitDimension( sciPointObj * pobj, int newWidth, int newHeight ) ;
int sciSetDimension(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

int sciInitWindowDim( sciPointObj * pobj, int newWidth, int newHeight ) ;
int sciSetWindowDim(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

int sciInitScreenPosition(sciPointObj * pobj, int pposx, int pposy);
int sciSetScreenPosition( sciPointObj * pobj, int pposx, int pposy); /* SET */


void sciSelectFirstSubwin( sciPointObj * parentFigure ) ; /* SET */

int sciInitSelectedSubWin (sciPointObj * psubwinobj);
int sciSetSelectedSubWin (sciPointObj * psubwinobj); /* SET */

int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin); /* SET */

int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol); /* SET */

int sciSetdrawmode(BOOL mode); /* SET */
int sciSwitchWindow(int winnum); /* SET */

int sciInitUsedWindow( int winNum ) ; /* SET */
int sciSetUsedWindow( int winNum ) ; /* SET */

int sciSetInterpVector(sciPointObj * pobj, int size, int * value);

int sciInitAutoRotation( sciPointObj * pObj, BOOL value ) ;
int sciSetAutoRotation ( sciPointObj * pObj, BOOL value ) ;

int sciInitAutoPosition( sciPointObj * pObj, BOOL value ) ;
int sciSetAutoPosition ( sciPointObj * pObj, BOOL value ) ;

int sciInitAutoSize( sciPointObj * pObj, BOOL autoSize ) ;
int sciSetAutoSize(  sciPointObj * pObj, BOOL autoSize ) ;

int sciInitAlignment( sciPointObj * pObj, sciTextAlignment align ) ;
int sciSetAlignment(  sciPointObj * pObj, sciTextAlignment align ) ;

int sciInitUserSize( sciPointObj * pObj, double width, double height ) ;
int sciSetUserSize(  sciPointObj * pObj, double width, double height ) ;

int sciInitCenterPos( sciPointObj * pObj, BOOL newCP ) ;
int sciSetCenterPos(  sciPointObj * pObj, BOOL newCP ) ;

int sciInitIs3d( sciPointObj * pObj, BOOL is3d ) ;
int sciSetIs3d(  sciPointObj * pObj, BOOL is3d ) ;

int sciInitSelectedObject( sciPointObj * pObj ) ;
int sciSetSelectObject(    sciPointObj * pObj ) ;

int sciInitHiddenColor( sciPointObj * pObj, int newColor ) ;
int sciSetHiddenColor(  sciPointObj * pObj, int newColor ) ;

int sciInitHiddenAxisColor( sciPointObj * pObj, int newColor ) ;
int sciSetHiddenAxisColor( sciPointObj * pObj, int newColor ) ;

int setSubWinAngles( sciPointObj *psubwin, double theta, double alpha );
int sciInitdrawmode( BOOL mode );

int sciInitGraphicsStyle( sciPointObj * pobj, BOOL value );

int sciInitGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; 
int sciSetGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; /* SET */

int sciSetViewport(  sciPointObj * pObj, const int viewport[4] ) ; /* SET */

int sciInitInfoMessage(sciPointObj * pObj, const char * newMessage);
int sciSetInfoMessage( sciPointObj * pObj, const char * newMessage ) ; /* SET */

int sciInitEventHandler( sciPointObj * pObj, char * name ) ;
int sciSetEventHandler(  sciPointObj * pObj, char * name ) ;

int sciInitIsEventHandlerEnable( sciPointObj * pObj, BOOL enable ) ;
int sciSetIsEventHandlerEnable(  sciPointObj * pObj, BOOL enable ) ;

int sciSetDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* SET */
int sciSetRealDataBounds(sciPointObj * pObj, const double bounds[6]); /* SET */

int sciInitViewingAngles( sciPointObj * pObj, double alpha, double theta) ;
int sciSetViewingAngles( sciPointObj * pObj, double alpha, double theta); /* SET */

int setInfoMessageWithRotationAngles(sciPointObj * pFigure, double alpha, double theta);

int sciInitPixmapMode(sciPointObj * pObj, BOOL onOrOff);
int sciSetPixmapMode(sciPointObj * pObj, BOOL onOrOff); /* SET */

int sciInitTextPos( sciPointObj * pObj, double posX, double posY, double posZ);
int sciSetTextPos( sciPointObj * pObj, double posX, double posY, double posZ);

int sciInitLogFlags(sciPointObj * pObj, char logFlags[3]);
int sciSetLogFlags(sciPointObj * pObj, char logFlags[3]) ; /* SET */

int sciInitAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ);
int sciSetAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ); /* SET */

int sciSetZoomBox(sciPointObj * pObj, const double zoomBox[6]); /* SET */

int sciInitImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);
int sciSetImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);

int sciInitUseNurbs(sciPointObj * pObj, BOOL useNurbs);
int sciSetUseNurbs(sciPointObj * pObj, BOOL useNurbs); /* SET */

int sciInitIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics);
int sciSetIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics); /* SET */

int sciInitColorRange(sciPointObj * pObj, int subset[2]); 
int sciSetColorRange(sciPointObj * pObj, int subset[2]); /* SET */

int sciInitOutsideColors(sciPointObj * pObj, int colors[2]);
int sciSetOutsideColors(sciPointObj * pObj, int colors[2]); /* SET */

int sciInitZBounds(sciPointObj * pObj, double bounds[2]);
int sciSetZBounds(sciPointObj * pObj, double bounds[2]); /* SET */

BOOL sciInitGridFront(sciPointObj * pObj, BOOL gridFront);
BOOL sciSetGridFront(sciPointObj * pObj, BOOL gridFront); /* SET */

BOOL sciCheckColorIndex(sciPointObj * pObj, int colorIndex);

/*---------------------------------------------------------------------------*/
/* return 0 if the string contains any % character, 1 if a %d has been found */
/* and -1 otherwise. */
int checkPercent( char * string ) ;
/*---------------------------------------------------------------------------*/

#endif /* __SCI_SET_PROPERTY__ */
