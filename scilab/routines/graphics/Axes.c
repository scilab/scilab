/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots 
 *--------------------------------------------------------------------------*/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "Math.h"
#include "PloEch.h"


#include "Entities.h" /* NG */
static sciPointObj *psubwin;/* NG */


static double  x_convert __PARAMS((char xy_type,double x[] ,int i));
static double  y_convert __PARAMS((char xy_type,double x[] ,int i));
extern void NumberFormat __PARAMS((char *str,integer k,integer a));
static void aplotv1 __PARAMS((char*));
static void aplotv2 __PARAMS((char*));
extern int version_flag();
/*--------------------------------------------------------------
 * Draw Axis or only rectangle
 *----------------------------------------------------------------*/

void axis_draw(strflag)
     char strflag[];
{ 
  /* using foreground to draw axis */
  integer verbose=0,narg,xz[10],fg,i,ixbox[5],iybox[5],p=5,n=1,color,color_kp; 
 /*  int isoflag = -1; */
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,xz+1,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 

  if (version_flag() == 0){
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
     
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
  }

  switch ( c) 
    {
    case '0' :
      break ;
    case '2' :
      if (version_flag() == 0) pSUBWIN_FEATURE (psubwin)->axes.rect = 1;  /* NG */
       
      C2F(dr)("xrect","xv",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3]
	      ,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      break;
    default :
    /*   /\* F.Leray 11.05.04 : EN DUR pour l'instant ici: *\/ */
/*       if (version_flag() == 0) { */
/* 	if(pSUBWIN_FEATURE(psubwin)->isoview == FALSE) */
/* 	  isoflag = -10; */
/* 	else */
/* 	  isoflag = 10; */
/*       } */

      if (version_flag() == 0)
	{
	  aplotv1(strflag); /* use 'i' xy_type */
	  break;
	}
      else
	{
	  if ( strflag[1] == '5' || strflag[1] =='6' )
	    /* using auto rescale */
	    aplotv1(strflag); /* use 'i' xy_type */
	  else
	    aplotv2(strflag); /* use 'r' xy_type */
	  break;
	}
    
    }



  C2F(dr)("xset","line style",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",xz+1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


}

/*--------------------------------------------------------------
 *  aplot: used to draw a box + x and y ticks and scales 
 *  xmin,ymin,xmax,ymax : are the boundary values
 *  xnax and ynax gives the ticks numbers ex: nax=[3,7];
 *  will give 8 big ticks (7 intervals) with numbers and 
 *  each big interval will be divided in 3 small intervals.
 *----------------------------------------------------------------*/

static void aplotv2(strflag) 
     char *strflag;
{
  char dir = 'l';
  int nx,ny;
  int fontsize=-1,textcolor=-1,ticscolor=-1 ; /*==> use default values  */
  int fontstyle= 0; 
  int seg =0;
  double x[4],y[4],x1,y1;
  char xstr,ystr;  
  char dirx = 'd';
  int i;
  
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  x[0] = Cscale.frect[0]; x[1] = Cscale.frect[2] ; x[2]=Cscale.Waaint1[1];
  y[0]=  Cscale.frect[1]; y[1] = Cscale.frect[3] ; y[2]=Cscale.Waaint1[3]; 

  /* Comments on the x and y arrays:
   x = [xmin,xmax,nb_subtics_on_x_axis]
   y = [ymin,ymax,nb_subtics_on_y_axis]
  */

  /** Cscale.frect[4]= xmin ymin xmax ymax **/ 

  if (version_flag() == 0) {
    Cscale.xtics[2]=pSUBWIN_FEATURE(psubwin)->axes.xlim[2];
    Cscale.ytics[2]=pSUBWIN_FEATURE(psubwin)->axes.ylim[2]; 

    /* Remis F.Leray 06.05.04 */
    for(i=0 ; i<4 ; i++ )
      Cscale.frect[i]=  pSUBWIN_FEATURE(psubwin)->FRect[i] ;

    Cscale.xtics[1] = (Cscale.frect[2] / (exp10( Cscale.xtics[2]))) ; 
    Cscale.xtics[0] = (Cscale.frect[0]  / (exp10( Cscale.xtics[2]))) ;
    Cscale.xtics[3] = inint(Cscale.xtics[1]-Cscale.xtics[0]);
    Cscale.ytics[1] = (Cscale.frect[3] / (exp10( Cscale.ytics[2]))) ; 
    Cscale.ytics[0] = (Cscale.frect[1]  / (exp10( Cscale.ytics[2]))) ;
    Cscale.ytics[3] = inint(Cscale.ytics[1]-Cscale.ytics[0]);
  }   


  switch ( c ) 
    { 
    case '3' : /* right axis */ 
      x1 = x[1]; y1 = y[0]; dir = 'r'; 
      break;
    case '4' : /* centred axis */
      seg=1;
      x1 = (x[0]+x[1])/2.0;y1=(y[0]+y[1])/2.0; 
      break ;
    case '5': /* centred at (0,0) */
      seg=1;
      x1 = y1 = 0.0; 
      break;
    case '1' : /* left axis */
    default : 
      x1 = x[0]; y1 = y[0];   
      break;
    } 
   
  if ((version_flag() == 0) && (c != '4'))
    {  
      xstr=pSUBWIN_FEATURE(psubwin)->axes.xdir;
      switch (xstr) 
	{
	case 'u':  
	  y1 = y[1];
	  dirx='u';   
	  break;
	case 'c':  
	  y1=(y[0]>0.0) ? y[0]: 0.0;
	  y1=(y[1]<0.0) ? y[0]: y1; 
	  seg =1; 
	  dirx ='d';                           
	  break;
	default :  
	  y1= y[0];;
	  dirx ='d'; 
	  break;
	}
      ystr=pSUBWIN_FEATURE(psubwin)->axes.ydir;
      switch (ystr) 
	{
	case 'r': 
	  x1=x[1];
	  dir='r';    
	  break;
	case 'c': 
	  x1=(x[0]>0.0) ? x[0]: 0.0;
	  x1=(x[1]<0.0) ? x[0]: x1; 
	  seg =1; 
	  dir ='l';                              
	  break; 
	default : 
	  x1= x[0];
	  dir ='l';  
	  break;
	}
    }
  if ( c != '4' && c != '5' ) {
    if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.rect == 0))
      seg=1; /* seg=1 means not to draw a rectangle (cases wherexy-axis is centered in the middle of the frame or in (0,0))*/
    else  
      /** frame rectangle **/
      C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3], 
	      PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if (version_flag() == 0){  
    /*  Cscale.Waaint1[i]= pSUBWIN_FEATURE (psubwin)->axes.subint[i]+1; */ /*SS 01/01/03 */ /* F.Leray Error here: Array overflowed: dim  Cscale.Waaint1 = 4 and i=4 */
    Cscale.Waaint1[0]= pSUBWIN_FEATURE (psubwin)->axes.subint[0]+1;
    Cscale.Waaint1[1]= (integer) (pSUBWIN_FEATURE (psubwin)->axes.xlim[3]); /*SS 02/01/03 */ /* Give the number of intervals on x*/
    Cscale.Waaint1[2]= pSUBWIN_FEATURE (psubwin)->axes.subint[1]+1; 
    Cscale.Waaint1[3]= (integer) (pSUBWIN_FEATURE (psubwin)->axes.ylim[3]);/*SS 02/01/03 */  /* Give the number of intervals on y*/

    /* Above: Number of tics on x-axis: Cscale.Waaint1[0]*/
    /*        Number of tics on y-axis: Cscale.Waaint1[2]*/

    /* Above: Number of subtics on x-axis: Cscale.Waaint1[1]*/
    /*        Number of subtics on y-axis: Cscale.Waaint1[3]*/

    ticscolor=pSUBWIN_FEATURE (psubwin)->axes.ticscolor;
    /*textcolor=pSUBWIN_FEATURE (psubwin)->axes.textcolor;
      fontsize=pSUBWIN_FEATURE (psubwin)->axes.fontsize;*/
    textcolor=sciGetFontForeground(psubwin);
    fontsize=sciGetFontDeciWidth(psubwin)/100;
    /* F.Leray 08.04.04 New data for Axes Font*/
    fontstyle=sciGetFontStyle(psubwin);
    
  }

 
  /** x-axis **/
  ny=1,nx=3;
  Sci_Axis(dirx,'r',x,&nx,&y1,&ny,NULL,Cscale.Waaint1[0],NULL,fontsize,textcolor,fontstyle,ticscolor,Cscale.logflag[0],seg,0);
  /** y-axis **/
  ny=3,nx=1;
  Sci_Axis(dir,'r',&x1,&nx,y,&ny,NULL,Cscale.Waaint1[2],NULL,fontsize,textcolor,fontstyle,ticscolor,Cscale.logflag[1],seg,0);
}

static void aplotv1(strflag)
     char *strflag;
{
  char dir = 'l';
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  int nx,ny,seg=0,i;
  int fontsize = -1 ,textcolor = -1 ,ticscolor = -1 ; /* default values */
  int fontstyle= 0;
  double  x1,y1;
  char xstr,ystr; 
  char dirx = 'd';
  double CSxtics[4], CSytics[4];

  seg=0; 
  
  switch ( c ) 
    { 
    case '3' : /* right axis */
      x1= Cscale.xtics[1]*exp10(Cscale.xtics[2]);
      y1= Cscale.ytics[0]*exp10(Cscale.ytics[2]);
      dir = 'r';
      break;
    case '4' : /* centred axis */
      seg=1;
      x1= (Cscale.xtics[0]+Cscale.xtics[1])*exp10(Cscale.xtics[2])/2.0;
      y1= (Cscale.ytics[0]+Cscale.xtics[1])*exp10(Cscale.ytics[2])/2.0;
      break ;
    case '5': /* centred at (0,0) */
      seg=1;
      x1 = y1 = 0.0; 
      break;
    case '1' : /* left axis */
    default :  
      x1= Cscale.xtics[0]*exp10(Cscale.xtics[2]);
      y1= Cscale.ytics[0]*exp10(Cscale.ytics[2]);
      break;
    }
       
  if ((version_flag() == 0) && (c != '4'))
    {  
      xstr=pSUBWIN_FEATURE(psubwin)->axes.xdir;
      switch (xstr) 
	{
	case 'u':  
	  y1=Cscale.ytics[1]*exp10(Cscale.ytics[2]);
	  dirx='u';   
	  break;
	case 'c':  
	  y1=(Cscale.ytics[0]*exp10(Cscale.ytics[2])>0.0)?Cscale.ytics[0]*exp10(Cscale.ytics[2]): 0.0;
	  y1=(Cscale.ytics[1]*exp10(Cscale.ytics[2])<0.0)?Cscale.ytics[0]*exp10(Cscale.ytics[2]): y1; 
	  seg =1; 
	  dirx ='d';                           
	  break;
	default :  
	  y1= Cscale.ytics[0]*exp10(Cscale.ytics[2]);
	  dirx ='d'; 
	  break;
	}
      ystr=pSUBWIN_FEATURE(psubwin)->axes.ydir;
      switch (ystr) 
	{
	case 'r': 
	  x1=Cscale.xtics[1]*exp10(Cscale.xtics[2]);
	  dir='r';    
	  break;
	case 'c': 
	  x1=(Cscale.xtics[0]*exp10(Cscale.xtics[2])>0.0)?Cscale.xtics[0]*exp10(Cscale.xtics[2]): 0.0;
	  x1=(Cscale.xtics[1]*exp10(Cscale.xtics[2])<0.0)?Cscale.xtics[0]*exp10(Cscale.xtics[2]): x1; 
	  seg =1; 
	  dir ='l';                              
	  break; 
	default : 
	  x1= Cscale.xtics[0]*exp10(Cscale.xtics[2]);
	  dir ='l';  
	  break;
	}
    }
  
  if ( c != '4' && c != '5' ) {
    if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.rect == 0))
      seg=1;
    else
      /** frame rectangle **/
      C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3], 
	      PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if (version_flag() == 0){  

    Cscale.Waaint1[0]= pSUBWIN_FEATURE (psubwin)->axes.subint[0]+1;
    Cscale.Waaint1[1]= (integer) (pSUBWIN_FEATURE (psubwin)->axes.xlim[3]); /*SS 02/01/03 */
    Cscale.Waaint1[2]= pSUBWIN_FEATURE (psubwin)->axes.subint[1]+1; 
    Cscale.Waaint1[3]= (integer) (pSUBWIN_FEATURE (psubwin)->axes.ylim[3]);/*SS 02/01/03 */

    ticscolor=pSUBWIN_FEATURE (psubwin)->axes.ticscolor;
    textcolor=sciGetFontForeground(psubwin);
    fontsize=sciGetFontDeciWidth(psubwin)/100;
    fontstyle=sciGetFontStyle(psubwin);
  }
 
  /* Handle CSxtics and CSytics instead of directly overwrite */
  /* Cscale.xtics and Cscale.ytics */

  for (i=0;i<4;i++) CSxtics[i] = Cscale.xtics[i];
  for (i=0;i<4;i++) CSytics[i] = Cscale.ytics[i];


  /** x-axis **/
  ny=1,nx=4;
  Sci_Axis(dirx,'i',CSxtics,&nx,&y1,&ny,NULL,Cscale.Waaint1[0],
	   NULL,fontsize,textcolor,fontstyle,ticscolor,Cscale.logflag[0],seg,0);
  
  /** y-axis **/
  ny=4,nx=1;
  Sci_Axis(dir,'i',&x1,&nx,CSytics,&ny,NULL,Cscale.Waaint1[2],
	   NULL,fontsize,textcolor,fontstyle,ticscolor,Cscale.logflag[1],seg,0);
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


void sci_axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag)
     char pos,xy_type;        
     double *x, *y; /* F.Leray 26.04.04 : before was double x[], y[];*/
     int *nx,*ny;
     char *str[];
     int  subtics;
     char *format; 
     int fontsize,textcolor,ticscolor,fontstyle;
     char logflag;
     int seg_flag;
{
  if (GetDriver()=='R') 
    StoreSciAxis("axis",pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag);
  Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag,0);
}

void Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag,axisbuild_flag)
     char pos,xy_type;        
     double *x, *y; 
     int *nx,*ny;
     char *str[];
     int  subtics;
     char *format; 
     int fontsize,textcolor,ticscolor,fontstyle;
     char logflag;
     int seg_flag;
     int axisbuild_flag; /* specifies if axis is built from a call to axis_draw or drawaxis method */
{
  int Nx,Ny,debug,j;
  double angle=0.0,vxx,vxx1;
  int vx[2],vy[2],xm[2],ym[2];
  char c_format[5];
  integer flag=0,xx=0,yy=0,posi[2],rect[4],dash[6],trois=3;/**DJ.Abdemouche 2003**/
  integer i,barlength;
  int ns=2,style=0,iflag=0;
  integer fontid[2],fontsize_kp, narg,verbose=0,logrect[4],smallersize,color_kp; 
  integer pstyle;
  int x3, y3, xpassed = 0, ypassed = 0; /* for log. case management */
 /*** 01/07/2002 -> 11.05.04 ***/ /* F.Leray : Re-put the Djalel modif. for a better display 
			 using tight_limits='on' and/or isoview='on' */
  double xmin,xmax,ymin, ymax; 
  sciPointObj * psubwin = NULL;
  double pas; /* used for logarithmic grid */

  fontid[0]= fontstyle;
  
  if (version_flag() == 0)
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  fontsize_kp = fontid[1] ;
  if ((version_flag() == 0) && ( fontsize == -1 ))
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

  if (logflag == 'l' )
    {
      C2F(dr)("xstringl","10",&xx,&yy,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      smallersize=fontid[1]-2;
      C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  if (version_flag() == 0) 
    {
      /* Pb here, dim of x and y can be lesser than 4 (ie in example_eng.tst : nx = 6 and ny = 1) F.Leray 25.02.04
       x and y are either double x[3] (resp. y[3]) or simply a double !  F.Leray 05.03.04
       So a test on x[3] (resp. y[3]) is ALWAYS bad!!
       NO!! It depends on the xy_type as follow (see in aplotv1):*/
      /*
       *   xy_type = 'v' (for vector) or 'r' (for range) 
       *         'v' means that tics position are given by a vector 
       *         'r' means that tics position are in a range i.e given by a vector of size 3 
       *             [min,max,number_of_intervals] 
       *         'i' means that tics positions are in a range given by four number (integers) 
       *             [k1,k2,e,number_of intervale] -> [k1*10^e,k2*10^e] */

      if(xy_type == 'i') {     /* Adding F.Leray 05.03.04*/
	/*F.Leray Modification on 09.03.04*/
	switch ( pos ) {
	case 'u' : case 'd' :  
	  if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n')
	    while (x[3]>10)  x[3]=floor(x[3]/2); 
	  else{
	    if(x[3] > 12){ /* F.Leray arbitrary value=12 for the moment */
	      x3=(int)x[3];     /* if x[3]>12 algo is triggered to search a divisor */
	      for(j=x3-1;j>1;j--)
		if(x3%j == 0){
		  x[3]=j; 
		  xpassed = 1;
		}
	      if(xpassed != 1) x[3] = 1;
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

      /*F.Leray 26.02.04*/
      debug = version_flag();

      vx[0] =  XScale(x_convert(xy_type, x , 0));
      vx[1] =  XScale(x_convert(xy_type, x , Nx-1));
      vy[0]= vy[1] = ym[0] = YScale(y[0]);
      if ( seg_flag == 1) 
	{
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      /******* 01/07/2002  -> 11.05.04 **********/
      if (version_flag() == 0) psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
      if ((version_flag() == 0) 
	  && (pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
	  && axisbuild_flag == 0){  
	xmax=Cscale.frect[2];
	xmin=Cscale.frect[0];

	if(xy_type == 'i')
	  {
	 /*    x[1] = floor(Cscale.frect[2] / (exp10( x[2]))) ;   */
/* 	    x[0] =  ceil(Cscale.frect[0]  / (exp10( x[2]))) ;  */

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
	  /*   x[1] = floor(Cscale.frect[2]) ;   */
/* 	    x[0] =  ceil(Cscale.frect[0]) ;  */
/* 	    x[2]=inint(x[1]-x[0]); */
/* 	    while (x[2]>10)  x[2]=floor(x[2]/2); */
/* 	    Nx=x[2]+1;  */
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
	  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /*** MAJ Djalel.A 21/01/2003 ***/ 
	  if (version_flag() == 0) 
	    if (pSUBWIN_FEATURE (psubwin)->grid[0] > -1) { /**DJ.Abdemouche 2003**/
	      if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'l'){
		if ((vx[0] != Cscale.WIRect1[0]) && (vx[0] != (Cscale.WIRect1[0]+ Cscale.WIRect1[2])))
		  {
		    pstyle=pSUBWIN_FEATURE (psubwin)->grid[0] ;
		    C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    C2F (dr) ("xset", "line style",&trois,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		    vy[0]=Cscale.WIRect1[1];
		    vy[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];  
		    C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }
		
		if(i < Nx -1 ){
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
		  C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	      }
	      else if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'n'){
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
	      int j;
	      double dx ; 
	      vxx1= x_convert(xy_type,x,i+1);
	      dx = (vxx1-vxx)/subtics;
	      for ( j = 1 ; j < subtics; j++){ 
		vx[0] = vx[1] = XScale(vxx+dx*j);
		if ( pos == 'd' ) 
		  { vy[0]= ym[0];vy[1]=  (integer) (ym[0] + barlength/2.0) ; }
		else 
		  { vy[0]= ym[0];vy[1]=  (integer) (ym[0] - barlength/2.0) ; }
		C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      }
	    } 
	  /***   01/07/2002  -> 11.05.04 ****/
	  if (version_flag() == 0) psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
	  if ((version_flag() == 0) 
	      && (pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
	      && axisbuild_flag == 0){  
	    if ( i == 0 ) 
	      {
		int j;
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
		    C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }}
	      } 
	    if ( i == Nx-1 ) 
	      {
		int j;
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
		    C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
	  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}

      /***   01/07/2002 -> 11 and 12.05.04 ****/
      if (version_flag() == 0) psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
      if ((version_flag() == 0) 
	  && (pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
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
	   /*  y[1] = floor(Cscale.frect[3]) ;   */
/* 	    y[0] =  ceil(Cscale.frect[1]) ;  */
/* 	    y[2]=inint(y[1]-y[0]); */
/* 	    while (y[2]>10)  y[2]=floor(y[2]/2); */
/* 	    Ny=y[2]+1;  */
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
	  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /*** MAJ Djalel.A 21/01/2003 ***/ 
	  if (version_flag() == 0)
	    if (pSUBWIN_FEATURE (psubwin)->grid[1] > -1 ){ /**DJ.Abdemouche 2003**/
	      if(pSUBWIN_FEATURE (psubwin)->logflags[1] == 'l'){
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
	      int j;
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
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	    }
	  /***   01/07/2002 -> 12.05.04 ****/
	  if (version_flag() == 0) psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
	  if ((version_flag() == 0) 
	      && (pSUBWIN_FEATURE (psubwin)->tight_limits == TRUE || pSUBWIN_FEATURE (psubwin)->isoview == TRUE )
	      && axisbuild_flag == 0){
	    if ( i == 0 )  
	      {
		int j;
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
		    C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		  }}
	      }
	    if ( i == Ny-1 )
	      {
		int j;
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
		    C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
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

/* from double to pixel */ 

static double  x_convert(xy_type, x , i)
     char xy_type;
     double *x;
     int i;
{
  switch ( xy_type ) { 
  case 'v' :  return x[i];
  case 'r' :  return x[0]+i*(x[1]-x[0])/x[2];
  case 'i' :  return exp10(x[2])*(x[0] + i*(x[1]-x[0])/x[3]);
  }
  return 0.0;
}

static double y_convert(xy_type, y , i)
     char xy_type;
     double *y;
     int i;
{
  switch ( xy_type ) { 
  case 'v' :  return y[i]; 
  case 'r' :  return y[0]+i*(y[1]-y[0])/y[2];
  case 'i' :  return exp10(y[2])*(y[0] + i*(y[1]-y[0])/y[3]); 
  }
  return 0.0; 
}



/* Format pour imprimer un nombre de la forme k10^a */

extern void NumberFormat(str, k, a)
     char *str;
     integer k;
     integer a;
{
  if ( k==0)
    {
      sprintf(str,"0");
    }
  else
    {
      switch (a)
	{
	case -1: sprintf(str,"%.1f",(double)k/10.0);break;
	case -2: sprintf(str,"%.2f",(double)k/100.0);break;
	case 0 : sprintf(str,"%d",(int)k);break;
	case 1 : sprintf(str,"%d0",(int)k);break;
	case 2 : sprintf(str,"%d00",(int)k);break;
	default: sprintf(str,"%de%d",(int)k,(int)a) ;break;

	}
    }

}

/*---------------------------------------------------------------------
 *Trace l'enveloppe convexe de la boite contenant le dessin 
 * et renvoit dans InsideU et InsideD les indices des points dans xbox et ybox
 * qui sont sur les 2 tri\`edres a l'interieur de l'enveloppe convexe
 *---------------------------------------------------------------------*/

void Convex3d_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox)
{
  double xmaxi;
  integer ixbox[8],iybox[8];
  integer xind[8];
  integer ind2,ind3,ind;
  integer p,n,dvect[1],dash[6];
  integer verbose=0,narg,pat;
  integer i,j;
  /** dans xbox[8] se trouve l'abscisse des points successifs   **/
  /** de la boite qui continent la surface                      **/
  /** on stocke dans xind[8] les indices des points de la boite **/
  /** qui sont sur l'enveloppe convexe en partant du pointeger en haut **/
  /** a droite et en tournant ds le sens trigonometrique           **/
  /** par exemple avec : **/
  /*      4 ----- 5        */
  /*       /    /|         */
  /*     7----6  |         */
  /*      | 0 | / 1        */
  /*     3----- 2          */
  /** on doit trouver xind={5,4,7,3,2,1}; **/
  /** on en profite pour stocker aussi les points des triedres **/

  xmaxi=((double) Maxi(xbox,8L));
  ind= -1;
  for (i =0 ; i < 8 ; i++)
    {
      MaxiInd(xbox,8L,&ind,xmaxi);
      if ( ind > 3)
	{
	  xind[0]=ind;
	  break;
	}
    }
  if (ind < 0 || ind > 8) 
    {
      Scistring("xind out of bounds");
      xind[0]=0;
    }
  UpNext(xind[0],&ind2,&ind3);
  if (ybox[ind2] > ybox[ind3]) 
    {
      xind[1]=ind2;InsideU[0]=ind3;
    }
  else 
    {
      xind[1]=ind3;InsideU[0]=ind2;
    }
  UpNext(ind2,&ind2,&ind3); InsideU[1]=xind[0];
  InsideU[2]=ind2; InsideU[3]=InsideU[0]-4;
  xind[2]=ind2;
  /* le pointeger en bas qui correspond */
  xind[3]=ind2-4;
  DownNext(xind[3],&ind2,&ind3);
  if (ybox[ind2] < ybox[ind3]) 
    {
      xind[4]=ind2;InsideD[0]=ind3;
    }
  else  
    {
      xind[4]=ind3;InsideD[0]=ind2;
    }
  DownNext(ind2,&ind2,&ind3);
  InsideD[1]=xind[3];
  InsideD[2]=ind2;
  InsideD[3]=InsideD[0]+4;
  xind[5]=ind2;
  for (i=0; i < 6 ; i++)
    {
      ixbox[i]=XScale(xbox[xind[i]]);
      iybox[i]=YScale(ybox[xind[i]]);
    }
  ixbox[6]=ixbox[0];iybox[6]=iybox[0];
  p=7,n=1;
  C2F(dr)("xget","foreground",&verbose,dvect,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /** On trace l'enveloppe cvxe **/
  C2F(dr)("xget","line style",&verbose,dash,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(j=1,&j),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
  if (flag[2]>=3){
    C2F(dr)("xpolys","v",ixbox,iybox,dvect,&n,&p
	    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  C2F(dr)("xget","pattern",&verbose,&pat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",dvect,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  if (flag[2]>=3)AxesStrings(flag[2],ixbox,iybox,xind,legend,bbox);
  C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}
