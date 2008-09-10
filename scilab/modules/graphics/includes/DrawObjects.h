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
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_DRAWINGS__
#define __SCI_DRAWINGS__

#define spINSIDE_SPARSE /* F.Leray to have LARGEST_REAL*/
#include "../../sparse/includes/spConfig.h"


#include "PloEch.h"
#include "Vect2Matrix.h"
#include "StringMatrix.h"
#include "Format.h"

char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads);
double * AllocUserGrads(double * u_xgrads, int nb);
int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);


extern int cf_type ; /* used by gcf to determine if current figure is a graphic (1) or a tksci (0) one */


/*******************************************************/

#ifdef _MSC_VER
extern HFONT sciCreateFont (HDC hdc, char *szFaceName, int iDeciPtHeight, int iDeciPtWidth, int iAttributes, BOOL fLogRes);  /* DRAWINGS */
/* extern HDC sciGetHDC (sciPointObj * pobj);    /\* DRAWINGS *\/ */ /* inexistant in Entities.c */
#endif

extern void sciRedrawFigure(void); /* DRAWINGS */
extern void sciRedrawF(int *num); /* DRAWINGS */
void sciClearFigure(sciPointObj * pFigure) ; /* DRAWINGS */
extern void sciXbasc(void);  /* DRAWINGS */
extern void sciXclear(void); /* DRAWINGS */
 
/* 3 next are in plo3d.c*/
extern void SetEch3d1(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha, integer flag); /* DRAWINGS */

extern void Nextind(integer ind1, integer *ind2, integer *ind3); /* DRAWINGS */
extern BOOL Ishidden(sciPointObj *pobj); /* DRAWINGS */
extern BOOL IsDownAxes(sciPointObj *pobj); /* DRAWINGS */
extern void sciGetDisplayedBounds( sciPointObj * pSubWin,
                                   double      * xmin   ,
                                   double      * xmax   ,
                                   double      * ymin   ,
                                   double      * ymax   ,
                                   double      * zmin   ,
                                   double      * zmax    ) ;
extern BOOL sci_update_frame_bounds_3d(sciPointObj *pobj);  /* DRAWINGS */
extern BOOL sci_update_frame_bounds_2d(sciPointObj *pobj);  /* DRAWINGS */

extern int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, const double * grads, int nbsubtics_input); /* DRAWINGS */
extern int ComputeCorrectXindAndInsideUD(double Teta,double Alpha, double * dbox, integer *xind, integer *InsideU, integer *InsideD); /* DRAWINGS */
extern int Axes3dStrings2(integer *ixbox, integer *iybox, integer *xind); /* DRAWINGS */

extern int CheckDisplay(double fact_h, double fact_w, char logflag, char *foo,int *posi,int *fontid,int *old_rect);

extern int IsInsideRectangle(int * rect, int *point);

extern int ComputeGoodTrans3d( sciPointObj * psubwin, int n, int *xm, int *ym, double * fx, double *fy, double *fz); /* DRAWINGS */
extern void DrawAxesIfRequired(sciPointObj*); /* DRAWINGS */
extern void DrawAxes(sciPointObj*); /* DRAWINGS */

extern int sciDrawObjIfRequired (sciPointObj * pobj); /* DRAWINGS */
extern int sciRefreshObj( sciPointObj * pobj ) ; /* DRAWINGS */


/* Other functions coming from Entities.c (old file) */


/**DJ.Abdemouche 2003**/

extern int Gen3DPoints (integer type,integer *polyx,integer *polyy,integer *fill,integer whiteid,double zmin,double zmax,double *,double *,double *,integer i,integer j,integer jj1,integer *p,integer dc,integer fg, sciPointObj *psurface); /* DRAWINGS */

extern int CheckPixelStatus(void);

extern int GetDPIFromDriver(int * DPI);
extern int GetBarNumber(sciPointObj * pobj);
extern double FindWidth(sciPointObj * pobj, int n1, int bar_number, double *x);

extern void sciGetAxisBox( double limits[6], double xbox[8], double ybox[8], double zbox[8] ) ;
extern void sciUpdateScaleAngles( double theta, double alpha ) ;
extern void sciAxesVerticesIndices( integer insideU[4],
                                    integer insideD[4],
                                    double  xbox[8]   ,
                                    double  ybox[8]   ,
                                    integer xind[8]    ) ;

extern void matrixLine2String( char ** matrix, int matrixSize[2], int numLine, char * dest ) ;

void sciDrawFigure( int numFigure ) ;


extern int computeRealArrowSize( sciPointObj * pSegs,
                                int nbSegs         ,
                                int xCoord[]       ,
                                int yCoord[]         ) ;


/**
 * Show the pixmap buffer of a figure
 */
void showPixmap(sciPointObj * pFigure);

/**
 * Clear the pixmap buffer of a figure
 */
void clearPixmap(sciPointObj * pFigure);

#endif /* __SCI_DRAWINGS__ */
