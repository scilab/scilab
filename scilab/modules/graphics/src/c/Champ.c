/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philipp Chancelier
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
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
 --------------------------------------------------------------------------*/

#include "math_graphics.h"
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"


#include "BuildObjects.h"
#include "Axes.h"
#include "Champ.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Plot2d.h"
#include "Scierror.h"

/*-----------------------------------------------------------------
 *  int C2F(champ)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *  int C2F(champ1)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *
 * Used for Drawing 2 dimensional vector fields 
 * (fx[i+(*n1)*j], fy[i+(*n1)*j]) is the value of the vector field 
 * at point X=(x[i],y[j]);
 * 
 * - fx and fy are (*n1)*(*n2) matrix of double
 * - arfact : a factor by which to multiply the default arrow size 
 *          usr 1.0 by defaut 
 * - strflag : a string of length 3 (see plot2d) 
 * - brect=[xmin,ymin,xmax,ymax]    (see plot2d) 
 *
 * - lstr : (used when called from Fortran code)
 -------------------------------------------------------------------*/

void champg(char *name, int colored, double *x, double *y, double *fx, double *fy, int *n1, 
	    int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
  int n;
  double  xx[2],yy[2];
  double boundingBox[6];
  int nn1=1,nn2=2;  
  /* NG */
  sciPointObj  * psubwin = NULL;
  sciPointObj * newSegs = NULL;
  int flag,type =1;
  double arsize1;
  int *style;
  int i;
  double drect[6];
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;
  /* champ with color inheritated from subwin */
  /* or champ1 (normed vector + color) is enabled */
  int typeofchamp = ( colored == 0 ? 0 : 1 ) ;

  /** The arrowsize acording to the windowsize **/
  n=2*(*n1)*((*n2)+1); /*F.Leray 17.02.04*/
  
  
  /* First create champ object */
  /* F.Leray Allocation de style[dim = Nbr1] */
  if ((style = MALLOC ((*n1) * sizeof (int) )) == NULL)
  {
    Scierror(999, _("%s: No more memory.\n"),"champg");
    return;
  }

  flag = 1; /* je le mets à 1 pour voir F.Leray 19.02.04*/
  arsize1 = *arfact;

  psubwin = sciGetCurrentSubWin() ;

  /* then modify subwindow if needed */
  checkRedrawing() ;

  /* force clipping */
  sciSetIsClipping(psubwin, 0);

  for(i=0;i<(*n1);i++) { style[i] = i ; }

  newSegs = ConstructSegs(psubwin,type,x,y, NULL,*n1,*n2,0,fx,fy,flag,
                          style,arsize1,colored,typeofchamp);
  sciSetCurrentObj(newSegs);

  if( style != NULL )
  {
    FREE( style ) ;
    style = NULL;
  }

  sciSetIsClipping(newSegs, 0);

  /* Get segs bounding box */
  forceRedraw(newSegs); /* update drawer */
  sciGetAABoundingBox(newSegs, boundingBox);
  xx[0] = boundingBox[0];
  xx[1] = boundingBox[1];
  yy[0] = boundingBox[2];
  yy[1] = boundingBox[3];

  /* Force psubwin->is3d to FALSE: we are in 2D mode */
  if (sciGetSurface(psubwin) == (sciPointObj *) NULL)
  {
    pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
    pSUBWIN_FEATURE (psubwin)->project[2]= 0;
  } 
  else
  {
    pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
    pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;  
  }

  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;


  if (sciGetGraphicMode (psubwin)->autoscaling)
  {
    /* compute and merge new specified bounds with psubwin->Srect */
    switch (strflag[1])  {
        case '0': 
          /* do not change psubwin->Srect */
          break;
        case '1' : case '3' : case '5' : case '7':
          /* Force psubwin->Srect=brect */
          re_index_brect(brect,drect);
          break;
        case '2' : case '4' : case '6' : case '8': case '9':
          /* Force psubwin->Srect to the x and y bounds */
          compute_data_bounds2(0,'g',pSUBWIN_FEATURE (psubwin)->logflags,xx,yy,nn1,nn2,drect);
          break;
    }
    if (!pSUBWIN_FEATURE(psubwin)->FirstPlot &&
      (strflag[1] == '7' || strflag[1] == '8'|| strflag[1] == '9')) { /* merge psubwin->Srect and drect */
        drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
        drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
        drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
        drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
    }

    if (strflag[1] != '0')
    {
      bounds_changed = update_specification_bounds(psubwin, drect,2);
    }
  }

  if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;

  axes_properties_changed = strflag2axes_properties(psubwin, strflag);

  pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */

  if( bounds_changed || axes_properties_changed )
  {
    forceRedraw(psubwin);
  }


  sciDrawObj(sciGetCurrentFigure());

  
}

int C2F(champ)(double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
  champg("champ",0,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0); 
}

int C2F(champ1)(double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr)
{
  champg("champ1",1,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0);
}
/*----------------------------------------------------------------------------------*/

