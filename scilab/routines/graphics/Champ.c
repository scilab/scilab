/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Math.h"
#include "PloEch.h"

static double MiniD __PARAMS((double *x,integer n));

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

void champg(name, colored, x, y, fx, fy, n1, n2, strflag, brect, arfact, lstr)
     char *name;
     integer colored;
     double x[],y[],fx[],fy[];
     integer *n1,*n2;
     char *strflag;
     double *brect;
     double *arfact;
     integer lstr;
{
  static integer aaint[]={2,10,2,10};
  integer *xm,*ym,*zm,i,j,n,na;
  double  xx[2],yy[2], maxx;
  double  nx,ny,sc,sfx,sfy,sfx2,sfy2;
  double  arsize1=0.5,arsize2=0.5;
  integer arsize,nn1=1,nn2=2,iflag=0;
  /* get default dash fo rarrows **/
  integer verbose=0,narg,xz[10],uc;

  C2F(dr)("xget","use color",&verbose, &uc, &narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (uc)
    C2F(dr)("xget","color",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  else
    C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /** The arrowsize acording to the windowsize **/
  n=2*(*n1)*(*n2);
  xx[0]=x[0];xx[1]=x[*n1-1];
  yy[0]=y[0];yy[1]=y[*n2-1];
  /** Boundaries of the frame **/

  update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);
  /* Storing values if using the Record driver */
  if (GetDriver()=='R') 
    StoreChamp(name,x,y,fx,fy,n1,n2,strflag,brect,arfact);

  /** Allocation **/
  xm = graphic_alloc(0,n,sizeof(int));
  ym = graphic_alloc(1,n,sizeof(int));
  if ( xm == 0 || ym == 0) 
    {
      sciprint("Running out of memory \n");
      return ;
    }      
  if ( colored != 0) {
    zm = graphic_alloc(2,n/2,sizeof(int));
    if (  zm == 0 ) 
      {
	sciprint("Running out of memory \n");
	return ;
      }      
  }
  /* From double to pixels */
  for ( i = 0 ; i < *n1 ; i++)
    for ( j =0 ; j < *n2 ; j++)
      {
	xm[2*(i +(*n1)*j)]= XScale(x[i]);
	ym[2*(i +(*n1)*j)]= YScale(y[j]);
      }
  /** Scaling **/
  nx=MiniD(x,*n1)*Cscale.Wscx1;
  ny=MiniD(y,*n2)*Cscale.Wscy1;
  sfx= Cscale.Wscx1;
  sfy= Cscale.Wscy1;
  sfx2= sfx*sfx;
  sfy2= sfy*sfy;
  maxx = sfx2*fx[0]*fx[0]+sfy2*fy[0]*fy[0];
  for (i = 1;  i < (*n1)*(*n2) ; i++)
    {
      double maxx1 = sfx2*fx[i]*fx[i]+sfy2*fy[i]*fy[i];
      if ( maxx1 > maxx) maxx=maxx1;
    }
  maxx = ( maxx < SMDOUBLE) ? SMDOUBLE : sqrt(maxx);
  sc=maxx;
  /*sc= Min(nx,ny)/sc;*/
  sc= sqrt(nx*nx+ny*ny)/sc;
  sfx *= sc;
  sfy *= sc;
  /** size of arrow **/
  arsize1= ((double) Cscale.WIRect1[2])/(5*(*n1));
  arsize2= ((double) Cscale.WIRect1[3])/(5*(*n2));
  arsize=  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
  arsize = (int)(arsize*(*arfact));

  set_clip_box(Cscale.WIRect1[0],Cscale.WIRect1[0]+Cscale.WIRect1[2],Cscale.WIRect1[1],
	       Cscale.WIRect1[1]+Cscale.WIRect1[3]);

  if ( colored == 0 ) 
    {
      int j=0;
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  integer x1n,y1n,x2n,y2n,flag1=0;
	  xm[1+2*j]= (int)(sfx*fx[i]/2+xm[2*i]);
	  xm[2*j]  = (int)(-sfx*fx[i]/2+xm[2*i]);
	  ym[1+2*j]= (int)(-sfy*fy[i]/2+ym[2*i]);
	  ym[2*j]  = (int)(sfy*fy[i]/2+ym[2*i]);
	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if (flag1 !=0)
	    {
	      if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
	      if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
	      /* sciprint("j'ai rajoute (%d,%d)->(%d,%d)\r\n",xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1]); */
	      j++;
	    }
	}
      na=2*j;
    }
  else 
    {
      integer x1n,y1n,x2n,y2n,flag1=0;
      integer whiteid;
      int j=0;
      C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  double nor= sqrt(sfx2*fx[i]*fx[i]+sfy2*fy[i]*fy[i]);
	  zm[j] = inint( ((double) whiteid)*(1.0 - nor/maxx));
	  nor= sqrt(fx[i]*fx[i]+fy[i]*fy[i]);
	  xm[1+2*j]= (int)(sfx*fx[i]/(2*nor)+xm[2*i]);
	  xm[2*j]  = (int)(-sfx*fx[i]/(2*nor)+xm[2*i]);
	  ym[1+2*j]= (int)(-sfy*fy[i]/(2*nor)+ym[2*i]);
	  ym[2*j]  = (int)(sfy*fy[i]/(2*nor)+ym[2*i]);
	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if (flag1 !=0)
	    {
	      if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
	      if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
	      j++;
	    }
       }
      na=2*j;
    }
  /** Drawing axes **/
  axis_draw(strflag);
  /** Drawing the curves **/
  frame_clip_on();
  if ( colored ==0) 
    C2F(dr)("xarrow","v",xm,ym,&na,&arsize,xz,&iflag,PD0,PD0,PD0,PD0,0L,0L);
  else
    C2F(dr)("xarrow","v",xm,ym,&na,&arsize,zm,(iflag=1,&iflag),PD0,PD0,PD0,PD0,0L,0L);
  frame_clip_off();
}

int C2F(champ)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
     double x[],y[],fx[],fy[],brect[],*arfact;
     integer *n1,*n2;
     char strflag[];
     integer lstr;
{
  champg("champ",0,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0); 
}

int C2F(champ1)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
     double x[],y[],fx[],fy[],brect[],*arfact;
     integer *n1,*n2;
     char strflag[];
     integer lstr;
{
  champg("champ1",1,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0);
}


/*----------------------------------
 * Returns min( abs(x)) excluding null x(i)  values 
 * if x==0 then 1 is returned 
 *----------------------------------*/

static double MiniD(x, n)
     double *x;
     integer n;
{
  int i;
  double dx=1,mindx=1;
  if ( n < 2 ) return(mindx);
  mindx= Abs(x[1]-x[0]);
  mindx = ( mindx != 0 ) ? mindx : 1;
  for ( i = 2 ; i < n ; i++) 
    {
      dx = Abs(x[i]-x[i-1]);
      if ( dx < mindx && dx != 0 ) mindx=dx;
    }
  return(mindx);
}







