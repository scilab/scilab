/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to SET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"
/* #include "GraphicGlobals.h" */


#ifndef __SCI_SET_PROPERTY__
#define __SCI_SET_PROPERTY__

extern void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha); /* SET  */

extern void sciSetEntityType (sciPointObj * pobj, sciEntityType value); /* SET */
extern int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   integer m, integer n); /* SET */
extern int sciSetNumColors (sciPointObj * pobj, int numcolors); /* SET */

extern int sciSetGoodIndex(sciPointObj * pobj, int colorindex);  /* SET */
extern void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m);  /* SET */
extern int sciUpdateBaW (sciPointObj * pobj, int flag, int value);  /* SET */
extern int sciSetBackground (sciPointObj * pobj, int colorindex);  /* SET */
extern int sciSetForeground (sciPointObj * pobj, int colorindex);  /* SET */
extern int sciSetLineWidth (sciPointObj * pobj, int linewidth);  /* SET */
extern int sciSetIsLine (sciPointObj * pobj, BOOL isline);  /* SET */
extern int sciSetLineStyle (sciPointObj * pobj, int linestyle); /* SET */
extern int sciSetIsMark (sciPointObj * pobj, BOOL ismark);  /* SET */
extern int sciSetMarkStyle (sciPointObj * pobj, int markstyle); /* SET */
extern int sciSetMarkSize (sciPointObj * pobj, int marksize); /* SET */
extern int sciSetMarkSizeUnit (sciPointObj * pobj, int marksizeunit); /* SET */
extern int sciSetMarkBackground (sciPointObj * pobj, int colorindex);  /* SET */
extern int sciSetMarkForeground (sciPointObj * pobj, int colorindex);  /* SET */
extern int sciSetFillStyle (sciPointObj * pobj, int fillstyle); /* SET */
extern int sciSetFillColor (sciPointObj * pobj, int fillcolor); /* SET */
extern int sciSetFillFlag (sciPointObj * pobj, int fillflag); /* SET */

/* Text Functions */
extern int sciSetFontDeciWidth (sciPointObj * pobj, int fontdeciwidth); /* SET */
extern int sciSetFontOrientation (sciPointObj * pobj, int textorientation); /* SET */
extern int sciSetText (sciPointObj * pobj, char text[], int n); /* SET */
extern int sciSetFontBackground (sciPointObj * pobj, int color); /* SET */
extern int sciSetFontForeground (sciPointObj * pobj, int color); /* SET */
extern int sciSetFontStyle (sciPointObj * pobj, int fontdeciwidth); /* SET */
extern int sciSetFontName (sciPointObj * pobj, char pfontname[], int n); /* SET */
extern int sciSetTextPosX (sciPointObj * pobj, double x); /* SET */
extern int sciSetTextPosY (sciPointObj * pobj, double x); /* SET */

/*Title Functions */
extern int sciSetTitlePos (sciPointObj * pobj, int x, int y); /* SET */
extern int sciSetTitlePlace (sciPointObj * pobj, sciTitlePlace place); /* SET */

/* Legend */
extern int sciSetLegendPlace (sciPointObj * pobj, sciLegendPlace place);
extern int sciSetLegendPos (sciPointObj * pobj, int x, int y);

/* GMODE */
extern void sciSetIsClipping (sciPointObj * pobj, int value); /* SET */
extern int  sciSetClipping (sciPointObj * pobj, double *pclip); /* SET */
extern void sciSetHighLight (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetAddPlot (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetAutoScale (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetZooming (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetGraphicsStyle (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetXorMode (sciPointObj * pobj, int value); /* SET */
extern void sciSetVisibility (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetResize (sciPointObj * pobj, BOOL value); /* SET */
extern void sciSetDefaultValues (); /* SET */


/* Window Functions */
extern void sciSetName (sciPointObj * pobj, char *pvalue, int length); /* SET */
extern void sciSetNum (sciPointObj * pobj, int *pvalue); /* SET */
extern void sciSetDim (sciPointObj * pobj, int *pwidth, int *pheight); /* SET */
extern int sciSetFigurePos (sciPointObj * pobj, int pposx, int pposy); /* SET */
extern void sciSetFigureIconify (sciPointObj * pobj, BOOL value); /* SET */
extern int sciSetSubWindowPos (sciPointObj * pobj, int *x, int *y); /* SET */
extern int sciSetSelectedSubWin (sciPointObj * psubwinobj); /* SET */
extern int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin); /* SET */

extern void sciSetCurrentFigure (sciPointObj * mafigure); /* SET */

extern int sciSetReplay (BOOL value); /* SET */
/* extern int sciSetHDC (sciPointObj * pobj); /\* SET *\/ /\* unused?? *\/ */
extern int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol); /* SET */

extern int sciSetdrawmode(BOOL mode); /* SET */
extern int sciSwitchWindow(int *winnum); /* SET */
void set_version_flag(int flag) ; /* SET */


#endif /* __SCI_SET_PROPERTY__ */
