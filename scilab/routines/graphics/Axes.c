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
sciPointObj *psubwin;/* NG */


static double  x_convert __PARAMS((char xy_type,double x[] ,int i));
static double  y_convert __PARAMS((char xy_type,double x[] ,int i));
static void NumberFormat __PARAMS((char *str,integer k,integer a));
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
  integer verbose=0,narg,xz[10],fg,i;
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","color",&verbose,xz+1,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
   if (version_flag() == 0)
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());

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
      if ( strflag[1] == '5' || strflag[1] =='6' )
	/* using auto rescale */
	aplotv1(strflag);
      else
	aplotv2(strflag);
      break;
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
  int seg =0;
  double x[3],y[3],x1,y1;
  /*** Ajout D.ABDEMOUCHE ****/
  char xstr,ystr;  
  char dirx = 'd';
  int i;
  
char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  x[0] = Cscale.frect[0]; x[1] = Cscale.frect[2] ; x[2]=Cscale.Waaint1[1];
  y[0]=  Cscale.frect[1]; y[1] = Cscale.frect[3] ; y[2]=Cscale.Waaint1[3]; 

  /** Cscale.frect[4]= xmin ymin xmax ymax **/ 

  if (version_flag() == 0) {
      Cscale.xtics[2]=pSUBWIN_FEATURE(psubwin)->axes.xlim[2];
      Cscale.ytics[2]=pSUBWIN_FEATURE(psubwin)->axes.ylim[2]; 
      for(i=0 ; i<4 ; i++ )
             Cscale.frect[i]=  pSUBWIN_FEATURE(psubwin)->FRect[i] ;
       Cscale.xtics[1]= (Cscale.frect[2] / (exp10( Cscale.xtics[2]))) ; 
       Cscale.xtics[0]  = (Cscale.frect[0]  / (exp10( Cscale.xtics[2]))) ;
       Cscale.xtics[3]=inint(Cscale.xtics[1]-Cscale.xtics[0]);
       Cscale.ytics[1]= (Cscale.frect[3] / (exp10( Cscale.ytics[2]))) ; 
       Cscale.ytics[0]  = (Cscale.frect[1]  / (exp10( Cscale.ytics[2]))) ;
       Cscale.ytics[3]=inint(Cscale.ytics[1]-Cscale.ytics[0]);}   
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
      seg=1;
    else  
    /** frame rectangle **/
    C2F(dr)("xrect","v",&Cscale.WIRect1[0],&Cscale.WIRect1[1],&Cscale.WIRect1[2],&Cscale.WIRect1[3], 
	    PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  if (version_flag() == 0){  
    Cscale.Waaint1[i]= pSUBWIN_FEATURE (psubwin)->axes.subint[i]+1; /*SS 01/01/03 */
    Cscale.Waaint1[0]= pSUBWIN_FEATURE (psubwin)->axes.subint[0]+1;
    Cscale.Waaint1[1]= pSUBWIN_FEATURE (psubwin)->axes.xlim[3]; /*SS 02/01/03 */
    Cscale.Waaint1[2]= pSUBWIN_FEATURE (psubwin)->axes.subint[1]+1; 
    Cscale.Waaint1[3]= pSUBWIN_FEATURE (psubwin)->axes.ylim[3];/*SS 02/01/03 */

    ticscolor=pSUBWIN_FEATURE (psubwin)->axes.ticscolor;
    textcolor=pSUBWIN_FEATURE (psubwin)->axes.textcolor;
    fontsize=pSUBWIN_FEATURE (psubwin)->axes.fontsize;
  }

 
  /** x-axis **/
  ny=1,nx=3;
  Sci_Axis(dirx,'r',x,&nx,&y1,&ny,NULL,Cscale.Waaint1[0],NULL,fontsize,textcolor,ticscolor,Cscale.logflag[0],seg);
  /** y-axis **/
  ny=3,nx=1;
  Sci_Axis(dir,'r',&x1,&nx,y,&ny,NULL,Cscale.Waaint1[2],NULL,fontsize,textcolor,ticscolor,Cscale.logflag[1],seg);
}

static void aplotv1(strflag)
     char *strflag;
{
  char dir = 'l';
  char c = (strlen(strflag) >= 3) ? strflag[2] : '1';
  int nx,ny,seg=0,i;
  int fontsize = -1 ,textcolor = -1 ,ticscolor = -1 ; /* default values */
  double  x1,y1;
  /*** Ajout D.ABDEMOUCHE ****/  
  char xstr,ystr; 
  char dirx = 'd';  

  seg=0; 

  if (version_flag() == 0){    
    Cscale.xtics[2]=pSUBWIN_FEATURE(psubwin)->axes.xlim[2];
    Cscale.ytics[2]=pSUBWIN_FEATURE(psubwin)->axes.ylim[2];
    for(i=0 ; i<4 ; i++ )
      Cscale.frect[i]=  pSUBWIN_FEATURE(psubwin)->FRect[i] ;
    /*** parceque c'est pas la subwin selectionnee mais celle en cours ***/
    Cscale.xtics[1]= (Cscale.frect[2] / (exp10( Cscale.xtics[2]))) ;
    Cscale.xtics[0]= (Cscale.frect[0]  / (exp10( Cscale.xtics[2]))) ;
    Cscale.xtics[3]= floor(Cscale.xtics[1])-ceil(Cscale.xtics[0]);  
    Cscale.ytics[1]= (Cscale.frect[3] / (exp10( Cscale.ytics[2]))) ; 
    Cscale.ytics[0]  = (Cscale.frect[1]  / (exp10( Cscale.ytics[2]))) ;
    Cscale.ytics[3]= floor(Cscale.ytics[1])-ceil(Cscale.ytics[0]); 

  }       
     
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
    Cscale.Waaint1[1]= pSUBWIN_FEATURE (psubwin)->axes.xlim[3]; /*SS 02/01/03 */
    Cscale.Waaint1[2]= pSUBWIN_FEATURE (psubwin)->axes.subint[1]+1; 
    Cscale.Waaint1[3]= pSUBWIN_FEATURE (psubwin)->axes.ylim[3];/*SS 02/01/03 */

     ticscolor=pSUBWIN_FEATURE (psubwin)->axes.ticscolor;
     textcolor=pSUBWIN_FEATURE (psubwin)->axes.textcolor;
     fontsize=pSUBWIN_FEATURE (psubwin)->axes.fontsize;
  
   }
 
  /** x-axis **/
  ny=1,nx=4;
  Sci_Axis(dirx,'i',Cscale.xtics,&nx,&y1,&ny,NULL,Cscale.Waaint1[0],
	   NULL,fontsize,textcolor,ticscolor,Cscale.logflag[0],seg);
  
  /** y-axis **/
  ny=4,nx=1;
  Sci_Axis(dir,'i',&x1,&nx,Cscale.ytics,&ny,NULL,Cscale.Waaint1[2],
	   NULL,fontsize,textcolor,ticscolor,Cscale.logflag[1],seg);
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


void sci_axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,ticscolor,logflag,seg_flag)
     char pos,xy_type;        
     double x[],y[];  
     int *nx,*ny;
     char *str[];
     int  subtics;
     char *format; 
     int fontsize,textcolor,ticscolor;
     char logflag;
     int seg_flag;
{
  if (GetDriver()=='R') 
    StoreSciAxis("axis",pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,ticscolor,logflag,seg_flag);
  Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,ticscolor,logflag,seg_flag);
}

void Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,ticscolor,logflag,seg_flag)
     char pos,xy_type;        
     double x[],y[];  
     int *nx,*ny;
     char *str[];
     int  subtics;
     char *format; 
     int fontsize,textcolor,ticscolor;
     char logflag;
     int seg_flag;
{
  int Nx,Ny;
  double angle=0.0,vxx,vxx1;
  int vx[2],vy[2],xm[2],ym[2];
  char c_format[5];
  integer flag=0,xx=0,yy=0,posi[2],rect[4];
  integer i,barlength;
  int ns=2,style=0,iflag=0;
  integer fontid[2],fontsize_kp, narg,verbose=0,logrect[4],smallersize,color_kp; 
  /*** 01/07/2002 ***/
  double xmin,xmax,ymin, ymax;
   /*** MAJ Djalel A 21/01/2003 ***/
   integer pstyle;


 if (version_flag() == 0)
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  
 C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  fontsize_kp = fontid[1] ;
  /** 28/10/2002 **/
  if ((version_flag() == 0) && ( fontsize == -1 ))
     { fontid[0]= 0; fontid[1]= 1;  fontsize_kp = fontid[1] ;
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);}
  
  if ( fontsize != -1 ) 
    {
      fontid[1] = fontsize ;
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
  /**** 01/07/2002 ***/ 
  if (version_flag() == 0) 
      { while (x[3]>10)  x[3]=floor(x[3]/2);
        while (y[3]>10)  y[3]=floor(y[3]/2); }
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
      barlength = Cscale.WIRect1[3]/50.0;
      /** compute a format **/
      if (str == NULL && format == NULL )  
	switch (xy_type ) {
	case 'v' : ChoixFormatE1(c_format,x,Nx);break;
	case 'r' : ChoixFormatE (c_format,x[0],x[1],(x[1]-x[0])/x[2]);break;
	}
      /** the horizontal segment **/
      vx[0] =  XScale(x_convert(xy_type, x , 0));
      vx[1] =  XScale(x_convert(xy_type, x , Nx-1));
      vy[0]= vy[1] = ym[0] = YScale(y[0]);
      if ( seg_flag == 1) 
	{
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&ticscolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if ( ticscolor != -1 ) C2F(dr)("xset","pattern",&color_kp,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
        /******* 01/07/2002 **********/
       if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.limits[0] ==1) && (sciGetEntityType (sciGetCurrentObj()) != SCI_AXES)){  
	 xmax=Cscale.frect[2];
	 xmin=Cscale.frect[0];
        x[1]= floor(Cscale.frect[2] / (exp10( x[2]))) ;  
        x[0]  = ceil(Cscale.frect[0]  / (exp10( x[2]))) ; 
        x[3]=inint(x[1]-x[0]);
        while (x[3]>10)  x[3]=floor(x[3]/2);
        Nx=x[3]+1;}
      /**********************/
      /** loop on the ticks **/
      for (i=0 ; i < Nx ; i++)
	{ 
	  char foo[100];
	  vxx = x_convert(xy_type,x,i);
	  if ( str != NULL)  
	    sprintf(foo,"%s",str[i]);
	  else if ( format == NULL) 
	    {
	      /*defaults format **/
	      if  ( xy_type == 'i') 
		NumberFormat(foo,((integer) (x[0] + i*(x[1]-x[0])/x[3])),
			     ((integer) x[2]));
	      else 
		sprintf(foo,c_format,vxx);
	    }
	  else 
	    sprintf(foo,format,vxx);
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
	    if ((vx[0] != Cscale.WIRect1[0]) && (vx[0] != (Cscale.WIRect1[0]+ Cscale.WIRect1[2])))
	      if (pSUBWIN_FEATURE (psubwin)->grid > -1)
		{
		  pstyle=pSUBWIN_FEATURE (psubwin)->grid ;
		vy[0]=Cscale.WIRect1[1];
		vy[1]=Cscale.WIRect1[1]+Cscale.WIRect1[3];  
		C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
		    { vy[0]= ym[0];vy[1]= ym[0] + barlength/2.0 ; }
		  else 
		    { vy[0]= ym[0];vy[1]= ym[0] - barlength/2.0; }
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	    } 
	  /***   01/07/2002 ****/
      if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.limits[0] ==1)&& (sciGetEntityType (sciGetCurrentObj()) != SCI_AXES)){
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
		    { vy[0]= ym[0];vy[1]= ym[0] + barlength/2.0 ; }
		  else 
		    { vy[0]= ym[0];vy[1]= ym[0] - barlength/2.0; }
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
		    { vy[0]= ym[0];vy[1]= ym[0] + barlength/2.0 ; }
		  else 
		    { vy[0]= ym[0];vy[1]= ym[0] - barlength/2.0; }
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
      barlength = Cscale.WIRect1[2]/75.0;
      if (str == NULL &&  format == NULL )  
	switch (xy_type ) {
	case 'v' : ChoixFormatE1(c_format,y,Ny);break;
	case 'r' : ChoixFormatE(c_format,y[0],y[1],(y[1]-y[0])/y[2]);break;
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
       /******* 01/07/2002 **********/
       if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.limits[0] ==1)&& (sciGetEntityType (sciGetCurrentObj()) != SCI_AXES)){  
	 ymax=Cscale.frect[3];
	 ymin=Cscale.frect[1];
        y[1]= floor(Cscale.frect[3] / (exp10( y[2]))) ;  
        y[0]  = ceil(Cscale.frect[1]  / (exp10( y[2]))) ; 
        y[3]=inint(y[1]-y[0]);
        while (y[3]>10)  y[3]=floor(y[3]/2);
        Ny=y[3]+1;}
      /**********************/
      /** loop on the ticks **/
      for (i=0 ; i < Ny ; i++)
	{ 
	  char foo[100];
	  vxx = y_convert(xy_type,y,i);
	  if ( str != NULL)  
	    sprintf(foo,"%s",str[i]);
	  else if ( format == NULL)
	    {
	      if ( xy_type == 'i') 
		NumberFormat(foo,((integer) (y[0] + i*(y[1]-y[0])/y[3])),
			     ((integer) y[2]));
	      else 
		sprintf(foo,c_format,vxx);
	    }
	  else 
	    sprintf(foo,format,vxx);

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
            if ((vy[0] != Cscale.WIRect1[1]) && (vy[0] != (Cscale.WIRect1[1]+ Cscale.WIRect1[3])))
	      if (pSUBWIN_FEATURE (psubwin)->grid > -1)
		{
		  pstyle=pSUBWIN_FEATURE (psubwin)->grid ;
		  vx[0]=Cscale.WIRect1[0];
		  vx[1]=Cscale.WIRect1[0]+Cscale.WIRect1[2]; 
		C2F(dr)("xsegs","v", vx, vy, &ns,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
		    { vx[0]= xm[0];vx[1]= xm[0] + barlength/2.0 ; }
		  else 
		    { vx[0]= xm[0];vx[1]= xm[0] - barlength/2.0; }
		  C2F(dr)("xsegs","v", vx, vy, &ns,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
		}
	    }
  /***   01/07/2002 ****/
      if ((version_flag() == 0) && (pSUBWIN_FEATURE (psubwin)->axes.limits[0] ==1)&& (sciGetEntityType (sciGetCurrentObj()) != SCI_AXES)){
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
		    { vx[0]= xm[0];vx[1]= xm[0] + barlength/2.0 ; }
		  else 
		    { vx[0]= xm[0];vx[1]= xm[0] - barlength/2.0; }
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
		    { vx[0]= xm[0];vx[1]= xm[0] + barlength/2.0 ; }
		  else 
		    { vx[0]= xm[0];vx[1]= xm[0] - barlength/2.0; }
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

static void NumberFormat(str, k, a)
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
