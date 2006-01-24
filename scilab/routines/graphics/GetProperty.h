/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"

#ifndef __SCI_GET_PROPERTY__
#define __SCI_GET_PROPERTY__

extern void *sciGetPointerToFeature (sciPointObj * pobj); /* GET */
void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr); /* GET */
extern sciEntityType sciGetEntityType (sciPointObj * pobj);  /* GET */
extern char *sciGetCharEntityType (sciPointObj * pobj);  /* GET */

/* Graphic Context Functions */
extern sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj); /* GET */
extern int sciGetNumColors (sciPointObj * pobj); /* GET */
extern int sciGetColormap (sciPointObj * pobj, double *rgbmat); /* GET */
extern int sciGetGoodIndex(sciPointObj * pobj, int colorindex);  /* GET */

extern int sciGetForegroundToDisplay (sciPointObj * pobj); /* GET */
extern int sciGetBackgroundToDisplay (sciPointObj * pobj); /* GET */

extern int sciGetBackground (sciPointObj * pobj); /* GET */
extern int sciGetForeground (sciPointObj * pobj); /* GET */
extern int sciGetLineWidth (sciPointObj * pobj); /* GET */
extern BOOL sciGetIsLine (sciPointObj * pobj); /* GET */
extern int sciGetLineStyle (sciPointObj * pobj); /* GET */
extern BOOL sciGetIsMark (sciPointObj * pobj); /* GET */
extern BOOL sciGetIsFilled (sciPointObj * pobj); /* GET */
extern BOOL sciGetIsBoxed (sciPointObj * pobj); /* GET */
extern int sciGetMarkStyle (sciPointObj * pobj); /* GET */
extern int sciGetMarkSize (sciPointObj * pobj); /* GET */
extern int sciGetMarkSizeUnit (sciPointObj * pobj); /* GET */
extern int sciGetFillStyle (sciPointObj * pobj); /* GET */

/* Text Functions */
extern sciFont *sciGetFontContext (sciPointObj * pobj);  /* GET */
extern int sciGetFontDeciWidth (sciPointObj * pobj); /* GET */
extern int sciGetFontOrientation (sciPointObj * pobj); /* GET */
extern char *sciGetText (sciPointObj * pobj); /* GET */
extern unsigned int sciGetTextLength (sciPointObj * pobj); /* GET */

extern int sciGetFontBackground (sciPointObj * pobj); /* GET */
extern int sciGetFontForeground (sciPointObj * pobj); /* GET */
extern int sciGetMarkBackground (sciPointObj * pobj); /* GET */
extern int sciGetMarkForeground (sciPointObj * pobj); /* GET */
/* F.Leray 09.04.04*/
extern int sciGetFontBackgroundToDisplay (sciPointObj * pobj); /* GET */
extern int sciGetFontForegroundToDisplay (sciPointObj * pobj); /* GET */
extern int sciGetMarkBackgroundToDisplay (sciPointObj * pobj); /* GET */
extern int sciGetMarkForegroundToDisplay (sciPointObj * pobj); /* GET */

extern int sciGetFontStyle (sciPointObj * pobj); /* GET */
extern char *sciGetFontName (sciPointObj * pobj); /* GET */
extern unsigned int sciGetFontNameLength (sciPointObj * pobj); /* GET */
extern double sciGetTextPosX (sciPointObj * pobj); /* GET */
extern double sciGetTextPosY (sciPointObj * pobj); /* GET */
extern double sciGetTextPosWidth (sciPointObj * pobj); /* GET */
extern double sciGetTextPosHeight (sciPointObj * pobj); /* GET */

/*Title Functions */
extern POINT2D sciGetTitlePos (sciPointObj * pobj); /* GET */
extern sciTitlePlace sciGetTitlePlace (sciPointObj * pobj); /* GET */

/* Legend */
extern sciTitlePlace sciGetLegendPlace (sciPointObj * pobj);
extern POINT2D sciGetLegendPos (sciPointObj * pobj);

/* Figure / Subwin main functions */
extern sciPointObj *sciGetParentFigure (sciPointObj * pobj); /* GET */
extern sciPointObj *sciGetParentSubwin (sciPointObj * pobj); /* GET */
extern int sciGetNumFigure (sciPointObj * pobj); /* GET */
extern struct BCG *sciGetScilabXgc (sciPointObj * pobj); /* GET */

/* GMODE */
extern scigMode *sciGetGraphicMode (sciPointObj * pobj);  /* GET */
extern int sciGetIsClipRegionValuated (sciPointObj * pobj);
extern int sciGetIsClipping (sciPointObj * pobj); /* GET */
extern double *sciGetClipping (sciPointObj * pobj); /* GET */
extern BOOL sciGetHighLight (sciPointObj * pobj);  /* GET */
extern BOOL sciGetAddPlot (sciPointObj * pobj);  /* GET */
extern BOOL sciGetAutoScale (sciPointObj * pobj);  /* GET */
extern BOOL sciGetZooming (sciPointObj * pobj);  /* GET */
extern BOOL sciGetGraphicsStyle (sciPointObj * pobj);  /* GET */
extern int sciGetXorMode (sciPointObj * pobj);  /* GET */
extern BOOL sciGetVisibility (sciPointObj * pobj);  /* GET */
extern BOOL sciGetResize (sciPointObj * pobj);  /* GET */


/* Window Functions */
extern char *sciGetName (sciPointObj * pobj);  /* GET */
extern int sciGetNameLength (sciPointObj * pobj);  /* GET */
extern int sciGetNum (sciPointObj * pobj);  /* GET */
extern double sciGetWidth (sciPointObj * pobj);   /* GET */
extern double sciGetHeight (sciPointObj * pobj);  /* GET */
extern int sciGetFigurePosX (sciPointObj * pobj);  /* GET */
extern int sciGetFigurePosY (sciPointObj * pobj);  /* GET */
extern BOOL sciGetIsFigureIconified (sciPointObj * pobj);  /* GET */
extern int sciGetSubwindowPosX (sciPointObj * pobj);  /* GET */
extern int sciGetSubwindowPosY (sciPointObj * pobj);  /* GET */
extern BOOL sciGetIsSelected (sciPointObj * pobj);  /* GET */
extern sciPointObj *sciGetSelectedSon (sciPointObj * pobj);  /* GET */
extern sciPointObj *sciGetSelectedSubWin (sciPointObj * pparent);  /* GET */
extern sciPointObj *sciIsExistingSubWin (double *WRect); /* GET */ /* WARNING special case here */
extern sciPointObj *sciIsExistingFigure(int *value); /* GET */ /* WARNING special case here */
extern int sciGetScrollPosV (sciPointObj * pobj);  /* GET */
extern int sciGetScrollPosH (sciPointObj * pobj);  /* GET */


extern sciPointObj *sciGetCurrentFigure (); /* GET */
extern struct BCG *sciGetCurrentScilabXgc (); /* GET */
extern BOOL sciGetReplay (); /* GET */


extern double *sciGetPoint (sciPointObj * pthis, int *num, int *numco); /* GET */
extern sciPointObj *sciGetObjClicked (sciPointObj *pthis,int x, int y); /* GET */ /* unused */
extern BOOL sciIsClicked(sciPointObj *pthis,int x, int y);  /* GET */

extern BOOL sciGetdrawmode (sciPointObj *pobj); /* GET */
extern sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin); /* GET */


extern int sciType (char *marker, sciPointObj *pobj); /* GET */

extern void sciGetIdFigure (int *vect, int *id, int *flag); /* GET */
extern int version_flag(); /* GET */
extern unsigned int sciGetFontNameLength (sciPointObj * pobj); /* GET */
/* extern struct BCG *sciGetCurrentScilabXgc (); /\* GET *\/ */

extern sciPointObj *sciGetSurface(sciPointObj *psubwin); /* GET */
extern BOOL Check3DObjs(sciPointObj *pobj); /* GET */
extern sciPointObj *CheckClickedSubwin(integer x, integer y); /* GET */

/* extern BOOL GetIsAxes(sciPointObj *psubwin); /\* GET *\/ */
extern BOOL GetIsAxes2D(sciPointObj *psubwin); /* GET */

extern int CheckForCompound (long *tabpointobj, int number); /* GET */
extern sciPointObj *sciGetMerge(sciPointObj *psubwin);  /* GET */

/* UNUSED ? */
extern sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);
extern HMENU sciGethPopMenu (sciPointObj * pthis);

extern int *sciGetInterpVector(sciPointObj * pobj);
extern int sciGetPosition (sciPointObj * pobj, double *x, double *y);

extern BOOL sciGetAutoRotation ( sciPointObj * pObj ) ;
extern BOOL sciGetAutoPosition ( sciPointObj * pObj ) ;

#endif /* __SCI_GET_PROPERTY__ */
