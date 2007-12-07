/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE,   INRIA 2001-2002
 *    Djalel ABDEMOUCHE,   INRIA 2002-2004
 *    Fabrice Leray,       INRIA 2004-2006
 *    Jean-Baptiste SILVY, INRIA 2005-xxxx
 *    Comment:
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

extern void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha); /* SET  */

extern int sciSetEntityType (sciPointObj * pobj, sciEntityType value); /* SET */
extern int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   integer m, integer n); /* SET */
extern int sciInitNumColors( sciPointObj * pobj, int numcolors ) ; /* SET */
extern int sciSetNumColors ( sciPointObj * pobj, int numcolors ) ; /* SET */

extern int sciSetGoodIndex(sciPointObj * pobj, int colorindex);  /* SET */
extern void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m);  /* SET */
extern int sciUpdateBaW (sciPointObj * pobj, int flag, int value ) ; /* SET */

extern int  sciInitMdlBackground( sciPointObj * pobj, int colorIndex ) ;
extern int sciInitBackground( sciPointObj * pobj, int colorindex ) ;
extern int sciSetBackground ( sciPointObj * pobj, int colorindex ) ; /* SET */

extern int sciInitMdlForeground( sciPointObj * pObj, int colorIndex ) ;
extern int sciInitForeground( sciPointObj * pobj, int colorindex ) ;
extern int sciSetForeground ( sciPointObj * pobj, int colorindex ) ; /* SET */

extern int sciInitLineWidth( sciPointObj * pobj, int linewidth ) ;
extern int sciSetLineWidth ( sciPointObj * pobj, int linewidth ) ;  /* SET */

extern int sciInitIsLine(sciPointObj * pobj, BOOL isline);
extern int sciSetIsLine (sciPointObj * pobj, BOOL isline);  /* SET */

extern int sciInitLineStyle( sciPointObj * pobj, int linestyle ) ;
extern int sciSetLineStyle ( sciPointObj * pobj, int linestyle ) ; /* SET */

extern int sciInitIsMark(sciPointObj * pobj, BOOL ismark);  /* SET */
extern int sciSetIsMark (sciPointObj * pobj, BOOL ismark);  /* SET */

extern int sciInitIsFilled(sciPointObj * pobj, BOOL isfilled);
extern int sciSetIsFilled (sciPointObj * pobj, BOOL isfilled);  /* SET */

extern int sciInitMarkStyle(sciPointObj * pobj, int markstyle);
extern int sciSetMarkStyle (sciPointObj * pobj, int markstyle); /* SET */

extern int sciInitMarkSize(sciPointObj * pobj, int marksize);
extern int sciSetMarkSize (sciPointObj * pobj, int marksize); /* SET */

extern int sciInitMarkSizeUnit(sciPointObj * pobj, int marksizeunit);
extern int sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit); /* SET */

extern int sciInitMarkBackground(sciPointObj * pobj, int colorindex);
extern int sciSetMarkBackground (sciPointObj * pobj, int colorindex);  /* SET */

extern int sciInitMarkForeground(sciPointObj * pobj, int colorindex);
extern int sciSetMarkForeground (sciPointObj * pobj, int colorindex);  /* SET */

extern int sciInitBoxType( sciPointObj * pobj, EAxesBoxType type ) ;
extern int sciSetBoxType ( sciPointObj * pobj, EAxesBoxType type ) ; /* SET */

extern int sciInitIsBoxed(sciPointObj * pobj, BOOL isboxed);
extern int sciSetIsBoxed (sciPointObj * pobj, BOOL isboxed); /* SET */

/* Text Functions */
extern int sciInitFontSize(sciPointObj * pobj, double fontSize); /* SET */
extern int sciSetFontSize (sciPointObj * pobj, double fontSize); /* SET */

extern int sciInitFontOrientation(sciPointObj * pobj, double textorientation); /* SET */
extern int sciSetFontOrientation (sciPointObj * pobj, double textorientation); /* SET */

extern int sciSetStrings( sciPointObj * pobj, const StringMatrix * pStrings ) ; /* SET */
extern int sciSetText (   sciPointObj * pobj, char ** text, int nbRow, int nbCol ) ; /* SET */

extern int sciInitFontBackground(sciPointObj * pobj, int color);
extern int sciSetFontBackground (sciPointObj * pobj, int color); /* SET */

extern int sciInitFontForeground(sciPointObj * pobj, int colorindex ) ;
extern int sciSetFontForeground (sciPointObj * pobj, int colorindex ) ; /* SET */

extern int sciInitFontStyle(sciPointObj * pobj, int iAttributes ) ;
extern int sciSetFontStyle (sciPointObj * pobj, int iAttributes ) ; /* SET */

extern int sciSetFontName (sciPointObj * pobj, char pfontname[], int n); /* SET */

/*Title Functions */
extern int sciSetTitlePos (sciPointObj * pobj, int x, int y); /* SET */
extern int sciSetTitlePlace (sciPointObj * pobj, sciTitlePlace place); /* SET */

/* Legend */
extern int sciInitLegendPlace(sciPointObj * pobj, sciLegendPlace place);
extern int sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place); /* SET */

extern int sciInitLegendPos(sciPointObj * pobj, int x, int y);
extern int sciSetLegendPos (sciPointObj * pobj, int x, int y); /* SET */

/* GMODE */
extern int sciInitIsClipping(sciPointObj * pobj, int value) ;
extern int sciSetIsClipping (sciPointObj * pobj, int value); /* SET */

extern int sciSetClipping (sciPointObj * pobj, double pclip[4] ); /* SET */

extern int sciInitHighLight(sciPointObj * pobj, BOOL value);
extern int sciSetHighLight (sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitAddPlot (sciPointObj * pobj, BOOL value);
extern int sciSetAddPlot (sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitAutoScale(sciPointObj * pobj, BOOL value);
extern int sciSetAutoScale (sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitZooming(sciPointObj * pobj, BOOL value);
extern int sciSetZooming (sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitXorMode(sciPointObj * pobj, int value);
extern int sciSetXorMode (sciPointObj * pobj, int value); /* SET */

extern int sciInitVisibility (sciPointObj * pobj, BOOL value);
extern int sciSetVisibility (sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitResize(sciPointObj * pobj, BOOL value);
extern int sciSetResize (sciPointObj * pobj, BOOL value); /* SET */

extern int sciSetDefaultValues (void); /* SET */


/* Window Functions */
extern int sciSetName (sciPointObj * pobj, char *pvalue, int length); /* SET */
extern int sciInitNum( sciPointObj * pobj, int value ) ;
extern int sciSetNum ( sciPointObj * pobj, int value ) ; /* SET */

extern int sciInitDimension( sciPointObj * pobj, int newWidth, int newHeight ) ;
extern int sciSetDimension(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

extern int sciInitWindowDim( sciPointObj * pobj, int newWidth, int newHeight ) ;
extern int sciSetWindowDim(  sciPointObj * pobj, int newWidth, int newHeight ) ; /* SET */

extern int sciInitScreenPosition(sciPointObj * pobj, int pposx, int pposy);
extern int sciSetScreenPosition( sciPointObj * pobj, int pposx, int pposy); /* SET */

extern int sciInitFigureIconify(sciPointObj * pobj, BOOL value);
extern int sciSetFigureIconify( sciPointObj * pobj, BOOL value); /* SET */

extern int sciInitSubWindowPos(sciPointObj * pobj, int *x, int *y);
extern int sciSetSubWindowPos( sciPointObj * pobj, int *x, int *y); /* SET */

extern void sciSelectFirstSubwin( sciPointObj * parentFigure ) ; /* SET */

extern int sciInitSelectedSubWin (sciPointObj * psubwinobj);
extern int sciSetSelectedSubWin (sciPointObj * psubwinobj); /* SET */

extern int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin); /* SET */

extern int sciSetReplay (BOOL value); /* SET */
/* extern int sciSetHDC (sciPointObj * pobj); /\* SET *\/ /\* unused?? *\/ */
extern int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol); /* SET */

extern int sciSetdrawmode(BOOL mode); /* SET */
extern int sciSwitchWindow(int winnum); /* SET */

extern int sciInitUsedWindow( int winNum ) ; /* SET */
extern int sciSetUsedWindow( int winNum ) ; /* SET */

extern int sciSetInterpVector(sciPointObj * pobj, int size, int * value);

extern int sciInitAutoRotation( sciPointObj * pObj, BOOL value ) ;
extern int sciSetAutoRotation ( sciPointObj * pObj, BOOL value ) ;

extern int sciInitAutoPosition( sciPointObj * pObj, BOOL value ) ;
extern int sciSetAutoPosition ( sciPointObj * pObj, BOOL value ) ;

extern int sciInitAutoSize( sciPointObj * pObj, BOOL autoSize ) ;
extern int sciSetAutoSize(  sciPointObj * pObj, BOOL autoSize ) ;

extern int sciInitAlignment( sciPointObj * pObj, sciTextAlignment align ) ;
extern int sciSetAlignment(  sciPointObj * pObj, sciTextAlignment align ) ;

extern int sciInitUserSize( sciPointObj * pObj, double width, double height ) ;
extern int sciSetUserSize(  sciPointObj * pObj, double width, double height ) ;

extern int sciInitCenterPos( sciPointObj * pObj, BOOL newCP ) ;
extern int sciSetCenterPos(  sciPointObj * pObj, BOOL newCP ) ;

extern int sciInitIs3d( sciPointObj * pObj, BOOL is3d ) ;
extern int sciSetIs3d(  sciPointObj * pObj, BOOL is3d ) ;

extern int sciInitSelectedObject( sciPointObj * pObj ) ;
extern int sciSetSelectObject(    sciPointObj * pObj ) ;

extern int sciInitHiddenColor( sciPointObj * pObj, int newColor ) ;
extern int sciSetHiddenColor(  sciPointObj * pObj, int newColor ) ;

int sciInitHiddenAxisColor( sciPointObj * pObj, int newColor ) ;
int sciSetHiddenAxisColor( sciPointObj * pObj, int newColor ) ;

int setSubWinAngles( sciPointObj *psubwin, double theta, double alpha );
int sciInitdrawmode( BOOL mode );

int sciInitGraphicsStyle( sciPointObj * pobj, BOOL value );

extern int sciInitGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; 
extern int sciSetGridStyle( sciPointObj * pObj, int xStyle, int yStyle, int zStyle ) ; /* SET */

int sciInitViewport( sciPointObj * pObj, int xSize, int ySize ) ;
int sciSetViewport(  sciPointObj * pObj, int xSize, int ySize ) ; /* SET */

int sciSetInfoMessage( sciPointObj * pObj, const char * newMessage ) ; /* SET */

int sciInitEventHandler( sciPointObj * pObj, char * name ) ;
int sciSetEventHandler(  sciPointObj * pObj, char * name ) ;

int sciInitIsEventHandlerEnable( sciPointObj * pObj, BOOL enable ) ;
int sciSetIsEventHandlerEnable(  sciPointObj * pObj, BOOL enable ) ;

int sciSetSelectedObject( sciPointObj * pObj ) ;
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

/*---------------------------------------------------------------------------*/
/* return 0 if the string contains any % character, 1 if a %d has been found */
/* and -1 otherwise. */
extern int checkPercent( char * string ) ;
/*---------------------------------------------------------------------------*/

#endif /* __SCI_SET_PROPERTY__ */
