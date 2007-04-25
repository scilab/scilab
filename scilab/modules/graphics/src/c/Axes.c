/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    Copyright (C) 2006      INRIA/Jean-Baptiste Silvy
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots 
 *--------------------------------------------------------------------------*/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "math_graphics.h"
#include "PloEch.h"
#include "Axes.h"
#include "DestroyObjects.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "Xcall1.h"
#include "SetProperty.h"
#include "handleDrawing/drawTextEntity.h"
#include "sciprint.h"
#include "Format.h"
#include "Plo3d.h"
#include "axesScale.h"
#include "CurrentObjectsManagement.h"


#include "MALLOC.h" /* MALLOC */

#include "GetProperty.h"

static double  x_convert __PARAMS((char xy_type,double x[] ,int i));
static double  y_convert __PARAMS((char xy_type,double x[] ,int i));


static void axesplot(char* ,sciPointObj*);
static int SciAxisNew(char pos,sciPointObj *psubwin, double xy,int fontsize,int fontstyle,int textcolor,int ticscolor, int seg);

static void DrawXTics(char pos, sciPointObj * psubwin, double xy, char * c_format, int * fontid, int textcolor,int ticscolor,int color_kp, int *logrect, int smallersize);
static void DrawYTics(char pos, sciPointObj * psubwin, double xy, char * c_format, int * fontid, int textcolor,int ticscolor,int color_kp, int *logrect, int smallersize);
static int XDrawAxisLine(double xminval,double xmaxval,double xy, int ticscolor, int color_kp);
static int YDrawAxisLine(double yminval,double ymaxval,double xy, int ticscolor, int color_kp);
static void FindXYMinMaxAccordingTL(sciPointObj * psubwin, double *xminval,double *yminval,double *xmaxval,double *ymaxval);

static int DrawXSubTics(char pos, sciPointObj * psubwin, double xy,int ticscolor,int color_kp);
static int DrawYSubTics(char pos, sciPointObj * psubwin, double xy,int ticscolor,int color_kp);

static int DrawXGrid(sciPointObj * psubwin);
static int DrawYGrid(sciPointObj * psubwin);

static void YGradPosition(sciPointObj * psubwin, int xx, int rect2);
static void XGradPosition(sciPointObj * psubwin, int yy, int rect3);

int SciDrawLines(char pos, sciPointObj * psubwin, double xy, int textcolor,int ticscolor);

void axis_draw(char strflag[]) ;

/*--------------------------------------------------------------
 * Draw Axis or only rectangle
 *----------------------------------------------------------------*/

void axis_draw(char strflag[])
{ 
	static sciPointObj * psubwin;
  /* using foreground to draw axis */
  integer verbose=0,narg,xz[10],fg,i,ixbox[5],iybox[5],p=5,n=1,color,color_kp; 
 /*  int isoflag = -1; */
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,xz+1,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 


  psubwin = sciGetCurrentSubWin();
  
  /* F.Leray test on color here*/
  color=sciGetBackground(psubwin);
  

  ixbox[0]=ixbox[4]=Cscale.WIRect1[0];iybox[0]=iybox[4]=Cscale.WIRect1[1];
  ixbox[1]=ixbox[0];iybox[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];
  ixbox[2]=Cscale.WIRect1[0]+Cscale.WIRect1[2];iybox[2]=iybox[1];
  ixbox[3]=ixbox[2];iybox[3]=iybox[0];
  C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",&color,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xarea", "v", &p, ixbox, iybox, &n, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
  C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  switch ( c) 
    {
    case '0' :
      break ;
    case '2' :
      pSUBWIN_FEATURE (psubwin)->axes.rect = BT_ON;  /* NG */
      C2F(dr)("xrect","xv",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
	      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      break;
    default :
      axesplot(strflag,psubwin);
      break;
    }



  C2F(dr)("xset","line style",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",xz+1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


}

/* F.Leray 16.12.04 */
/* Same thing as axis_draw without the background area set */
/* Only used in NG mode */
void axis_draw2(char strflag[])
{ 
	static sciPointObj * psubwin;
  /* using foreground to draw axis */
  integer verbose=0,narg,xz[10],fg,i,color; 
  /*  int isoflag = -1; */
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,xz+1,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  
  psubwin = sciGetCurrentSubWin();
  
  /* F.Leray test on color here*/
  color=sciGetBackground(psubwin);
  
  switch ( c) 
    {
      /*case '0' :
        break ;*/
    case '2' :
      pSUBWIN_FEATURE (psubwin)->axes.rect = BT_ON;
      
      C2F(dr)("xrect","xv",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
	      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      break;
    default :
      axesplot(strflag,psubwin);
      break;
    }

  C2F(dr)("xset","line style",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",xz+1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

/* Only paint the area of the axes */
/* ans does not draw any graduations or ticks, lines... */
void DrawAxesBackground( void )
{ 
	static sciPointObj * psubwin;
  /* using foreground to draw axis */
  integer verbose=0,narg,xz[10],fg,i,ixbox[5],iybox[5],p=5,n=1,color,color_kp; 
 /*  int isoflag = -1; */
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,xz+1,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 

  psubwin = sciGetCurrentSubWin();
    
  /* F.Leray test on color here*/
  color=sciGetBackground(psubwin);
  
  ixbox[0]=ixbox[4]=Cscale.WIRect1[0];iybox[0]=iybox[4]=Cscale.WIRect1[1];
  ixbox[1]=ixbox[0];iybox[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];
  ixbox[2]=Cscale.WIRect1[0]+Cscale.WIRect1[2];iybox[2]=iybox[1];
  ixbox[3]=ixbox[2];iybox[3]=iybox[0]; 
  C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",&color,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	 
  C2F(dr)("xarea", "v", &p, ixbox, iybox, &n, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
  C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	  

  C2F(dr)("xset","line style",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",xz+1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


}


/*-------------------------------------------------------------
 * Sci_Axis : 
 *   Drawing axis 
 *   pos = 'r' | 'l' | 'u' | 'd' : gives the tics directions 
 *         'r' and 'l' are used for vertical axis 
 *         'u' and 'd' for horizontal axis 
 *   xy_type = 'v' (for vector) or 'r' (for range) 
 *         'v' means that tics position are given by a vector 
 *         'r' means that tics position are in a range i.e given by a vector of size 3 
 *             [min,max,number_of_intervals] 
 *         'i' means that tics positions are in a range given by four number (integers) 
 *             [k1,k2,e,number_of intervale] -> [k1*10^e,k2*10^e] 
 *   x vector of size nx 
 *   y vector of size ny 
 *         if pos = 'r' or 'l' then x must be of size 1 
 *              the size of y i.e ny must be 3 if xy_type == 'r' or 4 if xy_type == 'i'
 *              str if non null gives the string to be used at ticks marks 
 *              it must be then of size ny or of size y[3]+1 (if xy_type == 'r') 
 *              or y[4]+1 (if xy_type == 'i') 
 *         if pos = 'u' or 'd  ' then y must be of size 1 
 *              the size of x i.e nx must be 3 if xy_type == 'r' or 4 if xy_type == 'i'
 *              str if non null gives the string to be used at ticks marks 
 *              it must be then of size ny or of size y[3]+1 (if xy_type == 'r')
 *              or y[4]+1 (if xy_type == 'i') 
 *   str = char *str[] string vector, see above for size constraints 
 *              Warning str must be null terminated 
 *   subtics or subints : number of sub intervals 
 *   format : format for tick marks.
 *            format is only used if str is NULL
 *            if format is null a format is computed else format is used 
 *   fontsize and textcolor : 
 *            XXXXX : to be done 
 *   seg_flag : 0 or 1, flag which control the drawing of the segment associated to the axis 
 *            if 1 the segment is drawn 
 *-------------------------------------------------------------*/

void Sci_Axis(char pos, char xy_type, double *x, int *nx, double *y, int *ny,
              char *str[], int subtics, char *format, int fontsize, int textcolor, 
              int fontstyle, int ticscolor, char logflag, int seg_flag, int axisbuild_flag)
{
  int Nx = 0 ,Ny = 0 ,j;
  double angle=0.0,vxx,vxx1;
  int vx[2],vy[2],xm[2],ym[2];
  char c_format[5];
  integer flag=0,xx=0,yy=0,posi[2],rect[4],dash[6],trois=3;/**DJ.Abdemouche 2003**/
  integer i,barlength;
  int ns=2,iflag=0;
  integer fontid[2],fontsize_kp, narg,verbose=0,logrect[4],smallersize,color_kp; 
  integer pstyle;
  int x3, y3, xpassed = 0, ypassed = 0; /* for log. case management */
  /*** 01/07/2002 -> 11.05.04 ***/ /* F.Leray : Re-put the Djalel modif. for a better display 
                                   using tight_limits='on' and/or isoview='on' */
  double xmin = 0.0 ;
  double xmax = 0.0 ;
  double ymin = 0.0 ;
  double ymax = 0.0 ; 
  sciPointObj * psubwin = NULL;
  double pas; /* used for logarithmic grid */

  fontid[0]= fontstyle;

  psubwin = sciGetCurrentSubWin();

  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  fontsize_kp = fontid[1] ;
  if ( fontsize == -1 )
  {
    fontid[0]= 0; fontid[1]= 1;  fontsize_kp = fontid[1] ;
    fontid[0]= fontstyle;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if ( fontsize != -1 ) 
  {
    fontid[1] = fontsize ;
    fontid[0]= fontstyle;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  if ( textcolor != -1 || ticscolor != -1 ) 
  {
    C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if (logflag == 'l' )
  {
    C2F(dr)("xstringl","10",&xx,&yy,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
    smallersize=fontid[1]-2;
    C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }


  /* Pb here, dim of x and y can be lesser than 4 (ie in example_eng.tst : nx = 6 and ny = 1) F.Leray 25.02.04
  x and y are either double x[3] (resp. y[3]) or simply a double !  F.Leray 05.03.04
  So a test on x[3] (resp. y[3]) is ALWAYS bad!!
  NO!! It depends on the xy_type as follow */
  /*
  *   xy_type = 'v' (for vector) or 'r' (for range) 
  *         'v' means that tics position are given by a vector 
  *         'r' means that tics position are in a range i.e given by a vector of size 3 
  *             [min,max,number_of_intervals] 
  *         'i' means that tics positions are in a range given by four number (integers) 
  *             [k1,k2,e,number_of intervals] -> [k1*10^e,k2*10^e] */

  if(xy_type == 'i')
  {     /* Adding F.Leray 05.03.04*/
    /*F.Leray Modification on 09.03.04*/
    switch ( pos ) {
      case 'u' : case 'd' :  
        if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n')
          while (x[3]>10)  x[3]=floor(x[3]/2); 
        else{
          if(x[3] > 12){ /* F.Leray arbitrary value=12 for the moment */
            x3=(int)x[3];     /* if x[3]>12 algo is triggered to search a divisor */
            for(j=x3-1;j>1;j--)
            {
              if(x3%j == 0){
                x[3]=j; 
                xpassed = 1;
              }
            }
            if(xpassed != 1) { x[3] = 1; }
          }
        }

        break;
      case 'r' : case 'l' :
        if(pSUBWIN_FEATURE(psubwin)->logflags[1] == 'n')
          while (y[3]>10)  y[3]=floor(y[3]/2);
        else{
          if(y[3] > 12){
            y3=(int)y[3];
            for(j=y3-1;j>1;j--)
              if(y3%j == 0){
                y[3]=j;
                ypassed = 1;
              }
              if(ypassed != 1) y[3] = 1;
          }

          break;
        }
    }


  }


  /** Real to Pixel values **/
  switch ( xy_type ) 
  {
  case 'v' : Nx= *nx; Ny= *ny; break;
  case 'r' :
    switch ( pos ) {
  case 'u' : case 'd' : Nx = (int) x[2]+1; break;
  case 'r' : case 'l' : Ny = (int) y[2]+1; break;
    }
    break;
  case 'i' : 
    switch ( pos ) {
  case 'u' : case 'd' : Nx = (int) x[3]+1; break; 
  case 'r' : case 'l' : Ny = (int) y[3]+1; break;
    }
    break;
  default: 
    sciprint("Sci_Axis: wrong type argument xy_type\r\n");
  }
  switch (pos ) 
  {
  case 'u' : 
  case 'd' :
    /** Horizontal axes **/
    barlength =  (integer) (Cscale.WIRect1[3]/50.0);
    /** compute a format **/
    if (str == NULL && format == NULL )  
      switch (xy_type ) {
  case 'v' : ChoixFormatE1(c_format,x,Nx);break;
  case 'r' : ChoixFormatE (c_format,x[0],x[1],(x[1]-x[0])/x[2]);break;
  case 'i' : 
    ChoixFormatE (c_format,
      (x[0] * exp10(x[2])),
      (x[1] * exp10(x[2])),
      ((x[1] * exp10(x[2])) - (x[0] * exp10(x[2])))/x[3]); break; /* Adding F.Leray 06.05.04 */
    }
    /** the horizontal segment **/

    vx[0] =  XScale(x_convert(xy_type, x , 0)); /* C EST LA que se calcule les positions initiales et finales (en x et y) de la barre support de l'axe des abscisses */
    vx[1] =  XScale(x_convert(xy_type, x , Nx-1));
    vy[0]= vy[1] = ym[0] = YScale(y[0]);
    if ( seg_flag == 1) 
    {
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    /******* 01/07/2002  -> 11.05.04 **********/
    psubwin = sciGetCurrentSubWin();
    if ((pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
      && axisbuild_flag == 0){{ 
        xmax=Cscale.frect[2];
        xmin=Cscale.frect[0];

        if(xy_type == 'i')
        {


          x[1] = floor(  Cscale.xtics[1] );
          x[0] =  ceil(  Cscale.xtics[0] );
          x[3]=inint(x[1]-x[0]);
          while (x[3]>10)  x[3]=floor(x[3]/2);
          Nx=(int)(x[3]+1);

          /* re-compute a format when tight_limits or isoview == ON */
          ChoixFormatE (c_format,
            (x[0] * exp10(x[2])),
            (x[1] * exp10(x[2])),
            ((x[1] * exp10(x[2])) - (x[0] * exp10(x[2])))/x[3]); /* Adding F.Leray 15.05.04 */
        }
        /* THESE 2 last cases are unreachable because we use the condition : */
        /* axisbuild_flag == 0 */
        else if (xy_type == 'r') /* normally unreachable */
        {
          sciprint(" Normally, unreachable case \n");
        }
        else if(xy_type == 'v') /* normally unreachable */
        {
          sciprint(" Normally, unavailable case  AAA \n");
        }
    }


    pas = ((double) Cscale.WIRect1[2]) / ((double) Cscale.Waaint1[1]);

    /**********************/
    /** loop on the ticks **/
    if (Nx==1) break;
    for (i=0 ; i < Nx ; i++)
    {  
      char foo[100];
      vxx = x_convert(xy_type,x,i);
      if ( str != NULL)  
        sprintf(foo,"%s",str[i]);
      else if ( format == NULL) 
      {
        /* defaults format */
        /*F.Leray 19.05.04 */
        /* When Sci_Axis is called by aplotv1, format is NULL : it can not be something else...*/
        /* format can also be NULL if non specified and coming from ConstructAxes method called in Objdrawaxis (see matdes.c) */
        sprintf(foo,c_format,vxx);
      }
      else {
        sprintf(foo,format,vxx);
      }


      C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */

      vx[0] = vx[1] = xm[0] =  XScale(vxx);
      posi[0] = inint( xm[0] -rect[2]/2.0);

      if ( pos == 'd' ) 
      {
        posi[1]=inint( ym[0] + 1.2*barlength + rect[3]);
        vy[0]= ym[0];vy[1]= ym[0] + barlength ;
      }
      else 
      { 
        posi[1]=inint( ym[0] - 1.2*barlength);
        vy[0]= ym[0];vy[1]= ym[0] - barlength;
      }
      if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
      if ( logflag == 'l' )
      {
        C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        C2F(dr)("xstring","10",(posi[0] -= logrect[2],&posi[0]),
          (posi[1] += logrect[3],&posi[1]),
          PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
        C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);

      /*** MAJ Djalel.A 21/01/2003 ***/ 
      if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1) 
      { /**DJ.Abdemouche 2003**/
        if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'l')
        {
          if ((vx[0] != Cscale.WIRect1[0]) && (vx[0] != (Cscale.WIRect1[0]+ Cscale.WIRect1[2])))
          {
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[0] ;
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            vy[0]=Cscale.WIRect1[1];
            vy[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];  
            C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }

          if(i < Nx -1 )
          {
            int jinit=1;
            /* In case there is a log. scale, add. sub-grid taking account of this special scale F.Leray 07.05.04 */
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[0] ;
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            vy[0]=Cscale.WIRect1[1];
            vy[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];

            if ( i== 0 ) jinit=2; /* no grid on plot boundary */
            for (j= jinit; j < 10 ; j++)
            {
              vx[0]=vx[1]= Cscale.WIRect1[0] + inint( ((double) i)*pas)+ inint(log10(((double)j))*pas);
              C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
          }
        }
        else if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'n')
        {
          if ((vx[0] != Cscale.WIRect1[0]) && (vx[0] != (Cscale.WIRect1[0]+ Cscale.WIRect1[2])))
          {
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[0] ;
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            vy[0]=Cscale.WIRect1[1];
            vy[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];  
            C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
      }
      /***/
      /* subtics */
      if ( i < Nx-1 ) 
      {
        double dx ; 
        vxx1= x_convert(xy_type,x,i+1);
        dx = (vxx1-vxx)/subtics;
        for ( j = 1 ; j < subtics; j++){ 
          vx[0] = vx[1] = XScale(vxx+dx*j);
          if ( pos == 'd' ) 
          { vy[0]= ym[0];vy[1]=  (integer) (ym[0] + barlength/2.0) ; }
          else 
          { vy[0]= ym[0];vy[1]=  (integer) (ym[0] - barlength/2.0) ; }
          C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
      } 
      /***   01/07/2002  -> 11.05.04 ****/
      psubwin = sciGetCurrentSubWin();
      if ( (pSUBWIN_FEATURE (psubwin)->tight_limits || pSUBWIN_FEATURE (psubwin)->isoview )
        && axisbuild_flag == 0)
      {  
        if ( i == 0 ) 
        {
          double dx ; 
          vxx1= x_convert(xy_type,x,i+1);
          dx = (vxx1-vxx)/subtics;
          for ( j = 1 ; j < subtics; j++) {  
            if ( vxx-dx*j > xmin){
              vx[0] = vx[1] = XScale(vxx-dx*j);
              if ( pos == 'd' ) 
              { vy[0]= ym[0];vy[1]=(int) (ym[0] + barlength/2.0) ; }
              else 
              { vy[0]= ym[0];vy[1]= (int)(ym[0] - barlength/2.0); }
              C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }}
        } 
        if ( i == Nx-1 ) 
        {
          double dx ; 
          vxx1= x_convert(xy_type,x,i+1);
          dx = (vxx1-vxx)/subtics;
          for ( j = 1 ; j < subtics; j++) {  
            if ( vxx+dx*j < xmax){
              vx[0] = vx[1] = XScale(vxx+dx*j);
              if ( pos == 'd' ) 
              { vy[0]= ym[0];vy[1]= (int)(ym[0] + barlength/2.0) ; }
              else 
              { vy[0]= ym[0];vy[1]= (int)(ym[0] - barlength/2.0); }
              C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }}
        } 
      }
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    break;
  case 'r' : 
  case 'l' :

    /** Vertical axes **/
    barlength =  (integer) (Cscale.WIRect1[2]/75.0);
    if (str == NULL &&  format == NULL )  
      switch (xy_type ) {
  case 'v' : ChoixFormatE1(c_format,y,Ny);break;
  case 'r' : ChoixFormatE(c_format,y[0],y[1],(y[1]-y[0])/y[2]);break;
  case 'i' : 
    ChoixFormatE (c_format,
      (y[0] * exp10(y[2])),
      (y[1] * exp10(y[2])),
      ((y[1] * exp10(y[2])) - (y[0] * exp10(y[2])))/y[3]); break; /* Adding F.Leray 06.05.04 */
    }
    /** the vertical segment **/
    vy[0] =  YScale(y_convert(xy_type, y , 0));
    vy[1] =  YScale(y_convert(xy_type, y , Ny-1));
    vx[0]= vx[1] = xm[0]= XScale(x[0]);
    if ( seg_flag == 1) 
    {
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }

    /***   01/07/2002 -> 11 and 12.05.04 ****/
    psubwin = sciGetCurrentSubWin();
    if ((pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
      && axisbuild_flag == 0){  
        ymax=Cscale.frect[3];
        ymin=Cscale.frect[1];

        if(xy_type == 'i')
        {
          /*   y[1] = floor(Cscale.frect[3] / (exp10( y[2]))) ;   */
          /* 	    y[0] =  ceil(Cscale.frect[1]  / (exp10( y[2]))) ;  */

          y[1] = floor( Cscale.ytics[1] );
          y[0] =  ceil( Cscale.ytics[0] );

          y[3]=inint(y[1]-y[0]);
          while (y[3]>10)  y[3]=floor(y[3]/2);
          Ny=(int)(y[3]+1);

          /* re-compute a format when tight_limits or isoview == ON */
          ChoixFormatE (c_format,
            (y[0] * exp10(y[2])),
            (y[1] * exp10(y[2])),
            ((y[1] * exp10(y[2])) - (y[0] * exp10(y[2])))/y[3]);
        }
        /* THESE 2 last cases are unreachable because we use the condition : */
        /* axisbuild_flag == 0 */
        else if (xy_type == 'r') /* normally unreachable */
        {
          sciprint(" Normally, unreachable case \n");
        }
        else if(xy_type == 'v')  /* normally unreachable */
        {
          sciprint(" Normally, unreachable case \n");
        }
    }



    pas = ((double) Cscale.WIRect1[3]) / ((double) Cscale.Waaint1[3]);
    /** loop on the ticks **/
    if (Ny==1) break; /*D.Abdemouche 16/12/2003*/
    for (i=0 ; i < Ny ; i++)
    { 
      char foo[100];
      vxx = y_convert(xy_type,y,i);
      if ( str != NULL)  
        sprintf(foo,"%s",str[i]);
      else if ( format == NULL)
      { 
        /* defaults format */
        /*F.Leray 19.05.04 */
        /* When Sci_Axis is called by aplotv1, format is NULL : it can not be something else...*/
        /* format can also be NULL if non specified and coming from ConstructAxes method called in Objdrawaxis (see matdes.c) */
        sprintf(foo,c_format,vxx);
      }
      else{ 
        sprintf(foo,format,vxx);
      }


      C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */

      vy[0]= vy[1] = ym[0] = YScale(vxx);
      posi[1]=inint( ym[0] +rect[3]/2.0);
      if ( pos == 'r' ) 
      {
        posi[0]=inint( xm[0] + 1.2*barlength);
        vx[0]= xm[0];vx[1]= xm[0]+barlength;
      }
      else 
      { 
        posi[0]=inint(xm[0] - 1.2*barlength - rect[2]);
        vx[0]= xm[0];vx[1]= xm[0] - barlength;
      }
      if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
      if ( logflag == 'l' )
      {
        C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        C2F(dr)("xstring","10",(posi[0] -= logrect[2],&posi[0]),
          (posi[1] += logrect[3],&posi[1]),
          PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
        C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
      /*** MAJ Djalel.A 21/01/2003 ***/ 
      if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1 )
      { /**DJ.Abdemouche 2003**/
        if(pSUBWIN_FEATURE (psubwin)->logflags[1] == 'l')
        {
          if ((vy[0] != Cscale.WIRect1[1]) && (vy[0] != (Cscale.WIRect1[1]+ Cscale.WIRect1[3])))
          {
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[1] ;
            vx[0]=Cscale.WIRect1[0];
            vx[1]=Cscale.WIRect1[0]+Cscale.WIRect1[2];  
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }

          if(i < Ny -1 ){
            int jinit=1;
            /* In case there is a log. scale, add. sub-grid taking account of this special scale F.Leray 07.05.04 */
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[1] ;
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            vx[0]=Cscale.WIRect1[0];
            vx[1]=Cscale.WIRect1[0]+Cscale.WIRect1[2]; 

            if ( i== Cscale.Waaint1[3]-1 ) jinit=2; /* no grid on plot boundary */
            for (j= jinit; j < 10 ; j++)
            {
              vy[0]=vy[1]= Cscale.WIRect1[1] + inint( ((double) i+1)*pas)- inint(log10(((double)j))*pas);
              C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
        else  if(pSUBWIN_FEATURE (psubwin)->logflags[1] == 'n'){
          if ((vy[0] != Cscale.WIRect1[1]) && (vy[0] != (Cscale.WIRect1[1]+ Cscale.WIRect1[3])))
          {
            pstyle=pSUBWIN_FEATURE (psubwin)->grid[1] ;
            vx[0]=Cscale.WIRect1[0];
            vx[1]=Cscale.WIRect1[0]+Cscale.WIRect1[2];  
            C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
      }
      /* subtics */
      if ( i < Ny-1 ) 
      {
        double dy ; 
        vxx1= y_convert(xy_type,y,i+1);
        dy = (vxx1-vxx)/subtics;
        for ( j = 1 ; j < subtics; j++) 
        {
          vy[0] = vy[1] = YScale(vxx+dy*j);
          if ( pos == 'r' ) 
          { vx[0]= xm[0];vx[1]= (integer) (xm[0] + barlength/2.0) ; }
          else 
          { vx[0]= xm[0];vx[1]= (integer) (xm[0] - barlength/2.0) ; }
          C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
      }
      /***   01/07/2002 -> 12.05.04 ****/
      psubwin = sciGetCurrentSubWin();
      if ( (pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
        && axisbuild_flag == 0){
          if ( i == 0 )  
          {
            double dy ; 
            vxx1= y_convert(xy_type,y,i+1);
            dy = (vxx1-vxx)/subtics;
            for ( j = 1 ; j < subtics; j++) {  
              if ( vxx-dy*j > ymin){
                vy[0] = vy[1] = YScale(vxx-dy*j);
                if ( pos == 'r' ) 
                { vx[0]= xm[0];vx[1]= (int)(xm[0] + barlength/2.0) ; }
                else 
                { vx[0]= xm[0];vx[1]= (int) (xm[0] - barlength/2.0); }
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }}
          }
          if ( i == Ny-1 )
          {
            double dy ; 
            vxx1= y_convert(xy_type,y,i+1);
            dy = (vxx1-vxx)/subtics;
            for ( j = 1 ; j < subtics; j++) {  
              if ( vxx+dy*j < ymax){
                vy[0] = vy[1] = YScale(vxx+dy*j);
                if ( pos == 'r' ) 
                { vx[0]= xm[0];vx[1]= (int)(xm[0] + barlength/2.0) ; }
                else 
                { vx[0]= xm[0];vx[1]= (int)(xm[0] - barlength/2.0); }
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }}
          }  
      }
      if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    break;
    }
    /* reset font to its current size */ 
    if ( fontsize != -1 || logflag == 'l' )
    {
      fontid[1] = fontsize_kp;
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    /* reset to current color */
    if ( textcolor != -1 || ticscolor != -1 ) 
    {
      C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }

  }

} 

/* from double to pixel */ 

static double  x_convert(char xy_type, double *x , int i)
{
  switch ( xy_type ) { 
  case 'v' :  return x[i];
  case 'r' :  return x[0]+i*(x[1]-x[0])/x[2];
  case 'i' :  return exp10(x[2])*(x[0] + i*(x[1]-x[0])/x[3]);
  }
  return 0.0;
}

static double y_convert(char xy_type, double *y , int i)
{
  switch ( xy_type ) { 
  case 'v' :  return y[i]; 
  case 'r' :  return y[0]+i*(y[1]-y[0])/y[2];
  case 'i' :  return exp10(y[2])*(y[0] + i*(y[1]-y[0])/y[3]); 
  }
  return 0.0; 
}

static void axesplot(char * strflag, sciPointObj * psubwin)
{
  char dir = 'l';
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  int seg=0;
  int fontsize = -1 ,textcolor = -1 ,ticscolor = -1 ; /* default values */
  int fontstyle= 0;
  double  x1,y1_;
  char xstr,ystr; 
  char dirx = 'd';


  int lastxindex = 0, lastyindex = 0;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  if( ppsubwin->tight_limits == TRUE || ppsubwin->isoview == TRUE)
    {
      switch ( c )
	{
	case '3' : /* right axis */
	  x1= ppsubwin->axes.xlim[1];
	  y1_= ppsubwin->axes.ylim[0];
	  dir = 'r';
	  break;
	case '4' : /* centred axis */
	  seg=1;
	  x1= (ppsubwin->axes.xlim[0]+ppsubwin->axes.xlim[1])/2.0;
	  y1_= (ppsubwin->axes.ylim[0]+ppsubwin->axes.ylim[1])/2.0;
	  break ;
	case '5': /* centred at (0,0) */
	  seg=1;
	  x1 = y1_ = 0.0;
	  break;
	case '1' : /* left axis */
	default :
	  x1=  ppsubwin->axes.xlim[0];
	  y1_=  ppsubwin->axes.ylim[1];
	  break;
	}
    }
  else  /* tight_limits == FALSE */
    {
      lastxindex = ppsubwin->axes.nxgrads - 1;
      lastyindex = ppsubwin->axes.nygrads - 1;
     
        switch ( c )
	{
	case '3' : /* right axis */
	  x1= ppsubwin->axes.xgrads[lastxindex];
	  y1_= ppsubwin->axes.ygrads[0];
	  dir = 'r';
	  break;
	case '4' : /* centred axis */
	  seg=1;
	  x1= (ppsubwin->axes.xgrads[0]+ppsubwin->axes.xgrads[lastxindex])/2.0;
	  y1_= (ppsubwin->axes.ygrads[0]+ppsubwin->axes.ygrads[lastyindex])/2.0;
	  break ;
	case '5': /* centred at (0,0) */
	  seg=1;
	  x1 = y1_ = 0.0;
	  break;
	case '1' : /* left axis */
	default :
	  x1= ppsubwin->axes.xgrads[0];
	  y1_= ppsubwin->axes.ygrads[0];
	break;
	}
    }
       


  if(ppsubwin->tight_limits == TRUE || ppsubwin->isoview == TRUE)
    {
      if (c != '4')
	{
	  xstr=ppsubwin->axes.xdir;
	  if(ppsubwin->axes.reverse[1] == FALSE)
	    { /* y reverse axis : NO */
	      switch (xstr)
		{
		case 'u':
		  y1_= ppsubwin->axes.ylim[1];
		  dirx='u';
		  break;
		case 'c':
		  y1_= (ppsubwin->axes.ylim[0]>0.0)?ppsubwin->axes.ylim[0]:0.0;
		  y1_= (ppsubwin->axes.ylim[1]<0.0)?ppsubwin->axes.ylim[0]:y1_;
		  seg =1;
		  dirx ='d';
		  break;
		default :
		  y1_= ppsubwin->axes.ylim[0];
		  dirx ='d';
		  break;
		}
	    }
	  else
	    { /* y reverse axis : YES */
	      switch (xstr)
		{
		case 'u':
		  y1_= ppsubwin->axes.ylim[0];
		  dirx='u';
		  break;
		case 'c':
		  y1_= (ppsubwin->axes.ylim[0]>0.0)?ppsubwin->axes.ylim[0]:0.0;
		  y1_= (ppsubwin->axes.ylim[1]<0.0)?ppsubwin->axes.ylim[0]:y1_;
		  seg =1;
		  dirx ='d';
		  break;
		default :
		  y1_= ppsubwin->axes.ylim[1];
		  dirx ='d';
		  break;
		}
	    }

	  ystr=ppsubwin->axes.ydir;
	  if(ppsubwin->axes.reverse[0] == FALSE)
	    { /* x reverse axis : NO */
	      switch (ystr)
		{
		case 'r':
		  x1= ppsubwin->axes.xlim[1];
		  dir='r';
		  break;
		case 'c':
		  x1=(ppsubwin->axes.xlim[0]>0.0)?ppsubwin->axes.xlim[0]:0.0;
		  x1=(ppsubwin->axes.xlim[1]<0.0)?ppsubwin->axes.xlim[0]:x1;
		  seg =1;
		  dir ='l';
		  break;
		default :
		  x1= ppsubwin->axes.xlim[0];
		  dir ='l';
		  break;
		}
	    }
	  else
	    { /* x reverse axis : YES */
	      switch (ystr)
		{
		case 'r':
		  x1= ppsubwin->axes.xlim[0];
		  dir='r';
		  break;
		case 'c':
		  x1=(ppsubwin->axes.xlim[0]>0.0)?ppsubwin->axes.xlim[0]:0.0;
		  x1=(ppsubwin->axes.xlim[1]<0.0)?ppsubwin->axes.xlim[0]:x1;
		  seg =1;
		  dir ='l';
		  break;
		default :
		  x1= ppsubwin->axes.xlim[1];
		  dir ='l';
		  break;
		}
	    }
	}
    }
  else  /* tight_limits == FALSE */
    {
      lastxindex = ppsubwin->axes.nxgrads - 1;
      lastyindex = ppsubwin->axes.nygrads - 1;
      
      if (c != '4')
	{
	  xstr=ppsubwin->axes.xdir;
	  if(ppsubwin->axes.reverse[1] == FALSE)
	    { /* y reverse axis : NO */
	      switch (xstr)
		{
		case 'u':
		  y1_=ppsubwin->axes.ygrads[lastyindex];
		  dirx='u';
		  break;
		case 'c':
		  y1_=(ppsubwin->axes.ygrads[0]>0.0)?ppsubwin->axes.ygrads[0]:0.0;
		  y1_=(ppsubwin->axes.ygrads[lastyindex]<0.0)?ppsubwin->axes.ygrads[0]:y1_;
		  seg =1;
		  dirx ='d';
	      break;
		default :
		  y1_= ppsubwin->axes.ygrads[0];
		  dirx ='d';
		  break;
		}
	    }
	  else
	    { /* y reverse axis : YES */
	      switch (xstr)
		{
		case 'u':
		  y1_=ppsubwin->axes.ygrads[0];
		  dirx='u';
		  break;
		case 'c':
		  y1_=(ppsubwin->axes.ygrads[0]>0.0)?ppsubwin->axes.ygrads[0]:0.0;
		  y1_=(ppsubwin->axes.ygrads[lastyindex]<0.0)?ppsubwin->axes.ygrads[0]:y1_;
		  seg =1;
		  dirx ='d';
		  break;
		default :
		  y1_= ppsubwin->axes.ygrads[lastyindex];
		  dirx ='d';
		  break;
		}
	    }
	  
	  ystr=ppsubwin->axes.ydir;
	  if(ppsubwin->axes.reverse[0] == FALSE)
	    { /* x reverse axis : NO */
	      switch (ystr)
		{
		case 'r':
		  x1= ppsubwin->axes.xgrads[lastxindex];
		  dir='r';
		  break;
		case 'c':
		  x1=(ppsubwin->axes.xgrads[0]>0.0)?ppsubwin->axes.xgrads[0]:0.0;
		  x1=(ppsubwin->axes.xgrads[lastxindex]<0.0)?ppsubwin->axes.xgrads[0]:x1;
		  seg =1;
		  dir ='l';
		  break;
		default :
		  x1= ppsubwin->axes.xgrads[0];
		  dir ='l';
		  break;
		}
	    }
	  else
	    { /* x reverse axis : YES */
	      switch (ystr)
		{
		case 'r':
		  x1= ppsubwin->axes.xgrads[0];
		  dir='r';
		  break;
		case 'c':
		  x1=(ppsubwin->axes.xgrads[0]>0.0)?ppsubwin->axes.xgrads[0]:0.0;
		  x1=(ppsubwin->axes.xgrads[lastxindex]<0.0)?ppsubwin->axes.xgrads[0]:x1;
		  seg =1;
		  dir ='l';
		  break;
		default :
		  x1= ppsubwin->axes.xgrads[lastxindex];
		  dir ='l';
		  break;
		}
	    }
	}
    }
  
  ticscolor=sciGetForeground(psubwin);
  textcolor=sciGetFontForeground(psubwin);
  fontsize=sciGetFontDeciWidth(psubwin)/100;
  fontstyle=sciGetFontStyle(psubwin);
 
  /** x-axis **/
  SciAxisNew(dirx,psubwin,y1_,fontsize,fontstyle,textcolor,ticscolor,seg);
  
  /** y-axis **/
  SciAxisNew(dir,psubwin,x1,fontsize,fontstyle,textcolor,ticscolor,seg);

  /* Once the 2 axes are plotted, we can draw :
     1. the axes lines
     2. the box lines over if necessary (i.e. seg == 1) */
  if ( ppsubwin->axes.axes_visible[0] )
  {
    SciDrawLines(dirx,psubwin,y1_,textcolor,ticscolor);
  }
  SciDrawLines(dir, psubwin,x1,textcolor,ticscolor);
  
  
  if ( c != '4' && c != '5' )
  {
    if ( sciGetBoxType(psubwin) == BT_OFF || sciGetBoxType(psubwin) == BT_HIDDEN_AXIS )
    {
      seg = 1 ;
    }
    else
    {
      /** frame rectangle **/
      C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3], 
	      PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  }
}


/****************************************************************************/
/********************************** COMMON to X and Y ***********************/
/****************************************************************************/


int SciDrawLines(char pos, sciPointObj * psubwin, double xy, int textcolor,int ticscolor)
{
  double xminval, yminval, xmaxval, ymaxval;
  int verbose = 0, narg, color_kp;

  if ( textcolor != -1 || ticscolor != -1 ) 
    {
      C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  
  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval);

  if(pos=='u' || pos=='d'){ /* X */
    if( pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] == TRUE )
      XDrawAxisLine(xminval,xmaxval,xy,ticscolor,color_kp);
  } else if(pos=='l' || pos=='r'){ /* Y */
    if(pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] == TRUE)
      YDrawAxisLine(yminval,ymaxval,xy,ticscolor,color_kp);
  }
  
  
  
  
  return 0;
}

/****************************************************************************/
/********************************** X ***************************************/
/****************************************************************************/


static int DrawXSubTics(char pos, sciPointObj * psubwin, double xy,int ticscolor,int color_kp)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;
   
  int vx[2],vy[2],ym[2];
  int barlength = 0;
  int ns=2,iflag=0;
  
  int nbtics = 0,i,j;
  int nbsubtics = ppsubwin->axes.nbsubtics[0];
  char logflag = ppsubwin->logflags[0];
  
  double *grads = (double *) NULL;

  ym[0] = YScale(xy);
   
  barlength =  (integer) (Cscale.WIRect1[3]/50.0);
  
  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need x data (ymin ymax are computed but not used after) */
 

  if(ppsubwin->axes.auto_ticks[0] == FALSE){
    grads =  ppsubwin->axes.u_xgrads;
    nbtics = ppsubwin->axes.u_nxgrads;
  }
  else{
    grads =  ppsubwin->axes.xgrads;
    nbtics = ppsubwin->axes.nxgrads;
  }
  
  
  if(logflag =='l')
    {
      double tmp[2];
      double pas=0;
      double * tmp_log_grads = (double *) NULL;
      
      for(i=0;i<nbtics-1;i++)
	{
	  int k;
	  tmp[0] = exp10(grads[i]);
	  tmp[1] = exp10(grads[i+1]);
	  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
	  
	  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
	    sciprint("Error allocating tmp_log_grads\n");
	    return -1;
	  }
	  
	  for(k=0;k<nbsubtics;k++) tmp_log_grads[k] = log10(tmp[0]+(k)*pas);
	  
	  for(j=0;j<nbsubtics;j++)
	    {
	      double val = tmp_log_grads[j];
              /* the length of the ticks in pixels */
              /* minimum 2 pixels */
              /* use of integer division for speed */
              int ticksLength =  Max( ( 3 * barlength ) / 4,  2 ) ;
	      vx[0] = vx[1] = XScale(val);
	      
	      if(val<xminval || val>xmaxval) continue;	   
	      
	      if ( pos == 'd' ) 
              { 
                vy[0] = ym[0] ;
                vy[1] = ym[0] + ticksLength ; 
              }
	      else 
              { 
                vy[0] = ym[0] ; 
                vy[1] = ym[0] - ticksLength ;
              }
              if ( ppsubwin->axes.axes_visible[0] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	  
	  FREE(tmp_log_grads); tmp_log_grads = NULL;
	}
    }
  else
    {
      for(i=0;i<nbtics-1;i++)
	{
	  double xtmp = grads[i];
	  double dx = (grads[i+1]-grads[i]) / nbsubtics;
	  for( j = 0 ; j < nbsubtics ; j++ )
	    {
	      double val = xtmp+dx*j;
              /* the length of the ticks in pixels */
              /* minimum 2 pixels */
              /* use of integer division for speed */
              int ticksLength =  Max( ( 3 * barlength ) / 4,  2 ) ;
	      vx[0] = vx[1] = XScale(val);
	      
	      if(val<xminval || val>xmaxval) continue;	   
	      
	      if ( pos == 'd' ) 
	      {
                
                vy[0]= ym[0];
                vy[1]= ym[0] + ticksLength ; 
              }
	      else 
              { 
                vy[0]= ym[0];
                vy[1]= ym[0] - ticksLength ; 
              }
              if ( ppsubwin->axes.axes_visible[0] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	}
    }
  
  return 0;
}


static int DrawXGrid(sciPointObj * psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;
  int verbose=0,narg;

  int vx[2],vy[2],ym[2];
  int dash[6],lineMode=2;
  int ns=2,style=0,iflag=0;
  
  double * grads = (double *) NULL;
  int nbtics = 0,i,j;
  int nbsubtics = ppsubwin->axes.nbsubtics[0];
  char logflag = ppsubwin->logflags[0];
   
  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval);
  
  
  ym[0] = YScale(yminval);
  ym[1] = YScale(ymaxval);

  if(ppsubwin->axes.auto_ticks[0] == FALSE){
    grads =  ppsubwin->axes.u_xgrads;
    nbtics = ppsubwin->axes.u_nxgrads;
  }
  else{
    grads =  ppsubwin->axes.xgrads;
    nbtics = ppsubwin->axes.nxgrads;
  }
  
  /* Grid style */
  C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "line style",&lineMode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  style = ppsubwin->grid[0];
  
  /* Grid based on tics */
  for(i=0;i<nbtics;i++)
    {
      double xtmp = grads[i];
      vx[0] = vx[1] = XScale(xtmp);
      
      if(xtmp<xminval || xtmp>xmaxval) continue;	   
      
      vy[0]= ym[0];vy[1]=  ym[1];
      if ( ppsubwin->axes.axes_visible[0] )
      {
        C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  
  /* Grid based on subtics : ONLY for log. case */
  if(logflag =='l')
    {
      double tmp[2];
      double pas=0;
      double * tmp_log_grads = (double *) NULL;
      
      for(i=0;i<nbtics-1;i++)
	{
	  int k;
	  tmp[0] = exp10(grads[i]);
	  tmp[1] = exp10(grads[i+1]);
	  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
	  
	  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
	    sciprint("Error allocating tmp_log_grads\n");
	    return -1;
	  }
	  
	  for(k=0;k<nbsubtics;k++) tmp_log_grads[k] = log10(tmp[0]+(k)*pas);
	  
	  for(j=0;j<nbsubtics;j++)
	    {
	      double val = tmp_log_grads[j];
	      vx[0] = vx[1] = XScale(val);
	      
	      if(val<xminval || val>xmaxval) continue;	  
	      
	      vy[0]= ym[0];vy[1]=  ym[1];
	      if ( ppsubwin->axes.axes_visible[0] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	  
	  FREE(tmp_log_grads); tmp_log_grads = NULL;
	}
    }
 
  /* return to solid mode (in default mode) */
  C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  

  return 0;
}



static void DrawXTics(char pos, sciPointObj * psubwin, double xy, char * c_format, int * fontid, int textcolor,int ticscolor,int color_kp, int *logrect, int smallersize)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;
  int  flag=0,xx=0,yy=0,rect[4];
  
  int vx[2],vy[2],xm[2],ym[2];
  int barlength = 0;
  int posi[2];
  double angle=0.0;
  int ns=2,iflag=0;
  
  int nbtics = 0,i;
  char logflag = ppsubwin->logflags[0];

  ym[0] = YScale(xy);
 
  barlength =  Max( (integer) (Cscale.WIRect1[3]/50.0), 2 ) ;
  
  /*ppsubwin->firsttime_x = TRUE;*/
  
  if(ppsubwin->axes.auto_ticks[0] == FALSE)
    {
      /* we display the x tics specified by the user*/
      FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need x data (ymin ymax are computed but not used after) */
      nbtics = ppsubwin->axes.u_nxgrads;
      
      for(i=0;i<nbtics;i++)
	{
	  double xtmp = ppsubwin->axes.u_xgrads[i];
	  char *foo = ppsubwin->axes.u_xlabels[i];
	  
	  if(xtmp<xminval || xtmp>xmaxval) continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichées de tte facon */
	                                             /* donc autant ne pas aller plus loin dans l'algo... */
	  

	  /***************************************************************/
	  /************************* COMMON PART *************************/
	  /***************************************************************/
	  
	  C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */
	  
	  vx[0] = vx[1] = xm[0] =  XScale(xtmp);
	  posi[0] = inint( xm[0] -rect[2]/2.0);
	  
	  if ( pos == 'd')
	    {
	      posi[1]=inint( ym[0] + 1.2*barlength + rect[3]);
	      vy[0]= ym[0];vy[1]= ym[0] + barlength ;
	    }
	  else 
	    {
	      posi[1]=inint( ym[0] - 1.2*barlength);
	      vy[0]= ym[0];vy[1]= ym[0] - barlength;
	    }
          
	  if ( ppsubwin->axes.axes_visible[0] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
          }
	  XGradPosition(psubwin,posi[1],rect[3]);

          if ( ppsubwin->axes.axes_visible[0] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            /* le tic proprement dit ("petit baton") */
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /***************************************************************/
	  /************************* END COMMON PART *********************/
	  /***************************************************************/
	  
	}
      
    }
  else
    {
      
      FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need x data (ymin ymax are computed but not used after) */
      nbtics = ppsubwin->axes.nxgrads;
      

      for(i=0;i<nbtics;i++)
	{
	  char foo[256]; 
	  double xtmp = ppsubwin->axes.xgrads[i];
          
          /* chack that xtmp < xminval || xtmp > xmalval */
          /* but if xtmp is very close to xminval or xmaxval we display it */
          if ( !SAFE_LT( xminval, xtmp, EPSILON ) || !SAFE_LT( xtmp, xmaxval, EPSILON ) )
          {
            continue ;
          }

	  sprintf(foo,c_format,xtmp);
	  
	  /***************************************************************/
	  /************************* COMMON PART *************************/
	  /***************************************************************/

	  C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */
	  
	  vx[0] = vx[1] = xm[0] =  XScale(xtmp);
	  posi[0] = inint( xm[0] -rect[2]/2.0);
	  
	  if ( pos == 'd')
	    {
	      posi[1]=inint( ym[0] + 1.2*barlength + rect[3]);
	      vy[0]= ym[0];vy[1]= ym[0] + barlength ;
	    }
	  else 
	    {
	      posi[1]=inint( ym[0] - 1.2*barlength);
	      vy[0]= ym[0];vy[1]= ym[0] - barlength;
	    }
	  
	  if ( ppsubwin->axes.axes_visible[0] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

            
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
          }
	  XGradPosition(psubwin,posi[1],rect[3]);
	  if ( logflag == 'l' )
	    {
              if ( ppsubwin->axes.axes_visible[0] )
              {
                C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                
                C2F(dr)("xstring","10",(posi[0] -= logrect[2],&posi[0]),
                        (posi[1] += logrect[3],&posi[1]),
                        PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
              }
	      XGradPosition(psubwin,posi[1],rect[3]);
              if ( ppsubwin->axes.axes_visible[0] )
              {
                C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
          if ( ppsubwin->axes.axes_visible[0] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ticscolor != -1 )
            {
              C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            
            /* le tic proprement dit ("petit baton") */
	    C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L); 
            
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
	  /***************************************************************/
	  /************************* END COMMON PART *********************/
	  /***************************************************************/


	}
      

    }

}


/****************************************************************************/
/********************************** Y ***************************************/
/****************************************************************************/



static int DrawYSubTics(char pos, sciPointObj * psubwin, double xy,int ticscolor,int color_kp)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;

  int vx[2],vy[2],xm[2];
  int barlength = 0;
 int ns=2,iflag=0;
  
  int nbtics = 0,i,j;
  int nbsubtics = ppsubwin->axes.nbsubtics[1];
  char logflag = ppsubwin->logflags[1];
  
  double *grads = (double *) NULL;

  xm[0] = XScale(xy);
   
  barlength =  (integer) (Cscale.WIRect1[2]/75.0);
  
  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need x data (ymin ymax are computed but not used after) */
 

  if(ppsubwin->axes.auto_ticks[1] == FALSE){
    grads =  ppsubwin->axes.u_ygrads;
    nbtics = ppsubwin->axes.u_nygrads;
  }
  else{
    grads =  ppsubwin->axes.ygrads;
    nbtics = ppsubwin->axes.nygrads;
  }
  
  
  if(logflag =='l')
    {
      double tmp[2];
      double pas=0;
      double * tmp_log_grads = (double *) NULL;
      
      for(i=0;i<nbtics-1;i++)
	{
	  int k;
	  tmp[0] = exp10(grads[i]);
	  tmp[1] = exp10(grads[i+1]);
	  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
	  
	  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
	    sciprint("Error allocating tmp_log_grads\n");
	    return -1;
	  }
	  
	  for(k=0;k<nbsubtics;k++) tmp_log_grads[k] = log10(tmp[0]+(k)*pas);
	  
	  for(j=0;j<nbsubtics;j++)
	    {
	      double val = tmp_log_grads[j];
              /* the length of the ticks in pixels */
              /* minimum 2 pixels */
              /* use of integer division for speed */
              int ticksLength =  Max( ( 3 * barlength ) / 4,  1 ) ;
	      vy[0] = vy[1] = YScale(val);
	      
	      if(val<yminval || val>ymaxval) continue;	   
	      
	      if ( pos == 'r' ) 
              { 
                vx[0] = xm[0] ;
                vx[1] = xm[0] + ticksLength ; 
              }
	      else 
              { 
                vx[0] = xm[0] ;
                vx[1] = xm[0] - ticksLength ; 
              }
              if ( ppsubwin->axes.axes_visible[1] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	  
	  FREE(tmp_log_grads); tmp_log_grads = NULL;
	}
    }
  else
    {
      for(i=0;i<nbtics-1;i++)
	{
	  double xtmp = grads[i];
	  double dx = (grads[i+1]-grads[i]) / nbsubtics;
	  for(j=0;j<nbsubtics;j++)
	    {
	      double val = xtmp+dx*j;
              /* the length of the ticks in pixels */
              /* minimum 2 pixels */
              /* use of integer division for speed */
              int ticksLength =  Max( ( 3 * barlength ) / 4, 1 ) ;
	      vy[0] = vy[1] = YScale(val);
	      
	      if(val<yminval || val>ymaxval) continue;	   
	      
	      if ( pos == 'r' ) 
              { 
                vx[0] = xm[0] ;
                vx[1]=  xm[0] + ticksLength ; 
              }
	      else 
              { 
                vx[0] = xm[0] ;
                vx[1] = xm[0] - ticksLength ; 
              }
              if ( ppsubwin->axes.axes_visible[1] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	}
    }
  
  return 0;
}


static int DrawYGrid(sciPointObj * psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;
  int verbose=0,narg;

  int vx[2],vy[2],xm[2];
  int dash[6],lineMode=2;
  int ns=2,style=0,iflag=0;
  
  double * grads = (double *) NULL;
  int nbtics = 0,i,j;
  int nbsubtics = ppsubwin->axes.nbsubtics[1];
  char logflag = ppsubwin->logflags[1];
   
  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval);
  
  
  xm[0] = XScale(xminval);
  xm[1] = XScale(xmaxval);

  if(ppsubwin->axes.auto_ticks[1] == FALSE){
    grads =  ppsubwin->axes.u_ygrads;
    nbtics = ppsubwin->axes.u_nygrads;
  }
  else{
    grads =  ppsubwin->axes.ygrads;
    nbtics = ppsubwin->axes.nygrads;
  }
  
  /* Grid style */
  C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "line style",&lineMode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  style = ppsubwin->grid[1];
  
  /* Grid based on tics */
  for(i=0;i<nbtics;i++)
    {
      double xtmp = grads[i];
      vy[0] = vy[1] = YScale(xtmp);
      
      if(xtmp<yminval || xtmp>ymaxval) continue;	   
      
      vx[0]= xm[0];vx[1]=  xm[1];
      if ( ppsubwin->axes.axes_visible[1] )
      {
        C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  
  /* Grid based on subtics : ONLY for log. case */
  if(logflag =='l')
    {
      double tmp[2];
      double pas=0;
      double * tmp_log_grads = (double *) NULL;
      
      for(i=0;i<nbtics-1;i++)
	{
	  int k;
	  tmp[0] = exp10(grads[i]);
	  tmp[1] = exp10(grads[i+1]);
	  pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );
	  
	  if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
	    sciprint("Error allocating tmp_log_grads\n");
	    return -1;
	  }
	  
	  for(k=0;k<nbsubtics;k++) tmp_log_grads[k] = log10(tmp[0]+(k)*pas);
	  
	  for(j=0;j<nbsubtics;j++)
	    {
	      double val = tmp_log_grads[j];
	      vy[0] = vy[1] = YScale(val);
	      
	      if(val<yminval || val>ymaxval) continue;	  
	      
	      vx[0]= xm[0];vx[1]=  xm[1];
	      if ( ppsubwin->axes.axes_visible[1] )
              {
                C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
	  
	  FREE(tmp_log_grads); tmp_log_grads = NULL;
	}
    }
 

  /* return to solid mode (in default mode) */
  C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  return 0;
}





static void DrawYTics(char pos, sciPointObj * psubwin, double xy, char * c_format, int * fontid, int textcolor,int ticscolor,int color_kp, int *logrect, int smallersize)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval,yminval,xmaxval,ymaxval;
  int  flag=0,xx=0,yy=0,rect[4];
  
  int vx[2],vy[2],xm[2],ym[2];
  int barlength = 0;
  int posi[2];
  double angle=0.0;
  int ns=2,iflag=0;
  
  int nbtics = 0,i;
  char logflag = ppsubwin->logflags[1];
  
  xm[0] = XScale(xy); /* F.Leray modified le 28.09.04 */
  
  /* at least 1 pixels */
  barlength =  Max( (integer) (Cscale.WIRect1[2]/75.0), 2 ) ;
  

  if(ppsubwin->axes.auto_ticks[1] == FALSE)
    {
      /* we display the x tics specified by the user*/
      FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need y data (xmin xmax are computed but not used after) */
      nbtics = ppsubwin->axes.u_nygrads;
      
      for(i=0;i<nbtics;i++)
	{
	  double ytmp = ppsubwin->axes.u_ygrads[i];
	  char *foo = ppsubwin->axes.u_ylabels[i];
	  
	  if(ytmp<yminval || ytmp>ymaxval) continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichées de tte facon */
	                                             /* donc autant ne pas aller plus loin dans l'algo... */


	  /***************************************************************/
	  /************************* COMMON PART *************************/
	  /***************************************************************/
	  
	  C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */
	  
	  vy[0]= vy[1] = ym[0] = YScale(ytmp);
	  posi[1]=inint( ym[0] +rect[3]/2.0);
	  if ( pos == 'r' ) 
	    {
	      posi[0]=inint( xm[0] + 1.2*barlength);
	      vx[0]= xm[0];vx[1]= xm[0]+barlength;
	    }
	  else 
	    { 
	      posi[0]=inint(xm[0] - 1.2*barlength - rect[2]);
	      vx[0]= xm[0];vx[1]= xm[0] - barlength;
	    }




          if ( ppsubwin->axes.axes_visible[1] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
          }
	  YGradPosition(psubwin,posi[0],rect[2]);

          if ( ppsubwin->axes.axes_visible[1] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	  
	  /***************************************************************/
	  /************************* END COMMON PART *********************/
	  /***************************************************************/
	  
	}
    }
  else
    {

      FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval); /* here i only need y data (xmin xmax are computed but not used after) */
      nbtics = ppsubwin->axes.nygrads;
      
      for(i=0;i<nbtics;i++)
	{
	  char foo[256];
	  double ytmp = ppsubwin->axes.ygrads[i];
	  
          
          /* chack that xtmp < xminval || xtmp > xmalval */
          /* but if xtmp is very close to xminval or xmaxval we display it */
          if ( !SAFE_LT( yminval, ytmp, EPSILON ) || !SAFE_LT( ytmp, ymaxval, EPSILON ) )
          {
            continue ;
          }

	  sprintf(foo,c_format,ytmp);
	  
	  
	  /***************************************************************/
	  /************************* COMMON PART *************************/
	  /***************************************************************/
	  
	  C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* tick is computed in vx,vy and string is displayed at posi[0],posi[1] position */

	  vy[0]= vy[1] = ym[0] = YScale(ytmp);
	  posi[1]=inint( ym[0] +rect[3]/2.0);

	  if ( pos == 'r' ) 
	    {
	      posi[0]=inint( xm[0] + 1.2*barlength);
	      vx[0]= xm[0];vx[1]= xm[0]+barlength;
	    }
	  else 
	    { 
	      posi[0]=inint(xm[0] - 1.2*barlength - rect[2]);
	      vx[0]= xm[0];vx[1]= xm[0] - barlength;
	    }
          
          
          if ( ppsubwin->axes.axes_visible[1] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&angle, PD0,PD0,PD0,0L,0L);
          }
	  YGradPosition(psubwin,posi[0],rect[2]);
	  
	  if ( logflag == 'l' )
	    {
              if ( ppsubwin->axes.axes_visible[1] )
              {
                C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                
                C2F(dr)("xstring","10",(posi[0] -= logrect[2],&posi[0]),
                        (posi[1] += logrect[3],&posi[1]),
                        PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
              }
	      YGradPosition(psubwin,posi[0],rect[2]); /* adding F.Leray 04.08.05 */
	      if ( ppsubwin->axes.axes_visible[1] )
              {
                C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
	    }
          if ( ppsubwin->axes.axes_visible[1] )
          {
            if ( textcolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          
            if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  }
	  /***************************************************************/
	  /************************* END COMMON PART *********************/
	  /***************************************************************/
	  
	}
    }
}

static int XDrawAxisLine(double xminval,double xmaxval,double xy, int ticscolor, int color_kp)
{
  int vx[2], vy[2];
  int ns=2, iflag=0;
  
  vx[0] =  XScale(xminval); /* C EST LA que se calcule les positions initiales et finales (en x et y) de la barre support de l'axe des abscisses */
  vx[1] =  XScale(xmaxval);
  vy[0]= vy[1] = YScale(xy);
  
  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


  return 0;
}

static int YDrawAxisLine(double yminval,double ymaxval,double xy, int ticscolor, int color_kp)
{
  int vx[2], vy[2];
  int ns=2, iflag=0;

  vy[0] =  YScale(yminval); /* C EST LA que se calcule les positions initiales et finales (en x et y) de la barre support de l'axe des abscisses */
  vy[1] =  YScale(ymaxval);
  vx[0]= vx[1] = XScale(xy);
  
  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


  return 0;
}


void FindXYMinMaxAccordingTL(sciPointObj * psubwin, double *xminval,double *yminval,double *xmaxval,double *ymaxval)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  *xminval = ppsubwin->axes.xlim[0];
  *yminval = ppsubwin->axes.ylim[0];
  
  *xmaxval = ppsubwin->axes.xlim[1];
  *ymaxval = ppsubwin->axes.ylim[1];
}




static int SciAxisNew(char pos,sciPointObj *psubwin, double xy, int fontsize,int fontstyle,int textcolor,int ticscolor,int seg)
{
  int switchXY = -1;
  
  char c_format[5];
  integer xx=0,yy=0;
  integer barlength;
  integer fontid[2],fontsize_kp, narg,verbose=0,logrect[4],smallersize=0,color_kp; 
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double xminval, yminval, xmaxval, ymaxval;
  
  char logflag = 'n' ;
  /* int lastxindex, lastyindex; */
  BOOL auto_ticks = FALSE ;


  if(pos=='u' || pos=='d'){
    switchXY = 0;
    logflag = ppsubwin->logflags[0];
    auto_ticks = ppsubwin->axes.auto_ticks[0];
  }
  else if(pos=='l' || pos=='r'){
    switchXY = 1;
    logflag = ppsubwin->logflags[1];
    auto_ticks = ppsubwin->axes.auto_ticks[1];
  }


  fontid[0]= fontstyle;
  
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  fontsize_kp = fontid[1] ;
  if (fontsize == -1 )
    { fontid[0]= 0; fontid[1]= 1;  fontsize_kp = fontid[1] ;
    fontid[0]= fontstyle;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);}
  
  if ( fontsize != -1 ) 
    {
      fontid[1] = fontsize ;
      fontid[0]= fontstyle;
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  if ( textcolor != -1 || ticscolor != -1 ) 
    {
      C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  
  if (logflag == 'l' && auto_ticks == TRUE)
    {
      C2F(dr)("xstringl","10",&xx,&yy,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      smallersize=fontid[1]-2;
      C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  


  FindXYMinMaxAccordingTL(psubwin,&xminval,&yminval,&xmaxval,&ymaxval);


  switch (switchXY)
  {
  case 0: /* x horizontal axis */
    {
      /** Horizontal axes **/

      char formatY[5] ;
      barlength =  (integer) (Cscale.WIRect1[3]/50.0);
      
      /* remove concealing tics labels */
      
      updateXaxesTics( psubwin, pos, xy, fontid, smallersize ) ;
      
      /* the format may have changed */
      findFormat( psubwin, c_format, formatY ) ;

      /* le "loop on the ticks" */
      DrawXTics(pos, psubwin, xy, c_format, fontid, textcolor, ticscolor, color_kp, logrect, smallersize);
      
      /* subtics display*/
      DrawXSubTics(pos, psubwin, xy, ticscolor, color_kp);
    }
    break;
  case 1: /* y vertical axis */
    {
    /** Vertical axes **/
      char formatX[5] ;
      
      /* int ticsDir[2] = {1,0} ; */
      barlength =  (integer) (Cscale.WIRect1[2]/75.0);

      /* remove concealing tics labels */
      updateYaxesTics( psubwin, pos, xy, fontid, smallersize ) ;

       /* the format may have changed */
      findFormat( psubwin, formatX, c_format ) ;

      /* le "loop on the ticks" */
      DrawYTics(pos, psubwin, xy, c_format, fontid, textcolor, ticscolor, color_kp, logrect, smallersize);
      
      /* subtics display*/
      DrawYSubTics(pos, psubwin, xy, ticscolor, color_kp);
    }
    
    break;
  default:
    sciprint("Axes.c : SciAxisNew : Impossible case");
    return -1;
  }
  
  return 0;
}

void YGradPosition(sciPointObj * psubwin, int xx, int rect2)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  if(ppsubwin->firsttime_y == TRUE)
    {
      ppsubwin->YGradMostOnLeft = xx;
      ppsubwin->YGradMostOnRight = xx + rect2;
      ppsubwin->firsttime_y = FALSE;
    }
  else
    {
      if(xx < ppsubwin->YGradMostOnLeft)  ppsubwin->YGradMostOnLeft  = xx;
      if(xx + rect2 > ppsubwin->YGradMostOnRight) ppsubwin->YGradMostOnRight = xx + rect2;
    }
}

void XGradPosition(sciPointObj * psubwin, int yy, int rect3)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  if( ppsubwin->firsttime_x )
  {
    ppsubwin->XGradMostOnTop = yy - rect3 ;
    ppsubwin->XGradMostOnBottom = yy ;
    ppsubwin->firsttime_x = FALSE;
  }
  else
  {
    if(yy > ppsubwin->XGradMostOnBottom)
    {
      ppsubwin->XGradMostOnBottom  = yy ;
    }
    if( yy - rect3 < ppsubwin->XGradMostOnTop)
    {
      ppsubwin->XGradMostOnTop = yy - rect3 ;
    }
  }
}

/*--------------------------------------------------------------------------------*/
/* clear a subwindow from all of its children */
void clearSubWin( sciPointObj * pSubWin )
{
  sciSons * curSon = sciGetSons (pSubWin);
  
  while ( curSon != NULL && curSon->pointobj != NULL )
  {
    if ( curSon->pointobj->entitytype != SCI_LABEL )
    {
      DestroyAllGraphicsSons (curSon->pointobj) ;
      curSon = sciGetSons ( pSubWin ) ;
    }
    else
    {
      curSon = curSon->pnext ;
    }
    
  }
}
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position ) */
void reinitSubWin( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin) ;
  
  clearSubWin(   pSubWin ) ;
 
  initSubWinBounds( pSubWin ) ;
  ppSubWin->axes.xdir = 'd' ;
  ppSubWin->axes.ydir = 'l' ;
  
  ppSubWin->visible = TRUE;
  
  initSubWinAngles( pSubWin ) ;
  
  ppSubWin->FirstPlot = TRUE;
  

}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
void initSubWinAngles( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->is3d     = ppAxesMdl->is3d     ;
  ppSubWin->alpha_kp = ppAxesMdl->alpha_kp ;
  ppSubWin->theta_kp = ppAxesMdl->theta_kp ;
  ppSubWin->alpha    = ppAxesMdl->alpha    ;
  ppSubWin->theta    = ppAxesMdl->theta    ;
}

/*--------------------------------------------------------------------------------*/
/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->WRect[0] = ppAxesMdl->WRect[0] ;
  ppSubWin->WRect[1] = ppAxesMdl->WRect[1] ;
  ppSubWin->WRect[2] = ppAxesMdl->WRect[2] ;
  ppSubWin->WRect[3] = ppAxesMdl->WRect[3] ;

}
/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
void initSubWinBounds( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->FRect[0] = ppAxesMdl->FRect[0] ;
  ppSubWin->FRect[1] = ppAxesMdl->FRect[1] ;
  ppSubWin->FRect[2] = ppAxesMdl->FRect[2] ;
  ppSubWin->FRect[3] = ppAxesMdl->FRect[3] ;
  ppSubWin->FRect[4] = ppAxesMdl->FRect[4] ;
  ppSubWin->FRect[5] = ppAxesMdl->FRect[5] ;

  ppSubWin->SRect[0] = ppAxesMdl->SRect[0] ;
  ppSubWin->SRect[1] = ppAxesMdl->SRect[1] ;
  ppSubWin->SRect[2] = ppAxesMdl->SRect[2] ;
  ppSubWin->SRect[3] = ppAxesMdl->SRect[3] ;
  ppSubWin->SRect[4] = ppAxesMdl->SRect[4] ;
  ppSubWin->SRect[5] = ppAxesMdl->SRect[5] ;
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void )
{
  
  sciPointObj * pSubWin = sciGetCurrentSubWin() ;
  if ( !sciGetAddPlot( pSubWin ) )
  {
      /* redraw the axis */
      reinitSubWin( pSubWin ) ;
      return TRUE ;
  }
  return FALSE ;
}

/*--------------------------------------------------------------------------------*/

/* 1----------4 */
/* |          | */
/* |          | */
/* |          | */
/* 2----------3 */
/* check if the graduation text foo is inside the old_rect */
/* in this case it should not been displayed */
/* taken from drawobject.c */
BOOL CheckDisplay(double fact_h, double fact_w, char logflag, char *foo,int *posi,int *fontid,int *old_rect) 
{
  int rect[4],i;
  int point[4][2];
  int logrect[4] ;
  int XX = 0 ;
  int YY = 0 ;
  
  if(old_rect[0] == 0 && old_rect[1] == 0 && old_rect[2] == 0 && old_rect[3] == 0)
  {
    return TRUE ;
  }
  
  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
  
  /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
  C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
 
  getTicksLabelBox( 1.0, 1.0, foo, posi, fontid, logflag, rect, TRUE ) ;
 
  point[0][0] = rect[0]; /* upper left point */
  point[0][1] = rect[1];
  
  point[1][0] = rect[0]; /* lower left point */
  point[1][1] = rect[1]+rect[3];
  
  point[2][0] = rect[0]+rect[2]; /* lower right point */
  point[2][1] = rect[1]+rect[3];
  
  point[3][0] = rect[0]+rect[2]; /* upper right point */
  point[3][1] = rect[1];
  
  for(i=0;i<4;i++)
  {
    if( !IsInsideRectangle(old_rect,point[i]) )
    {
      return FALSE; /* If one inside the old_rect, DO NOT display the graduation ! */
    }
  }

  return TRUE;
}

/*--------------------------------------------------------------------------------*/

/* The unit is the pixel */
/* return 0 if the point is inside the rect */
/* 1 if it is actually outside the rect */
/* (rect[0],rect[1]) : upper left point of the bounding box  in pixel */
/* (rect[2],rect[3]) : width and height in pixel */
/* point[0] : x component */
/* point[1] : y component */
BOOL IsInsideRectangle(int * rect, int *point)
{

  if((point[0] >= rect[0] && point[0] <= rect[0]+rect[2]) &&
     (point[1] >= rect[1] && point[1] <= rect[1]+rect[3]))
    return FALSE;
  
  return TRUE;
}

/*--------------------------------------------------------------------------------*/

/* get the bounding rectangle of a label of a ticks */
/* similar to stringBoundingRect but only for ticks labels */
void getTicksLabelBox( double   fact_h   ,
                       double   fact_w   , 
                       char   * label    ,
                       int      pos[2]   ,
                       int      fontId[2],
                       char     logflag  ,
                       int      bbox[4]  ,
                       BOOL     changeFont )
{  
  int prevFontId[2] ;
  
  if ( changeFont )
  {
    int verbose = 0 ;
    int narg ;
    C2F(dr)("xget","font",&verbose,prevFontId,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
    C2F(dr)("xset","font",fontId,fontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
  }
  
  if ( logflag == 'n' )
  { 
    C2F(dr)("xstringl",label,&pos[0],&pos[1],bbox,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    bbox[3] = (int)(fact_h * bbox[3]);
    bbox[2] = (int)(fact_w * bbox[2]);
  }
  else
  {
    int smallersize = fontId[1]-2;
    int rect10[4];
    int posi10[2];
    int logrect[4] ;
    int XX = 1 ;
    int YY = 1 ;
    /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
    C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
    
    posi10[0] = pos[0] - logrect[2];
    posi10[1] = pos[1] + logrect[3];
    
    /* get the bbox of the 10 */
    C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
    pos[0] = rect10[0] + rect10[2];
    pos[1] = (int) (rect10[1] - rect10[3]*.1);
    
    C2F(dr)("xset","font",fontId,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
    /* get the bbox of the exponent */
    C2F(dr)("xstringl",label,(&pos[0]),(&pos[1]),bbox,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
    bbox[2] = (integer)(fact_w*(bbox[2] + rect10[2]));
    bbox[3] = (integer)(fact_h*(bbox[3] + rect10[3] + (int) (rect10[3]*.1))); /* added the 01.06.05 */
    bbox[0] = rect10[0];
    /* bbox[1] = bbox[1]; */
    
    C2F(dr)("xset","font",fontId,fontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if ( changeFont )
  {
    C2F(dr)("xset","font",prevFontId,prevFontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
  }
}

/*--------------------------------------------------------------------------------*/

/* remove the concealed ticks before display on the X axis */
BOOL checkXAxes2dTics( sciPointObj * psubwin  ,
                       char          side     ,
                       double        y        ,
                       char        * cFormat  ,
                       int           fontId[2],
                       int           smallersize )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin) ;
  double xminval, yminval, xmaxval, ymaxval ;
  int  /* flag=0, */xx=0,yy=0,rect[4] ;
  int xm[2],ym[2];
  int barlength = 0;
  int posi[2];
  int prevFontId[2] ;
  int verbose = 0 ;
  int narg ;
  
  double factX = 1.2 ;
  double factY = 1.2 ;
  int nbtics = 0,i ;
  int prevRect[4] = { 0, 0, 0, 0 } ;
  
  /*--------*/
  /* x axis */
  /*--------*/
  
  ym[0] = YScale(y) ;

  barlength =  Max( (integer) (Cscale.WIRect1[3]/50.0), 2 ) ;

  ppsubwin->firsttime_x = TRUE ;

  /* suppose auto_ticks is off */
  /* here i only need x data (ymin ymax are computed but not used after) */
  FindXYMinMaxAccordingTL( psubwin, &xminval, &yminval, &xmaxval, &ymaxval ) ;
  nbtics = ppsubwin->axes.nxgrads ;
  
  C2F(dr)("xget","font",&verbose,prevFontId,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
  C2F(dr)("xset","font",fontId,fontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
  
  for( i = 0 ; i < nbtics; i++ )
  {
    
    char foo[256];
    double xtmp = ppsubwin->axes.xgrads[i] ;

    if( xtmp < xminval || xtmp > xmaxval ) 
    {
      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichées de tte facon */
                /* donc autant ne pas aller plus loin dans l'algo... */
    }
    
    sprintf(foo, cFormat,xtmp);
    
    /* compute the bounding box of the label */
   
    C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
    xm[0] = XScale(xtmp);
    posi[0] = inint( xm[0] - rect[2]/2.0 ) ;
    
    if ( side == 'd')
    {
      /* ticks are drawn under the axis */
      posi[1] = inint( ym[0] + 1.2 * barlength + rect[3] ) ;
    }
    else /* side = 'u' */
    {
      /* ticks are drawn above the axis */
      posi[1] = inint( ym[0] - 1.2 * barlength ) ;
    }

    /* check that the new display doesnot concealed the previous */
    if ( !CheckDisplay( factX, factY,
                        ppsubwin->logflags[0],
                        foo, posi,
                        fontId, prevRect ) )
    {
      /* put the font back */
      C2F(dr)("xset","font",prevFontId,prevFontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
      return FALSE ;
    }
    
    /* get the boundig rectangle of the new label */
    getTicksLabelBox( factX, factY, foo, posi, fontId, ppsubwin->logflags[0],  prevRect, FALSE ) ;

    XGradPosition(psubwin,posi[1],rect[3]) ;
    
  }


  /* put the font back */
  C2F(dr)("xset","font",prevFontId,prevFontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;

  return TRUE ;    
}

/*--------------------------------------------------------------------------------*/

/* remove the concealed ticks before display on the X axis */
BOOL checkYAxes2dTics( sciPointObj * psubwin  ,
                       char          side     ,
                       double        x        ,
                       char        * cFormat  ,
                       int           fontId[2],
                       int           smallersize )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin) ;
  double xminval, yminval, xmaxval, ymaxval ;
  int  /* flag=0, */xx=0,yy=0,rect[4] ;
  int xm[2],ym[2];
  int barlength = 0;
  int posi[2];
  int prevFontId[2] ;
  int verbose = 0 ;
  int narg ;
  double factX = 1.5 ;
  double factY = 1.5 ;
  int nbtics = 0,i ;
  int prevRect[4] = { 0, 0, 0, 0 } ;
  
  /*--------*/
  /* y axis */
  /*--------*/
  
  xm[0] = XScale(x) ;

  barlength =  Max( (integer) (Cscale.WIRect1[2]/75.0), 2 ) ;

  ppsubwin->firsttime_y = TRUE ;

  /* suppose auto_ticks is off */
  /* here i only need x data (ymin ymax are computed but not used after) */
  FindXYMinMaxAccordingTL( psubwin, &xminval, &yminval, &xmaxval, &ymaxval ) ;
  nbtics = ppsubwin->axes.nygrads ;
  
  C2F(dr)("xget","font",&verbose,prevFontId,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
  C2F(dr)("xset","font",fontId,fontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
  
  for( i = 0 ; i < nbtics; i++ )
  {
    
    char foo[256];
    double ytmp = ppsubwin->axes.ygrads[i] ;

    if( ytmp < yminval || ytmp > ymaxval ) 
    {
      continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichées de tte facon */
                /* donc autant ne pas aller plus loin dans l'algo... */
    }
    
    sprintf(foo, cFormat,ytmp);
    
    /* compute the bounding box of the label */
   
    C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
    ym[0] = YScale(ytmp);
    posi[1] = inint( ym[0] + rect[2]/2.0 ) ;
    
    if ( side == 'r')
    {
      /* ticks are drawn under the axis */
      posi[0] = inint( xm[0] + 1.2 * barlength ) ;
    }
    else /* side = 'l' */
    {
      /* ticks are drawn above the axis */
      posi[0] = inint( xm[0] - 1.2 * barlength - rect[2] ) ;
    }

    /* check that the new display doesnot concealed the previous */
    if ( !CheckDisplay( factX, factY,
                        ppsubwin->logflags[1],
                        foo, posi,
                        fontId, prevRect ) )
    {
      /* put the font back */
      C2F(dr)("xset","font",prevFontId,prevFontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
      return FALSE ;
    }
    
    /* get the boundig rectangle of the new label */
    getTicksLabelBox( factX, factY, foo, posi, fontId, ppsubwin->logflags[1],  prevRect, FALSE ) ;

    YGradPosition(psubwin,posi[0],rect[2]) ;
    
  }


  /* put the font back */
  C2F(dr)("xset","font",prevFontId,prevFontId+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ;
  
  return TRUE ;
}

/*--------------------------------------------------------------------------------*/

/* check if the ticks are not concealing each others */
/* in this case some are removed until it's fit */
void updateXaxesTics( sciPointObj * psubwin  ,
                      char          side     ,
                      double        y        ,
                      int           fontId[2] ,
                      int           smallersize )
{
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE( psubwin ) ;
  char labelXFormat[5] ;
  char labelYFormat[5] ;
  findFormat( psubwin, labelXFormat, labelYFormat ) ;
  
  /* check if the ticks are concealing or not */
  if ( !checkXAxes2dTics( psubwin, side, y, labelXFormat, fontId, smallersize ) ) 
  {
    double xMin ;
    double xMax ;
    double yMin ;
    double yMax ;
    double zMin ;
    double zMax ;
    int    ticksStatus = 0 ;
    
    sciGetDisplayedBounds( psubwin, &xMin, &xMax, &yMin, &yMax, &zMin, &zMax ) ;
    
    ppSubWin->axes.nxgrads = ( ppSubWin->axes.nxgrads + 1 ) / 2 ;
    
    /* recompute the ticks */
    if ( ppSubWin->logflags[0] == 'n' )
    {
      TheTicks( &xMin, &xMax, ppSubWin->axes.xgrads, &ppSubWin->axes.nxgrads, TRUE ) ;
    }
    else
    {
      GradLog( xMin, xMax, ppSubWin->axes.xgrads, &ppSubWin->axes.nxgrads, TRUE ) ;
    }

    findFormat( psubwin, labelXFormat, labelYFormat ) ;

    while( ticksStatus == 0 && !checkXAxes2dTics( psubwin, side, y, labelXFormat, fontId, smallersize ) )
    {
      ppSubWin->axes.nxgrads = ( ppSubWin->axes.nxgrads + 1 ) / 2 ;
      if (  ppSubWin->logflags[0] == 'n' )
      {
        ticksStatus = TheTicks( &xMin, &xMax, ppSubWin->axes.xgrads, &ppSubWin->axes.nxgrads, TRUE ) ;
      }
      else
      {
        GradLog( xMin, xMax, ppSubWin->axes.xgrads, &ppSubWin->axes.nxgrads, TRUE ) ;
      }

      findFormat( psubwin, labelXFormat, labelYFormat ) ;

    }

  }
  /* else everything ok */
}

/*--------------------------------------------------------------------------------*/

/* check if the ticks are not concealing each others */
/* in this case some are removed until it's fit */
void updateYaxesTics( sciPointObj * psubwin  ,
                      char          side     ,
                      double        x        ,
                      int           fontId[2],
                      int           smallersize )
{
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE( psubwin ) ;
  char labelXFormat[5] ;
  char labelYFormat[5] ;
  findFormat( psubwin, labelXFormat, labelYFormat ) ;
  
  /* check if the ticks are concealing or not */
  if ( !checkYAxes2dTics( psubwin, side, x, labelYFormat, fontId, smallersize ) ) 
  {
    double xMin ;
    double xMax ;
    double yMin ;
    double yMax ;
    double zMin ;
    double zMax ;
    int    ticksStatus = 0 ;
    
    sciGetDisplayedBounds( psubwin, &xMin, &xMax, &yMin, &yMax, &zMin, &zMax ) ;
    
    ppSubWin->axes.nygrads = ( ppSubWin->axes.nygrads + 1 ) / 2 ;
    
    /* recompute the ticks */
    if ( ppSubWin->logflags[1] == 'n' )
    {
      TheTicks( &yMin, &yMax, ppSubWin->axes.ygrads, &ppSubWin->axes.nygrads, TRUE ) ;
    }
    else
    {
      GradLog( yMin, yMax, ppSubWin->axes.ygrads, &ppSubWin->axes.nygrads, TRUE ) ;
    }

    findFormat( psubwin, labelXFormat, labelYFormat ) ;

    while( ticksStatus == 0 && !checkYAxes2dTics( psubwin, side, x, labelYFormat, fontId, smallersize ) )
    {
      ppSubWin->axes.nygrads = ( ppSubWin->axes.nygrads + 1 ) / 2 ;
      if (  ppSubWin->logflags[1] == 'n' )
      {
        ticksStatus = TheTicks( &yMin, &yMax, ppSubWin->axes.ygrads, &ppSubWin->axes.nygrads, TRUE ) ;
      }
      else
      {
        GradLog( yMin, yMax, ppSubWin->axes.ygrads, &ppSubWin->axes.nygrads, TRUE ) ;
      }

      findFormat( psubwin, labelXFormat, labelYFormat ) ;

    }

  }
  /* else everything ok */
}

/*--------------------------------------------------------------------------------*/
 
/* remove in the ticks array the indices i such as removedTicks[i] */
/* is true. The value nbtics is an in-out variable */
void removeBadTicks( double * ticks, BOOL * removedTicks, int * nbTicks )
{
  int i ;
  for ( i = *nbTicks - 1 ; i >= 0 ; i-- )
  {
    if ( removedTicks[i] )
    {
      removeIndex( ticks, *nbTicks, i ) ;
      *nbTicks = *nbTicks - 1 ;
    }
  }
}

/*--------------------------------------------------------------------------------*/

/* remove an element in the array from translating the next
   elements on step backward */
void removeIndex( double * changedArray, int size, int ind )
{
  int i ;
  for ( i = ind + 1 ; i < size ; i++ )
  {
    changedArray[i-1] = changedArray[i] ;
  } 
}

/*--------------------------------------------------------------------------------*/

/* compute the needed formats to display the ticks of a subWin */
void findFormat( sciPointObj * pSubWin, char formatX[5], char formatY[5] )
{
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE( pSubWin ) ;
  int lastIndex ;

  /* x axis */
  lastIndex = Max( ppSubWin->axes.nxgrads - 1, 0 ) ;

  ChoixFormatE( formatX,
                ppSubWin->axes.xgrads[0],
                ppSubWin->axes.xgrads[lastIndex],
               ( (ppSubWin->axes.xgrads[lastIndex])-(ppSubWin->axes.xgrads[0]))/(lastIndex)); /* Adding F.Leray 06.05.04 */

  /* y-axis */
  lastIndex = Max( ppSubWin->axes.nygrads - 1, 0 ) ;
  
  ChoixFormatE( formatY,
                ppSubWin->axes.ygrads[0],
                ppSubWin->axes.ygrads[lastIndex],
                ((ppSubWin->axes.ygrads[lastIndex])-(ppSubWin->axes.ygrads[0]))/(lastIndex)); /* Adding F.Leray 06.05.04 */
  
}

/*--------------------------------------------------------------------------------*/
void updateSubWinScale( sciPointObj * pSubWin )
{
  double xBox[8] ;
  double yBox[8] ;
  double zBox[8] ;
  double dBox[6] ;


  set_scale ("tttftt", pSUBWIN_FEATURE(pSubWin)->WRect,
    pSUBWIN_FEATURE(pSubWin)->FRect,
    NULL, pSUBWIN_FEATURE(pSubWin)->logflags, 
    pSUBWIN_FEATURE(pSubWin)->ARect); 

  /* Scales are not be updated with an automatic drawing */
  if ( pSUBWIN_FEATURE(pSubWin)->is3d )
  {
    updateScale3d( pSubWin, dBox, xBox, yBox, zBox ) ;
  }
  else
  {
    sci_update_frame_bounds_2d( pSubWin ) ;
  }
}
/*-----------------------------------------------------------------------------------------*/
void updateScaleIfRequired( sciPointObj * pSubWin )
{
  if ( !sciGetIsAutoDrawable( pSubWin ) )
  {
    updateSubWinScale( pSubWin ) ;
  }
}
/*-----------------------------------------------------------------------------------------*/
void drawAxesGrid( sciPointObj * psubwin )
{
  if( pSUBWIN_FEATURE(psubwin)->grid[0] > -1 )
  {
    DrawXGrid(psubwin);
  }

  if( pSUBWIN_FEATURE(psubwin)->grid[1] > -1 )
  {
    DrawYGrid(psubwin);
  }
}
/*-----------------------------------------------------------------------------------------*/
/**axis_3ddraw 10/2003
* @author Djalel Abdemouche
* Should be in Axes.c file
*/
void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD) 
{
  double dbox[6];
  integer flag,i,p,n,pat,hiddencolor, x[5]; /* F. Leray : redimmensionnment (+1) du tableau x[4];*/
  static double Alpha, Teta;
  int verbose=0,narg;
  integer ixbox[8],iybox[8],xind[8],dash[6];
  integer background,zero=0, color_old; /* Adding color_old 04.03.04*/
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);
  EAxesBoxType subWinBoxType = sciGetBoxType( pobj ) ;

  /* Initialisation phase for x (to detect bug): x set to -1000 F.Leray 05.03.04*/
  for(i=0;i<5;i++) { x[i] = -1000 ; }

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
  {  

    updateScale3d( pobj, dbox, xbox, ybox, zbox ) ;
    /* Until here we have computed + reset the 3d scale*/
    Teta  = ppsubwin->theta  ;
    Alpha =  ppsubwin->alpha ;
    if( Abs( Teta ) < 0.1 )
    {
      /* to avoid bug at limit when theta == 0 */
      /* I recompute temp value xyzbox with theta == 0.1 */
      /* to have a correct xind, InsideU et InsideD */
      ComputeCorrectXindAndInsideUD( Teta, Alpha, dbox, xind, InsideU, InsideD ) ;
    }
    else
    {
      sciAxesVerticesIndices( InsideU, InsideD, xbox, ybox, xind ) ;
    }

    /* F.Leray Rajout 02.04.04 :*/
    background = sciGetBackground(pobj) ;

    for ( i = 0 ; i < 6 ; i++ )
    {
      ixbox[i]=XScale(xbox[xind[i]]);
      iybox[i]=YScale(ybox[xind[i]]);
    }
    ixbox[6] = ixbox[0] ;
    iybox[6] = iybox[0] ;
    p = 7 ;
    n = 1 ; 

    C2F(dr)("xget","pattern",&verbose,&color_old,&zero,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","pattern",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	 
    C2F (dr) ("xarea", "v", &p, ixbox, iybox, &n, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);
    C2F(dr)("xset","pattern",&color_old,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    /***********/
    /***  hidden axis */

    flag = ppsubwin->axes.flag[2]; /* box drawing */

    if ( subWinBoxType != BT_OFF )
    { 
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      hiddencolor = sciSetGoodIndex( pobj, ppsubwin->axes.hiddenAxisColor ) ;

      if (zbox[InsideU[0]] > zbox[InsideD[0]])
      {
        DrawAxis(xbox,ybox,InsideD,hiddencolor);
      }	 
      else
      {
        DrawAxis(xbox,ybox,InsideU,hiddencolor); 	
      }
      if (Ishidden(pobj))
      {
        ppsubwin->hiddenstate=(InsideU[0] % 4);
      }
      else
      {
        ppsubwin->hiddenstate=(InsideD[0] % 4);
      }
    }
    /**  l'enveloppe cvxe*/
    x[0] = sciGetForeground (pobj);	 /* F.Leray 05.03.04 Useless or not?? because we used set pattern instead of set foreground (because Windows uses BRUSH and PEN...)*/
    /* Wrong explanation: We use sciGetForeground in NG mode and used set foreground in old graphic mode*/
    x[2] = sciGetLineWidth (pobj); /* Adding this line 05.03.04*/
    x[3] = sciGetLineStyle (pobj);
    x[4] = 0; 
    C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
    C2F(dr)("xget","pattern",&verbose,&pat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    /* We are in axis_3ddraw() and sciGetEntityType (pobj) == SCI_SUBWIN*/
    C2F (dr) ("xset", "dashes", x, x, x+4, x+4, x+4,PI0,PD0,PD0,PD0,PD0, 5L, 6L);
    C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096); /* F.Leray 05.03.04 Useless too*/
    C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    p = 7 ;
    n = 1 ;
    for ( i = 0 ; i < p ; i++ )
    {
      ixbox[i] = XScale( xbox[ xind[i] ] ) ;
      iybox[i] = YScale( ybox[ xind[i] ] ) ;
    }
    ixbox[p-1]=ixbox[0];iybox[p-1]=iybox[0]; 

    if ( subWinBoxType == BT_BACK_HALF || subWinBoxType == BT_ON )
    {
      C2F(dr)("xpolys","v",ixbox,iybox,x,&n,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);

    }
    Axes3dStrings2(ixbox,iybox,xind) ;

    C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
}
/*-----------------------------------------------------------------------------------------*/
void triedre(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD)
{
  integer  x[5],narg = 0;
  integer color_kp,verbose = 0,thick_kp ;
  integer style_kp[3] ;

  C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray Replacement*/
  C2F(dr)("xget","thickness",&verbose,&thick_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/
  C2F(dr)("xget","line style",&verbose,style_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/

  if(sciGetEntityType (pobj) == SCI_SUBWIN)
  {
    if(pSUBWIN_FEATURE (pobj)->axes.rect == BT_ON)
    {
      x[0] = sciGetForeground (pobj);	
      x[2] = sciGetLineWidth (pobj);
      x[3] = sciGetLineStyle (pobj);
      x[4] = 0;

      /* C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,PI0,PD0,PD0,PD0,PD0,5L,4096);*/
      C2F(dr)("xset","pattern",x,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      C2F(dr)("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset", "line style", x+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      if (zbox[InsideU[0]] > zbox[InsideD[0]])
      {
        DrawAxis(xbox,ybox,InsideU,x[0]);
      }
      else
      {
        DrawAxis(xbox,ybox,InsideD,x[0]);
      }
    }
  }
  C2F(dr)("xset", "line style",&(style_kp[0]),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);/*F.Leray addings here*/
  C2F(dr)("xset","thickness",&thick_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);/*F.Leray addings here*/
  C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /*F.Leray addings here*/

}
/*-----------------------------------------------------------------------------------------*/
/* F.Leray 07.05.04 */
/* This routine rebuild a needed strflag for axis_draw call in sciDrawObj*/
/* It may be less complete than the original strflag given in plot2dn (or other 2D - plot function)
due to less info. Nevertheless this new strflag is sufficient for axis_draw because
tests are on strflag[1] (case =='5' or '6') and strflag[2] */
void rebuild_strflag( sciPointObj * psubwin, char * STRFLAG)
{

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  BOOL isaxes = GetIsAxes2D(psubwin);

  /* strflag[0]*/
  STRFLAG[0] = '0'; /* flag for caption display unused here so set to NULL by default */
  if( pSUBWIN_FEATURE (psubwin)->with_leg == 1)
    STRFLAG[0] = '0';
  else
    STRFLAG[0] = '1';

  /* strflag[1]*/
  /* Here isoview mode test is not needed because axis_draw do not use it */
  STRFLAG[1] = '0'; /* Init. to NULL <=> use the previous scale */
  if(ppsubwin->tight_limits == TRUE)
  {
    STRFLAG[1] = '8';
  }
  else /* using auto rescale : enlarged for pretty axes*/
  {
    STRFLAG[1] = '6';
  }

  /* strflag[2]*/
  STRFLAG[2] = '1'; /* Init with y-axis on the left AND axes is on*/
  if(isaxes == TRUE)
  {
    if(ppsubwin->axes.ydir =='l')
      STRFLAG[2] = '1';
    else if(ppsubwin->axes.ydir =='r')
      STRFLAG[2] = '3';
    else if((ppsubwin->axes.xdir =='c') && (ppsubwin->axes.ydir =='c'))
      STRFLAG[2] = '5';
  }
  else
  {
    if(ppsubwin->axes.rect == BT_ON)
      STRFLAG[2] = '2';
    else
      STRFLAG[2] = '0';
  }

  STRFLAG[3] = '\0';
}
/*-----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/* draw the title and the two labels of a 2d axis                                 */
/*----------------------------------------------------------------------------------*/
int labels2D_draw( sciPointObj * psubwin )
{
  /* Rewritten by jb Silvy 06/2006 */

  integer rect1[4] ;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  int offset[2] ; /* vertical and horizontal offsets to move labels a little from axes */

  if ( !sciGetVisibility(psubwin) )
  {
    return 0 ;
  }

  /* get the size of the current subwin in pixels */
  rect1[0]= Cscale.WIRect1[0] ; /* upper left point of the axes x coordinate. */
  rect1[1]= Cscale.WIRect1[1] ; /* upper left point of the axes y coordinate. */
  rect1[2]= Cscale.WIRect1[2] ; /* width of the axes */
  rect1[3]= Cscale.WIRect1[3] ; /* height of the axes */

  /* the little displacment of the labels from the axes box */
  offset[0] = rect1[2] / 50 + 1 ;
  offset[1] = rect1[3] / 25 + 1 ;


  /*********/
  /* TITLE */
  /*********/

  if ( sciGetVisibility(ppsubwin->mon_title) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;

    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      sciSetFontOrientation( ppsubwin->mon_title, 0 ) ;
    }


    if ( ppLabel->auto_position )
    {
      int segmentStart[2] = { rect1[0] + rect1[2], rect1[1] } ;
      int segmentEnd[2]   = { rect1[0]           , rect1[1] } ;
      computeLabelAutoPos( ppsubwin->mon_title, segmentStart, segmentEnd, offset ) ;
    }
    /* draw the label */
    drawTextEntity( ppLabel->text ) ;

  }

  /***********/
  /* x label */
  /***********/

  if( sciGetVisibility(ppsubwin->mon_x_label) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_x_label ) ;

    if( ppLabel->auto_rotation )
    {
      sciSetFontOrientation(ppsubwin->mon_x_label, 0 ) ;
    }


    if( ppLabel->auto_position )
    {
      int segmentStart[2] ;
      int segmentEnd[2]   ;

      if(ppsubwin->axes.xdir == 'u')
      {
        segmentStart[0] = rect1[0] + rect1[2] ;
        segmentEnd[0]   = rect1[0] ;

        /* we add the size of the numbers to the height */
        segmentStart[1] = ppsubwin->XGradMostOnTop ;
        segmentEnd[1]   = ppsubwin->XGradMostOnTop ;
      }
      else
      {
        segmentStart[0] = rect1[0] ;
        segmentEnd[0]   = rect1[0] + rect1[2] ;

        segmentStart[1] = ppsubwin->XGradMostOnBottom ;
        segmentEnd[1]   = ppsubwin->XGradMostOnBottom ;
      }
      computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
    }

    drawTextEntity( ppLabel->text ) ;

  }


  /***********/
  /* y label */
  /***********/


  if ( sciGetVisibility(ppsubwin->mon_y_label) )
  {
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_y_label ) ;

    if( ppLabel->auto_rotation )
    {
      sciInitFontOrientation( ppsubwin->mon_y_label, 270 * 10 ) ;
    }

    if( ppLabel->auto_position )
    {
      int segmentStart[2] ;
      int segmentEnd[2]   ;

      if(ppsubwin->axes.ydir == 'r')
      {

        /* we add the size of the numbers to the height */
        segmentStart[0] = ppsubwin->YGradMostOnRight ;
        segmentEnd[0]   = ppsubwin->YGradMostOnRight ;

        segmentStart[1] = rect1[1] + rect1[3] ;
        segmentEnd[1]   = rect1[1] ;
      }
      else
      {
        segmentStart[0] = ppsubwin->YGradMostOnLeft ;
        segmentEnd[0]   = ppsubwin->YGradMostOnLeft ;

        segmentStart[1] = rect1[1] ;
        segmentEnd[1]   = rect1[1] + rect1[3] ;
      }
      computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
    }
    drawTextEntity( ppLabel->text ) ;
  }

  return 0;
}
/*-----------------------------------------------------------------------------------------*/
void DrawAxis(double xbox[8], double ybox[8], integer Indices[4], integer style)
{
  integer ixbox[6],iybox[6],npoly=6,lstyle[6],verbose=0,narg_,hiddencolor;
  integer i, iflag=0, j=1;
  sciPointObj *psubwin;
  for ( i = 0 ; i <= 4 ; i=i+2)
  {
    ixbox[i]=XScale(xbox[Indices[0]]);iybox[i]=YScale(ybox[Indices[0]]);
  }
  ixbox[1]=XScale(xbox[Indices[1]]);iybox[1]=YScale(ybox[Indices[1]]);
  ixbox[3]=XScale(xbox[Indices[2]]);iybox[3]=YScale(ybox[Indices[2]]);
  ixbox[5]=XScale(xbox[Indices[3]]);iybox[5]=YScale(ybox[Indices[3]]);
  C2F(dr)("xget","line style",&verbose,lstyle,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  /**DJ.Abdemouche 2003**/

  psubwin = sciGetCurrentSubWin(); 
  hiddencolor = pSUBWIN_FEATURE (psubwin)->hiddencolor;
  j = sciGetLineStyle (psubwin) ;

  C2F(dr)("xset","line style",&j,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsegs","v",ixbox,iybox,&npoly,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",lstyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}
/*-----------------------------------------------------------------------------------------*/
int Axes3dStrings2( integer * ixbox, integer * iybox, integer * xind )
{
  integer verbose=0,narg,xz[2],fontid[2],fontsize_kp,color_kp,size;
  integer iof,barlengthx = 0,barlengthy = 0, posi[2]; 
  integer rect[4],flag=0,x=0,y=0;
  double ang=0.0, bbox[6];
  int fontsize=-1,textcolor=-1,ticscolor=-1;
  int fontstyle=0; /* F.Leray 08.04.04 */
  sciPointObj *psubwin = NULL;
  sciSubWindow * ppsubwin = NULL;
  int ns=2,iflag=0,gstyle,gridStyle=2,dash[6];
  double xx[4],yy[4],zz[4],vxx1,vyy1,vzz1;
  integer i,xm,ym,vx[2],vy[2],xg[2],yg[2],j;
  integer fontid_old[2], textcolor_old;

  int lastzindex = 0, lastxindex = 0, lastyindex = 0;
  double xminval, yminval, zminval, xmaxval, ymaxval, zmaxval;
  int nbtics = 0;
  int nbsubtics = 0;

  int logrect[4], XX = 0, YY = 0; /* see below */ /* ah ouais ou ca ? */
  double angle=0.0;

  int constOffset[2] ; /* displacment of labels from the axes segments. */
  int maxTicksLabelSize[2] = {0,0} ; /* for each axis the maximum size of ticks label */


  psubwin= sciGetCurrentSubWin();
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  /** le cot\'e gauche ( c'est tjrs un axe des Z **/
  xz[0] = Cscale.WIRect1[2] ;
  xz[1] = Cscale.WIRect1[2] ;
  iof = (xz[0]+xz[1])/50;

  ticscolor = sciGetForeground(psubwin);
  textcolor=sciGetFontForeground(psubwin);

  fontsize=sciGetFontDeciWidth(psubwin)/100;
  fontstyle=sciGetFontStyle(psubwin);

  if(sciGetEntityType (psubwin) != SCI_SUBWIN) { 
    sciprint("Impossible case\n");
    return 0;
  }


  bbox[0] =  xminval = pSUBWIN_FEATURE (psubwin)->FRect[0]; /*xmin*/
  bbox[1] =  xmaxval = pSUBWIN_FEATURE (psubwin)->FRect[2]; /*xmax*/
  bbox[2] =  yminval = pSUBWIN_FEATURE (psubwin)->FRect[1]; /*ymin*/
  bbox[3] =  ymaxval = pSUBWIN_FEATURE (psubwin)->FRect[3]; /*ymax*/ 
  bbox[4] =  zminval = pSUBWIN_FEATURE (psubwin)->FRect[4]; /*zmin*/
  bbox[5] =  zmaxval = pSUBWIN_FEATURE (psubwin)->FRect[5]; /*zmax*/

  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  fontid[0]= fontstyle;
  fontsize_kp = fontid[1] ;
  if( fontsize == -1 ){ 
    fontid[1]= 1;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else {
    fontid[1] = fontsize ;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  if ( textcolor != -1 || ticscolor != -1 ) 
    C2F(dr)("xget","pattern",&verbose,&color_kp,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    

  for (i=0; i<3 ; i++) {
    xx[i]=pSUBWIN_FEATURE (psubwin)->axes.xlim[i];
    yy[i]=pSUBWIN_FEATURE (psubwin)->axes.ylim[i];
    zz[i]=pSUBWIN_FEATURE (psubwin)->axes.zlim[i];
  } 

  /* main title */ /* We fix the title always at the top */
  rect[0]= Cscale.WIRect1[0] ;
  rect[1]= Cscale.WIRect1[1] ;
  rect[2]= Cscale.WIRect1[2] ;
  rect[3]= Cscale.WIRect1[3]/6 ;
  textcolor_old = textcolor;
  fontid_old[0] = fontid[0];
  fontid_old[1] = fontid[1];

  constOffset[0] = Cscale.WIRect1[2] / 50 + 1 ;
  constOffset[1] = Cscale.WIRect1[3] / 25 + 1 ;

  if( sciGetVisibility(ppsubwin->mon_title) )
  {
    /* get the pointer on the title */
    sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_title ) ;

    /* get position and orientation of the title */
    if ( ppLabel->auto_rotation )
    {
      sciSetFontOrientation( ppsubwin->mon_title, 0 ) ;
    }

    if ( ppLabel->auto_position )
    {
      /* same as in 2d */
      int segmentStart[2] = { rect[0] + rect[2], rect[1] } ;
      int segmentEnd[2]   = { rect[0]          , rect[1] } ;
      computeLabelAutoPos( ppsubwin->mon_title, segmentStart, segmentEnd, constOffset ) ;
    }
    /* draw the label */
    drawTextEntity( ppLabel->text ) ;
  }

  textcolor = textcolor_old;
  fontid[0] = fontid_old[0];
  fontid[1] = fontid_old[1];

  size = xz[0]>=xz[1] ? (integer) (xz[1]/50.0) : (integer) (xz[0]/50.0); 

  /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
  C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	

  /********************************************/
  /* draw the axis line if axes_visible is on */
  /********************************************/
  {
    int x2[5] ;
    int two = 2 ;
    int one = 1 ;
    int verbose2 = 0 ;
    int dash2[6] ;
    int pat ;
    int narg2 ;
    x2[0] = sciGetForeground (psubwin);
    x2[2] = sciGetLineWidth (psubwin) ;
    x2[3] = sciGetLineStyle (psubwin);
    x2[4] = 0;
    C2F(dr)("xget","line style",&verbose2,dash2,&narg2,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
    C2F(dr)("xget","pattern",&verbose2,&pat,&narg2,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset", "dashes", x2, x2, x2+4, x2+4, x2+4,PI0,PD0,PD0,PD0,PD0, 5L, 6L);
    C2F (dr) ("xset","foreground",x2,x2,x2+4,x2+4,x2+4,PI0,PD0,PD0,PD0,PD0,5L,4096); /* F.Leray 05.03.04 Useless too*/
    C2F (dr) ("xset","thickness",x2+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset", "line style", x2+3,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    if ( ppsubwin->axes.axes_visible[2] )
    {
      /* draw the line of the axis like in 2d */
      /* the two bounds are (ixbox[2],iybox[2]) and (ixbox[2],iybox[2]) */
      C2F(dr)("xpolys","v",&(ixbox[2]),&(iybox[2]),x2,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
    if ( ( xind[4]+xind[5] == 3) || ( xind[4]+xind[5] == 11 ) )
    {
      /* i copy this test but I don't know what it mean... jb Silvy 03/2006 */
      if ( ppsubwin->axes.axes_visible[1] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[3],iybox[3]) and (ixbox[3],iybox[3]) */
        C2F(dr)("xpolys","v",&(ixbox[3]),&(iybox[3]),x2,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( ppsubwin->axes.axes_visible[0] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[4],iybox[4]) and (ixbox[4],iybox[4]) */
        C2F(dr)("xpolys","v",&(ixbox[4]),&(iybox[4]),x2,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
    else
    {
      if ( ppsubwin->axes.axes_visible[1] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[3],iybox[3]) and (ixbox[3],iybox[3]) */
        C2F(dr)("xpolys","v",&(ixbox[4]),&(iybox[4]),x2,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      if ( ppsubwin->axes.axes_visible[0] )
      {
        /* draw the line of the axis like in 2d */
        /* the two bounds are (ixbox[4],iybox[4]) and (ixbox[4],iybox[4]) */
        C2F(dr)("xpolys","v",&(ixbox[3]),&(iybox[3]),x2,&one,&two, PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
    C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","line style",dash2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }  

  /********************/
  /*** le z scaling ***/ /* DISPLAY Z graduations */
  /********************/


  if (pSUBWIN_FEATURE (psubwin)->project[2]==1)
  {
    double fx,fy,fz; 
    char c_format[5] ;

    integer Ticsdir[2];
    Ticsdir[0]=ixbox[3]-ixbox[4]; /* <=> en pixel direction/vecteur non norme(e)s des tics en x */
    Ticsdir[1]=iybox[3]-iybox[4]; /* <=> idem pour y */
    /* NB: for a default plot3d (simply calling plot3d in siclab console) */
    /* Ticsdir[0] & Ticsdir[1] are negative : we do min - max here... */

    BBoxToval(&fx,&fy,&fz,xind[3],bbox); /* xind[3] <=> en bas a gauche <=> zmin */
    x=ixbox[2]-(xz[0]+xz[1])/20;
    y=(iybox[3]+iybox[2])/2;

    /*       NumberFormat(str,((integer) zz[0]),((integer) zz[2])); */


    if( !ppsubwin->axes.auto_ticks[2] )
    {
      /* we display the z tics specified by the user*/
      nbtics = ppsubwin->axes.u_nzgrads;
      nbsubtics = ppsubwin->axes.nbsubtics[2];

      maxTicksLabelSize[0] = 0 ;
      maxTicksLabelSize[1] = 0 ;

      for(i=0;i<nbtics;i++)
      {
        char *foo = ppsubwin->axes.u_zlabels[i]; 
        double ztmp = ppsubwin->axes.u_zgrads[i];

        if(ztmp<zminval || ztmp>zmaxval) 
        {
          /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
          continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
          /* donc autant ne pas aller plus loin dans l'algo... */
        }


        /***************************************************************/
        /************************* COMMON PART *************************/
        /***************************************************************/

        if( ppsubwin->axes.reverse[2] )
        {
          ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
        }

        ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);


        vx[0]=xm;vy[0]=ym;

        barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
        barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
        vx[1]=vx[0]+barlengthx;
        vy[1]=vy[0]+barlengthy;

        /* foo is set above with sprintf(foo,c_format,xtmp); */

        C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

        /* get the size of ticks label */
        C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
        maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;


        posi[0] = inint( xm+2*barlengthx - rect[2]); 
        posi[1]=inint( ym + 2*barlengthy + rect[3]/2);


        if(ppsubwin->axes.axes_visible[2] == TRUE){
          C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
          C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
        /* grid to put here */
        if ( ppsubwin->grid[2] > -1 && ppsubwin->axes.axes_visible[2] )
        {
          gstyle = pSUBWIN_FEATURE (psubwin)->grid[2];

          if((ppsubwin->logflags[2] =='l') && (i != nbtics-1))
          {
            double tmp[2];
            double pas=0;
            double * tmp_log_grads = (double *) NULL;


            double * grads = ppsubwin->axes.u_zgrads;

            tmp[0] = exp10(grads[i]);
            tmp[1] = exp10(grads[i+1]);
            pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

            if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
              sciprint("Error allocating tmp_log_grads\n");
              return -1;
            }

            for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

            for(j=0;j<nbsubtics;j++)
            {
              vzz1 = tmp_log_grads[j];

              if(vzz1<=zminval || vzz1>=zmaxval) continue;	 

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);

              /*  if ((ym != iybox[3]) && (ym != iybox[2])) */
              /* 			    { */
              C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]= ixbox[3];yg[0]= ym;
              if (Ishidden(psubwin))
              {  xg[1]=ixbox[4];  yg[1]= iybox[4]- iybox[3]+ym;}
              else
              {xg[1]=ixbox[1];  yg[1]= iybox[1]- iybox[2]+ym;}
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]=xg[1];  ; xg[1] =ixbox[0];
              yg[0]=yg[1]; yg[1]= ym- iybox[3]+ iybox[5];
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              /*   } */
            }
            FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
          }
          else
          { 
            if(ztmp>zminval && ztmp<zmaxval) 
            {
              C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]= ixbox[3];yg[0]= ym;
              if (Ishidden(psubwin))
              {  xg[1]=ixbox[4];  yg[1]= iybox[4]- iybox[3]+ym;}
              else
              {xg[1]=ixbox[1];  yg[1]= iybox[1]- iybox[2]+ym;}
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]=xg[1];  ; xg[1] =ixbox[0];
              yg[0]=yg[1]; yg[1]= ym- iybox[3]+ iybox[5];
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
          }
        }

        /* and subtics */
        if(i != nbtics-1)
        {
          if(ppsubwin->logflags[2] =='l')
          {
            double tmp[2];
            double pas=0;
            double * tmp_log_grads = (double *) NULL;


            double * grads = ppsubwin->axes.u_zgrads;

            tmp[0] = exp10(grads[i]);
            tmp[1] = exp10(grads[i+1]);
            pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

            if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
              sciprint("Error allocating tmp_log_grads\n");
              return -1;
            }

            for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

            for(j=0;j<nbsubtics;j++)
            {
              vzz1 = tmp_log_grads[j];

              if(vzz1<zminval || vzz1>zmaxval) continue;

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);

              vx[0]=xm;vy[0]=ym;
              vx[1]= (integer) (vx[0]+barlengthx/2.0);
              vy[1]= (integer) (vy[0]+barlengthy/2.0);

              if(ppsubwin->axes.axes_visible[2] == TRUE)
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
          } /* end NEW */
          else
          {
            double xtmp = ppsubwin->axes.u_zgrads[i];
            double dx = (ppsubwin->axes.u_zgrads[i+1] - ppsubwin->axes.u_zgrads[i]) / nbsubtics;
            for(j=0;j<nbsubtics;j++)
            {
              vzz1=xtmp+dx*j;

              if(vzz1<zminval || vzz1>zmaxval) continue;	 

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
              /* 		      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1); */


              vx[0]=xm;vy[0]=ym;
              vx[1]= (integer) (vx[0]+barlengthx/2.0);
              vy[1]= (integer) (vy[0]+barlengthy/2.0);

              if(ppsubwin->axes.axes_visible[2] == TRUE)
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
          } 
        }

        /***************************************************************/
        /************************* END OF COMMON PART ******************/
        /***************************************************************/
      }
    }
    else /* we display the computed tics */
    {
      AdaptGraduationsOnZ(x,y,size,Ticsdir,fontid,psubwin,zminval,zmaxval,fx,fy,0.);

      lastzindex = ppsubwin->axes.nzgrads - 1;

      if(lastzindex == 0)
        ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.zgrads[0]));
      else
        ChoixFormatE(c_format,
        ppsubwin->axes.zgrads[0],
        ppsubwin->axes.zgrads[lastzindex],
        ((ppsubwin->axes.zgrads[lastzindex])-(ppsubwin->axes.zgrads[0]))/(lastzindex));

      nbtics = ppsubwin->axes.nzgrads;
      nbsubtics = ppsubwin->axes.nbsubtics[2];

      maxTicksLabelSize[0] = 0 ;
      maxTicksLabelSize[1] = 0 ;

      for(i=0;i<nbtics;i++)
      {
        char foo[256]; 
        double ztmp = ppsubwin->axes.zgrads[i];

        if(ztmp<zminval || ztmp>zmaxval) 
        {
          /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
          continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
          /* donc autant ne pas aller plus loin dans l'algo... */
        }

        sprintf(foo,c_format,ztmp);

        /***************************************************************/
        /************************* COMMON PART *************************/
        /***************************************************************/
        if(ppsubwin->axes.reverse[2] == TRUE)
          ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);

        ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);

        /* 	      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp); */


        vx[0]=xm;vy[0]=ym;

        barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
        barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
        vx[1]=vx[0]+barlengthx;
        vy[1]=vy[0]+barlengthy;

        /* foo is set above with sprintf(foo,c_format,xtmp); */

        C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

        /* get the size of ticks label */
        C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
        maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
        maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

        posi[0] = inint( xm+2*barlengthx - rect[2]); 
        posi[1]=inint( ym + 2*barlengthy + rect[3]/2);

        if(ppsubwin->axes.axes_visible[2] == TRUE){
          C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          if ( ppsubwin->logflags[2] == 'l' )
          {
            int smallersize = fontid[1]-2;
            int old_rect10[4];
            /* 		    char str[2] = "xv"; */
            int posi10[2];

            posi10[0] = posi[0] - logrect[2];
            posi10[1] = posi[1] + logrect[3];

            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

            C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

            posi[0] = old_rect10[0] + old_rect10[2];
            posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

            C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

            /* put back the current fontid */
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          else{
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
          }

          C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
          C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
        /* grid to put here */
        if ( ppsubwin->grid[2] > -1 && ppsubwin->axes.axes_visible[2] )
        {
          gstyle = pSUBWIN_FEATURE (psubwin)->grid[2];

          if((ppsubwin->logflags[2] =='l') && (i != nbtics-1))
          {
            double tmp[2];
            double pas=0;
            double * tmp_log_grads = (double *) NULL;


            double * grads = ppsubwin->axes.zgrads;

            tmp[0] = exp10(grads[i]);
            tmp[1] = exp10(grads[i+1]);
            pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

            if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
              sciprint("Error allocating tmp_log_grads\n");
              return -1;
            }

            for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

            for(j=0;j<nbsubtics;j++)
            {
              vzz1 = tmp_log_grads[j];

              if(vzz1<=zminval || vzz1>=zmaxval) continue;	 

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);

              /*  if ((ym != iybox[3]) && (ym != iybox[2])) */
              /* 			    { */
              C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]= ixbox[3];yg[0]= ym;
              if (Ishidden(psubwin))
              {  xg[1]=ixbox[4];  yg[1]= iybox[4]- iybox[3]+ym;}
              else
              {xg[1]=ixbox[1];  yg[1]= iybox[1]- iybox[2]+ym;}
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]=xg[1];  ; xg[1] =ixbox[0];
              yg[0]=yg[1]; yg[1]= ym- iybox[3]+ iybox[5];
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              /*   } */
            }
            FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
          }
          else
          {
            if(ztmp>zminval && ztmp<zmaxval) 
            {
              C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]= ixbox[3];yg[0]= ym;
              if (Ishidden(psubwin))
              {  xg[1]=ixbox[4];  yg[1]= iybox[4]- iybox[3]+ym;}
              else
              {xg[1]=ixbox[1];  yg[1]= iybox[1]- iybox[2]+ym;}
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              xg[0]=xg[1];  ; xg[1] =ixbox[0];
              yg[0]=yg[1]; yg[1]= ym- iybox[3]+ iybox[5];
              C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
          }
        }

        /* and subtics */
        if(i != nbtics-1)
        {
          if(ppsubwin->logflags[2] =='l')
          {
            double tmp[2];
            double pas=0;
            double * tmp_log_grads = (double *) NULL;


            double * grads = ppsubwin->axes.zgrads;

            tmp[0] = exp10(grads[i]);
            tmp[1] = exp10(grads[i+1]);
            pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

            if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
              sciprint("Error allocating tmp_log_grads\n");
              return -1;
            }

            for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

            for(j=0;j<nbsubtics;j++)
            {
              vzz1 = tmp_log_grads[j];

              if(vzz1<zminval || vzz1>zmaxval) continue;

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);

              vx[0]=xm;vy[0]=ym;
              vx[1]= (integer) (vx[0]+barlengthx/2.0);
              vy[1]= (integer) (vy[0]+barlengthy/2.0);

              if(ppsubwin->axes.axes_visible[2] == TRUE)
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
          } /* end NEW */
          else
          {
            double ztmp2 = ppsubwin->axes.zgrads[i];
            double dz = (ppsubwin->axes.zgrads[i+1] - ppsubwin->axes.zgrads[i]) / nbsubtics;
            for(j=0;j<nbsubtics;j++)
            {
              vzz1=ztmp2+dz*j;

              if(vzz1<zminval || vzz1>zmaxval) continue;

              if(ppsubwin->axes.reverse[2] == TRUE)
                vzz1 = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],vzz1);

              ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1);
              /* 		      trans3d(psubwin,1,&xm,&ym,&fx,&fy,&vzz1); */


              vx[0]=xm;vy[0]=ym;
              vx[1]= (integer) (vx[0]+barlengthx/2.0);
              vy[1]= (integer) (vy[0]+barlengthy/2.0);

              if(ppsubwin->axes.axes_visible[2] == TRUE)
                C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
          }
        }
        /***************************************************************/
        /************************* END OF COMMON PART ******************/
        /***************************************************************/

      }
    }
  }

  if ( sciGetVisibility(ppsubwin->mon_z_label) )
  {
    /* draw z label */
    sciLabel * ppLabel = pLABEL_FEATURE(ppsubwin->mon_z_label) ;

    if ( ppLabel->auto_rotation )
    {
      /* mult by 10 because */
      sciSetFontOrientation(ppsubwin->mon_z_label, 270 * 10 ) ;
    }

    if( ppLabel->auto_position )
    {

      int segmentStart[2] = { ixbox[2], iybox[2] } ;
      int segmentEnd[2]   = { ixbox[3], iybox[3] } ;

      /* add the bar size and ticks label size to the offset */
      int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
        constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
      computeLabelAutoPos( ppsubwin->mon_z_label, segmentStart, segmentEnd, offset ) ;
    }
    /* a trick to force the display with 2d scale */
    drawTextEntity( ppLabel->text ) ;

  }


  /***********************/ /** bottom right side ***/
  /*** le  x-y scaling ***/ /* DISPLAY x or y graduations */
  /***********************/


  if (( xind[4]+xind[5] == 3) || ( xind[4]+xind[5] == 11))
  {
    if (pSUBWIN_FEATURE (psubwin)->project[0]==1) /* x HERE */
    {
      double fx,fy,fz;
      char c_format[5];

      integer Ticsdir[2]; 
      Ticsdir[0]=ixbox[4]-ixbox[3];
      Ticsdir[1]=iybox[4]-iybox[3];
      BBoxToval(&fx,&fy,&fz,xind[4],bbox);
      x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
      y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);

      if( !ppsubwin->axes.auto_ticks[0] )
      {
        /* we display the x tics specified by the user*/
        nbtics = ppsubwin->axes.u_nxgrads;
        nbsubtics = ppsubwin->axes.nbsubtics[0];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char *foo = ppsubwin->axes.u_xlabels[i]; 
          double xtmp = ppsubwin->axes.u_xgrads[i];

          if(xtmp<xminval || xtmp>xmaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[0] == TRUE)
            xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
          /*   trans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz); */


          vx[0]=xm;vy[0]=ym;

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          /* get the size of the icks label */
          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;


          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            posi[0] = inint( xm+2*barlengthx);
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if( ppsubwin->axes.axes_visible[0] )
          {    
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

            if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<=xminval || vxx1>=xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin)) 
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 

                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(xtmp>xminval && xtmp<xmaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin)) 
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[0] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }

              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            } /* end NEW */
            else
            {
              double xtmp2 = ppsubwin->axes.u_xgrads[i];
              double dx = (ppsubwin->axes.u_xgrads[i+1] - ppsubwin->axes.u_xgrads[i]) / nbsubtics;

              for (j=1;j<nbsubtics;j++)
              {  
                vxx1=xtmp2+dx*j;

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
                /* 			  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */


                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)   
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }
          /***************************************************************/
          /************************* END OF COMMON PART ******************/
          /***************************************************************/

        }
      }
      else /* we display the computed tics */
      {
        AdaptGraduationsOnXBottomRight(iof,x,y,size,Ticsdir,fontid,psubwin,xminval,xmaxval,0.,fy,fz);

        lastxindex = ppsubwin->axes.nxgrads - 1;

        if(lastxindex == 0)
        {
          ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
        }
        else
        {
          ChoixFormatE(c_format,
            ppsubwin->axes.xgrads[0],
            ppsubwin->axes.xgrads[lastxindex],
            ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
        }

        nbtics = ppsubwin->axes.nxgrads;
        nbsubtics = ppsubwin->axes.nbsubtics[0];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char foo[256]; 
          double xtmp = ppsubwin->axes.xgrads[i];

          if(xtmp<xminval || xtmp>xmaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          sprintf(foo,c_format,xtmp);

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/


          /* F.Leray 03.11.04 Test if log scale to perform a : exp10(x) because trans3d will */
          /* re-do a log10() (that is needed for data computations) */

          if( ppsubwin->axes.reverse[0] )
          {
            xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
          }

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);

          vx[0]=xm;vy[0]=ym; 

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          /* get the size of ticks label */
          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            posi[0] = inint( xm+2*barlengthx);
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if( ppsubwin->axes.axes_visible[0] )
          {
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ppsubwin->logflags[0] == 'l' )
            {
              int smallersize = fontid[1]-2;
              int old_rect10[4];
              int posi10[2];

              posi10[0] = posi[0] - logrect[2];
              posi10[1] = posi[1] + logrect[3];

              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

              C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

              posi[0] = old_rect10[0] + old_rect10[2];
              posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

              C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

              /* put back the current fontid */
              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
            {
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
            }	    
            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0]  )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

            if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++)
              {
                tmp_log_grads[j] = log10( tmp[0] + (j) * pas ) ;
              }

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<=xminval || vxx1>=xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin)) 
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 

                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(xtmp>xminval && xtmp<xmaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin)) 
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }	
          }

          /* and subtics */
          if(i != nbtics-1) /* F.Leray NEW 03.11.04 */
          {

            if(ppsubwin->logflags[0] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if( ppsubwin->axes.axes_visible[0] )
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }

              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            } /* end NEW */
            else
            {
              double xtmp2 = ppsubwin->axes.xgrads[i];
              double dx = (ppsubwin->axes.xgrads[i+1] - ppsubwin->axes.xgrads[i]) / nbsubtics;

              for (j=1;j<nbsubtics;j++)
              {  
                vxx1=xtmp2+dx*j;

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }

          /***************************************************************/
          /************************* END OF COMMON PART ******************/
          /***************************************************************/

        }
      }
    }
    if ( sciGetVisibility(ppsubwin->mon_x_label) )
    {
      sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_x_label ) ;
      if( ppLabel->auto_rotation )
      {
        sciSetFontOrientation(ppsubwin->mon_x_label, 0) ;
      }


      if( ppLabel->auto_position )
      {

        int segmentStart[2] = { ixbox[4], iybox[4] } ;
        int segmentEnd[2]   = { ixbox[5], iybox[5] } ;

        /* add the bar size and ticks label size to the offset */
        int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
          constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
        computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
      }
      /* a trick to force the display with 2d scale */
      drawTextEntity( ppLabel->text ) ;

    }
  }
  else
  {

    if ( pSUBWIN_FEATURE (psubwin)->project[1]==1) /* y is HERE */
    {
      double fx,fy,fz; 
      char c_format[5];

      integer Ticsdir[2];
      Ticsdir[0]=ixbox[4]-ixbox[3];
      Ticsdir[1]=iybox[4]-iybox[3];
      BBoxToval(&fx,&fy,&fz,xind[4],bbox);

      x=inint((ixbox[4]+ixbox[5])/2+1.5*rect[2] +iof);
      y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+1.5*rect[3]+iof);

      if(ppsubwin->axes.auto_ticks[1] == FALSE)
      {
        /* we display the y tics specified by the user*/
        nbtics = ppsubwin->axes.u_nygrads;
        nbsubtics = ppsubwin->axes.nbsubtics[1];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char *foo = ppsubwin->axes.u_ylabels[i]; 
          double ytmp = ppsubwin->axes.u_ygrads[i];

          if(ytmp<yminval || ytmp>ymaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }


          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[1] == TRUE)
            ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
          /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */



          vx[0]=xm;vy[0]=ym;

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          /* 	NumberFormat(foo,((integer) (yy[0] + i*ceil((yy[1]-yy[0])/yy[3]))), */
          /* 			     ((integer) yy[2])); */
          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          /* get the size of the ticks */
          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;


          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{ 
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            /* 		    posi[0] = inint( xm+2*barlengthx - rect[2]/2); */
            /* 		    posi[0] = inint( xm+2*barlengthx - rect[2]); */
            posi[0] = inint( xm+2*barlengthx);
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if(ppsubwin->axes.axes_visible[1] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

            if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<=yminval || vyy1>=ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(ytmp>yminval && ytmp<ymaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[1] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;

            } /* end NEW */
            else
            {
              double ytmp2 = ppsubwin->axes.u_ygrads[i];
              double dy = (ppsubwin->axes.u_ygrads[i+1] - ppsubwin->axes.u_ygrads[i]) / nbsubtics;
              for(j=0;j<nbsubtics;j++)
              {
                vyy1=ytmp2+dy*j;

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }

          /***************************************************************/
          /************************* END OF COMMON PART ******************/
          /***************************************************************/
        }
      }
      else /* we display the computed tics */
      {
        AdaptGraduationsOnYBottomRight(iof,x,y,size,Ticsdir,fontid,psubwin,yminval,ymaxval,fx,0.,fz);

        lastyindex = ppsubwin->axes.nygrads - 1;

        if(lastyindex == 0)
          ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
        else
          ChoixFormatE(c_format,
          ppsubwin->axes.ygrads[0],
          ppsubwin->axes.ygrads[lastyindex],
          ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));

        nbtics = ppsubwin->axes.nygrads;
        nbsubtics = ppsubwin->axes.nbsubtics[1];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char foo[256]; 
          double ytmp2 = ppsubwin->axes.ygrads[i];

          if(ytmp2<yminval || ytmp2>ymaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          sprintf(foo,c_format,ytmp2);

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          /* F.Leray 03.11.04 Test if log scale to perform a : exp10(x) because trans3d will */
          /* re-do a log10() (that is needed for data computations) */


          if(ppsubwin->axes.reverse[1] == TRUE)
            ytmp2 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp2);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp2,&fz);


          vx[0]=xm;vy[0]=ym;

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */


          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{ 
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            /* 		    posi[0] = inint( xm+2*barlengthx - rect[2]/2); */
            /* 		    posi[0] = inint( xm+2*barlengthx - rect[2]); */
            posi[0] = inint( xm+2*barlengthx);
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if(ppsubwin->axes.axes_visible[1] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ppsubwin->logflags[1] == 'l' )
            {
              int smallersize = fontid[1]-2;
              int old_rect10[4];
              /* 		    char str[2] = "xv"; */
              int posi10[2];

              posi10[0] = posi[0] - logrect[2];
              posi10[1] = posi[1] + logrect[3];

              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

              C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

              posi[0] = old_rect10[0] + old_rect10[2];
              posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

              C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

              /* put back the current fontid */
              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }

          /* grid to put here */

          if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

            if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<=yminval || vyy1>=ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                /*  if ((xm != ixbox[5]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(ytmp2>yminval && ytmp2<ymaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[2] -iybox[3]+ym; }
                else
                {xg[1]= ixbox[3] - ixbox[4] +xm; yg[1]= iybox[3] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[3] - ixbox[4] +xm; 
                yg[1]=  iybox[2] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[1] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;

            } /* end NEW */
            else
            {
              double ytmp3 = ppsubwin->axes.ygrads[i];
              double dy = (ppsubwin->axes.ygrads[i+1] - ppsubwin->axes.ygrads[i]) / nbsubtics;
              for(j=0;j<nbsubtics;j++)
              {
                vyy1=ytmp3+dy*j;

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
                /*   trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz); */

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }
          /***************************************************************/
          /************************* END OF COMMON PART ******************/
          /***************************************************************/

        }
      }
    }
    if ( sciGetVisibility(ppsubwin->mon_y_label) )
    {
      sciLabel * ppLabel = pLABEL_FEATURE( ppsubwin->mon_y_label ) ;
      if( ppLabel->auto_rotation )
      {
        sciSetFontOrientation( ppsubwin->mon_y_label, 0 ) ;
      }

      if( ppLabel->auto_position )
      {

        int segmentStart[2] = { ixbox[4], iybox[4] } ;
        int segmentEnd[2]   = { ixbox[5], iybox[5] } ;

        /* add the bar size and ticks label size to the offset */
        int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
          constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
        computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
      }
      /* a trick to force the display with 2d scale */
      drawTextEntity( ppLabel->text ) ;

    }
  }



  /***********************/ /** bottom left side ***/
  /*** le  x-y scaling ***/ /* DISPLAY x or y graduations */
  /***********************/

  if (( xind[3]+xind[4] == 3) || ( xind[3]+xind[4] == 11))
  {
    if (pSUBWIN_FEATURE (psubwin)->project[0]==1) /* x HERE */
    {
      double fx,fy,fz;
      char c_format[5];


      integer Ticsdir[2];
      Ticsdir[0]=ixbox[4]-ixbox[5];
      Ticsdir[1]=iybox[4]-iybox[5];
      BBoxToval(&fx,&fy,&fz,xind[3],bbox);

      x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
      y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof+ 1.5*rect[3]); 

      if(ppsubwin->axes.auto_ticks[0] == FALSE)
      {
        /* we display the x tics specified by the user*/
        nbtics = ppsubwin->axes.u_nxgrads;
        nbsubtics = ppsubwin->axes.nbsubtics[0];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char *foo = ppsubwin->axes.u_xlabels[i]; 
          double xtmp = ppsubwin->axes.u_xgrads[i];

          if(xtmp<xminval || xtmp>xmaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[0] == TRUE)
            xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
          /* 		  trans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz); */


          vx[0]=xm;vy[0]=ym;

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;


          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            posi[0] = inint( xm+2*barlengthx-rect[2]); 
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if(ppsubwin->axes.axes_visible[0] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0]  )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

            if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<=xminval || vxx1>=xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                /*   if ((xm != ixbox[3]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(xtmp>xminval && xtmp<xmaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[0] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }

              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            } /* end NEW */
            else
            {
              double xtmp2 = ppsubwin->axes.u_xgrads[i];
              double dx = (ppsubwin->axes.u_xgrads[i+1] - ppsubwin->axes.u_xgrads[i]) / nbsubtics;

              for (j=1;j<nbsubtics;j++)
              {  
                vxx1=xtmp2+dx*j;

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
                /* 		  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */


                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }
          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/


        }

      }
      else /* we display the computed tics */
      {
        AdaptGraduationsOnXBottomLeft(iof,x,y,size,Ticsdir,fontid,psubwin,xminval,xmaxval,0.,fy,fz);

        lastxindex = ppsubwin->axes.nxgrads - 1;

        if(lastxindex == 0)
          ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
        else
          ChoixFormatE(c_format,
          ppsubwin->axes.xgrads[0],
          ppsubwin->axes.xgrads[lastxindex],
          ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));

        nbtics = ppsubwin->axes.nxgrads;
        nbsubtics = ppsubwin->axes.nbsubtics[0];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char foo[256]; 
          double xtmp = ppsubwin->axes.xgrads[i];

          if(xtmp<xminval || xtmp>xmaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          sprintf(foo,c_format,xtmp);

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[0] == TRUE)
            xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);

          vx[0]=xm;vy[0]=ym;

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            posi[0] = inint( xm+2*barlengthx-rect[2]); 
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if(ppsubwin->axes.axes_visible[0] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ppsubwin->logflags[0] == 'l' )
            {
              int smallersize = fontid[1]-2;
              int old_rect10[4];
              int posi10[2];

              posi10[0] = posi[0] - logrect[2];
              posi10[1] = posi[1] + logrect[3];

              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

              C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

              posi[0] = old_rect10[0] + old_rect10[2];
              posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

              C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

              /* put back the current fontid */
              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[0] > -1 && ppsubwin->axes.axes_visible[0] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[0];

            if((ppsubwin->logflags[0] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<=xminval || vxx1>=xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);


                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(xtmp>xminval && xtmp<xmaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[0] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.xgrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vxx1 = tmp_log_grads[j];

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            } /* end NEW */
            else
            {
              double xtmp2 = ppsubwin->axes.xgrads[i];
              double dx = (ppsubwin->axes.xgrads[i+1] - ppsubwin->axes.xgrads[i]) / nbsubtics;

              for (j=1;j<nbsubtics;j++)
              {  
                vxx1=xtmp2+dx*j;

                if(vxx1<xminval || vxx1>xmaxval) continue;

                if(ppsubwin->axes.reverse[0] == TRUE)
                  vxx1 = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],vxx1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz);
                /* 		  trans3d(psubwin,1,&xm,&ym,&vxx1,&fy,&fz); */


                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[0] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }
          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

        }
      }
    }

    if ( sciGetVisibility(ppsubwin->mon_x_label) )
    { 

      sciLabel * ppLabel = pLABEL_FEATURE(ppsubwin->mon_x_label) ;
      if( ppLabel->auto_rotation )
      {
        sciSetFontOrientation(ppsubwin->mon_x_label, 0 ) ;
      }


      if( ppLabel->auto_position )
      {

        int segmentStart[2] = { ixbox[3], iybox[3] } ;
        int segmentEnd[2]   = { ixbox[4], iybox[4] } ;

        /* add the bar size and ticks label size to the offset */
        int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
          constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
        computeLabelAutoPos( ppsubwin->mon_x_label, segmentStart, segmentEnd, offset ) ;
      }
      /* a trick to force the display with 2d scale */
      drawTextEntity( ppLabel->text ) ;

    }
  }
  else 
  {
    if  (pSUBWIN_FEATURE (psubwin)->project[1]==1) /* y is HERE */
    {
      double fx,fy,fz;
      char c_format[5];

      integer Ticsdir[2];
      Ticsdir[0]=ixbox[4]-ixbox[5];
      Ticsdir[1]=iybox[4]-iybox[5];
      BBoxToval(&fx,&fy,&fz,xind[3],bbox);

      x=inint((ixbox[3]+ixbox[4])/2.0 -rect[2] -iof);
      y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+ iof + 1.5*rect[3]);  

      if(ppsubwin->axes.auto_ticks[1] == FALSE)
      {
        /* we display the y tics specified by the user*/
        nbtics = ppsubwin->axes.u_nygrads;
        nbsubtics = ppsubwin->axes.nbsubtics[1];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char *foo = ppsubwin->axes.u_ylabels[i]; 
          double ytmp = ppsubwin->axes.u_ygrads[i];

          if(ytmp<yminval || ytmp>ymaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[1] == TRUE)
            ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
          /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */


          vx[0]=xm;vy[0]=ym; 

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            /* 		    posi[0] = inint( xm+2*barlengthx-rect[2]/2);  */
            posi[0] = inint( xm+2*barlengthx-rect[2]); 
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}


          if(ppsubwin->axes.axes_visible[1] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

            if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<=yminval || vyy1>=ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                /*   if ((xm != ixbox[3]) && (xm != ixbox[4])) */
                /* 				{ */ 
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(ytmp>yminval && ytmp<ymaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[1] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.u_ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;

            } /* end NEW */
            else
            {

              double ytmp2 = ppsubwin->axes.u_ygrads[i];
              double dy = (ppsubwin->axes.u_ygrads[i+1] - ppsubwin->axes.u_ygrads[i]) / nbsubtics;
              for(j=0;j<nbsubtics;j++)
              {
                vyy1=ytmp2+dy*j;

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);
                /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz); */


                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }
          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

        }
      }
      else /* we display the computed tics */
      {
        AdaptGraduationsOnYBottomLeft(iof,x,y,size,Ticsdir,fontid,psubwin,yminval,ymaxval,fx,0.,fz);

        lastyindex = ppsubwin->axes.nygrads - 1;

        if(lastyindex == 0)
          ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
        else
          ChoixFormatE(c_format,
          ppsubwin->axes.ygrads[0],
          ppsubwin->axes.ygrads[lastyindex],
          ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));

        nbtics = ppsubwin->axes.nygrads;
        nbsubtics = ppsubwin->axes.nbsubtics[1];

        maxTicksLabelSize[0] = 0 ;
        maxTicksLabelSize[1] = 0 ;

        for(i=0;i<nbtics;i++)
        {
          char foo[256]; 
          double ytmp = ppsubwin->axes.ygrads[i];

          if(ytmp<yminval || ytmp>ymaxval) 
          {
            /*   sciprint("je rejete la valeur: %lf\n\n",xtmp); */
            continue; /* cas ou TL est ON et on a des graduations qui ne seront pas affichees de tte facon */
            /* donc autant ne pas aller plus loin dans l'algo... */
          }

          sprintf(foo,c_format,ytmp);

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/

          if(ppsubwin->axes.reverse[1] == TRUE)
            ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);

          ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
          /* 		  trans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz); */


          vx[0]=xm;vy[0]=ym; 

          barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
          barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

          C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */

          C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          maxTicksLabelSize[0] = Max( maxTicksLabelSize[0], rect[2] ) ;
          maxTicksLabelSize[1] = Max( maxTicksLabelSize[1], rect[3] ) ;

          if (IsDownAxes(psubwin)){
            vx[1]=vx[0];
            vy[1]=vy[0]+iof/2;
            posi[0] = inint(xm-rect[2]/2); 
            posi[1]=inint( vy[0] + iof + rect[3]);}
          else{
            vx[1]=vx[0]+barlengthx;
            vy[1]=vy[0]+barlengthy;
            /* 		    posi[0] = inint( xm+2*barlengthx-rect[2]/2);  */
            posi[0] = inint( xm+2*barlengthx-rect[2]); 
            posi[1]=inint( ym + 2*barlengthy + rect[3]);}

          if(ppsubwin->axes.axes_visible[1] == TRUE){
            C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            if ( ppsubwin->logflags[1] == 'l' )
            {
              int smallersize = fontid[1]-2;
              int old_rect10[4];
              /* 		    char str[2] = "xv"; */
              int posi10[2];

              posi10[0] = posi[0] - logrect[2];
              posi10[1] = posi[1] + logrect[3];

              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring","10",(&posi10[0]),(&posi10[1]),PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);

              C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

              posi[0] = old_rect10[0] + old_rect10[2];
              posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);

              C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

              /* put back the current fontid */
              C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
              C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag,PI0,PI0,&ang, PD0,PD0,PD0,0L,0L);

            C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);   
            C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
          /* grid to put here */
          if ( ppsubwin->grid[1] > -1 && ppsubwin->axes.axes_visible[1] )
          {
            gstyle = pSUBWIN_FEATURE (psubwin)->grid[1];

            if((ppsubwin->logflags[1] =='l') && (i != nbtics-1))
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<=yminval || vyy1>=ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                /*    if ((xm != ixbox[3]) && (xm != ixbox[4])) */
                /* 				{  */
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
                /* 	} */
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;
            }
            else
            {
              if(ytmp>yminval && ytmp<ymaxval) 
              {
                xg[0]= xm;  yg[0]= ym;  
                if (Ishidden(psubwin))
                { xg[1]= xm; yg[1]= iybox[0] -iybox[5]+ym; }
                else
                {xg[1]= ixbox[1] - ixbox[3] +xm; yg[1]= iybox[5] - iybox[4] +ym; } 
                C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F (dr) ("xset", "line style",&gridStyle,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                xg[0]= xg[1]; yg[0]= yg[1];
                xg[1] = ixbox[1] - ixbox[3] +xm; yg[1]=  iybox[0] - iybox[4] +ym;
                C2F(dr)("xsegs","v", xg, yg, &ns,&gstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
                C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
              }
            }
          }

          /* and subtics */
          if(i != nbtics-1)
          {
            if(ppsubwin->logflags[1] =='l')
            {
              double tmp[2];
              double pas=0;
              double * tmp_log_grads = (double *) NULL;


              double * grads = ppsubwin->axes.ygrads;

              tmp[0] = exp10(grads[i]);
              tmp[1] = exp10(grads[i+1]);
              pas = (exp10(grads[i+1]) - exp10(grads[i])) / (nbsubtics );

              if((tmp_log_grads = (double *)MALLOC(nbsubtics*sizeof(double)))==NULL){
                sciprint("Error allocating tmp_log_grads\n");
                return -1;
              }

              for(j=0;j<nbsubtics;j++) tmp_log_grads[j] = log10(tmp[0]+(j)*pas);

              for(j=0;j<nbsubtics;j++)
              {
                vyy1 = tmp_log_grads[j];

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              FREE(tmp_log_grads); tmp_log_grads = (double *) NULL;

            } /* end NEW */
            else
            {

              double ytmp2 = ppsubwin->axes.ygrads[i];
              double dy = (ppsubwin->axes.ygrads[i+1] - ppsubwin->axes.ygrads[i]) / nbsubtics;
              for(j=0;j<nbsubtics;j++)
              {
                vyy1=ytmp2+dy*j;

                if(vyy1<yminval || vyy1>ymaxval) continue;

                if(ppsubwin->axes.reverse[1] == TRUE)
                  vyy1 = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],vyy1);

                ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&vyy1,&fz);

                if (IsDownAxes(psubwin))
                {
                  vx[1]=vx[0]=xm;
                  vy[0]=ym;
                  vy[1]=vy[0]+iof/4;
                }
                else
                {
                  vx[0]=xm;vy[0]=ym;
                  vx[1]= (integer) (vx[0]+barlengthx/2.0);
                  vy[1]= (integer) (vy[0]+barlengthy/2.0);
                }

                if(ppsubwin->axes.axes_visible[1] == TRUE)
                  C2F(dr)("xsegs","v", vx, vy, &ns,&ticscolor,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
            }
          }

          /***************************************************************/
          /************************* COMMON PART *************************/
          /***************************************************************/
        }
      }
    }

    if ( sciGetVisibility(ppsubwin->mon_y_label) )
    {  
      sciLabel * ppLabel =  pLABEL_FEATURE(ppsubwin->mon_y_label) ;
      if( ppLabel->auto_rotation )
      {
        sciSetFontOrientation(ppsubwin->mon_y_label, 0 ) ;
      }

      if( ppLabel->auto_position )
      {

        int segmentStart[2] = { ixbox[3], iybox[3] } ;
        int segmentEnd[2]   = { ixbox[4], iybox[4] } ;

        /* add the bar size and ticks label size to the offset */
        int offset[2] = { constOffset[0] + maxTicksLabelSize[0] + abs( barlengthx ), 
          constOffset[1] + maxTicksLabelSize[1] + abs( barlengthy )  } ;
        computeLabelAutoPos( ppsubwin->mon_y_label, segmentStart, segmentEnd, offset ) ;
      }
      /* a trick to force the display with 2d scale */
      drawTextEntity( ppLabel->text ) ;

    }
  }
  /* reset font to its current size & to current color*/ 
  if ( fontsize != -1 ){
    fontid[1] = fontsize_kp;
    C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  if ( textcolor != -1 || ticscolor != -1 ) 
    C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /***/
  /* FREE(loc); */


  return 0;
}
/*-----------------------------------------------------------------------------------------*/
