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
 *    This file contains all functions used to BUILD new objects : 
      - allocating memory
      - setting default value
      - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_BUILD__
#define __SCI_BUILD__

#include "BOOL.h"


sciPointObj * ConstructFigure (sciPointObj * pparent, int * figureIndex) ;/* BUILD */
sciPointObj *ConstructSubWin (sciPointObj * pparentfigure); /* BUILD */

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
    

sciPointObj *ConstructLegend (sciPointObj * pparentfigure, char *text[], long long tabofhandles[],
				     int nblegends);  /* BUILD */

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
					double height, double width,  int *foreground, int *background,
					int isfilled, int isline);

sciPointObj *ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
				      double * pvecx, double * pvecy, double * pvecz,
				      double *zcol, int izcol, int dimzx, int dimzy, 
				      int *flag, double *ebox, int flagcolor, int *isfac,
				      int *m1, int *n1, int *m2, int *n2, 
				      int *m3, int *n3, int *m3n, int *n3n); /* BUILD */


sciPointObj *ConstructGrayplot (sciPointObj * pparentfigure,double *vx,double *vy, 
                                   double *vz,int nx,int ny, int type); /* BUILD */

sciPointObj *ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx,
                                   int nx, double *vy, int ny, char *str[], int subint, char *format, 
                                   int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels);  /* BUILD */

sciPointObj *ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
				  double *pfun, int Nnode, int Ntr, double *zminmax, 
				  int *colminmax, int *colout, BOOL with_mesh); /* BUILD */

sciPointObj *ConstructSegs (sciPointObj * pparentsubwin, int type,double *vx, double *vy, int Nbr1, 
               int Nbr2, double *vfx, double *vfy, int flag, 
              int *style, double arsize1,  int colored, double arfact, int typeofchamp);  /* BUILD */

sciPointObj *ConstructCompound (long *tabpointobj, int number); /* BUILD */
sciPointObj *ConstructCompoundSeq (int number); /* BUILD */

sciPointObj * ConstructLabel (sciPointObj * pparentsubwin, char *text, int type); /* BUILD */

sciPointObj * sciStandardBuildOperations( sciPointObj * pObj, sciPointObj * parent ) ;

void SciWin(void);

sciPointObj * createFullFigure(int * winNum);

sciPointObj * createFirstSubwin(sciPointObj * pFigure);

void createDefaultRelationShip(sciPointObj * pObj);

void initUserData(sciPointObj * pObj);


#endif /* __SCI_BUILD__ */
