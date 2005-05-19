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
#if defined(THINK_C) || defined (__MWERKS__)
#include "::sparse:spConfig.h" 
#else
#include "../sparse/spConfig.h"
#endif

#include "bcg.h"
#include "PloEch.h" 

/* DJ.A 2003 */
#ifdef __STDC__
void wininfo(char *format,...);
#else
/*VARARGS0*/
void wininfo();
#endif


extern void GPopupResize();
/* static struct BCG *tmpScilabXgc; */

extern void newfec __PARAMS((integer *xm,integer *ym,double *triangles,double *func,integer *Nnode,
			     integer *Ntr,double *zminmax, integer *colminmax, integer *colout, BOOL with_mesh));
extern void GraySquare1(integer *x, integer *y, double *z, integer n1, integer n2);
extern void GraySquare1_NGreverse(integer *x, integer *y, double *z, integer n1, integer n2, sciPointObj * psubwin);

extern void GraySquare(integer *x, integer *y, double *z, integer n1, integer n2);
extern void GraySquareDirect(integer *x, integer *y, double *z, integer n1, integer n2); /* for NG, grayplot direct mode */
extern void GraySquareScaled(integer *x, integer *y, double *z, integer n1, integer n2); /* for NG, grayplot direct mode */
extern void Plo2d1RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf);
extern void Plo2d2RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void Plo2d3RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void Plo2d4RealToPixel __PARAMS((integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf));
extern void sciChamp2DRealToPixel __PARAMS((integer *xm,integer *ym,integer *zm,integer *na,
					 integer *arsize,double *x,double *y,
                                         double  *fx,double *fy,integer *n1,integer *n2,double *arfact, int * typeofchamp));

char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads);
double * AllocUserGrads(double * u_xgrads, int nb);
int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);
char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);

extern int cf_type; /* used by gcf to determine if current figure is a graphic (1) or a tksci (0) one */


/*******************************************************/

#ifdef WIN32
extern HFONT sciCreateFont (HDC hdc, char *szFaceName, int iDeciPtHeight, int iDeciPtWidth, int iAttributes, BOOL fLogRes);  /* DRAWINGS */
extern HDC sciGetDC (sciPointObj * pobj);     /* DRAWINGS */
/* extern HDC sciGetHDC (sciPointObj * pobj);    /\* DRAWINGS *\/ */ /* inexistant in Entities.c */
#endif


extern void sciRedrawFigure(); /* DRAWINGS */
extern void sciRedrawF(int *num); /* DRAWINGS */
extern void sciXbasc();  /* DRAWINGS */
extern void sciXclear(); /* DRAWINGS */
extern void sciXdraw();  /* DRAWINGS */

/* 3 next are in plo3d.c*/
extern void SetEch3d1(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha, integer flag); /* DRAWINGS */
extern void Convex_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox); /* DRAWINGS */
extern void DrawAxis(double *xbox, double *ybox, integer *Indices, integer style); /* DRAWINGS */

extern void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD); /* DRAWINGS */
extern void triedre(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD); /* DRAWINGS */
extern void Nextind(integer ind1, integer *ind2, integer *ind3); /* DRAWINGS */
extern int trans3d(sciPointObj *pobj,integer n,integer *xm,integer *ym,double *x, double *y,double *z); /* DRAWINGS */
extern int GradLog(double _min, double _max, double *_grads, int * n_grads);
extern BOOL Ishidden(sciPointObj *pobj); /* DRAWINGS */
extern BOOL IsDownAxes(sciPointObj *pobj); /* DRAWINGS */
extern void Plo2dTo3d(integer type, integer *n1, integer *n2, double *x, double *y, double *z, double *x1, double *y1, double *z1); /* DRAWINGS */
extern BOOL sci_update_frame_bounds_3d(sciPointObj *pobj);  /* DRAWINGS */
extern BOOL sci_update_frame_bounds_2d(sciPointObj *pobj);  /* DRAWINGS */

extern void rebuild_strflag( sciPointObj * psubwin, char * STRFLAG); /* DRAWINGS */
extern int labels2D_draw(sciPointObj * psubwin); /* DRAWINGS */

extern int ComputeC_format(sciPointObj * pobj, char * c_format); /* DRAWINGS */ /* inside matdes.c */
extern int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input); /* DRAWINGS */
extern int  ComputeCorrectXindAndInsideUD(double Teta,double Alpha, double * dbox, integer *xind, integer *InsideU, integer *InsideD); /* DRAWINGS */
extern int Axes3dStrings2(integer *ixbox, integer *iybox, integer *xind); /* DRAWINGS */
extern int AdaptGraduations(char xyz, sciPointObj * psubwin, double _minval, double _maxval, double fx, double fy, double fz); /* DRAWINGS */
extern int FindGrads(double *grads,int * n_grads); /* DRAWINGS */
extern int CheckIfiisNan(int j, int dim, int * tab); /* DRAWINGS */
extern int  BuildXYZvectForClipping_IfNanOrLogON(sciPointObj *ppolyline, sciPointObj * psubwin, int * nb_curves, 
						 double *** xvect, double *** yvect, double *** zvect, int ** curves_size); /* DRAWINGS */
extern int ComputeGoodTrans3d( sciPointObj * psubwin, int n, int *xm, int *ym, double * fx, double *fy, double *fz); /* DRAWINGS */
extern double InvAxis(double min, double max, double u); /* DRAWINGS */
extern int ReverseDataFor3D(sciPointObj * psubwin, double * xvect, double * yvect, double * zvect, int n1); /* DRAWINGS */
extern int ReverseDataFor3DXonly(sciPointObj * psubwin, double * xvect, int n1); /* DRAWINGS */
extern int ReverseDataFor3DYonly(sciPointObj * psubwin, double * yvect, int n1); /* DRAWINGS */
extern int ReverseDataFor3DZonly(sciPointObj * psubwin, double * zvect, int n1); /* DRAWINGS */
extern void DrawAxesIfRequired(sciPointObj*); /* DRAWINGS */
extern void DrawAxes(sciPointObj*); /* DRAWINGS */

extern void UpdateSubwinScale(sciPointObj * pobj); /* DRAWINGS */

extern void sciClip (sciPointObj *pobj); /* DRAWINGS */
extern void sciUnClip (sciPointObj *pobj); /* DRAWINGS */

extern int sciDrawObj (sciPointObj * pobj); /* DRAWINGS */
extern int sciDrawObjIfRequired (sciPointObj * pobj); /* DRAWINGS */


extern int ComputeXIntervals(sciPointObj *pobj, char xy_type, double **vector, int * N, int checkdim); /* DRAWINGS */ /* inside matdes.c */

/* Other functions coming from Entities.c (old file) */


/*F.Leray : Format pour imprimer un nombre de la forme k10^a ; cf. Axes.c*/
extern void NumberFormat __PARAMS((char *str,integer k,integer a));
extern void C2F(plot3dn)(sciPointObj *pobj, 
			 double *x, double *y, double *z, 
			 integer *p, integer *q, int *DPI);

extern void C2F(fac3dn)(sciPointObj *pobj, 
			double *x, double *y, double *z, 
			double *cvect, integer *p, integer *q, int *DPI);
/**DJ.Abdemouche 2003**/

extern int Gen3DPoints (integer type,integer *polyx,integer *polyy,integer *fill,integer whiteid,double zmin,double zmax,double *,double *,double *,integer i,integer j,integer jj1,integer *p,integer dc,integer fg); /* DRAWINGS */
extern void Merge3d(sciPointObj *psubwin);/*DJ.A merge*/   /* DRAWINGS */
extern void Merge3dBuildTable(sciPointObj *pparent, int *index_in_entity, long *from_entity, int *pos);
extern int Merge3dDimension(sciPointObj *pparent);
extern void DrawMerge3d(sciPointObj *psubwin, sciPointObj *pmerge, int *DPI);  /* DRAWINGS */

extern int DrawNewMarks(sciPointObj * pobj, int n1, int *xm, int *ym, int *DPI);
extern void DrawMarks3D(sciPointObj *pobj, int n1, int *xm, int *ym, int *DPI);
extern int CheckPixelStatus(void);

extern int DrawMark_FullDot(int x1, int yy1, double size, int foreground, int background, int pixel_offset);
extern int DrawMark_Plus(int xmi, int ymi, int size, int foreground, int pixel_offset);
extern int DrawMark_Cross(int xmi, int ymi, int size, int foreground, int pixel_offset);
extern int DrawMark_FullDiamond(int xmi, int ymi, int size, int foreground, int background);
extern int DrawMark_FullTriangleUp(int xmi, int ymi, int size, int foreground, int background);
extern int DrawMark_FullTriangleDown(int xmi, int ymi, int size, int foreground, int background);
extern int DrawMark_Asterisk(int xmi, int ymi, int size, int foreground, int pixel_offset);
extern int DrawMark_FullSquare(int xmi, int ymi, int size, int foreground, int background, int pixel_offset);
extern int DrawMark_FullTriangleRight(int xmi, int ymi, int size, int foreground, int background);
extern int DrawMark_FullTriangleLeft(int xmi, int ymi, int size, int foreground, int background);
extern int DrawMark_FullPentagram(int xmi, int ymi, int size, int foreground, int background);
extern int GetDPIFromDriver(int * DPI);
#endif /* __SCI_DRAWINGS__ */
