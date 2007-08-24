/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
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

int get_cf_type( void ) ;

extern void GPopupResize (struct BCG * ScilabXgc,int * width,int * height);
/* static struct BCG *tmpScilabXgc; */

void set_cf_type( int val ) ;



char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads);
double * AllocUserGrads(double * u_xgrads, int nb);
int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);
char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);


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
extern int GradLog(double _min, double _max, double *_grads, int * n_grads, int compNgrads );
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

extern int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input); /* DRAWINGS */
extern int ComputeCorrectXindAndInsideUD(double Teta,double Alpha, double * dbox, integer *xind, integer *InsideU, integer *InsideD); /* DRAWINGS */
extern int Axes3dStrings2(integer *ixbox, integer *iybox, integer *xind); /* DRAWINGS */

extern int CheckDisplay(double fact_h, double fact_w, char logflag, char *foo,int *posi,int *fontid,int *old_rect);

extern int IsInsideRectangle(int * rect, int *point);

extern int ComputeGoodTrans3d( sciPointObj * psubwin, int n, int *xm, int *ym, double * fx, double *fy, double *fz); /* DRAWINGS */
extern void DrawAxesIfRequired(sciPointObj*); /* DRAWINGS */
extern void DrawAxes(sciPointObj*); /* DRAWINGS */

extern int sciDrawObjIfRequired (sciPointObj * pobj); /* DRAWINGS */
extern int sciRefreshObj( sciPointObj * pobj ) ; /* DRAWINGS */


extern int ComputeXIntervals(sciPointObj *pobj, char xy_type, double **vector, int * N, int checkdim); /* DRAWINGS */ /* inside matdes.c */

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


void computeLabelAutoPos( sciPointObj * pLabel, int axisStart[2], int axisEnd[2], int offsets[2] ) ;

void sciDrawFigure( int numFigure ) ;


extern int computeRealArrowSize( sciPointObj * pSegs,
                                int nbSegs         ,
                                int xCoord[]       ,
                                int yCoord[]         ) ;

#endif /* __SCI_DRAWINGS__ */
