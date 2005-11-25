/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 *    Fabrice Leray,     INRIA 2004-xxxx
 *    Comment:
 *    This file contains all functions used to BUILD new objects : 
      - allocating memory
      - setting default value
      - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_BUILD__
#define __SCI_BUILD__

extern sciPointObj *ConstructStatusBar (sciPointObj * pparentfigure); /* BUILD */

extern sciPointObj *ConstructFigure ();  /* BUILD */
extern sciPointObj *ConstructSubWin (sciPointObj * pparentfigure,
				     int pwinnum); /* BUILD */
extern sciPointObj *ConstructScrollV (sciPointObj * pparentfigure); /* BUILD */
extern sciPointObj *ConstructScrollH (sciPointObj * pparentfigure); /* BUILD */

extern sciPointObj *ConstructText (sciPointObj * pparentsubwin, char text[], int n, double x,
				   double y, double *wh, int fill,int *foreground, int *background, 
				   BOOL isboxed, BOOL isline, BOOL isfilled); /* BUILD */
     
extern sciPointObj *ConstructTitle (sciPointObj * pparentfigure, char text[],
				    int type); /* BUILD */

extern sciPointObj *ConstructLegend (sciPointObj * pparentfigure, char text[],
				     int n, int nblegends, int *pstyle, sciPointObj **pptabofpointobj);  /* BUILD */

extern sciPointObj *ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
				       int closed, int n1, int n2,int plot, int *foreground, int *background,
				       int *mark_style, int *mark_foreground, int *mark_background,
				       BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded); /* BUILD */
extern sciPointObj *ConstructArc (sciPointObj * pparentsubwin, double x, double y,
				  double height, double width, double alphabegin, double alphaend, 
				  int *foreground, int *background, BOOL isfilled, BOOL isline);

extern sciPointObj *ConstructRectangle (sciPointObj * pparentsubwin, double x, double y,
					double height, double width, double horzcurvature,
					double vertcurvature,  int *foreground, int *background,
					int isfilled, int isline, int str, BOOL flagstring);

extern sciPointObj *ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
				      double * pvecx, double * pvecy, double * pvecz,
				      double *zcol, integer izcol, integer dimzx, integer dimzy, 
				      integer *flag, double *ebox, integer flagcolor, integer *isfac,
				      integer *m1, integer *n1, integer *m2, integer *n2, 
				      integer *m3, integer *n3, integer *m3n, integer *n3n); /* BUILD */


extern sciPointObj *ConstructMerge (sciPointObj * pparentsubwin,int N,int *index_in_entity,long *from_entity);  /* BUILD */

extern sciPointObj *ConstructGrayplot (sciPointObj * pparentfigure,double *vx,double *vy, 
                                   double *vz,int nx,int ny, int type); /* BUILD */

extern sciPointObj *ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx,
                                   int nx, double *vy, int ny, char *str[], int subint, char *format, 
                                   int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels);  /* BUILD */

extern sciPointObj *ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
				  double *pfun, int Nnode, int Ntr, double *zminmax, 
				  integer *colminmax, integer *colout, BOOL with_mesh); /* BUILD */

extern sciPointObj *ConstructSegs (sciPointObj * pparentsubwin, integer type,double *vx, double *vy, integer Nbr1, 
               integer Nbr2, double *vfx, double *vfy, integer flag, 
              integer *style, double arsize1,  integer colored, double arfact, int typeofchamp);  /* BUILD */

extern sciPointObj *ConstructCompound (long *tabpointobj, int number); /* BUILD */
extern sciPointObj *ConstructCompoundSeq (int number); /* BUILD */

extern sciPointObj * ConstructLabel (sciPointObj * pparentsubwin, char *text, int type); /* BUILD */


/* UNUSED : */

extern sciPointObj *ConstructMenu (sciPointObj * pparentfigure,
				      char plabel[], int n);

extern sciPointObj *ConstructMenuContext (sciPointObj * pparentfigure);

extern int sciAddLabelMenu (sciPointObj * pthis, char plabel[], int n);
extern int sciAttachPopMenu (sciPointObj *pthis, sciPointObj *pPopMenu);

#endif /* __SCI_BUILD__ */
