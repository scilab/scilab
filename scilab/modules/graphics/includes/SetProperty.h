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

void Obj_RedrawNewAngle(sciPointObj * pSubwin, double alpha, double theta);

int sciSetEntityType (sciPointObj * pobj, sciEntityType value);
int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   int m, int n);
int sciInitNumColors( sciPointObj * pobj, int numcolors ) ;
int sciSetNumColors ( sciPointObj * pobj, int numcolors ) ;

int sciSetGoodIndex(sciPointObj * pobj, int colorindex); 
void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m); 
int sciUpdateBaW (sciPointObj * pobj, int flag, int value ) ;

/**
* sciSetBackgroundColor function 
* Set the stored value of BackgroundColor index
* @return 0 if ok, -1 if not (no GraphicContext)
*/
int sciSetBackgroundColor (sciPointObj * pobj, int colorindex);

/**
* sciSetForegroundColor function 
* Set the stored value of ForegroundColor index
* @return 0 if ok, -1 if not (no GraphicContext)
*/
int sciSetForegroundColor (sciPointObj * pobj, int colorindex);

/**
* sciSetMarkBackgroundColor function 
* Set the stored value of MarkBackgroundColor index
* @return 0 if ok, -1 if not (no GraphicContext)
*/
int sciSetMarkBackgroundColor (sciPointObj * pobj, int colorindex);

/**
* sciSetMarkForegroundColor function 
* Set the stored value of MarkForegroundColor index
* @return 0 if ok, -1 if not (no GraphicContext)
*/
int sciSetMarkForegroundColor (sciPointObj * pobj, int colorindex);

int sciInitBackground( sciPointObj * pobj, int colorindex ) ;
int sciSetBackground ( sciPointObj * pobj, int colorindex ) ;

int sciInitForeground( sciPointObj * pobj, int colorindex ) ;
int sciSetForeground ( sciPointObj * pobj, int colorindex ) ;

int sciInitLineWidth( sciPointObj * pobj, int linewidth ) ;
int sciSetLineWidth ( sciPointObj * pobj, int linewidth ) ; 

int sciInitIsLine(sciPointObj * pobj, BOOL isline);
int sciSetIsLine (sciPointObj * pobj, BOOL isline); 

int sciInitLineStyle( sciPointObj * pobj, int linestyle ) ;
int sciSetLineStyle ( sciPointObj * pobj, int linestyle ) ;

int sciInitIsMark(sciPointObj * pobj, BOOL ismark); 
int sciSetIsMark (sciPointObj * pobj, BOOL ismark); 

int sciInitIsFilled(sciPointObj * pobj, BOOL isfilled);
int sciSetIsFilled (sciPointObj * pobj, BOOL isfilled); 

int sciInitPolylineStyle( sciPointObj * pobj, int plot );
int sciInitArrowSize( sciPointObj * pobj, double size );

int sciInitMarkStyle(sciPointObj * pobj, int markstyle);
int sciSetMarkStyle (sciPointObj * pobj, int markstyle);

int sciInitMarkSize(sciPointObj * pobj, int marksize);
int sciSetMarkSize (sciPointObj * pobj, int marksize);

int sciInitMarkSizeUnit(sciPointObj * pobj, int marksizeunit);
int sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit);

int sciInitMarkBackground(sciPointObj * pobj, int colorindex);
int sciSetMarkBackground (sciPointObj * pobj, int colorindex); 

int sciInitMarkForeground(sciPointObj * pobj, int colorindex);
int sciSetMarkForeground (sciPointObj * pobj, int colorindex); 

int sciInitBoxType( sciPointObj * pobj, EAxesBoxType type ) ;
int sciSetBoxType ( sciPointObj * pobj, EAxesBoxType type ) ;

int sciInitIsBoxed(sciPointObj * pobj, BOOL isboxed);
int sciSetIsBoxed (sciPointObj * pobj, BOOL isboxed);

/* Text Functions */
int sciInitFontSize(sciPointObj * pobj, double fontSize);
int sciSetFontSize (sciPointObj * pobj, double fontSize);

int sciInitFontOrientation(sciPointObj * pobj, double textorientation);
int sciSetFontOrientation (sciPointObj * pobj, double textorientation);

int sciSetStrings( sciPointObj * pobj, const StringMatrix * pStrings ) ;
int sciSetText (   sciPointObj * pobj, char ** text, int nbRow, int nbCol ) ;

int sciInitFontBackground(sciPointObj * pobj, int color);
int sciSetFontBackground (sciPointObj * pobj, int color);

int sciInitFontForeground(sciPointObj * pobj, int colorindex ) ;
int sciSetFontForeground (sciPointObj * pobj, int colorindex ) ;

int sciInitFontStyle(sciPointObj * pobj, int iAttributes ) ;
int sciSetFontStyle (sciPointObj * pobj, int iAttributes ) ;

/* Legend */
int sciInitLegendPlace(sciPointObj * pobj, sciLegendPlace place);
int sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place);

int sciInitLegendPos(sciPointObj * pobj, double position[]);
int sciSetLegendPos (sciPointObj * pobj, double position[]);

/* GMODE */
int sciInitIsClipping(sciPointObj * pobj, int value) ;
int sciSetIsClipping (sciPointObj * pobj, int value);

int sciSetClipping (sciPointObj * pobj, double pclip[4] );

int sciInitAddPlot (sciPointObj * pobj, BOOL value);
int sciSetAddPlot (sciPointObj * pobj, BOOL value);

int sciInitAutoScale(sciPointObj * pobj, BOOL value);
int sciSetAutoScale (sciPointObj * pobj, BOOL value);

int sciInitZooming(sciPointObj * pobj, BOOL value);
int sciSetZooming (sciPointObj * pobj, BOOL value);

int sciInitXorMode(sciPointObj * pobj, int value);
int sciSetXorMode (sciPointObj * pobj, int value);

int sciInitVisibility (sciPointObj * pobj, BOOL value);
int sciSetVisibility (sciPointObj * pobj, BOOL value);

int sciInitResize(sciPointObj * pobj, BOOL value);
int sciSetResize (sciPointObj * pobj, BOOL value);

int sciSetDefaultValues (void);


/* Window Functions */
int sciInitName(sciPointObj * pobj, char * newName);
int sciSetName (sciPointObj * pobj, char * newName);
int sciInitNum( sciPointObj * pobj, int value ) ;
int sciSetNum ( sciPointObj * pobj, int value ) ;

int sciInitDimension( sciPointObj * pobj, int newWidth, int newHeight ) ;
int sciSetDimension(  sciPointObj * pobj, int newWidth, int newHeight ) ;

int sciInitWindowDim( sciPointObj * pobj, int newWidth, int newHeight ) ;
int sciSetWindowDim(  sciPointObj * pobj, int newWidth, int newHeight ) ;

int sciInitScreenPosition(sciPointObj * pobj, int pposx, int pposy);
int sciSetScreenPosition( sciPointObj * pobj, int pposx, int pposy);


void sciSelectFirstSubwin( sciPointObj * parentFigure ) ;

int sciInitSelectedSubWin (sciPointObj * psubwinobj);
int sciSetSelectedSubWin (sciPointObj * psubwinobj);

int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin);

int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol);

int sciSetdrawmode(BOOL mode);
int sciSwitchWindow(int winnum);

int sciInitUsedWindow( int winNum ) ;
int sciSetUsedWindow( int winNum ) ;

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
int sciSetGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ;

int sciSetViewport(  sciPointObj * pObj, const int viewport[4] ) ;

int sciInitInfoMessage(sciPointObj * pObj, const char * newMessage);
int sciSetInfoMessage( sciPointObj * pObj, const char * newMessage ) ;

int sciInitEventHandler( sciPointObj * pObj, char * name ) ;
int sciSetEventHandler(  sciPointObj * pObj, char * name ) ;

int sciInitIsEventHandlerEnable( sciPointObj * pObj, BOOL enable ) ;
int sciSetIsEventHandlerEnable(  sciPointObj * pObj, BOOL enable ) ;

int sciSetDataBounds( sciPointObj * pObj, double bounds[6] ) ;
int sciSetRealDataBounds(sciPointObj * pObj, const double bounds[6]);

int sciInitViewingAngles( sciPointObj * pObj, double alpha, double theta) ;
int sciSetViewingAngles( sciPointObj * pObj, double alpha, double theta);

int setInfoMessageWithRotationAngles(sciPointObj * pFigure, double alpha, double theta);

int sciInitPixmapMode(sciPointObj * pObj, BOOL onOrOff);
int sciSetPixmapMode(sciPointObj * pObj, BOOL onOrOff);

int sciInitTextPos( sciPointObj * pObj, double posX, double posY, double posZ);
int sciSetTextPos( sciPointObj * pObj, double posX, double posY, double posZ);

int sciInitLogFlags(sciPointObj * pObj, char logFlags[3]);
int sciSetLogFlags(sciPointObj * pObj, char logFlags[3]) ;

int sciInitAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ);
int sciSetAutoTicks(sciPointObj * pObj, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ);

int sciSetZoomBox(sciPointObj * pObj, const double zoomBox[6]);

int sciInitImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);
int sciSetImmediateDrawingMode(sciPointObj * pObj, BOOL autoRedraw);

int sciInitUseNurbs(sciPointObj * pObj, BOOL useNurbs);
int sciSetUseNurbs(sciPointObj * pObj, BOOL useNurbs);

int sciInitIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics);
int sciSetIsUsingFractionalMetrics(sciPointObj * pObj, BOOL useFractionalMetrics);

int sciInitColorRange(sciPointObj * pObj, int subset[2]); 
int sciSetColorRange(sciPointObj * pObj, int subset[2]);

int sciInitOutsideColors(sciPointObj * pObj, int colors[2]);
int sciSetOutsideColors(sciPointObj * pObj, int colors[2]);

int sciInitZBounds(sciPointObj * pObj, double bounds[2]);
int sciSetZBounds(sciPointObj * pObj, double bounds[2]);

int sciInitGridFront(sciPointObj * pObj, BOOL gridFront);
int sciSetGridFront(sciPointObj * pObj, BOOL gridFront);

int sciInitLegendLocation(sciPointObj * pObj, sciLegendPlace location);
int sciSetLegendLocation(sciPointObj * pObj, sciLegendPlace location);

BOOL sciCheckColorIndex(sciPointObj * pObj, int colorIndex);

/*---------------------------------------------------------------------------*/
/* return 0 if the string contains any % character, 1 if a %d has been found */
/* and -1 otherwise. */
int checkPercent( char * string ) ;
/*---------------------------------------------------------------------------*/

#endif /* __SCI_SET_PROPERTY__ */
