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

sciPointObj *ConstructStatusBar (sciPointObj * pparentfigure); /* BUILD */

sciPointObj * ConstructFigure (sciPointObj * pparent, int * figureIndex) ;/* BUILD */
sciPointObj *ConstructSubWin (sciPointObj * pparentfigure); /* BUILD */
sciPointObj *ConstructScrollV (sciPointObj * pparentfigure); /* BUILD */
sciPointObj *ConstructScrollH (sciPointObj * pparentfigure); /* BUILD */

sciPointObj * allocateText( sciPointObj        * pparentsubwin,
                                   char             ** text          ,
                                   int                 nbRow         ,
                                   int                 nbCol         ,
                                   double              x             ,
                                   double              y             ,
                                   BOOL                autoSize      ,
                                   double              userSize[2]   ,
                                   BOOL                centerPos     ,
                                   int               * foreground    ,
                                   int               * background    , 
                                   BOOL                isboxed       ,
                                   BOOL                isline        ,
                                   BOOL                isfilled      ,
                                   sciTextAlignment    align          ) ;

sciPointObj * ConstructText (sciPointObj * pparentsubwin, char ** text, int nbRow, int nbCol, double x,
                                    double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background, 
                                    BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align ) ;
     
sciPointObj *ConstructTitle (sciPointObj * pparentfigure, char text[],
				    int type); /* BUILD */

sciPointObj *ConstructLegend (sciPointObj * pparentfigure, char text[],
				     int n, int nblegends, int *pstyle, sciPointObj **pptabofpointobj);  /* BUILD */

sciPointObj * allocatePolyline(sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
                               int closed, int n1,int plot, int *foreground, int *background,
                               int *mark_style, int *mark_foreground, int *mark_background,
                               BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded);
sciPointObj *ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
				       int closed, int n1, int plot, int *foreground, int *background,
				       int *mark_style, int *mark_foreground, int *mark_background,
				       BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded); /* BUILD */
sciPointObj *ConstructArc (sciPointObj * pparentsubwin, double x, double y,
				  double height, double width, double alphabegin, double alphaend, 
				  int *foreground, int *background, BOOL isfilled, BOOL isline);

sciPointObj *ConstructRectangle (sciPointObj * pparentsubwin, double x, double y,
					double height, double width, double horzcurvature,
					double vertcurvature,  int *foreground, int *background,
					int isfilled, int isline, int str, BOOL flagstring);

sciPointObj *ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
				      double * pvecx, double * pvecy, double * pvecz,
				      double *zcol, integer izcol, integer dimzx, integer dimzy, 
				      integer *flag, double *ebox, integer flagcolor, integer *isfac,
				      integer *m1, integer *n1, integer *m2, integer *n2, 
				      integer *m3, integer *n3, integer *m3n, integer *n3n); /* BUILD */


sciPointObj *ConstructGrayplot (sciPointObj * pparentfigure,double *vx,double *vy, 
                                   double *vz,int nx,int ny, int type); /* BUILD */

sciPointObj *ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx,
                                   int nx, double *vy, int ny, char *str[], int subint, char *format, 
                                   int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels);  /* BUILD */

sciPointObj *ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
				  double *pfun, int Nnode, int Ntr, double *zminmax, 
				  integer *colminmax, integer *colout, BOOL with_mesh); /* BUILD */

sciPointObj *ConstructSegs (sciPointObj * pparentsubwin, integer type,double *vx, double *vy, integer Nbr1, 
               integer Nbr2, double *vfx, double *vfy, integer flag, 
              integer *style, double arsize1,  integer colored, double arfact, int typeofchamp);  /* BUILD */

sciPointObj *ConstructCompound (long *tabpointobj, int number); /* BUILD */
sciPointObj *ConstructCompoundSeq (int number); /* BUILD */

sciPointObj * ConstructLabel (sciPointObj * pparentsubwin, char *text, int type); /* BUILD */


/* UNUSED : */

sciPointObj *ConstructMenu (sciPointObj * pparentfigure,
				      char plabel[], int n);

sciPointObj *ConstructMenuContext (sciPointObj * pparentfigure);

int sciAddLabelMenu (sciPointObj * pthis, char plabel[], int n);
int sciAttachPopMenu (sciPointObj *pthis, sciPointObj *pPopMenu);

sciPointObj * ConstructUimenu (sciPointObj * pparent, char *label,char *callback,BOOL handle_visible) ;

sciPointObj * sciConstructConsole(     sciPointObj * pparent ) ;
sciPointObj * sciConstructFrame(       sciPointObj * pparent ) ;
sciPointObj * sciConstructWindow(      sciPointObj * pparent ) ;
sciPointObj * sciConstructWindowFrame( sciPointObj * pparent ) ;
sciPointObj * sciConstructScreen(      sciPointObj * pparent ) ;

sciPointObj * sciStandardBuildOperations( sciPointObj * pObj, sciPointObj * parent ) ;

void SciWin(void);

#endif /* __SCI_BUILD__ */
