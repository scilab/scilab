/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philippe Chancelier
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

#include <string.h> /* in case of dbmalloc use */
#include <math.h>
#include <stdio.h>
#include "math_graphics.h"
#include "PloEch.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "Plo3d.h"
#include "axesScale.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "Format.h"
#include "CurrentObjectsManagement.h"

#include "MALLOC.h" /* MALLOC */


extern char GetDriver(void);
extern int Check3DPlots(char *, integer *);
/** PGEOX and PGEOY are like GEOX or GEOY in PloEch.h but we keep values in xx1 and yy1 for finite check **/
#define PGEOX(x1,y1,z1) inint(xx1= Cscale.Wscx1*(TRX(x1,y1,z1)-Cscale.frect[0]) +Cscale.Wxofset1);
#define PGEOY(x1,y1,z1) inint(yy1= Cscale.Wscy1*(-TRY(x1,y1,z1)+Cscale.frect[3])+Cscale.Wyofset1);

/*-------------------------------------------------------------------
 * functions for 3D scales 
 *-------------------------------------------------------------------*/

/* 
 * if flag==1,2,3  m and bbox and Cscale are  recomputed  
 * if flag==0      we only change m without changing scales 
 */

void SetEch3d1(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha, integer flag)
{
  double xmmin = 0 ;
  double ymmax = 0 ;
  double xmmax = 0 ;
  double ymmin = 0 ;
  double FRect[4],WRect[4],ARect[4];
  integer ib;
  static integer aaint[]={2,10,2,10};
  int wdim[2];
  char logf_[2];
  double R,xo,yo,zo,dx,dy,dz,hx,hy,hx1,hy1,Teta,Alpha;
  integer wmax = 0, hmax = 0 ;
  static double cost=0.5,sint=0.5,cosa=0.5,sina=0.5;
  Teta=*teta;
  Alpha=*alpha;
  
  Cscale.alpha = Alpha;
  Cscale.theta = Teta;
  cost=cos((Teta)*M_PI/180.0);
  sint=sin((Teta)*M_PI/180.0);
  cosa=cos((Alpha)*M_PI/180.0);
  sina=sin((Alpha)*M_PI/180.0);
  Cscale.m[0][0]= -sint    ;    Cscale.m[0][1]= cost      ;    Cscale.m[0][2]= 0;
  Cscale.m[1][0]= -cost*cosa;   Cscale.m[1][1]= -sint*cosa;    Cscale.m[1][2]= sina;
  Cscale.m[2][0]=  cost*sina;   Cscale.m[2][1]= sint*sina;     Cscale.m[2][2]= cosa;
  /* Coordonn\'ees apr\`es transformation g\'eometrique de la
   * boite qui entoure le plot3d                            
   * le plan de projection est defini par x et y            
   */
  for (ib=0;ib<6 ;ib++) 
    { 
      if (flag==0) 
	bbox[ib]=Cscale.bbox1[ib];
      else 
	Cscale.bbox1[ib]=bbox[ib];
    }
  xbox[0]=TRX(bbox[0],bbox[2],bbox[4]);
  ybox[0]=TRY(bbox[0],bbox[2],bbox[4]);
  zbox[0]=TRZ(bbox[0],bbox[2],bbox[4]);
  xbox[1]=TRX(bbox[0],bbox[3],bbox[4]);
  ybox[1]=TRY(bbox[0],bbox[3],bbox[4]);
  zbox[1]=TRZ(bbox[0],bbox[3],bbox[4]);
  xbox[2]=TRX(bbox[1],bbox[3],bbox[4]);
  ybox[2]=TRY(bbox[1],bbox[3],bbox[4]);
  zbox[2]=TRZ(bbox[1],bbox[3],bbox[4]);
  xbox[3]=TRX(bbox[1],bbox[2],bbox[4]);
  ybox[3]=TRY(bbox[1],bbox[2],bbox[4]);
  zbox[3]=TRZ(bbox[1],bbox[2],bbox[4]);
  xbox[4]=TRX(bbox[0],bbox[2],bbox[5]);
  ybox[4]=TRY(bbox[0],bbox[2],bbox[5]);
  zbox[4]=TRZ(bbox[0],bbox[2],bbox[5]);
  xbox[5]=TRX(bbox[0],bbox[3],bbox[5]);
  ybox[5]=TRY(bbox[0],bbox[3],bbox[5]);
  zbox[5]=TRZ(bbox[0],bbox[3],bbox[5]);
  xbox[6]=TRX(bbox[1],bbox[3],bbox[5]);
  ybox[6]=TRY(bbox[1],bbox[3],bbox[5]);
  zbox[6]=TRZ(bbox[1],bbox[3],bbox[5]);
  xbox[7]=TRX(bbox[1],bbox[2],bbox[5]);
  ybox[7]=TRY(bbox[1],bbox[2],bbox[5]);
  zbox[7]=TRZ(bbox[1],bbox[2],bbox[5]);
  /** Calcul des echelles en fonction de la taille du dessin **/
  if ( flag == 1 || flag == 3 )
    {
      xmmin=  (double) Mini(xbox,8L);xmmax= (double) Maxi(xbox,8L);
      ymmax=  (double) - Mini(ybox,8L);
      ymmin=  (double) - Maxi(ybox,8L);
    }
  /* code added by es: isoview scaling */
  if ( flag == 2 || flag == 3 )
    {
      /* get current window size */
      wdim[0] = sciGetWindowWidth(sciGetCurrentFigure());
      wdim[1] = sciGetWindowHeight(sciGetCurrentFigure());
      getscale2d(WRect,FRect,logf_,ARect);
      wmax=linint((double)wdim[0] * WRect[2]);
      hmax=linint((double)wdim[1] * WRect[3]); 
    }
  if ( flag == 2 )
    {
      /* radius and center of the sphere circumscribing the box */
      dx=bbox[1]-bbox[0]; dy=bbox[3]-bbox[2]; dz=bbox[5]-bbox[4];
      R= (double) sqrt(dx*dx + dy*dy + dz*dz)/2;
      xo= (double) (xbox[0]+xbox[6])/2 ;
      yo= (double) (ybox[0]+ybox[6])/2 ;
      zo= (double) (zbox[0]+zbox[6])/2 ;
      xmmin=  (double) xo - R ;
      xmmax=  (double) xo + R ;
      ymmax=  (double) -yo + R ;
      ymmin=  (double) -yo - R ;
    }
  if (flag==2 || flag==3)
    {
      /* adjust limits (code adapted from Plo2d.c & Stephane's patch) */
      hx=xmmax-xmmin;
      hy=ymmax-ymmin;
      if ( hx/(double)wmax  < hy/(double)hmax ) 
        {
          hx1=wmax*hy/hmax;
          xmmin=xmmin-(hx1-hx)/2.0;
          xmmax=xmmax+(hx1-hx)/2.0;
        }
      else 
        {
          hy1=hmax*hx/wmax;
          ymmin=ymmin-(hy1-hy)/2.0;
          ymmax=ymmax+(hy1-hy)/2.0;
        }
    }
  if (flag !=0 )
    {
      FRect[0]=xmmin;FRect[1]= -ymmax;FRect[2]=xmmax;FRect[3]= -ymmin;
      set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
      Cscale.metric3d=flag; /* the metric mode is stored into the
                             * List of Scales */
    }
  /* end of code added by es */
}

/** Returns the [x,y,z] values of a pointeger given its xbox or ybox indices **/

void BBoxToval(double *x, double *y, double *z, integer ind, double bbox[6] )
{
  switch ( ind)
    {
    case 0:	*x=bbox[0],*y=bbox[2],*z=bbox[4];break;
    case 1:	*x=bbox[0],*y=bbox[3],*z=bbox[4];break;
    case 2:	*x=bbox[1],*y=bbox[3],*z=bbox[4];break;
    case 3:	*x=bbox[1],*y=bbox[2],*z=bbox[4];break;
    case 4:	*x=bbox[0],*y=bbox[2],*z=bbox[5];break;
    case 5:	*x=bbox[0],*y=bbox[3],*z=bbox[5];break;
    case 6:	*x=bbox[1],*y=bbox[3],*z=bbox[5];break;
    case 7:	*x=bbox[1],*y=bbox[2],*z=bbox[5];break;
    }
}

/*---------------------------------------------------------------------------------
 *This function sorts the vertices such that the color value is in decreasing order
 *---------------------------------------------------------------------------------*/

int  triangleSort(integer *polyxin, integer *polyyin, integer *fillin, integer *polyx, integer *polyy, integer *fill)
{ 
  integer tmp,k;
  for (k=0;k<3;k++) {polyx[k]=polyxin[k]; polyy[k]=polyyin[k]; fill[k]=Abs(fillin[k]);}
      
  if (fill[0]<fill[1]) {  
    tmp=fill[0]; fill[0]=fill[1]; fill[1]=tmp;
    tmp=polyx[0]; polyx[0]=polyx[1]; polyx[1]=tmp;
    tmp=polyy[0]; polyy[0]=polyy[1]; polyy[1]=tmp;
  }
  if (fill[0]<fill[2]) {  
    tmp=fill[0]; fill[0]=fill[2]; fill[2]=tmp;
    tmp=polyx[0]; polyx[0]=polyx[2]; polyx[2]=tmp;
    tmp=polyy[0]; polyy[0]=polyy[2]; polyy[2]=tmp;
  }
  if (fill[1]<fill[2]) {  
    tmp=fill[1]; fill[1]=fill[2]; fill[2]=tmp;
    tmp=polyx[1]; polyx[1]=polyx[2]; polyx[2]=tmp;
    tmp=polyy[1]; polyy[1]=polyy[2]; polyy[2]=tmp;
  }
  return 0;
}
     


