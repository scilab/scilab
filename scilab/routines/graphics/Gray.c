/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Math.h"
#include "PloEch.h"

static void GraySquare __PARAMS((integer *x,integer *y,double *z,
				integer n1,integer n2));
static void GraySquare1 __PARAMS((integer *x,integer *y,double *z,
				 integer n1,integer n2));

/*------------------------------------------------------------
 * - z is a (n1,n2) matrix 
 * - x is a (1,n1) matrix 
 * - y is a (1,n2) matrix 
 * - x,y,z are stored as one dimensionnal array in C 
 *
 *  z : is the value of a function on the grid defined by x,y 
 *  on each rectangle the average value of z is computed 
 *  and [zmin,zmax] is linearly remapped to the [colormin,colormap]
 *  values of colors in the current colormap 
 *  the color associated to zmoy is used for filling a specific rectangle 
 *---------------------------------------------------------------*/

int C2F(xgray)(x,y,z,n1,n2,strflag,brect,aaint,l1)
     double x[],y[],z[],brect[];
     integer *n1,*n2,aaint[];
     long int l1;
     char strflag[];
{
  int N = Max((*n1),(*n2));
  double xx[2],yy[2];
  integer *xm,*ym, j, nn1=1,nn2=2;
  /** If Record is on **/

  xx[0]=Mini(x,*n1);xx[1]=Maxi(x,*n1);
  yy[0]=Mini(y,*n2);yy[1]=Maxi(y,*n2);
  /** Boundaries of the frame **/
  update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);
  /** Allocation **/

  if (GetDriver()=='R') 
    StoreGray("gray",x,y,z,n1,n2,strflag,brect,aaint);


  xm = graphic_alloc(0,N,sizeof(int));
  ym = graphic_alloc(1,N,sizeof(int));
  if ( xm == 0 || ym == 0) 
    {
      sciprint("Running out of memory \n");
      return 0;
    }      

  /** Draw Axis or only rectangle **/
  axis_draw(strflag);
  /** Drawing the curves **/
  frame_clip_on();
  for ( j =0 ; j < (*n1) ; j++)	 xm[j]= XScale(x[j]);
  for ( j =0 ; j < (*n2) ; j++)	 ym[j]= YScale(y[j]); 
  GraySquare(xm,ym,z,*n1,*n2);
  frame_clip_off();
  C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(0);
}


static void GraySquare_base(x, y, z, n1, n2)
     integer x[],y[];
     double *z;
     integer n1,n2;
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill[1],cpat,xz[2];
  zmin=Mini(z,(n1)*(n2));
  zmax=Maxi(z,(n1)*(n2));
  zmaxmin=zmax-zmin;
  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);
	fill[0]=1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin));
	C2F(dr)("xset","pattern",&(fill[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        w=Abs(x[i+1]-x[i]);h=Abs(y[j+1]-y[j]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[i] < xz[0] && y[j+1] < xz[1] && x[i]+w > 0 && y[j+1]+h > 0 )
	  {
	    if ( Abs(x[i]) < int16max && Abs(y[j+1]) < int16max && w < uns16max && h < uns16max)
	      {
		/* fprintf(stderr,"Rectangle %d,%d : %d,%d,%d,%d\n",i,j,x[i],y[j+1],w,h); */
		C2F(dr)("xfrect","v",&x[i],&y[j+1],&w,&h,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    else 
	      {
		/* fprintf(stderr,"Rectangle too large \n"); */
	      }
	  }
      }
  C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}


static void GraySquare(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  if ( GetDriverId() == 0 ) 
    /** accelerated version for X11 or Windows **/
    fill_grid_rectangles(x, y, z, n1, n2);
  else 
    GraySquare_base(x, y, z, n1, n2);
}




/*-------------------------------------------------------
 *  z : of size n1*n2 
 *  the z value is interpreted as a color number inside the current colormap
 *  z[i,j] is used as the color of a square [i-0.5,i+0.5] [j-0.5,j+0.5]
 *-------------------------------------------------------*/

int C2F(xgray1)(z,n1,n2,strflag,brect,aaint,l1)
     double z[],brect[];
     integer *n1,*n2,aaint[];
     long int l1;
     char strflag[];
{
  int N = Max((*n1+1),(*n2+1));
  double xx[2],yy[2];
  static integer *xm,*ym,j, nn1=1,nn2=2;
  xx[0]=0.5;xx[1]= *n2+0.5;
  yy[0]=0.5;yy[1]= *n1+0.5;
  /** Boundaries of the frame **/
  update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);

  /** If Record is on **/
  if (GetDriver()=='R') 
    StoreGray1("gray1",z,n1,n2,strflag,brect,aaint);

  /** Allocation **/
  xm = graphic_alloc(0,N,sizeof(int));
  ym = graphic_alloc(1,N,sizeof(int));
  if ( xm == 0 || ym == 0) 
    {
      sciprint("Running out of memory \n");
      return 0;
    }      

  /** Draw Axis or only rectangle **/
  axis_draw(strflag);
  /** Drawing the curves **/
  frame_clip_on();
  for ( j =0 ; j < (*n2+1) ; j++) xm[j]= XScale(j+0.5);
  for ( j =0 ; j < (*n1+1) ; j++) ym[j]= YScale(((*n1)-j+0.5));
  GraySquare1(xm,ym,z,*n1+1,*n2+1);
  frame_clip_off();
  C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(0);
}

/*-------------------------------------------------------
 * like xgray1 : 
 * but xrect here give the rectangle in which the 
 * grayplot is to be drawn using the current scale
 -------------------------------------------------------*/

int C2F(xgray2)(z,n1,n2,xrect)
     double z[],xrect[];
     integer *n1,*n2;
{
  double xx[2],yy[2];
  integer xx1[2],yy1[2],nn1=1,nn2=2;
  integer *xm,*ym,  j;
  /** If Record is on **/
  if (GetDriver()=='R') 
    StoreGray2("gray2",z,n1,n2,xrect);
  xx[0]=xrect[0];xx[1]=xrect[2];
  yy[0]=xrect[1];yy[1]=xrect[3];
  /** Boundaries of the frame **/
  C2F(echelle2d)(xx,yy,xx1,yy1,&nn1,&nn2,"f2i",3L);  
  xm = graphic_alloc(0,*n2,sizeof(int));
  ym = graphic_alloc(1,*n1,sizeof(int));
  if ( xm == 0 || ym == 0 )
    {
      Scistring("Xgray: running out of memory\n");
      return 0;
    }
  for ( j =0 ; j < (*n2+1) ; j++)	 
    xm[j]= (int) (( xx1[1]*j + xx1[0]*((*n2)-j) )/((double) *n2));
  
  for ( j =0 ; j < (*n1+1) ; j++)	 
    ym[j]= (int) (( yy1[0]*j + yy1[1]*((*n1)-j) )/((double) *n1));
  frame_clip_on();
  GraySquare1(xm,ym,z,*n1+1,*n2+1);
  frame_clip_off();
  return(0);
}


/*-------------------------------------------------------
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : of size (n1-1)*(n2-1)  gives the f-values on the middle 
 *  of each rectangle. 
 *  z[i,j] is the value on the middle of rectangle 
 *        P1= x[i],y[j] x[i+1],y[j+1]
 *-------------------------------------------------------*/

static void GraySquare1_base(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  integer i,j,verbose=0,narg,fill[1],cpat,xz[2];
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	fill[0]= z[i+(n1-1)*j];
	C2F(dr)("xset","pattern",&(fill[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
	w=Abs(x[j+1]-x[j]);
	h=Abs(y[i+1]-y[i]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[j] < xz[0] && y[i] < xz[1] && x[j]+w > 0 && y[i]+h > 0 )
	  if ( Abs(x[j]) < int16max && Abs(y[i+1]) < int16max && w < uns16max && h < uns16max)
	    C2F(dr)("xfrect","v",&x[j],&y[i],&w,&h,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
  C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

static void GraySquare1(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  if ( GetDriverId() == 0 ) 
    /** accelerated version for X11 or Windows **/
    fill_grid_rectangles1(x, y, z, n1, n2);
  else 
    GraySquare1_base(x, y, z, n1, n2);
}







