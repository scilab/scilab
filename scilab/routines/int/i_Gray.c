/*------------------------------------------------------------------------
    Graphic library for 2D and 3D plotting 
    Copyright (C) 1998 Chancelier Jean-Philippe
    jpc@cergrene.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Math.h"

static void GraySquare();
static void GraySquare1();
/*------------------------------------------------------------
 - z is a (n1,n2) matrix 
 - x is a (1,n1) matrix 
 - y is a (1,n2) matrix 
 - x,y,z are stored as one dimensionnal array in C 

  z : is the value of a function on the grid defined by x,y 
  on each rectangle the average value of z is computed 
  and [zmin,zmax] is linearly remapped to the [colormin,colormap]
  values of colors in the current colormap 
  the color associated to zmoy is used for filling a specific rectangle 
---------------------------------------------------------------*/

int C2F(i_xgray)(x,y,z,n1,n2,strflag,brect,aaint,l1)
     double x[],y[],z[],brect[];
     integer *n1,*n2,aaint[];
     long int l1;
     char strflag[];
{
  double FRect[4],scx,scy,xofset,yofset;
  static char logflag[]="nn";
  integer IRect[4],Xdec[3],Ydec[3];
  double xx[2],yy[2];
  static integer *xm,*ym,err=0;
  integer j,job=1;
  integer nn1=1,nn2=2;
  /** If Record is on **/
  if (GetDriver()=='R') 
    i_StoreGray("gray",x,y,z,n1,n2,strflag,brect,aaint);
  xx[0]=Mini(x,*n1);xx[1]=Maxi(x,*n1);
  yy[0]=Mini(y,*n2);yy[1]=Maxi(y,*n2);
  /** Boundaries of the frame **/
  FrameBounds("gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect,FRect,Xdec,Ydec);
  if ( (int)strlen(strflag) >=2 && strflag[1]=='0') job=0;
  Scale2D(job,FRect,IRect,aaint,&scx,&scy,&xofset,&yofset,logflag,&xm,&ym,Max((*n1),(*n2)),&err);
  if ( err == 0) return(0);
  /** Draw Axis or only rectangle **/
  AxisDraw(FRect,IRect,Xdec,Ydec,aaint,scx,scy,xofset,yofset,strflag,logflag);
  /** Drawing the curves **/
  C2F(dr)("xset","clipping",&IRect[0],&IRect[1],&IRect[2],&IRect[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for ( j =0 ; j < (*n1) ; j++)	 
    xm[j]= (int)(scx*(x[j]-FRect[0])  +xofset);
  for ( j =0 ; j < (*n2) ; j++)	 
    ym[j]= (int)(scy*(-y[j]+FRect[3]) +yofset);
  i_GraySquare(xm,ym,z,*n1,*n2);
  C2F(dr)("xset","clipoff",PI0,PI0,PI0,PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xrect","v",&IRect[0],&IRect[1],&IRect[2],&IRect[3]
      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(0);
}


static void i_GraySquare(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill[1],cpat,xz[2];
  zmin=Mini(z,(n1)*(n2));
  zmax=Maxi(z,(n1)*(n2));
  zmaxmin=zmax-zmin;
  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,
      PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

/*-------------------------------------------------------
  z : of size n1*n2 
  the z value is interpreted as a color number inside the current colormap
  z[i,j] is used as the color of a square [i-0.5,i+0.5] [j-0.5,j+0.5]
-------------------------------------------------------*/


int C2F(i_xgray1)(z,n1,n2,strflag,brect,aaint,l1)
     double z[],brect[];
     integer *n1,*n2,aaint[];
     long int l1;
     char strflag[];
{
  double FRect[4],scx,scy,xofset,yofset;
  static char logflag[]="nn";
  integer IRect[4],Xdec[3],Ydec[3];
  double xx[2],yy[2];
  static integer *xm,*ym,err=0;
  integer j,job=1;
  integer nn1=1,nn2=2;
  /** If Record is on **/
  if (GetDriver()=='R') 
    i_StoreGray1("gray1",z,n1,n2,strflag,brect,aaint);
  xx[0]=0.5;xx[1]= *n2+0.5;
  yy[0]=0.5;yy[1]= *n1+0.5;
  /** Boundaries of the frame **/
  FrameBounds("gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect,FRect,Xdec,Ydec);
  if ( (int)strlen(strflag) >=2 && strflag[1]=='0') job=0;
  Scale2D(job,FRect,IRect,aaint,&scx,&scy,&xofset,&yofset,logflag,&xm,&ym,Max((*n1+1),(*n2+1)),&err);
  if ( err == 0) return(0);
  /** Draw Axis or only rectangle **/
  AxisDraw(FRect,IRect,Xdec,Ydec,aaint,scx,scy,xofset,yofset,strflag,logflag);
  /** Drawing the curves **/
  C2F(dr)("xset","clipping",&IRect[0],&IRect[1],&IRect[2],&IRect[3],PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for ( j =0 ; j < (*n2+1) ; j++)	 
    xm[j]= (int)(scx*(j+0.5 -FRect[0])  +xofset);
  for ( j =0 ; j < (*n1+1) ; j++)	 
    ym[j]= (int)(scy*(-((*n1)-j+0.5+1)+FRect[3]) +yofset);
  i_GraySquare1(xm,ym,z,*n1+1,*n2+1);
  C2F(dr)("xset","clipoff",PI0,PI0,PI0,PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xrect","v",&IRect[0],&IRect[1],&IRect[2],&IRect[3]
      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(0);
}


/*-------------------------------------------------------
  x : of size n1 gives the x-values of the grid 
  y : of size n2 gives the y-values of the grid 
  z : of size (n1-1)*(n2-1)  gives the f-values on the middle 
  of each rectangle. 
  z[i,j] is the value on the middle of rectangle 
        P1= x[i],y[j] x[i+1],y[j+1]
-------------------------------------------------------*/

static void i_GraySquare1(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill[1],cpat,xz[2];
  zmin=Mini(z,(n1-1)*(n2-1));
  zmax=Maxi(z,(n1-1)*(n2-1));
  zmaxmin=zmax-zmin;
  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,
      PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for (i = 0 ; i < (n1)-1 ; i++)
  for (j = 0 ; j < (n2)-1 ; j++)
    {
      integer w,h;
      fill[0]=Max(Min(z[i+(n1-1)*j],whiteid),0); 
      C2F(dr)("xset","pattern",&(fill[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      w=Abs(x[j+1]-x[j]);h=Abs(y[i+1]-y[i]);
      /* We don't trace rectangle which are totally out **/
      if ( w != 0 && h != 0 && x[j] < xz[0] && y[i+1] < xz[1] && x[j]+w > 0 && y[i+1]+h > 0 )
	{
	  if ( Abs(x[j]) < int16max && Abs(y[i+1]) < int16max && w < uns16max && h < uns16max)
	    {
	      /* fprintf(stderr,"Rectangle %d,%d : %d,%d,%d,%d\n",i,j,x[i],y[j+1],w,h); */
	      C2F(dr)("xfrect","v",&x[j],&y[i+1],&w,&h,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  else 
	    {
	      /* fprintf(stderr,"Rectangle too large \n"); */
	    }
	}
    }
  C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

/*---------------------------------------------------------------------
\encadre{Le cas des niveaux de gris }
---------------------------------------------------------------------------*/


/* Store the plot in the plot list 
   same arguments as gray */

void i_StoreGray(name, x, y, z, n1, n2, strflag, brect, aaint)
     char *name;
     double *x;
     double *y;
     double *z;
     integer *n1;
     integer *n2;
     char *strflag;
     double *brect;
     integer *aaint;
{
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*n1) &&
	  CopyVectF(&(lplot->y), y,*n2) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aaint),aaint,4)  &&
	  CopyVectLI(&(lplot->aaint_kp),aaint,4) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storegray): No more place \n");
}

/** For matrices  z(i,j) **/

void i_StoreGray1(name,  z, n1, n2, strflag, brect, aaint)
     char *name;
     double *z;
     integer *n1;
     integer *n2;
     char *strflag;
     double *brect;
     integer *aaint;
{
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      lplot->x = NULL;
      lplot->y = NULL;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aaint),aaint,4)  &&
	  CopyVectLI(&(lplot->aaint_kp),aaint,4) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storegray): No more place \n");
}


/*---------------------------------------------------------------------
\encadre{Routines de recopie de Vecteurs avec alocation dynamique}
  a appeller avec l > 0 
---------------------------------------------------------------------------*/

int i_CopyVectI(nx, x, l)
     int **nx;
     int *x;
     integer l;
{ 
  int i;
  if ( x != ( int *) 0) 
    {
      *nx = (int *)  MALLOC(l*sizeof(int));
      if ( *nx == NULL) return(0);
      for ( i=0 ; i < l ; i++) (*nx)[i]= x[i];
    }
  return(1);
}
