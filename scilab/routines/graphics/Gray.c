/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr
 *
 *    Modified 2002 Djalel Abdemouche INRIA for entity mode (NG)
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Math.h" 
#include "Graphics.h"
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

extern int version_flag(void); /* NG */

extern void fill_grid_rectangles __PARAMS(( integer *x,  integer *y, 
					    double *z, integer n1, integer n2));

extern void fill_grid_rectangles1 __PARAMS(( integer *x,  integer *y,  
					     double *z, integer n1, integer n2));

extern void GraySquare __PARAMS((integer *x,integer *y,double *z,
				integer n1,integer n2));
extern void GraySquare1 __PARAMS((integer *x,integer *y,double *z,
				 integer n1,integer n2));

extern void GraySquare1_NGreverse(integer *x, integer *y, double *z, 
				  integer n1, integer n2, sciPointObj * psubwin);

extern void GraySquareDirect(integer *x, integer *y, double *z, integer n1, integer n2); /* for NG, grayplot direct mode */
extern void GraySquareScaled(integer *x, integer *y, double *z, integer n1, integer n2); /* for NG, grayplot direct mode */

extern void initsubwin();
/*extern void compute_data_bounds(int cflag,char dataflag,double *x,double *y,int n1,int n2,double *drect);*/
extern void compute_data_bounds2(int cflag,char dataflag,char *logflags,double *x,double *y,int n1,int n2,double *drect);
extern BOOL update_specification_bounds(sciPointObj *psubwin, double *rect, int flag);
extern int re_index_brect(double * brect, double * drect);
extern BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag);
extern int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax);

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

int C2F(xgray)(double *x, double *y, double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint, BOOL flagNax, long int l1)
{
  int N = Max((*n1),(*n2));
  double xx[2],yy[2];
  integer *xm,*ym,j,nn1=1,nn2=2/*,i*/;
  sciPointObj  *psubwin = NULL;
  double drect[6];
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;
  
  xx[0]=Mini(x,*n1);xx[1]=Maxi(x,*n1);
  yy[0]=Mini(y,*n2);yy[1]=Maxi(y,*n2);
  
  /* NG beg */
  if (version_flag() == 0){
   
    if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
      sciXbasc(); 
      initsubwin();
      sciRedrawFigure();
      psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());  /* F.Leray 25.02.04*/
    } 

    /* Adding F.Leray 22.04.04 */
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    
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
    
    /* Force psubwin->axes.aaint to those given by argument aaint*/
    /*****TO CHANGE F.Leray 10.09.04  for (i=0;i<4;i++) pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; */
    
    /* Force "cligrf" clipping */
    sciSetIsClipping (psubwin,0); 

    /* Force  axes_visible property */
    /* pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;*/

    if (sciGetGraphicMode (psubwin)->autoscaling) {
      /* compute and merge new specified bounds with psubwin->Srect */
      switch (strflag[1])  {
      case '0': 
	/* do not change psubwin->Srect */
	break;
      case '1' : case '3' : case '5' : case '7':
	/* Force psubwin->Srect=brect */
	re_index_brect(brect, drect);
	break;
      case '2' : case '4' : case '6' : case '8': case '9':
	/* Force psubwin->Srect to the x and y bounds */
	/* compute_data_bounds(0,'g',xx,yy,nn1,nn2,drect); */
	compute_data_bounds2(0,'g',pSUBWIN_FEATURE(psubwin)->logflags,xx,yy,nn1,nn2,drect);
	break;
      }
      if (!pSUBWIN_FEATURE(psubwin)->FirstPlot &&(strflag[1] == '7' || strflag[1] == '8')) { /* merge psubwin->Srect and drect */
	drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
	drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
	drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
	drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
      }
      if (strflag[1] != '0') 
	bounds_changed = update_specification_bounds(psubwin, drect,2);
    } 

    if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;

    axes_properties_changed = strflag2axes_properties(psubwin, strflag);
    
    pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */
   
    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
       u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */
    
    pSUBWIN_FEATURE (psubwin)->flagNax = flagNax; /* store new value for flagNax */
    
    if(pSUBWIN_FEATURE (psubwin)->flagNax == TRUE){
      if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'n' && pSUBWIN_FEATURE (psubwin)->logflags[1] == 'n')
	{
	  pSUBWIN_FEATURE (psubwin)->axes.auto_ticks[0] = FALSE; /* x and y graduations are imposed by Nax */
	  pSUBWIN_FEATURE (psubwin)->axes.auto_ticks[1] = FALSE;
	  
	  CreatePrettyGradsFromNax(psubwin,aaint);
	}
      else{
	sciprint("Warning : Nax does not work with logarithmic scaling\n");}
    }
    
    if(bounds_changed == TRUE || axes_properties_changed == TRUE)
      sciDrawObj(sciGetCurrentFigure());
/*       EraseAndOrRedraw(psubwin); /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
    
    sciSetCurrentObj (ConstructGrayplot 
		      ((sciPointObj *)
		       sciGetSelectedSubWin (sciGetCurrentFigure ()),
		       x,y,z,*n1,*n2,0));
    sciDrawObjIfRequired(sciGetCurrentObj ());
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
  }

  else { 
    /** Boundaries of the frame **/
    update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);
    /** If Record is on **/
    if (GetDriver()=='R') 
      StoreGray("gray",x,y,z,n1,n2,strflag,brect,aaint);
    /** Allocation **/
    xm = graphic_alloc(0,N,sizeof(int));
    ym = graphic_alloc(1,N,sizeof(int));
    if ( xm == 0 || ym == 0)  {
      sciprint("Running out of memory \n");return 0;}      

    axis_draw(strflag);
    /** Drawing the curves **/
    frame_clip_on();
    for ( j =0 ; j < (*n1) ; j++)	 xm[j]= XScale(x[j]);
    for ( j =0 ; j < (*n2) ; j++)	 ym[j]= YScale(y[j]);
    GraySquare(xm,ym,z,*n1,*n2); 
    frame_clip_off(); 
    C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
	    ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  return(0);
}


static void GraySquare_base(integer *x, integer *y, double *z, integer n1, integer n2)
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
        if (x[n1] == 1) fill[0]= inint(z[j+ (i*n2)]); /* NG ????? */
	C2F(dr)("xset","pattern",&(fill[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        w=Abs(x[i+1]-x[i]);h=Abs(y[j+1]-y[j]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[i] < xz[0] && y[j+1] < xz[1] && x[i]+w > 0 && y[j+1]+h > 0 )
	  {
	    if ( Abs(x[i]) < int16max && Abs(y[j+1]) < int16max && w < uns16max && h < uns16max)
	      {
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


extern void GraySquare(integer *x, integer *y, double *z, integer n1, integer n2)
{
  if ( GetDriverId() == 0 )
    /** accelerated version for X11 or Windows **/
    fill_grid_rectangles(x, y, z, n1, n2);
  else 
      GraySquare_base(x, y, z, n1, n2);
}




/*-------------------------------------------------------
 * Matplot subroutine
 *  z : of size n1*n2 
 *  the z value is interpreted as a color number inside the current colormap
 *  z[i,j] is used as the color of a square [i-0.5,i+0.5] [j-0.5,j+0.5]
 *-------------------------------------------------------*/

int C2F(xgray1)(double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint, BOOL flagNax, long int l1)
{
  int N = Max((*n1+1),(*n2+1))/*,i*/;
  double xx[2],yy[2];
  static integer *xm,*ym,j, nn1=1,nn2=2;
  sciPointObj  *psubwin = NULL;
  double drect[6];
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;
  
  xx[0]=0.5;xx[1]= *n2+0.5;
  yy[0]=0.5;yy[1]= *n1+0.5;
  
  if (version_flag() == 0){
    if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
      sciXbasc(); 
      initsubwin();
      sciRedrawFigure();
    } 
    
    /* Adding F.Leray 22.04.04 */
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    
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
          
    /*****TO CHANGE F.Leray 10.09.04    for (i=0;i<4;i++)     pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; */
    
    /*---- Boundaries of the frame ----*/
    if (sciGetGraphicMode (psubwin)->autoscaling){
      /* compute and merge new specified bounds with psubwin->Srect */
      switch (strflag[1])  {
      case '0': 
	/* do not change psubwin->Srect */
	break;
      case '1' : case '3' : case '5' : case '7':
	/* Force psubwin->Srect=brect */
	re_index_brect(brect, drect);
	break;
      case '2' : case '4' : case '6' : case '8': case '9':
	/* Force psubwin->Srect to the x and y bounds */
/* 	compute_data_bounds(0,'g',xx,yy,nn1,nn2,drect); */
	compute_data_bounds2(0,'g',pSUBWIN_FEATURE(psubwin)->logflags,xx,yy,nn1,nn2,drect);
	break;
      }
      if (!pSUBWIN_FEATURE(psubwin)->FirstPlot && 
	  (strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9')) { /* merge psubwin->Srect and drect */
	drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
	drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
	drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
	drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
      }
      if (strflag[1] != '0') 
	bounds_changed = update_specification_bounds(psubwin, drect,2);
    } 
  
    if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;
    
    axes_properties_changed = strflag2axes_properties(psubwin, strflag);
     
    pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */
 
    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
       u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */
    
    pSUBWIN_FEATURE(psubwin)->flagNax = flagNax; /* store new value for flagNax */
    
    if(pSUBWIN_FEATURE(psubwin)->flagNax == TRUE){
      if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n' && pSUBWIN_FEATURE(psubwin)->logflags[1] == 'n')
	{
	  pSUBWIN_FEATURE(psubwin)->axes.auto_ticks[0] = FALSE; /* x and y graduations are imposed by Nax */
	  pSUBWIN_FEATURE(psubwin)->axes.auto_ticks[1] = FALSE;
	  
	  CreatePrettyGradsFromNax(psubwin,aaint);
	}
      else{
	sciprint("Warning : Nax does not work with logarithmic scaling\n");}
    }
    

  /*   sciDrawObj(psubwin); /\* ???? *\/ */
    
    if(bounds_changed == TRUE || axes_properties_changed == TRUE)
      sciDrawObj(sciGetCurrentFigure());
 /*      EraseAndOrRedraw(psubwin); /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
    
    sciSetCurrentObj (ConstructGrayplot 
		      ((sciPointObj *)
		       sciGetSelectedSubWin (sciGetCurrentFigure ()),
		       NULL,NULL,z,*n1 + 1,*n2 + 1,1)); 
    sciDrawObjIfRequired(sciGetCurrentObj ());
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
  }
  else { /* NG end */
    /** Boundaries of the frame **/
    update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);

    /** If Record is on **/
    if (GetDriver()=='R')
      StoreGray1("gray1",z,n1,n2,strflag,brect,aaint);

    /** Allocation **/
    xm = graphic_alloc(0,N,sizeof(int));
    ym = graphic_alloc(1,N,sizeof(int));
    if ( xm == 0 || ym == 0) {
      sciprint("Running out of memory \n");return 0;}      

    axis_draw(strflag);
    frame_clip_on();
    for ( j =0 ; j < (*n2+1) ; j++) xm[j]= XScale(j+0.5);
    for ( j =0 ; j < (*n1+1) ; j++) ym[j]= YScale(((*n1)-j+0.5));
    GraySquare1(xm,ym,z,*n1+1,*n2+1);
    frame_clip_off(); 
    C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
	    ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  return (0);
}
  
 
/*-------------------------------------------------------
 * like xgray1 : 
 * but xrect here give the rectangle in which the 
 * grayplot is to be drawn using the current scale
 -------------------------------------------------------*/

int C2F(xgray2)(double *z, integer *n1, integer *n2, double *xrect)
{

  /* NG beg */
  if (version_flag() == 0){
    double y; /* void for ConstructGrayplot */ 
    sciPointObj *psubwin = NULL;
    if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
      sciXbasc(); 
      initsubwin();
      sciRedrawFigure();
    } 
  
    /*---- Boundaries of the frame ----*/
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
    sciSetIsClipping (psubwin,0); 

    sciDrawObj(sciGetSelectedSubWin (sciGetCurrentFigure ())); 
    sciSetCurrentObj (ConstructGrayplot 
		      ((sciPointObj *)
		       sciGetSelectedSubWin (sciGetCurrentFigure ()),
		       xrect,&y,z,*n1+1,*n2+1,2));
    sciDrawObjIfRequired(sciGetCurrentObj ()); 
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
  }
  else { /* NG end */
    double xx[2],yy[2];
    integer xx1[2],yy1[2],nn1=1,nn2=2;
    integer *xm,*ym,  j;


    xx[0]=xrect[0];xx[1]=xrect[2];
    yy[0]=xrect[1];yy[1]=xrect[3];
 
    /** If Record is on **/
    if  (GetDriver()=='R')
      StoreGray2("gray2",z,n1,n2,xrect);
      /** Boundaries of the frame **/
    C2F(echelle2d)(xx,yy,xx1,yy1,&nn1,&nn2,"f2i",3L);  

    xm = graphic_alloc(0,(*n2)+1,sizeof(int));
    ym = graphic_alloc(1,(*n1)+1,sizeof(int));
    if ( xm == 0 || ym == 0 ) {
      Scistring("Xgray: running out of memory\n");return 0;}
    frame_clip_on();
    for ( j =0 ; j < (*n2+1) ; j++)	 
      xm[j]= (int) (( xx1[1]*j + xx1[0]*((*n2)-j) )/((double) *n2));
  
    for ( j =0 ; j < (*n1+1) ; j++)	 
      ym[j]= (int) (( yy1[0]*j + yy1[1]*((*n1)-j) )/((double) *n1));
    GraySquare1(xm,ym,z,*n1+1,*n2+1);
    frame_clip_off(); 
 
    }
  return (0);
}
 


/*-------------------------------------------------------
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : of size (n1-1)*(n2-1)  gives the f-values on the middle 
 *  of each rectangle. 
 *  z[i,j] is the value on the middle of rectangle 
 *        P1= x[i],y[j] x[i+1],y[j+1]
 *-------------------------------------------------------*/

static void GraySquare1_base(integer *x, integer *y, double *z, integer n1, integer n2)
{
  integer i,j,verbose=0,narg,fill[1],cpat,xz[2];
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	fill[0]= (integer) (z[i+(n1-1)*j]);
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

extern void GraySquare1(integer *x, integer *y, double *z, integer n1, integer n2)
{
 
  if ( GetDriverId() == 0 ) 
    /** accelerated version for X11 or Windows **/
    fill_grid_rectangles1(x, y, z, n1, n2);
  else 
    GraySquare1_base(x, y, z, n1, n2);
}



/* Only for new graphics */
/* NG: New Graphics */
/* reverse means reverse case on X and/or Y axis */
extern void GraySquare1_NGreverse(integer * x, integer *y, double *z, integer n1, integer n2, sciPointObj * psubwin)
{
  int i,j;
  integer verbose=0,narg,fill[1],cpat,xz[2];
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  integer *tmpx = MALLOC(n2*sizeof(integer));
  integer *tmpy = MALLOC(n1*sizeof(integer));

  for (i = 0 ; i < (n2) ; i++) tmpx[i] = x[i];
  for (i = 0 ; i < (n1) ; i++) tmpy[i] = y[i];
  
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	int xx,yy;
	fill[0]= (integer) (z[i+(n1-1)*j]);
	C2F(dr)("xset","pattern",&(fill[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
	
	w=Abs(tmpx[j+1]-tmpx[j]);
	h=Abs(tmpy[i+1]-tmpy[i]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[j] < xz[0] && y[i] < xz[1] && x[j]+w > 0 && y[i]+h > 0 )
	  if ( Abs(x[j]) < int16max && Abs(y[i+1]) < int16max && w < uns16max && h < uns16max)
	    {
	      if(ppsubwin->axes.reverse[0] == TRUE)
		xx = x[j] -w;
	      else
		xx = x[j];
	      
	      if(ppsubwin->axes.reverse[1] == TRUE)
		yy = y[i] -h;
	      else
		yy = y[i];
	      
	      C2F(dr)("xfrect","v",&xx,&yy,&w,&h,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
  
  
  FREE(tmpx); tmpx = NULL;
  FREE(tmpy); tmpy = NULL;
  
}










extern void GraySquareDirect(integer *x, integer *y, double *z, integer n1, integer n2)
{
  integer i,j,verbose=0,whiteid,narg,fill,cpat,xz[2];
  integer vertexx[5], vertexy[5];
  int cinq = 5, un = 1;
  
  int *xm = x;
  int *ym = y;

  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	fill= - (int) z[j+n1*i];
	C2F(dr)("xset","pattern",&fill,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
 	
	vertexx[0] = xm[i+n1*j];
	vertexx[1] = xm[i+n1*(j+1)];
	vertexx[2] = xm[i+1+n1*(j+1)];
	vertexx[3] = xm[i+1+n1*j];
	vertexx[4] = xm[i+n1*j];
	
	vertexy[0] = ym[j+n2*i];
	vertexy[1] = ym[j+1+n2*i];
	vertexy[2] = ym[j+1+n2*(i+1)];
	vertexy[3] = ym[j+n2*(i+1)];
	vertexy[4] = ym[j+n2*i];
		  
	C2F(dr)("xliness","str",vertexx,vertexy,&fill,&un,&cinq,
		PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
  
  C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}



extern void GraySquareScaled(integer *x, integer *y, double *z, integer n1, integer n2)
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill,cpat,xz[2];
  integer vertexx[5], vertexy[5];
  int cinq = 5, un = 1;
  
  int *xm = x;
  int *ym = y;
  
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
	zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);
	fill= - (1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin)));
	
	C2F(dr)("xset","pattern",&fill,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	
	vertexx[0] = xm[i+n1*j];
	vertexx[1] = xm[i+n1*(j+1)];
	vertexx[2] = xm[i+1+n1*(j+1)];
	vertexx[3] = xm[i+1+n1*j];
	vertexx[4] = xm[i+n1*j];
	
	vertexy[0] = ym[j+n2*i];
	vertexy[1] = ym[j+1+n2*i];
	vertexy[2] = ym[j+1+n2*(i+1)];
	vertexy[3] = ym[j+n2*(i+1)];
	vertexy[4] = ym[j+n2*i];
		  	
	C2F(dr)("xliness","str",vertexx,vertexy,&fill,&un,&cinq,
		PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
  
  C2F(dr)("xset","pattern",&cpat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

