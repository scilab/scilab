/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "Math.h"
#include "PloEch.h"

typedef void (level_f) __PARAMS((integer ival, double Cont, double xncont,
			       double yncont));
typedef void (*ptr_level_f) __PARAMS((integer ival, double Cont, double xncont,
			       double yncont));

static int 
Contour2D __PARAMS((ptr_level_f,char *,double *x,double *y,double *z,integer *n1,
		   integer *n2,integer *flagnz,integer *nz,double *zz,
		   integer *style,char *strflag,char *legend,double *brect,
		   integer *aaint,integer lstr1,integer lstr2));
static void 
contourI __PARAMS((ptr_level_f,double *, double *, double *,
		  double *, integer *, integer *, integer *));
static void
look __PARAMS((ptr_level_f, integer i, integer j, integer ib,
	      integer jb, integer qq,double Cont, integer style));

static integer ffnd __PARAMS((ptr_level_f,integer,integer,integer,integer,integer,
			     integer,integer,integer,integer,integer,
			     double,integer *));

static int Gcont_size = 0;

static void ContourTrace __PARAMS((double Cont, integer style));
static level_f ContStore, ContStore1, ContStore2,GContStore2;
static void GContStore2Last __PARAMS((void));
static double x_cont __PARAMS((integer i));
static double y_cont __PARAMS((integer i));
static double phi_cont __PARAMS((integer, integer)); 
static double f_intercept  __PARAMS((double, double, double, double, double ));
static integer not_same_sign  __PARAMS((double val1, double val2)); 
static integer get_itg_cont  __PARAMS((integer i, integer j)); 
static void inc_itg_cont  __PARAMS((integer i, integer j, integer val)); 
static integer oddp  __PARAMS((integer i));

/*-----------------------------------------------------------------------
 *  Level curves 
 *  The computer journal vol 15 nul 4 p 382 (1972)
 *  from the Lisp Macsyma source (M.I.T)
 * -------------------------------------------------------------------------*/

#define HIDDENFRAMECOLOR 2L /* default color for hidden frame */

/*---------------------------------------------------------------------------
 * General functions (could be changed in #define or
 *   inline functions to increase speed)
 *---------------------------------------------------------------------------*/

static double *GX,*GY,*GZ;
static integer Gn1,Gn2;

static void InitValues(double *x, double *y, double *z, integer n1, integer n2)
{
  Gn1=n1;  Gn2=n2;  GX = x;  GY = y;  GZ = z;
}

/*--------return the  value of f for a pointeger on the grid-----*/

static double phi_cont(integer i, integer j)
{
  return(GZ[i+Gn1*j]);
}

/*---------return the coordinates between  [xi,xj] along one axis 
 *  for which the value of f is zCont */ 

static double f_intercept(double zCont, double fi, double xi, double fj, double xj)
{
  return( xi+ (zCont-fi)*(xj-xi)/ (fj-fi));
}

/* check for boundary points */

static integer  bdyp(integer i, integer j)
{
  return (  j == 0 || i == 0 || j == Gn2-1 || i == Gn1-1 );
}

/* store or get flag values */

static  integer *itg_cont, *xbd_cont,*ybd_cont;

static integer get_itg_cont(integer i, integer j)
{
  return( itg_cont[i+Gn1*j]);
}

static void inc_itg_cont(integer i, integer j, integer val)
{
  itg_cont[i+Gn1*j] += val;
}

static integer not_same_sign(double val1, double val2)
{
  if ( ISNAN(val1) ==1 || ISNAN(val2) == 1) return(0);
  /** 0.0 est consid\'er\'e comme positif **/
  if ( val1 >= 0.0) 
    {
      if (val2 < 0.0) return(1) ; else return(0);}
  else 
    {
      if ( val2 >= 0.0) return(1) ; else return(0);}
}

static integer oddp(integer i) { return( i == 1 || i ==3 );}

/*---------return the x-value of a grid point--------*/

static double x_cont(integer i) {  return GX[i] ;}

/*---------return the y-value of a grid point --------*/

static double y_cont(integer i) {  return GY[i] ;}

/*------------------------------------------------------------
 * Draw level curves for a function f(x,y) which values 
 * at points x(i),y(j) are given by z(i,j)
 * - z is a (n1,n2) matrix 
 * - x is a (1,n1) matrix 
 * - y is a (1,n2) matrix 
 * - x,y,z are stored as one dimensionnal array in C 
 * - if *flagnz =0 
 * -   then  nz is an integer pointer to the number of level curves. 
 *     else  zz is an array which gives th requested level values.
 *            (and nz is the size of thos array)
 * Computed from min and max of z
 * Exemple Contour(1:5,1:10,rand(5,10),5);
 *---------------------------------------------------------------*/

static double ZC=0.0;
static char   ContNumFormat[100];

int C2F(contour)(double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, double *teta, double *alpha, char *legend, integer *flag, double *bbox, double *zlev, integer lstr)
{
  int err=0;
  integer verbose=0,narg,fg;
  integer InsideU[4],InsideD[4];
  void (*func) __PARAMS((integer, double,double,double));
  static double *zconst;
  double zmin,zmax;
  integer N[3],i;
  double xbox[8],ybox[8],zbox[8];
  /** If Record is on **/
  if (GetDriver()=='R') 
    StoreContour("contour",x,y,z,n1,n2,flagnz,nz,zz,teta,alpha,
		 legend,flag,bbox,zlev);
  switch (flag[0])
    {
    case 0: func=ContStore; break;  /* 3D geometry with projection on the surface */
    case 1: func=ContStore1;ZC= *zlev; break;    /* 3D geometry with projection on a plan */
    case 2: func=ContStore2; break; /** 2D geometry **/
    default:func=ContStore2; break; /** 2D geometry **/
    }
  zmin=(double) Mini(z,*n1*(*n2)); 
  zmax=(double) Maxi(z,*n1*(*n2));

  if (flag[0] == 2)
    {
      /* Contour on a 2D plot */
      double FRect[4];
      static int aaint[4] = {2,10,2,10};
      FRect[0]=x[0];FRect[1]= y[0];FRect[2]=x[*n1-1];FRect[3]= y[*n2-1];
      set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
      /** Drawing axes **/
      axis_draw("111");
      frame_clip_on();
    }
  else
    {
      /* Contour on a 3D plot */
       if (flag[1]!=0 && flag[1]!=1 && flag[1]!=3 && flag[1]!=5)
	 {
	   bbox[0]=x[0];bbox[1]=x[*n1-1];
	   bbox[2]=y[0];bbox[3]=y[*n2-1];
	   bbox[4]=zmin;bbox[5]=zmax;
	 }
       if ( flag[1] !=0)
	 SetEch3d1(xbox,ybox,zbox,bbox,teta,alpha,(long)(flag[1]+1)/2);
       else
	 SetEch3d1(xbox,ybox,zbox,bbox,teta,alpha,0L);
       /** Calcule l' Enveloppe Convexe de la boite **/
       /** ainsi que les triedres caches ou non **/
       Convex_Box(xbox,ybox,InsideU,InsideD,legend,flag,bbox);
       /** Le triedre cach\'e **/
       if (zbox[InsideU[0]] > zbox[InsideD[0]])
	 {
	   /* cache=InsideD[0]; */
	   if (flag[2] >=2 )DrawAxis(xbox,ybox,InsideD,HIDDENFRAMECOLOR);
	 }
       else 
	 {
	   /* cache=InsideU[0]-4; */
	   if (flag[2] >=2 )DrawAxis(xbox,ybox,InsideU,HIDDENFRAMECOLOR);
	 }
     }
  if (*flagnz == 0)
    {
      if ((zconst = graphic_alloc(5,(*nz),sizeof(double)))== 0) 
	{
	  sciprint("Running out of memory\r\n");
	  return 0;
	}
      for ( i =0 ; i < *nz ; i++) 
	zconst[i]=zmin + (i+1)*(zmax-zmin)/(*nz+1);
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(func,x,y,z,zconst,N,(integer *) 0,&err);
    }
   else
    {
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(func,x,y,z,zz,N,(integer *) 0,&err);
    }
  C2F(dr)("xget","foreground",&verbose,&fg,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (flag[0]!=2 &&  flag[2] >=3 )
    {
      /** Le triedre que l'on doit voir **/
      if (zbox[InsideU[0]] > zbox[InsideD[0]])
	DrawAxis(xbox,ybox,InsideU,fg);
      else 
	DrawAxis(xbox,ybox,InsideD,fg);
    }
  frame_clip_off();
  return(0);
}

/** interface for contour2d **/

int C2F(contour2)(double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer lstr1, integer lstr2)
{
  Contour2D(ContStore2,"contour2",x,y,z,n1,n2,flagnz,nz,zz,style,strflag,
	    legend,brect,aaint,lstr1,lstr2);
  return 0;
}

/* interface for contour2di used in macro contourf 
 * used when we want to get the values which constitues the contour inside Scilab 
 * contour2di + c2dex 
 */

static int Contour2D(ptr_level_f func, char *name, double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer lstr1, integer lstr2)
{
  integer err=0;
  static double *zconst;
  double zmin,zmax;
  integer N[3],i;

  /** Boundaries of the frame **/
  update_frame_bounds(1,"gnn",x,y,n1,n2,aaint,strflag,brect);

  /** If Record is on **/
  if (GetDriver()=='R' && strcmp(name,"contour2")==0 ) 
    StoreContour2D("contour2",x,y,z,n1,n2,flagnz,nz,zz,style,strflag,legend,brect,aaint);

  zmin=(double) Mini(z,*n1*(*n2)); 
  zmax=(double) Maxi(z,*n1*(*n2));

  /** Scales **/
  if (strcmp(name,"contour2")==0 )
    {
      axis_draw(strflag);
      frame_clip_on();
    }
  if (*flagnz==0)
    {
      if ((zconst = graphic_alloc(5,(*nz),sizeof(double)))== 0) 
	{
	  sciprint("Running out of memory\r\n");
	  return 0;
	}
      for ( i =0 ; i < *nz ; i++) 
	zconst[i]=zmin + (i+1)*(zmax-zmin)/(*nz+1);
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(func,x,y,z,zconst,N,style,&err);
    }
  else
    {
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(func,x,y,z,zz,N,style,&err);
    }

  if (strcmp(name,"contour2")==0 )frame_clip_off();
  return(0);
}

int C2F(contourif)(double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, integer *style)
{
  integer err=0;
  static double *zconst;
  double zmin,zmax;
  integer N[3],i;

  zmin=(double) Mini(z,*n1*(*n2)); 
  zmax=(double) Maxi(z,*n1*(*n2));

  if (*flagnz==0)
    {
      if ((zconst = graphic_alloc(5,(*nz),sizeof(double)))== 0) 
	{
	  sciprint("Running out of memory\r\n");
	  return 0;
	}
      for ( i =0 ; i < *nz ; i++) 
	zconst[i]=zmin + (i+1)*(zmax-zmin)/(*nz+1);
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(GContStore2,x,y,z,zconst,N,style,&err);
    }
  else
    {
      N[0]= *n1;N[1]= *n2;N[2]= *nz;
      contourI(GContStore2,x,y,z,zz,N,style,&err);
    }
  return(0);
}

/*-------------------------------------------------------
 *  The function f is given on a grid and we want the level curves 
 *  for the zCont[N[2]] values 
 *  x : of size N[0] gives the x-values of the grid 
 *  y : of size N[1] gives the y-values of the grid 
 *  z : of size N[0]*N[1]  gives the f-values on the grid 
 *  style: size ncont (=N[2]) or empty integer pointer 
 *  gives the dash style for contour i
 *-------------------------------------------------------*/

static void contourI(ptr_level_f func, double *x, double *y, double *z, double *zCont, integer *N, integer *style, integer *err)
{
  int check = 1;
  char *F;
  integer n1,n2,ncont,i,c,j,k,n5;
  integer stylec;
  n1=N[0];n2=N[1];ncont=N[2];
  F=getFPF();
  if ( F[0] == '\0') 
    ChoixFormatE1(ContNumFormat,zCont,N[2]);
  InitValues(x,y,z,n1,n2);
  n5 =  2*(n1)+2*(n2)-3;
  /* Allocation */
  Gcont_size = 0; /** initialize the array indices for storing contours **/
  xbd_cont = graphic_alloc(0,n5,sizeof(int));
  ybd_cont = graphic_alloc(1,n5,sizeof(int));
  itg_cont = graphic_alloc(2,n1*n2,sizeof(int));
  if ( (xbd_cont == NULL) && n5 != 0) check= 0;
  if ( (ybd_cont == NULL) && n5 != 0) check= 0;
  if ( (itg_cont == NULL) && n1*n2 != 0) check= 0;
  if ( check == 0) 
    {
      Scistring("contourI_: Running out of memory\n");
      return;
    }
  /* just a parametrization of the boundary points */
  for ( i = 0 ; i <  n2 ; i++)
	{
	  ybd_cont[i] = i ;
	  xbd_cont[i] = 0 ;
	}
  for ( i = 1 ; i <  n1 ; i++)
	{
	  ybd_cont[n2+i-1] = n2-1 ;
	  xbd_cont[n2+i-1] = i  ;
	}
  for ( i = n2-2;  i >= 0  ; i--)
	{
	  ybd_cont[2*n2 +n1-3-i] = i ;
	  xbd_cont[2*n2 +n1-3-i] = n1-1  ;
	}
  for ( i = n1-2 ; i >= 0 ; i--)
	{
	  ybd_cont[2*n2 +2*n1-4-i] = 0 ;
	  xbd_cont[2*n2 +2*n1-4-i] = i  ;
	}
  for ( c= 0 ; c < ncont ; c++)
    {
      stylec = ( style != (integer *) 0) ? style[c] : c;
      /** itg-cont is a flag array to memorize checked parts of the grid **/
      for ( i = 0 ; i < n1; i++)
	for ( j =0 ; j < n2 ; j++)
	  itg_cont[i+n1*j]=0 ;
      /** all the boundary segments **/
      for ( k = 1 ; k < n5 ; k++)
	{ integer ib,jb;
	i = xbd_cont[k] ; j = ybd_cont[k];
	ib = xbd_cont[k-1] ; jb= ybd_cont[k-1];
	if  (not_same_sign (phi_cont(i,j)-zCont[c] , 
			    phi_cont(ib,jb)-zCont[c]))
	  look(func,i,j,ib,jb,1L,zCont[c],stylec);
	}
      /** inside segments **/
      for ( i = 1 ; i < n1-1; i++)
	for ( j = 1 ; j < n2-1 ; j++)
	  if  (not_same_sign ( phi_cont(i,j)-zCont[c] , 
			       phi_cont(i, j-1)-zCont[c]))
	    look(func,i,j,i,j-1,2L,zCont[c],stylec);
    }
}

/*--------------------------------------------------------------------
 *  the level curve is crossing the segment (i,j) (ib,jb)
 *  look store the level curve point and try to find the next segment to look at
 *  Cont: value of f along the contour 
 *  ncont: number of contour 
 *  c: indice of the contour Cont 
 *---------------------------------------------------------------------*/

static void look(ptr_level_f func, integer i, integer j, integer ib, integer jb, integer qq, double Cont, integer style)
{
  integer ip,jp,im,jm,zds,ent=0,flag=0,wflag;
  jp= j+1; ip= i+1; jm=j-1;im=i-1;
  /*  on regarde comment est le segment de depart */
  if  ( jb == jm)  flag = 1; 
  else  { 
    if ( ib == im ) flag = 2 ;
    else  {
      if ( jb == jp ) flag = 3 ;
      else  if ( ib == ip ) flag = 4;}}
  switch  (  flag)
    {
    case  1 :
      if  (get_itg_cont(i,jm) > 1) return;
      ent=1 ; /* le segment est vertical vers le bas */
      /* Storing intersection point */
      (*func)(0,Cont, x_cont(i), 
		f_intercept(Cont,phi_cont(i,jm),
			    y_cont(jm),phi_cont(i,j),y_cont(j)));
      break;
    case 2 : 
      if  (get_itg_cont(im,j) == 1 || get_itg_cont(im,j)==3 ) return;
      ent=2 ; /* le segment est horizontal gauche */
      /* Storing intersection point */
      (*func)( 0,Cont,
		f_intercept(Cont,phi_cont(im,j),
			    x_cont(im),phi_cont(i,j),x_cont(i)), y_cont(j));
      break ; 
    case 3 :
      if  (get_itg_cont(i,j) > 1 ) return;
      ent=3 ; /* le segment est vertical haut */
      /* Storing intersection point */
      (*func)(0,Cont,x_cont(i), f_intercept(Cont,phi_cont(i,j),
					    y_cont(j),phi_cont(i,jp),y_cont(jp)));
      break ;
    case 4 :
      if  (get_itg_cont(i,j) == 1 || get_itg_cont(i,j)==3 ) return;
      ent=4 ; /* le segment est horizontal droit */
      /* Storing intersection point */
      (*func)(0,Cont,f_intercept(Cont,phi_cont(i,j),
				 x_cont(i),phi_cont(ip,j),x_cont(ip)),
	      y_cont(j));
      break;
      default :
	Scistring(" Error in case wrong value ");
      break;
    }
  wflag=1;
  while ( wflag) 
    { 
      jp= j+1; ip= i+1; jm=j-1;im=i-1;
      switch  ( ent) 
	{case 1 :
	  inc_itg_cont(i,jm,2L);
	  ent = ffnd(func,i,ip,ip,i,j,j,jm,jm,ent,qq,Cont,&zds);
	  /* on calcule le nouveau point, ent donne la 
	     direction du segment a explorer */
	  switch ( ent)
	    {
	    case -1: wflag=0; break;
	    case 1 : i=ip ; break ;
	    case 2 : i=ip;j=jm; break ;
	    }
	  break ;
	case 2  :
	  inc_itg_cont(im,j,1L);
	  ent = ffnd(func,i,i,im,im,j,jm,jm,j,ent,qq,Cont,&zds);
	  switch ( ent)
	    { 
	    case -1: wflag=0; break;
	    case 2 : j = jm ;break ;
	    case  3  : i=im;j=jm; break ;
	    }
	  break ;
	case 3 :
	  inc_itg_cont(i,j,2L);
	  ent = ffnd(func,i,im,im,i,j,j,jp,jp,ent,qq,Cont,&zds);
	  switch ( ent)
	    { 
	    case -1: wflag=0; break;
	    case 3 : i=im; break ;
	    case 4 : i=im;j=jp; break ;
	    }
	  break ;
	case 4 :
	  inc_itg_cont(i,j,1L);
	  ent = ffnd(func,i,i,ip,ip,j,jp,jp,j,ent,qq,Cont,&zds);
	  switch ( ent)
	    {
	    case -1: wflag=0; break;
	    case 4 :j=jp;break ;
	    case 1 :i=ip;j=jp;break ;
	    }
	  break ;
	}
     
      /** new segment is on the boundary **/
      if ( zds == 1) 
	{
	  switch ( ent) 
	    {
	    case 1 : inc_itg_cont(i,(j-1),2L); break ; 
	    case 2 : inc_itg_cont(i-1,j,1L);  break ; 
	    case 3 : inc_itg_cont(i,j,2L); break ; 
	    case 4 : inc_itg_cont(i,j,1L); break ; 
	    }
	  /** we must quit the while loop **/
	  wflag = 0 ;
	  }
      /**  init point was inside the domain */
      if ( qq == 2) 
	{
	  switch ( ent) 
	    {
	    case 1 : if  ( get_itg_cont (i,j-1)  > 1) wflag = 0 ; break ; 
	    case 2 : if  ( oddp(get_itg_cont(i-1,j))) wflag = 0 ; break ; 
	    case 3 : if  ( get_itg_cont(i,j) > 1)     wflag = 0 ; break ; 
	    case 4 : if  ( oddp(get_itg_cont(i,j)))   wflag = 0 ; break ; 
	    }
	}
    }
  if ( func == GContStore2 ) 
    GContStore2Last();
  else 
    ContourTrace(Cont,style);
}


/*-----------------------------------------------------------------------
 *   ffnd : cette fonction  recoit en entree quatre points 
 *       on sait que la courbe de niveau passe entre le point 1 et le quatre 
 *       on cherche a savoir ou elle resort, 
 *       et on fixe une nouvelle valeur de ent qui indiquera le segment 
 *       suivant a explorer 
 *-----------------------------------------------------------------------*/

static integer ffnd (ptr_level_f func, integer i1, integer i2, integer i3, integer i4, integer jj1, integer jj2, integer jj3, integer jj4, integer ent, integer qq, double Cont, integer *zds)
{
  double phi1,phi2,phi3,phi4,xav,yav,phiav;
  integer revflag,i;
  phi1=phi_cont(i1,jj1)-Cont;
  phi2=phi_cont(i2,jj2)-Cont;
  phi3=phi_cont(i3,jj3)-Cont;
  phi4=phi_cont(i4,jj4)-Cont;
  revflag = 0;
  *zds = 0;
  /* le point au centre du rectangle */
  xav = ( x_cont(i1)+ x_cont(i3))/2.0 ; 
  yav = ( y_cont(jj1)+ y_cont(jj3))/2.0 ; 
  phiav = ( phi1+phi2+phi3+phi4) / 4.0;
  if (ISNAN(phiav)==1) 
    {
      return -1;
    }
  if (  not_same_sign( phiav,phi4)) 
    {
      integer l1, k1; 
      double phi;
      revflag = 1 ; 
      l1= i4; k1= jj4;
      i4=i1; jj4 = jj1; i1= l1; jj1= k1;
      l1= i3; k1= jj3;
      i3=i2; jj3= jj2; i2=l1; jj2= k1;
      phi = phi1; phi1 = phi4; phi4= phi;
      phi = phi3; phi3 = phi2; phi2= phi;
    }
  /* on stocke un nouveau point  */
  (*func)(1,Cont,f_intercept(0.0,phi1,x_cont(i1),phiav,xav),
	    f_intercept(0.0,phi1,y_cont(jj1),phiav,yav));
  /*
   * on parcourt les segments du rectangle pour voir sur quelle face
   * on sort 
   */
  for  ( i = 0 ;  ; i++)
    { integer l1,k1;
      double phi;
      if ( not_same_sign ( phi1,phi2))   /** sortir du for **/ break ; 
      if  ( phiav != 0.0 ) 
	{
	  (*func)(1,Cont,f_intercept(0.0,phi2,x_cont(i2),phiav,xav),
		    f_intercept(0.0,phi2,y_cont(jj2),phiav,yav));
	} 
      /** on permutte les points du rectangle **/
      l1=i1; k1= jj1;
      i1=i2;jj1=jj2;i2=i3;jj2=jj3;i3=i4;jj3=jj4;i4=l1;jj4=k1;
      phi=phi1; phi1=phi2;phi2=phi3;phi3=phi4;phi4=phi;
    }
  (*func)(1,Cont,f_intercept(0.0,phi1,x_cont(i1),phi2,x_cont(i2)),
	    f_intercept(0.0,phi1,y_cont(jj1),phi2,y_cont(jj2)));
  if ( qq==1 && bdyp(i1,jj1) && bdyp(i2,jj2)) *zds = 1 ;
  if ( revflag == 1  &&  ! oddp (i) )  i = i+2;
  return ( 1 + (  ( i + ent + 2) % 4));
}

/*--------------------------------------------------------------
 * Storing and tracing level curves 
 *----------------------------------------------------------------*/

static integer *xcont,*ycont;
static integer cont_size ;

/*
 * store a point in the current level curve if ival == 0 the level 
 * curve is reinitialized 
 * used for a contour in a 3D drawing 
 */

static void
G_ContStore(integer ival, int xncont, int yncont)
{
  int n;
  /* nouveau contour */
  if ( ival == 0) cont_size =0 ;
  n= cont_size + 1;
  xcont = graphic_alloc(3,n,sizeof(int));
  ycont = graphic_alloc(4,n,sizeof(int));
  if ( (xcont == NULL) && n != 0) return ; 
  if ( (ycont == NULL) && n != 0) return ;
  xcont[cont_size]= xncont;
  ycont[cont_size++]= yncont;
}

/*
 * store a point in the current level curve if ival == 0 the level 
 * curve is reinitialized 
 * used for a contour in a 3D drawing 
 */

static void
ContStore(integer ival, double Cont, double xncont, double yncont)
{
  G_ContStore(ival,GEOX(xncont,yncont,Cont),
	      GEOY(xncont,yncont,Cont));
}

/*
 * store a point in the current level curve if ival == 0 the level 
 * curve is reinitialized 
 * used for a contour in a 3D drawing with projection at level ZC 
 */

static void
ContStore1(integer ival, double Cont, double xncont, double yncont)
{
  G_ContStore(ival,GEOX(xncont,yncont,ZC),
	      GEOY(xncont,yncont,ZC));
}

/*
 * store a point in the current level curve if ival == 0 the level 
 * curve is reinitialized 
 * used for a contour in a 2D drawing 
 */

static void
ContStore2(integer ival, double Cont, double xncont, double yncont)
{
  G_ContStore(ival,XScale(xncont),YScale(yncont));
}

/* 
 * Explicit drawing of the current level curve with a dash style 
 * The curve level is also drawn as a string according to current 
 * floating point format 
 */

static void ContourTrace(double Cont, integer style)
{ 
  char *F;
  integer verbose=0 ,Dnarg,Dvalue[10];
  integer close=0, flag=0, uc;
  double angle=0.0;
  char str[100];

  C2F(dr)("xget","use color",&verbose,&uc,&Dnarg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (uc) {
    C2F(dr)("xget","color",&verbose,Dvalue,&Dnarg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","color",&style,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xlines","void",&cont_size,xcont,ycont,&close,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","color",Dvalue,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else {
    C2F(dr)("xget","line style",&verbose,Dvalue,&Dnarg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","line style",&style,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xlines","void",&cont_size,xcont,ycont,&close,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","line style",Dvalue,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  F=getFPF();
  if ( F[0] == '\0') 
    sprintf(str,ContNumFormat,Cont);
  else 
    sprintf(str,F,Cont);
  C2F(dr)("xstring",str, &xcont[cont_size / 2],&ycont[cont_size /2],
	  PI0,&flag,PI0,PI0, &angle,PD0,PD0,PD0,0L,0L);
}

/*--------------------------------------------------------------
 * Following code is used to store the current level curves as 
 * double in order to access to the stored data at Scilab level 
 *----------------------------------------------------------------*/


double *Gxcont,*Gycont;
static int last=-1;
static int count=0; 
 
/** used to bring back data to Scilab Stack **/

int C2F(getconts)(double **x, double **y, integer *mm, integer *n)
{
  *x = Gxcont;
  *y = Gycont;
  *mm= 1;
  *n= Gcont_size;
  return 0;
}

static void GContStore2(integer ival, double Cont, double xncont, double yncont)
{
  int n;
  if ( ival == 0) 
    {
      /* Here : ival == 0 means stop the current level curve and 
       * store data at the end but do reset Gcont_size to zero 
       */
      n= Gcont_size + 1;
      Gxcont = graphic_alloc(3,n,sizeof(double));
      Gycont = graphic_alloc(4,n,sizeof(double));
      if ( (Gxcont == NULL) && n != 0) return ; 
      if ( (Gycont == NULL) && n != 0) return ;
      Gxcont[Gcont_size] = Cont;
      if ( last != -1 ) Gycont[last]= count;
      last = Gcont_size;
      Gcont_size++;
      count = 0;
    }
  n= Gcont_size + 1;
  Gxcont = graphic_alloc(3,n,sizeof(double));
  Gycont = graphic_alloc(4,n,sizeof(double));
  if ( (Gxcont == NULL) && n != 0) return ; 
  if ( (Gycont == NULL) && n != 0) return ;
  Gxcont[Gcont_size]=xncont;
  Gycont[Gcont_size++]=yncont;
  count++;
}

static void GContStore2Last(void)
{
  if ( last != -1 ) Gycont[last]= count;
}











