/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

/*----------------------BEGIN----------------------
\def\encadre#1{\paragraph{}\fbox{\begin{minipage}[t]{15cm}#1 \end{minipage}}}
\section{A Postscript Driver}
---------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif



#if defined(THINK_C) || defined (__MWERKS__)|| defined(WIN32)
#define CoordModePrevious 0
#define CoordModeOrigin 1
#define GXclear 0
#define GXand 1
#define GXandReverse 2
#define GXcopy 3
#define GXandInverted 4
#define GXnoop 5
#define GXxor 6
#define GXor 7
#define GXnor 8
#define GXequiv 9
#define GXinvert 10
#define GXorReverse 11
#define GXcopyInverted 12
#define GXorInverted 13
#define GXnand 14
#define GXset 15
#else
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#endif
#include "Math.h"
#include "periPos.h"
#include "color.h"


static int C2F(PosQueryFont)();
static void C2F(displaysymbolsPos)();
static void WriteColorRGB();
static void WriteColorRGBDef();

#define Char2Int(x)   ( x & 0x000000ff )

static double *vdouble = 0; /* used when a double argument is needed */

#if defined(__CYGWIN32__) || defined(__MINGW32__) || defined(__GNUC__)
static FILE *file= (FILE *) 0;
#define FPRINTF(x) ( file != (FILE*) 0) ?  fprintf x  : 0 
#else 
#define FPRINTF(x) fprintf x  
static FILE *file= stdout ;
#endif

void FileInit  __PARAMS((void));

/** Structure to keep the graphic state  **/

struct BCG 
{ 
   int FontSize;
   int FontId;
   int CurHardSymb;
   int CurHardSymbSize;
   int CurLineWidth;
   int CurPattern;
   int CurColor;
   int CurWindow;
   int CurVectorStyle;
   int CurDrawFunction;
   int ClipRegionSet;
   int CurClipRegion[4];
   int CurDashStyle;
   char CurNumberDispFormat[20];
   int CurColorStatus;
   int IDLastPattern; /* number of last patter or color */
   int Numcolors; /* number of colors */
  int   NumBackground;  /* number of Background in the color table */
  int  NumForeground; /* number of Foreground in the color table */
  int NumHidden3d;  /* color for hidden 3d facets **/
}  ScilabGCPos ;

/*-----------------------------------------------------
\encadre{General routines}
-----------------------------------------------------*/

/** To select the graphic Window  **/

void C2F(xselgraphicPos)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{}

/** End of graphic (close the file) **/

void C2F(xendgraphicPos)()
{
  if (file != stdout && file != (FILE*) 0) {
    FPRINTF((file,"\n showpage\n"));
    FPRINTF((file,"\n end saved restore \n"));
    fclose(file);
    file=stdout;}
}

void C2F(xendPos)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  C2F(xendgraphicPos)();
}


/** Clear the current graphic window     **/
/** In Postscript : nothing      **/

void C2F(clearwindowPos)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  /* FPRINTF((file,"\n showpage")); */
  /** Sending the scale etc.. in case we want an other plot **/
  /* FileInit(file); */
}

/** To generate a pause : Empty here **/

void C2F(xpausePos)(str, sec_time, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *sec_time;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{}

/** Wait for mouse click in graphic window : Empty here **/

void C2F(xclickPos)(str, ibutton, xx1, yy1, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *ibutton;
     integer *xx1;
     integer *yy1;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ }

void C2F(xclick_anyPos)(str, ibutton, xx1, yy1, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *ibutton;
     integer *xx1;
     integer *yy1;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ }

void C2F(xgetmousePos)(str, ibutton, xx1, yy1, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *ibutton;
     integer *xx1;
     integer *yy1;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ }

/** Clear a rectangle **/

void C2F(clearareaPos)(str, x, y, w, h, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *w;
     integer *h;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  FPRINTF((file,"\n [ %d %d %d %d ] clearzone",(int)*x,(int)*y,(int)*w,(int)*h));
}



/*---------------------------------------------------------------------
\section{Function for graphic context modification}
------------------------------------------------------------------------*/

/** to get the window upper-left pointeger coordinates **/

void C2F(getwindowposPos)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{
  *narg = 2;
  x[0]= x[1]=0;
  if (*verbose == 1) 
    sciprint("\n CWindow position :%d,%d\r\n",(int)x[0],(int)x[1]);
 }

/** to set the window upper-left pointeger position (Void) **/

void C2F(setwindowposPos)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
}

/** To get the window size **/
/** In Postscript we choose (600,424) **/
/** This size was chosen to have good compatibility with X11 **/
/** for line thickness etc \ldots **/

static integer prec_fact =10;

void C2F(getwindowdimPos)(verbose, x, narg, dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{     
  *narg = 2;
  x[0]= 600*prec_fact;
  x[1]= 424*prec_fact;
  if (*verbose == 1) 
    sciprint("\n CWindow dim :%d,%d\r\n",(int)x[0],(int)x[1]);
} 

/** To change the window dimensions : do Nothing in Postscript  **/

void C2F(setwindowdimPos)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
}


/** Select a graphic Window : Empty for Postscript **/

void C2F(setcurwinPos)(intnum, v2, v3, v4)
     integer *intnum;
     integer *v2;
     integer *v3;
     integer *v4;
{
  ScilabGCPos.CurWindow = *intnum;
}

/** Get the id number of the Current Graphic Window **/

void C2F(getcurwinPos)(verbose, intnum, narg,dummy)
     integer *verbose;
     integer *intnum;
     integer *narg;
     double *dummy;
{
  *narg =1 ;
  *intnum = ScilabGCPos.CurWindow ;
  if (*verbose == 1) 
    Scistring("\nJust one graphic page at a time ");
}

/** Set a clip zone (rectangle ) **/

void C2F(setclipPos)(x, y, w, h)
     integer *x;
     integer *y;
     integer *w;
     integer *h;
{
  ScilabGCPos.ClipRegionSet = 1;
  ScilabGCPos.CurClipRegion[0]= *x;
  ScilabGCPos.CurClipRegion[1]= *y;
  ScilabGCPos.CurClipRegion[2]= *w;
  ScilabGCPos.CurClipRegion[3]= *h;
  FPRINTF((file,"\n%d %d %d %d setclipzone",(int)*x,(int)*y,(int)*w,(int)*h));
}

/** unset clip zone **/

void C2F(unsetclipPos)(v1, v2, v3, v4)
     integer *v1;
     integer *v2;
     integer *v3;
     integer *v4;
{
  ScilabGCPos.ClipRegionSet = 0;
  ScilabGCPos.CurClipRegion[0]= -1;
  ScilabGCPos.CurClipRegion[1]= -1;
  ScilabGCPos.CurClipRegion[2]= 200000;
  ScilabGCPos.CurClipRegion[3]= 200000;
  FPRINTF((file,"\n%d %d %d %d setclipzone",-1,-1,200000,200000));
}

/** Get the boundaries of the current clip zone **/

void C2F(getclipPos)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{
  x[0] = ScilabGCPos.ClipRegionSet;
  if ( x[0] == 1)
    {
      *narg = 5;
      x[1] =ScilabGCPos.CurClipRegion[0];
      x[2] =ScilabGCPos.CurClipRegion[1];
      x[3] =ScilabGCPos.CurClipRegion[2];
      x[4] =ScilabGCPos.CurClipRegion[3];
    }
  else *narg = 1;
  if (*verbose == 1)
    {
      if (ScilabGCPos.ClipRegionSet == 1)
	sciprint("\nThere's a Clip Region :x:%d,y:%d,w:%d,h:%d\r\n",
		 ScilabGCPos.CurClipRegion[0],
		 ScilabGCPos.CurClipRegion[1],
		 ScilabGCPos.CurClipRegion[2],
		 ScilabGCPos.CurClipRegion[3]);
      else 
	Scistring("\nNo Clip Region");
    }
}

/*----------------------------------------------------------
\encadre{For the drawing functions dealing with vectors of 
 points, the following routine is used to select the mode 
 absolute or relative }
 Absolute mode if *num==0, relative mode if *num != 0
------------------------------------------------------------*/

void C2F(setabsourelPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if (*num == 0 )
    ScilabGCPos.CurVectorStyle =  CoordModeOrigin;
  else 
    ScilabGCPos.CurVectorStyle =  CoordModePrevious ;
}

/** to get information on absolute or relative mode **/

void C2F(getabsourelPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *narg = 1;
  *num = ScilabGCPos.CurVectorStyle  ;
  if (*verbose == 1) 
    {
      if (ScilabGCPos.CurVectorStyle == CoordModeOrigin)
	Scistring("\nTrace Absolu");
      else 
	Scistring("\nTrace Relatif");
    }
}


/** The alu function for drawing : Works only with X11 **/
/** Not in Postscript **/

void C2F(setalufunctionPos)(string)
     char *string;
{    
  integer value;
  
  C2F(idfromnamePos)(string,&value);
  if ( value != -1)
    {ScilabGCPos.CurDrawFunction = value;
     FPRINTF((file,"\n%% %d setalufunction",(int)value));
      }
}

/** All the possibilities : Read The X11 manual to get more informations **/

struct alinfo { 
  char *name;
  char id;
  char *info;} AluStrucPos[] =
{ 
  {"GXclear" ,GXclear," 0 "},
  {"GXand" ,GXand," src AND dst "},
  {"GXandReverse" ,GXandReverse," src AND NOT dst "},
  {"GXcopy" ,GXcopy," src "},
  {"GXandInverted" ,GXandInverted," NOT src AND dst "},
  {"GXnoop" ,GXnoop," dst "},
  {"GXxor" ,GXxor," src XOR dst "},
  {"GXor" ,GXor," src OR dst "},
  {"GXnor" ,GXnor," NOT src AND NOT dst "},
  {"GXequiv" ,GXequiv," NOT src XOR dst "},
  {"GXinvert" ,GXinvert," NOT dst "},
  {"GXorReverse" ,GXorReverse," src OR NOT dst "},
  {"GXcopyInverted" ,GXcopyInverted," NOT src "},
  {"GXorInverted" ,GXorInverted," NOT src OR dst "},
  {"GXnand" ,GXnand," NOT src OR NOT dst "},
  {"GXset" ,GXset," 1 "}
};

void C2F(idfromnamePos)(name1, num)
     char *name1;
     integer *num;
{integer i;
 *num = -1;
 for ( i =0 ; i < 16;i++)
   if (strcmp(AluStrucPos[i].name,name1)== 0) 
     *num=AluStrucPos[i].id;
 if (*num == -1 ) 
   {
     Scistring("\n Use the following keys :");
     for ( i=0 ; i < 16 ; i++)
       sciprint("\nkey %s -> %s\r\n",AluStrucPos[i].name,
	       AluStrucPos[i].info);
   }
}


void C2F(setalufunction1Pos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{     
  integer value;
  value=AluStrucPos[Min(15,Max(0,*num))].id;
  if ( value != -1)
    {
      ScilabGCPos.CurDrawFunction = value;
      /* to be done */
    }
}

/** To get the value of the alufunction **/

void C2F(getalufunctionPos)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{ 
  *narg =1 ;
  *value = ScilabGCPos.CurDrawFunction ;
   if (*verbose ==1 ) 
     { sciprint("\nThe Alufunction is %s -> <%s>\r\n",
	       AluStrucPos[*value].name,
	       AluStrucPos[*value].info);}
 }

/** to set the thickness of lines : 0 is a possible value **/
/** give the thinest line **/

#define Thick_prec 5

void C2F(setthicknessPos)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{ 
  ScilabGCPos.CurLineWidth =Max(0, *value);
  FPRINTF((file,"\n%d Thickness",(int)Max(0,*value*Thick_prec)));
}

/** to get the thicknes value **/

void C2F(getthicknessPos)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{
  *narg =1 ;
  *value = ScilabGCPos.CurLineWidth ;
  if (*verbose ==1 ) 
    sciprint("\nLine Width:%d\r\n",
	    ScilabGCPos.CurLineWidth ) ;
}
     

/*-------------------------------------------------
\encadre{To set grey level for filing areas.
  from black (*num =0 ) to white 
  you must use the get function to get the id of 
  the white pattern }
----------------------------------------------------*/

void C2F(setpatternPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
 integer i ; 
  if ( ScilabGCPos.CurColorStatus ==1) 
    {
      i= Max(0,Min(*num-1,ScilabGCPos.Numcolors+1));
      ScilabGCPos.CurColor = i ;
      C2F(set_cPos)(i);
    }
  else 
    {
      /* used when printing from color to b&white color after GREYNUMBER 
       are translated to black */
      if ( *num-1 > GREYNUMBER -1 ) 
	i=0;
      else 
	i= Max(0,Min(*num-1,GREYNUMBER-1));
      ScilabGCPos.CurPattern = i;
      if (i ==0)
	FPRINTF((file,"\nfillsolid"));
      else 
	FPRINTF((file,"\n%d Setgray",(int)i));
    }
}

/** To get the id of the current pattern  **/

void C2F(getpatternPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{ 

  *narg=1;
  if ( ScilabGCPos.CurColorStatus ==1) 
    {
      *num = ScilabGCPos.CurColor +1 ;
      if (*verbose == 1) 
	sciprint("\n Color : %d\r\n",*num);
    }
  else 
    {
      *num = ScilabGCPos.CurPattern +1 ;
      if (*verbose == 1) 
	sciprint("\n Pattern : %d\r\n",*num);
    }
}


/** To get the id of the last pattern **/

void C2F(getlastPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *num = ScilabGCPos.IDLastPattern +1 ;
  if (*verbose==1) 
    sciprint("\n Id of White Pattern %d\r\n",(int)*num);
  *narg=1;
}

/** To set dash-style : **/
/**  use a table of dashes and set default dashes to **/
/**  one of the possible value. value pointeger **/
/**  to a strictly positive integer **/
/**  the first entry in DashTabPos is not used since  **/
/**  0 => solid line **/

#define MAXDASH 6
static integer DashTabPos[MAXDASH][4] = {
  {2,5,2,5}, {5,2,5,2},  {5,3,2,3}, {8,3,2,3},
  {11,3,2,3}, {11,3,5,3}};

/* old version of setdashPos retained for compatibility */

void C2F(set_dash_or_color_Pos)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  static integer  l2=4,l3 ;

  if ( ScilabGCPos.CurColorStatus == 1) 
    {
      int i;
      i= Max(0,Min(*value-1,ScilabGCPos.Numcolors+1));
      ScilabGCPos.CurColor =i;
      C2F(set_cPos)(i);
    }
  else 
    {
      l3 = Max(0,Min(MAXDASH - 1,*value - 1));
      C2F(setdashstylePos)(&l3,DashTabPos[l3],&l2);
      ScilabGCPos.CurDashStyle = l3;
    }
}


void C2F(setdashPos)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  static integer l2=4,l3 ;

  l3 = Max(0,Min(MAXDASH - 1,*value - 1));
  C2F(setdashstylePos)(&l3,DashTabPos[l3],&l2);
  ScilabGCPos.CurDashStyle = l3;
}

void C2F(set_dash_and_color_Pos)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  C2F(setdashPos)(value, v2, v3, v4); 
  C2F(setpatternPos)(value+6, v2, v3, v4);
}

/* style arguments sets either dash style either color */

void C2F(set_line_style_Pos)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  integer j;
  if (ScilabGCPos.CurColorStatus == 0)
    C2F(setdashPos)(value,PI0,PI0,PI0);
  else {
    j= ScilabGCPos.CurDashStyle + 1;
    C2F(setdashPos)(&j,PI0,PI0,PI0);
    C2F(setpatternPos)(value,PI0,PI0,PI0);
  }
}

/** To change The Pos-default dash style **/
/** if *value == 0, use a solid line, if *value != 0 **/
/** the dash style is specified by the xx vector of n values **/
/** xx[3]={5,3,7} and *n == 3 means :  5white 3 void 7 white \ldots **/
  
void C2F(setdashstylePos)(value, xx, n)
     integer *value;
     integer *xx;
     integer *n;
{
  integer i ;
  if ( *value == 0) FPRINTF((file,"\n[] 0 setdash"));
  else 
    {
      FPRINTF((file,"\n["));
      for (i=0;i<*n;i++)
	FPRINTF((file,"%d ",(int) (xx[i]*prec_fact)));
      FPRINTF((file,"] 0 setdash"));
    }
}


/** to get the current dash-style **/

/* old version of getdashPos retained for compatibility */
void C2F(get_dash_or_color_Pos)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{integer i ;
 *narg =1 ;
 if ( ScilabGCPos.CurColorStatus ==1) 
   {
     *value= ScilabGCPos.CurColor + 1;
     if (*verbose == 1) sciprint("Color %d",(int)*value);
     return;
   }
 *value=ScilabGCPos.CurDashStyle+1;
 if ( *value == 1) 
   { if (*verbose == 1) Scistring("\nLine style = Line Solid");}
 else 
   {
     value[1]=4;
     *narg = value[1]+2;
     for ( i =0 ; i < value[1]; i++) value[i+2]=DashTabPos[*value-1][i];
     if (*verbose ==1 ) 
       {
	 sciprint("\nDash Style %d:<",(int)*value);
	 for ( i =0 ; i < value[1]; i++)
	   sciprint("%d ",(int)value[i+2]);
	 Scistring(">\n");
       }
   }
}

void C2F(getdashPos)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{integer i ;
 *narg =1 ;
 *value=ScilabGCPos.CurDashStyle+1;
 if ( *value == 1) 
   { if (*verbose == 1) Scistring("\nLine style = Line Solid");}
 else 
   {
     value[1]=4;
     *narg = value[1]+2;
     for ( i =0 ; i < value[1]; i++) value[i+2]=DashTabPos[*value-1][i];
     if (*verbose ==1 ) 
       {
	 sciprint("\nDash Style %d:<",(int)*value);
	 for ( i =0 ; i < value[1]; i++)
	   sciprint("%d ",(int)value[i+2]);
	 Scistring(">\n");
       }
   }
}

void C2F(get_dash_and_color_Pos)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{
  /*may be improved replacing 6 by narg */
  C2F(getdashPos)(verbose, value, narg,dummy);
  C2F(getpatternPos)(verbose, value+6, narg,dummy);
  *narg = 6;
}

void C2F(usecolorPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  integer i;
  i =  Min(Max(*num,0),1);
  FPRINTF((file,"\n%%--use color %d ",i));
  if ( ScilabGCPos.CurColorStatus != (int)i)
    {
      if (ScilabGCPos.CurColorStatus == 1) 
	{
	  /* je passe de Couleur a n&b */
	  /* remise des couleurs a vide */
	  ScilabGCPos.CurColorStatus = 1;
	  C2F(setpatternPos)((i=1,&i),PI0,PI0,PI0);
	  /* passage en n&b */
	  ScilabGCPos.CurColorStatus = 0;
	  i= ScilabGCPos.CurPattern+1;
	  C2F(setpatternPos)(&i,PI0,PI0,PI0);
	  i= ScilabGCPos.CurDashStyle+1;
	  C2F(setdashPos)(&i,PI0,PI0,PI0);
	  ScilabGCPos.IDLastPattern = GREYNUMBER - 1;
	  FPRINTF((file,"\n/WhiteLev %d def",ScilabGCPos.IDLastPattern));
	  FPRINTF((file,"\n/Setgray { WhiteLev div setgray } def "));
	  FPRINTF((file,"\n/Setcolor { WhiteLev div setgray } def "));
	  FPRINTF((file,"\n/usecolor 0  def "));
	}
      else 
	{
	  /* je passe en couleur */
	  /* remise a zero des patterns et dash */
	  /* remise des couleurs a vide */
	  ScilabGCPos.CurColorStatus = 0;
	  C2F(setpatternPos)((i=1,&i),PI0,PI0,PI0);
	  C2F(setdashPos)((i=1,&i),PI0,PI0,PI0);
	  /* passage en couleur  */
	  ScilabGCPos.CurColorStatus = 1;
	  i= ScilabGCPos.CurColor+1;
	  C2F(setpatternPos)(&i,PI0,PI0,PI0);
	  ScilabGCPos.IDLastPattern = ScilabGCPos.Numcolors - 1;
	  FPRINTF((file,"\n/WhiteLev %d def",ScilabGCPos.IDLastPattern));
	  FPRINTF((file,"\n/Setgray {/i exch def ColorR i get ColorG i get ColorB i get setrgbcolor } def "));
	  FPRINTF((file,"\n/Setcolor {/i exch def ColorR i get ColorG i get ColorB i get setrgbcolor } def "));
	  FPRINTF((file,"\n/usecolor 1 def "));
	}
    }
    FPRINTF((file,"\n%%--end use color %d ",ScilabGCPos.CurColorStatus));
}


void C2F(getusecolorPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *num = ScilabGCPos.CurColorStatus;
  if (*verbose == 1) 
    sciprint("\n Use color %d\r\n",(int)*num);
  *narg=1;
}


/*******************************************************
 * Setting the colormap 
 * WARNING 
 * -------
 *   This function is only used when the Postscript driver is on 
 *   and xset('colormap',..) is used 
 *   (i.e driver('Pos');xset('colormap',....) 
 *   In the usual case (i.e when someone exports a graphic 
 *   which is displayed in a window) only the graphics 
 *   recorded commands are replayed and xset('colormap') belongs 
 *   to the non-recorded Scilab graphic commands 
 *   
 *   Only the <<current colormap>> of the window is translated 
 *   to Postscript when the Postscript file is opened 
 *   ( see  if (  CheckColormap(&m) == 1) in FileInt) 
 ******************************************************/

void C2F(setcolormapPos)(v1,v2,v3,v4,v5,v6,a)
     integer *v1,*v2;
     integer *v3;
     integer *v4,*v5,*v6;
     double *a;
{
  int i,m;
  if (*v2 != 3 ||  *v1 < 0) {
    Scistring("Colormap must be a m x 3 array \n");
    return;
  }
  m = *v1;
  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      return;
    }
  }
  ScilabGCPos.Numcolors = m;
  ScilabGCPos.IDLastPattern = m - 1;
  ScilabGCPos.NumForeground = m;
  ScilabGCPos.NumBackground = m + 1;
  WriteColorRGB("R",a,0);
  WriteColorRGB("G",a,1);
  WriteColorRGB("B",a,2);
  C2F(usecolorPos)((i=1,&i) ,PI0,PI0,PI0);
  C2F(setalufunction1Pos)((i=3,&i),PI0,PI0,PI0);
  C2F(setpatternPos)((i=ScilabGCPos.NumForeground+1,&i),PI0,PI0,PI0);  
  C2F(setforegroundPos)((i=ScilabGCPos.NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundPos)((i=ScilabGCPos.NumForeground+2,&i),PI0,PI0,PI0);
}

static void WriteColorRGB(str,tab,ind) 
     char *str;
     double tab[];
     int ind;
{
  int i;
  FPRINTF((file,"\n/Color%s [",str));
  for ( i=0; i < ScilabGCPos.Numcolors; i++)
    {
      FPRINTF((file,"%f ",(float) tab[3*i+ind]));
      if ( (i % 10 ) == 0 ) FPRINTF((file,"\n"));
    }
  FPRINTF((file,"0.0 1.0] def"));
}

/** 
  Initial colormap : The arrays were filled with the numbers that we get with xget("colormap") 
**/

void ColorInit()
{
  int   m = DEFAULTNUMCOLORS;
  ScilabGCPos.Numcolors = m;
  WriteColorRGBDef("R",default_colors,0);
  WriteColorRGBDef("G",default_colors,1);
  WriteColorRGBDef("B",default_colors,2);
}

static void WriteColorRGBDef(str,tab,ind) 
     char *str;
     unsigned short tab[];
     int ind;
{
  int i;
  FPRINTF((file,"\n/Color%s [",str));
  for ( i=0; i < ScilabGCPos.Numcolors; i++)
    {
      FPRINTF((file,"%f ",(float) tab[3*i+ind]/255.0));
      if ( (i % 10 ) == 0 ) FPRINTF((file,"\n"));
    }
  FPRINTF((file,"0.0 1.0] def"));
}

void C2F(set_cPos)(i)
     integer i;
{
  integer j;
  j=Max(Min(i,ScilabGCPos.Numcolors+1),0);
  FPRINTF((file,"\n%d Setcolor",(int)j));
}


/** set and get the number of the background or foreground */

void C2F(setbackgroundPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{ 
  if (ScilabGCPos.CurColorStatus == 1) 
    {
      ScilabGCPos.NumBackground = Max(0,Min(*num - 1,ScilabGCPos.Numcolors + 1));
      FPRINTF((file,"\n/background %d def PaintBackground",ScilabGCPos.NumBackground));
    }
}

void C2F(getbackgroundPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{ 
  *narg=1;
  if ( ScilabGCPos.CurColorStatus == 1 ) 
    {
      *num = ScilabGCPos.NumBackground + 1;
    }
  else 
    {
      *num = 1;
    }
  if (*verbose == 1) 
    sciprint("\n Background : %d\r\n",*num);
}


/** set and get the number of the background or foreground */

void C2F(setforegroundPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{ 
  if (ScilabGCPos.CurColorStatus == 1) 
    {
      ScilabGCPos.NumForeground = Max(0,Min(*num - 1,ScilabGCPos.Numcolors + 1));
    }
}

void C2F(getforegroundPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{ 
  *narg=1;
  if ( ScilabGCPos.CurColorStatus == 1 ) 
    {
      *num = ScilabGCPos.NumForeground + 1;
    }
  else 
    {
      *num = 1; /** the foreground is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Foreground : %d\r\n",*num);
}



/** set and get the number of the hidden3d color */

void C2F(sethidden3dPos)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{ 
  if (ScilabGCPos.CurColorStatus == 1) 
    {
      /* es: Max(0,... -> Max(-1,... */
      ScilabGCPos.NumHidden3d = Max(-1,Min(*num - 1,ScilabGCPos.Numcolors + 1));
    }
}

void C2F(gethidden3dPos)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{ 
  *narg=1;
  if ( ScilabGCPos.CurColorStatus == 1 ) 
    {
      *num = ScilabGCPos.NumHidden3d + 1;
    }
  else 
    {
      *num = 1; /** the hidden3d is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Hidden3d : %d\r\n",*num);
}



/*--------------------------------------------------------
\encadre{general routines accessing the  set<> or get<>
 routines } 
-------------------------------------------------------*/

void C2F(semptyPos)(verbose, v2, v3, v4)
     integer *verbose;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}
void C2F(setwwhowPos)(verbose, v2, v3, v4)
     integer *verbose;
     integer *v2;
     integer *v3;
     integer *v4;
{
  FPRINTF((file,"\n%% SPLIT HERE"));

}

void C2F(gemptyPos)(verbose, v2, v3,dummy)
     integer *verbose;
     integer *v2;
     integer *v3;
     double *dummy;

{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}

#define NUMSETFONC 28

/** Table in lexicographic order **/

struct bgc { char *name ;
	     void  (*setfonc )() ;
	     void (*getfonc )() ;}
  ScilabGCTabPos[] = {
    {"alufunction",C2F(setalufunction1Pos),C2F(getalufunctionPos)},
    {"background",C2F(setbackgroundPos),C2F(getbackgroundPos)},
    {"clipoff",C2F(unsetclipPos),C2F(getclipPos)},
    {"clipping",C2F(setclipPos),C2F(getclipPos)},
    {"color",C2F(setpatternPos),C2F(getpatternPos)},
    {"colormap",C2F(setcolormapPos),C2F(gemptyPos)},
    {"dashes",C2F(set_dash_or_color_Pos),C2F(get_dash_or_color_Pos)},
    {"default",InitScilabGCPos, C2F(gemptyPos)},
    {"font",C2F(xsetfontPos),C2F(xgetfontPos)},
    {"foreground",C2F(setforegroundPos),C2F(getforegroundPos)},
    {"hidden3d",C2F(sethidden3dPos),C2F(gethidden3dPos)},
    {"lastpattern",C2F(semptyPos),C2F(getlastPos)},
    {"line mode",C2F(setabsourelPos),C2F(getabsourelPos)},
    {"line style",C2F(setdashPos),C2F(getdashPos)},
    {"mark",C2F(xsetmarkPos),C2F(xgetmarkPos)},
    {"pattern",C2F(setpatternPos),C2F(getpatternPos)},
    {"pixmap",C2F(semptyPos),C2F(gemptyPos)},
    {"thickness",C2F(setthicknessPos),C2F(getthicknessPos)},
    {"use color",C2F(usecolorPos),C2F(getusecolorPos)},
    {"viewport",C2F(semptyPos),C2F(gemptyPos)},
    {"wdim",C2F(setwindowdimPos),C2F(getwindowdimPos)},
    {"white",C2F(semptyPos),C2F(getlastPos)},
    {"window",C2F(setcurwinPos),C2F(getcurwinPos)},
    {"wpdim",C2F(semptyPos),C2F(gemptyPos)},
    {"wpos",C2F(setwindowposPos),C2F(getwindowposPos)},
    {"wresize",C2F(semptyPos),C2F(gemptyPos)},
    {"wshow",C2F(setwwhowPos),C2F(gemptyPos)},
    {"wwpc",C2F(semptyPos),C2F(gemptyPos)}
 };

#ifdef lint

/* pour forcer linteger a verifier ca */

static  test(str,flag,verbose,x1,x2,x3,x4,x5)
     char str[];
     integer flag ;
     integer  *verbose,*x1,*x2,*x3,*x4,*x5;
{ 
  double *dv;
  C2F(setalufunction1Pos)(x1,x2,x3,x4);C2F(getalufunctionPos)(verbose,x1,x2,dv);
  C2F(setclipPos)(x1,x2,x3,x4);C2F(getclipPos)(verbose,x1,x2,dv);
  C2F(setdashPos)(x1,x2,x3,x4);C2F(getdashPos)(verbose,x1,x2,dv);
  InitScilabGCPos(x1,x2,x3,x4); C2F(gemptyPos)(verbose,x1,x2,dv);
  C2F(xsetfontPos)(x1,x2,x3,x4);C2F(xgetfontPos)(verbose,x1,x2,dv);
  C2F(setabsourelPos)(x1,x2,x3,x4);C2F(getabsourelPos)(verbose,x1,x2,dv);
  C2F(xsetmarkPos)(x1,x2,x3,x4);C2F(xgetmarkPos)(verbose,x1,x2,dv);
  C2F(setpatternPos)(x1,x2,x3,x4);C2F(getpatternPos)(verbose,x1,x2,dv);
  C2F(setthicknessPos)(x1,x2,x3,x4);C2F(getthicknessPos)(verbose,x1,x2,dv);
  C2F(usecolorPos)(x1,x2,x3,x4);C2F(gemptyPos)(verbose,x1,x2,dv);
  C2F(setwindowdimPos)(x1,x2,x3,x4);C2F(getwindowdimPos)(verbose,x1,x2,dv);
  C2F(semptyPos)(x1,x2,x3,x4);C2F(getlastPos)(verbose,x1,x2,dv);
  C2F(setcurwinPos)(x1,x2,x3,x4);C2F(getcurwinPos)(verbose,x1,x2,dv);
  C2F(setwindowposPos)(x1,x2,x3,x4);getwindowposPos(verbose,x1,x2,dv);
}

#endif 


void C2F(scilabgcgetPos)(str, verbose, x1, x2, x3, x4, x5, dv1, dv2, dv3, dv4)
     char *str;
     integer *verbose;
     integer *x1;
     integer *x2;
     integer *x3;
     integer *x4;
     integer *x5;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  int x6=0;
  C2F(ScilabGCGetorSetPos)(str,(integer)1L,verbose,x1,x2,x3,x4,x5,&x6,dv1);
}

void C2F(scilabgcsetPos)(str, x1, x2, x3, x4, x5, x6, dv1, dv2, dv3, dv4)
     char *str;
     integer *x1;
     integer *x2;
     integer *x3;
     integer *x4;
     integer *x5;
     integer *x6;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
 integer verbose ;
 verbose = 0 ;
 C2F(ScilabGCGetorSetPos)(str,(integer)0L,&verbose,x1,x2,x3,x4,x5,x6,dv1);}

void C2F(ScilabGCGetorSetPos)(str, flag, verbose, x1, x2, x3, x4, x5,x6,dv1)
     char *str;
     integer flag;
     integer *verbose;
     integer *x1;
     integer *x2;
     integer *x3;
     integer *x4;
     integer *x5,*x6;
     double *dv1;
{ integer i ;
  for (i=0; i < NUMSETFONC ; i++)
     {
       integer j;
       j = strcmp(str,ScilabGCTabPos[i].name);
       if ( j == 0 ) 
	 { if (*verbose == 1)
	     sciprint("\nGettting Info on %s\r\n",str);
	   if (flag == 1)
	     (ScilabGCTabPos[i].getfonc)(verbose,x1,x2,dv1);
	   else 
	     (ScilabGCTabPos[i].setfonc)(x1,x2,x3,x4,x5,x6,dv1);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("\nUnknow Postscript operator <%s>\r\n",str);
	       return;
	     }
	 }
     }
  sciprint("\n Unknow Postscript operator <%s>\r\n",str);
}

/*-----------------------------------------------------------
\encadre{Functions for drawing}
-----------------------------------------------------------*/



/*----------------------------------------------------
\encadre{display of a string
 at (x,y) position whith angle (alpha). Angles in degree
 positive when clockwise. If *flag ==1 a framed  box is added 
 around the string.}
-----------------------------------------------------*/
void C2F(displaystringPos)(string, x, y, v1, flag, v6, v7, angle, dv2, dv3, dv4)
     char *string;
     integer *x;
     integer *y;
     integer *v1;
     integer *flag;
     integer *v6;
     integer *v7;
     double *angle;
     double *dv2;
     double *dv3;
     double *dv4;
{     
  integer i,rect[4] ;
  C2F(boundingboxPos)(string,x,y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  FPRINTF((file,"\n("));
  for ( i=0; i < (int)strlen(string);i++)
    {
      if (string[i]== '(' || string[i] == ')' )
	FPRINTF((file,"%c%c",'\\',string[i]));
      else 
	FPRINTF((file,"%c",string[i]));
    }
  FPRINTF((file,") %d %d %d %5.2f [%d %d %d %d] Show",
	  (int)*x,(int)*y,(int)*flag,*angle,(int)rect[0],
	  (int)rect[1],(int)rect[2],(int)rect[3]));
 }


double bsizePos[6][4]= {{ 0.0,-7.0,4.63,9.0  },
		{ 0.0,-9.0,5.74,12.0 },
		{ 0.0,-11.0,6.74,14.0},
		{ 0.0,-12.0,7.79,15.0},
		{0.0, -15.0,9.72,19.0 },
		{0.0,-20.0,13.41,26.0}};

/** To get the bounding rectangle of a string **/
/** we can't ask Postscript directly so we have an **/
/** approximative result in Postscript : use the X11 driver **/
/** with the same current font to have a good result **/

void C2F(boundingboxPos)(string, x, y, rect, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *string;
     integer *x;
     integer *y;
     integer *rect;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{integer verbose,nargs,font[2];
 verbose=0;
 C2F(xgetfontPos)(&verbose,font,&nargs,vdouble);
 rect[0]= (int)(*x+bsizePos[font[1]][0]*((double) prec_fact));
 rect[1]= (int)(*y+bsizePos[font[1]][1]*((double) prec_fact));
 rect[2]= (int)(bsizePos[font[1]][2]*((double)prec_fact)*(int)strlen(string));
 rect[3]= (int)(bsizePos[font[1]][3]*((double)prec_fact));
}

/** Draw a single line in current style **/

void C2F(drawlinePos)(xx1, yy1, x2, y2)
     integer *xx1;
     integer *yy1;
     integer *x2;
     integer *y2;
{
    FPRINTF((file,"\n %d %d %d %d L",(int)*xx1,(int)*yy1,(int)*x2,(int)*y2));
  }

/** Draw a set of segments **/
/** segments are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/

void C2F(drawsegmentsPos)(str, vx, vy, n, style, iflag, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vx;
     integer *vy;
     integer *n;
     integer *style;
     integer *iflag;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  int i;
  /* store the current values */
  C2F(getpatternPos)(&verbose,Dvalue,&Dnarg,vdouble);
  if ((int)  *iflag == 0 )
    {
      /** all segments have the same color or dash style */
      NDvalue= (*style < 1) ? Dvalue[0] : *style; 
      C2F(WriteGenericPos)("drawsegs",(integer)1L,(*n)*2,vx,vy,*n,(integer)1L,&NDvalue); 
    }
  else
    {
      for ( i=0 ; i < *n/2 ; i++) 
	{
	  integer NDvalue;
	  NDvalue = style[i];
	  C2F(WriteGenericPos)("drawsegs",(integer)1L,(integer)4L,&vx[2*i],&vy[2*i],(integer)2L,(integer)1L,&NDvalue);
	}
    }
  /*C2F(set_dash_and_color_Pos)( Dvalue,PI0,PI0,PI0);*/
}

/** Draw a set of arrows **/

void C2F(drawarrowsPos)(str, vx, vy, n, as, style, iflag, dv1, dv2, dv3, dv4)
     char *str;
     integer *vx;
     integer *vy;
     integer *n;
     integer *as;
     integer *style;
     integer *iflag;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  int i;
  C2F(get_dash_and_color_Pos)(&verbose,Dvalue,&Dnarg,vdouble);
  /* store the current values */
  if ((int)  *iflag == 0 )
    {
      /** all the arrows have the same color **/
      NDvalue=(*style < 1) ?  Dvalue[0] : *style;
      C2F(set_line_style_Pos)(&NDvalue,PI0,PI0,PI0);
      C2F(WriteGenericPos)("drawarrows",(integer)1L,(*n)*2,vx,vy,*n,(integer)1L,as); 
    }
  else
    {
      for ( i=0 ; i < *n/2 ; i++) 
	{
	  NDvalue = style[i];
	  C2F(set_line_style_Pos)(&NDvalue,PI0,PI0,PI0);
	  /*C2F(setpatternPos)(&NDvalue,PI0,PI0,PI0); commented out ss 13/09/00 */
	  C2F(WriteGenericPos)("drawarrows",(integer)1L,(integer)4L,&vx[2*i],&vy[2*i],(integer)2L,(integer)1L,as);
	}
    }
  C2F(set_dash_and_color_Pos)( Dvalue,PI0,PI0,PI0);
}

/** Draw one rectangle **/

/** Draw or fill a set of rectangle **/
/** rectangles are defined by (vect[i],vect[i+1],vect[i+2],vect[i+3]) **/
/** for i=0 step 4 **/
/** (*n) : number of rectangles **/
/** fillvect[*n] : specify the action (see periX11.c) **/

void C2F(drawrectanglesPos)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *fillvect;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer cpat,verb=0,num;
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericPos)("drawbox",*n,(integer)4L,vects,vects,4*(*n),(integer)0L,fillvect);
  C2F(setpatternPos)(&(cpat),PI0,PI0,PI0);
}

void C2F(drawrectanglePos)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i = 1;
  integer fvect[1] ;
  integer vects[4];
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;
  /** fvect set to tell that we only want to draw not to fill  */
  fvect[0] = 0;
  C2F(drawrectanglesPos)(str,vects,fvect,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/** Draw a filled rectangle **/

void C2F(fillrectanglePos)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i = 1;
  integer vects[4];
  integer cpat,verb=0,num;
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height ; 
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble);
  C2F(drawrectanglesPos)(str,vects,&cpat,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);    
}

/** Draw or fill a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** fillvect[*n] : specify the action <?> **/
/** caution angle=degreangle*64          **/

void C2F(fillarcsPos)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *fillvect;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer cpat,verb,num;
  verb=0;
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericPos)("fillarc",*n,(integer)6L,vects,vects,6*(*n),(integer)0L,fillvect);
  C2F(setpatternPos)(&(cpat),PI0,PI0,PI0);
}

/** Draw a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/

void C2F(drawarcsPos)(str, vects, style, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *style;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer verbose=0,Dnarg,Dvalue[10];
  int i;
  /* store the current values */
  C2F(get_dash_and_color_Pos)(&verbose,Dvalue,&Dnarg,vdouble);
  for ( i=0 ; i < *n ; i++) 
    {
      integer fvect,na=1;
      /** to fix the style */
      C2F(set_line_style_Pos)(&style[i],PI0,PI0,PI0);

      /** to say that we don't want to fill the arcs */
      fvect = ScilabGCPos.IDLastPattern  +2;
      C2F(fillarcsPos)(str,&vects[(6)*i],&fvect,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    }
  C2F(set_dash_and_color_Pos)( Dvalue,PI0,PI0,PI0);
}


/** Draw a single ellipsis or part of it **/
/** caution angle=degreAngle*64          **/

void C2F(drawarcPos)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *angle1;
     integer *angle2;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i =1;
  integer fvect[1] ;
  integer vects[6];
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;vects[4]= *angle1;vects[5]= *angle2;
  /** fvect set to tell that we only want to draw not to fill  */
  fvect[0] = ScilabGCPos.IDLastPattern + 2  ;
  C2F(fillarcsPos)(str,vects,fvect,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/** Fill a single elipsis or part of it **/
/** with current pattern **/

void C2F(fillarcPos)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *angle1;
     integer *angle2;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i =1,vects[6];
  integer cpat,verb=0,num;
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;vects[4]= *angle1;vects[5]= *angle2;
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble);
  C2F(fillarcsPos)(str,vects,&cpat,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
 }

/*--------------------------------------------------------------
\encadre{Filling or Drawing Polylines and Polygons}
---------------------------------------------------------------*/

/** Draw a set of *n polylines (each of which have (*p) points) **/
/** with lines or marks **/
/** drawvect[i] >= use a mark for polyline i **/
/** drawvect[i] < 0 use a line style for polyline i **/

void C2F(drawpolylinesPos)(str, vectsx, vectsy, drawvect, n, p, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vectsx;
     integer *vectsy;
     integer *drawvect;
     integer *n;
     integer *p;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ integer verbose ,symb[2],Mnarg,Dnarg,Dvalue[10],NDvalue,i,close;
  verbose =0 ;
  /* store the current values */
  C2F(xgetmarkPos)(&verbose,symb,&Mnarg,vdouble);
  C2F(get_dash_and_color_Pos)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (drawvect[i] <= 0)
	{ /** on utilise la marque de numero associ\'ee **/
	  NDvalue = - drawvect[i];
	  C2F(xsetmarkPos)(&NDvalue,symb+1,PI0,PI0);
	  C2F(drawpolymarkPos)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else
	{/** on utilise un style pointill\'e  **/

	  C2F(set_line_style_Pos)(drawvect+i,PI0,PI0,PI0);
	  close = 0;
	  C2F(drawpolylinePos)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,&close,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  /** back to default values **/
  C2F(set_dash_and_color_Pos)(Dvalue,PI0,PI0,PI0);
  C2F(xsetmarkPos)(symb,symb+1,PI0,PI0);
}

/**************************************************************
  fill a set of polygons each of which is defined by 
 (*p) points (*n) is the number of polygons 
 the polygon is closed by the routine 
 fillvect[*n] :         
 if fillvect[i] == 0 draw the boundaries with current color 
 if fillvect[i] > 0  draw the boundaries with current color 
                then fill with pattern fillvect[i]
 if fillvect[i] < 0  fill with pattern - fillvect[i]
 **************************************************************/

void C2F(fillpolylinesPos)(str, vectsx, vectsy, fillvect, n, p, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vectsx;
     integer *vectsy;
     integer *fillvect;
     integer *n;
     integer *p;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer cpat,verb=0,num;
  if ( ScilabGCPos.CurVectorStyle !=  CoordModeOrigin)
    FPRINTF((file,"\n/absolu false def"));
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericPos)("drawpoly",*n,(*p)*2,vectsx,vectsy,(*p)*(*n),(integer)1L,
			fillvect);
  C2F(setpatternPos)(&(cpat),PI0,PI0,PI0);
  FPRINTF((file,"\n/absolu true def"));
}

/** Only draw one polygon with current line style **/
/** according to *closeflag : it's a polyline or a polygon **/

void C2F(drawpolylinePos)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *closeflag;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ integer i =1;
  integer fvect[1] ;
  /** fvect set to tell that we only want to draw not to fill  */
  fvect[0] = 0;
  if (*closeflag == 1 )
    FPRINTF((file,"\n/closeflag true def"));
  else 
    FPRINTF((file,"\n/closeflag false def"));
  C2F(fillpolylinesPos)(str,vx,vy,fvect,&i,n,PI0,PD0,PD0,PD0,PD0);
}

/** Fill the polygon **/

void C2F(fillpolylinePos)(str, n, vx, vy, closeareaflag, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *closeareaflag;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer i =1;
  integer cpat,verb=0,num;
  C2F(getpatternPos)(&verb,&cpat,&num,vdouble); 
  /** just fill  ==> cpat < 0 **/
  cpat = -cpat;
  C2F(fillpolylinesPos)(str,vx,vy,&cpat,&i,n,PI0,PD0,PD0,PD0,PD0);
}

/** Draw a set of  current mark centred at points defined **/
/** by vx and vy (vx[i],vy[i]) **/

void C2F(drawpolymarkPos)(str, n, vx, vy, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer keepid,keepsize,i=1,sz=ScilabGCPos.CurHardSymbSize;
  keepid =  ScilabGCPos.FontId;
  keepsize= ScilabGCPos.FontSize;
  C2F(xsetfontPos)(&i,&sz,PI0,PI0);
  C2F(displaysymbolsPos)(str,n,vx,vy);
  C2F(xsetfontPos)(&keepid,&keepsize,PI0,PI0);
}

/*-----------------------------------------------------
\encadre{Routine for initialisation}
------------------------------------------------------*/

void C2F(initgraphicPos)(string, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *string;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  char string1[256];
  static integer EntryCounter = 0;
  integer fnum;

  if (EntryCounter >= 1) C2F(xendgraphicPos)();
  strncpy(string1,string,256);

  /* Not so useful   
     sprintf(string2,"%d",(int)EntryCounter);
     strcat(string1,string2); */
  file=fopen(string1,"w");
  if (file == 0) 
    {
      sciprint("Can't open file %s, I'll use stdout\r\n",string1);
      file =stdout;
    }
  if (EntryCounter == 0)
    { 
      fnum=0;      C2F(loadfamilyPos)("Courier",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=1;      C2F(loadfamilyPos)("Symbol",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=2;      C2F(loadfamilyPos)("Times-Roman",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      fnum=3;      C2F(loadfamilyPos)("Times-Italic",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=4;      C2F(loadfamilyPos)("Times-Bold",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      fnum=5;      C2F(loadfamilyPos)("Times-BoldItalic",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 

    }
  FileInit();
  ScilabGCPos.CurWindow =EntryCounter;
  EntryCounter =EntryCounter +1;
}

void FileInit()
{
  int m;
  /** Just send Postscript commands to define scales etc....**/
  integer x[2],verbose,narg;
  verbose = 0; 
  C2F(getwindowdimPos)(&verbose,x,&narg,vdouble);
  ColorInit();
  FPRINTF((file,"\n%% Dessin en bas a gauche de taille %d,%d",(int)x[0]/2,(int)x[1]/2));
  FPRINTF((file,"\n[0.5 %d div 0 0 0.5 %d div neg  0 %d %d div] concat",
	  (int)prec_fact, (int)prec_fact,(int)x[1]/2,(int) prec_fact ));
  FPRINTF((file,"\n%% Init driver "));
  FPRINTF((file,"\n/PaintBackground {WhiteLev 2 add background eq {}{ (drawbox) 4 [background 1 add] [0 0 %d %d] dogrey}ifelse } def", x[0],x[1]));

  InitScilabGCPos(PI0,PI0,PI0,PI0) ;
  FPRINTF((file,"\n%% End init driver "));
  FPRINTF((file,"\n/WhiteLev %d def",ScilabGCPos.IDLastPattern));
  /** If the X window exists we check its colormap **/
  if (  CheckColormap(&m) == 1) 
    { 
      int i;
      float r,g,b;
      ScilabGCPos.Numcolors = m;
      ScilabGCPos.NumForeground = m;
      ScilabGCPos.NumBackground = m + 1;
      if (ScilabGCPos.CurColorStatus == 1) 
	{
	  ScilabGCPos.IDLastPattern = ScilabGCPos.Numcolors - 1;
	  FPRINTF((file,"\n/WhiteLev %d def",ScilabGCPos.IDLastPattern));
	}
      FPRINTF((file,"\n/ColorR ["));
      for ( i=0; i < m ; i++)
	{
	  get_r(i,&r);
	  FPRINTF((file,"%f ",r));
	  if ( (i % 10 ) == 0 ) FPRINTF((file,"\n"));
	}
      FPRINTF((file,"0.0 1.0 ] def"));
      FPRINTF((file,"\n/ColorG ["));
      for ( i=0; i < m ; i++) 
	{
	  get_g(i,&g);
	  FPRINTF((file,"%f ",g));
	  if ( (i % 10 ) == 0 ) FPRINTF((file,"\n"));
	}
      FPRINTF((file,"0.0 1.0] def"));
      FPRINTF((file,"\n/ColorB ["));
      for ( i=0; i < m; i++)
	{
	  get_b(i,&b);
	  FPRINTF((file,"%f ",b));
	  if ( (i % 10 ) == 0 ) FPRINTF((file,"\n"));
	}
      FPRINTF((file,"0.0 1.0] def"));
    }
}

/*--------------------------------------------------------
\encadre{Initialisation of the graphic context. Used also 
to come back to the default graphic state}
---------------------------------------------------------*/


void InitScilabGCPos(v1, v2, v3, v4)
     integer *v1;
     integer *v2;
     integer *v3;
     integer *v4;
{ integer i,j,col;
  ScilabGCPos.IDLastPattern = GREYNUMBER-1;
  ScilabGCPos.CurLineWidth=0 ;
  i=1;
  C2F(setthicknessPos)(&i,PI0,PI0,PI0);
  C2F(setalufunctionPos)("GXcopy");
  /** retirer le clipping **/
  i=j= -1;
  C2F(unsetclipPos)(PI0,PI0,PI0,PI0);
  C2F(setdashPos)((i=0,&i),PI0,PI0,PI0);
  C2F(xsetfontPos)((i=2,&i),(j=1,&j),PI0,PI0);
  C2F(xsetmarkPos)((i=0,&i),(j=0,&j),PI0,PI0);
  /** trac\'e absolu **/
  ScilabGCPos.CurVectorStyle = CoordModeOrigin ;
  /* initialisation des pattern dash par defaut en n&b */
  ScilabGCPos.CurColorStatus =0;
  C2F(setpatternPos)((i=1,&i),PI0,PI0,PI0);
  C2F(setdashPos)((i=1,&i),PI0,PI0,PI0);
  C2F(sethidden3dPos)((i=1,&i),PI0,PI0,PI0);
  /* initialisation de la couleur par defaut */ 
  ScilabGCPos.Numcolors = DEFAULTNUMCOLORS;
  ScilabGCPos.NumForeground = DEFAULTNUMCOLORS;
  ScilabGCPos.CurColorStatus = 1 ;
  C2F(setpatternPos)((i=1,&i),PI0,PI0,PI0);
  C2F(setforegroundPos)((i=ScilabGCPos.NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundPos)((i=ScilabGCPos.NumForeground+2,&i),PI0,PI0,PI0);
  C2F(sethidden3dPos)((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic_ */
  getcolordef(&col);
  /** we force CurColorStatus to the opposite value of col 
    to force usecolorPos to perform initialisations 
    **/
  ScilabGCPos.CurColorStatus = (col == 1) ? 0: 1;
  C2F(usecolorPos)(&col,PI0,PI0,PI0);
  if (col == 1) ScilabGCPos.IDLastPattern = ScilabGCPos.Numcolors - 1;
  strcpy(ScilabGCPos.CurNumberDispFormat,"%-5.2g");
}


/*-----------------------------------------------------
\encadre{Draw an axis whith a slope of alpha degree (clockwise)
 . Along the axis marks are set in the direction ( alpha + pi/2), in the 
  following way :
\begin{itemize}
\item   $n=<n1,n2>$,
\begin{verbatim}
     |            |           |
     |----|---|---|---|---|---|
     <-----n1---->                 
     <-------------n2-------->
\end{verbatim}
$n1$and $n2$ are integer numbers for interval numbers.
\item $size=<dl,r,coeff>$. $dl$ distance in points between 
     two marks, $r$ size in points of small mark, $r*coeff$ 
     size in points of big marks. (they are doubleing points numbers)
\item $init$. Initial pointeger $<x,y>$. 
\end{itemize}
}

-------------------------------------------------------------*/
void C2F(drawaxisPos)(str, alpha, nsteps, v2, initpoint, v6, v7, size, dx2, dx3, dx4)
     char *str;
     integer *alpha;
     integer *nsteps;
     integer *v2;
     integer *initpoint;
     integer *v6;
     integer *v7;
     double *size;
     double *dx2;
     double *dx3;
     double *dx4;
{
  FPRINTF((file,"\n %d [%d %d] [%f %f %f] [%d %d] drawaxis",
	  (int)*alpha,(int)nsteps[0],(int)nsteps[1],size[0],size[1],size[2],
	  (int)initpoint[0],(int)initpoint[1]));
}


/*-----------------------------------------------------
\encadre{Display numbers z[i] at location (x[i],y[i])
  with a slope alpha[i] (see displaystring_), if flag==1
  add a box around the string.
-----------------------------------------------------*/
void C2F(displaynumbersPos)(str, x, y, v1, v2, n, flag, z, alpha, dx3, dx4)
     char *str;
     integer *x;
     integer *y;
     integer *v1;
     integer *v2;
     integer *n;
     integer *flag;
     double *z;
     double *alpha;
     double *dx3;
     double *dx4;
{ integer i ;
  char buf[20];
  for (i=0 ; i< *n ; i++)
    { 
      sprintf(buf,ScilabGCPos.CurNumberDispFormat,z[i]);
      C2F(displaystringPos)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0) ;
    }
}

/*-------------------------------------------------------
\encadre{General routine for generating Postscript Code 
to deal with Vectors. The difficulty is that the size 
of vectors is limited by Postscript, so the routine 
must check size and cut into pieces big objects}
\begin{verbatim}
  clear (string) sizeobj [fvect[0],...,fvect[nobj]]
  (si flag=1)  [ vx[0] vy[0] vx[1] vy[1] ...... vx[sizev] vy[sizev]]
  (si flag=0)  [ vx[0] vx[1] ..... vx[sizev] ] dogrey 
\end{verbatim}
----------------------------------------------------------*/

/** WARNING  MAXSIZE must be a multiple of  2 4 8 et 6  **/
/** 432=8*6*9 **/

#define MAXSIZE 432
#define PERLINE 20
#define FORMATNUM "%d "

void C2F(WriteGenericPos)(string, nobj, sizeobj, vx, vy, sizev, flag, fvect)
     char *string;
     integer nobj;
     integer sizeobj;
     integer *vx;
     integer *vy;
     integer sizev;
     integer flag;
     integer *fvect;
{   
  integer nobjpos,objbeg;
  objbeg= 0 ;
  /**-- si MAXSIZE/sizeobj vaut zero chaque objet est trop gros **/
  /** calcule le nombre d'object que l'on peut imprimer \`a la fois**/
  /** sans risquer un overflow dans un array postscript **/
  if (nobj ==0 || sizeobj ==0) return;
  nobjpos =Min( Max(1,MAXSIZE /sizeobj),nobj);
  while ( objbeg < nobj)
    {
      integer objres;
      objres= nobj-objbeg;
      C2F(WriteGeneric1Pos)(string,Min(nobjpos,objres),objbeg,sizeobj,vx,vy,flag,fvect);
      objbeg = objbeg+nobjpos;
   }
  
}

void C2F(WriteGeneric1Pos)(string, nobjpos, objbeg, sizeobj, vx, vy, flag, fvect)
     char *string;
     integer nobjpos;
     integer objbeg;
     integer sizeobj;
     integer *vx;
     integer *vy;
     integer flag;
     integer *fvect;
{
  integer from,n,i;
  if (flag == 1) 
    {  from= (objbeg*sizeobj)/2;
       n= (nobjpos*sizeobj)/2;}
  else 
    {  from= (objbeg*sizeobj);
       n= (nobjpos*sizeobj);
    }
  FPRINTF((file,"\n (%s) %d [",string,(int)Min(sizeobj,MAXSIZE)));
  /** exept for the drawarrows case fvect[i] is a pattern **/
  /** and we must change it **/
  if ( strcmp(string,"drawarrows")== 0 
       || strcmp(string,"drawbox")==0
       || strcmp(string,"drawpoly")==0)
    {
      for ( i =objbeg  ; i < (nobjpos+objbeg) ; i++)
	{
	  FPRINTF((file," %d",(int)fvect[i]));
	}
    }
  else
    {
      for ( i =objbeg  ; i < (nobjpos+objbeg) ; i++)
	{
	  int pat;
	  pat = Max(0,(int)fvect[i]-1);
	  FPRINTF((file," %d",pat));
	}
    }

  FPRINTF((file,"]\n"));
  /* Reste un probleme car si un unique objet doit etre dessine
     et qu'il est trop gros cet objet est decoupe en bout mais 
     il manque alors les raccords eventuels */
  C2F(Write2VectPos)(vx,vy,from,n,string,flag,fvect[objbeg]);
}

/*--------------------------------------------------
  [  perline*valeurs de vx et vy 
     ......
     .....
  ] string 
  [ 

  ] string 
  chaque zone entre [] ne doit pas contenir plus de 
  maxsize valeurs.
-------------------------------------------------------*/


void C2F(Write2VectPos)(vx, vy, from, n, string, flag, fv)
     integer *vx;
     integer *vy;
     integer from;
     integer n;
     char *string;
     integer flag;
     integer fv;
{ 
  integer i,j,k,co,nco;
  int fv1;
  if ( flag == 1) nco=2*n;else nco=n;
  co = 1;
  i =0;

  if ( strcmp(string,"drawarrows")== 0)
    {
      fv1= fv ;
    }
  else
    {
      fv1 = Max(0,fv-1);
    }

  while( i < n)
    {
      if ( i > 0) 
	FPRINTF((file,"\n (%s) %d [%d]\n",
		string,(int)Min(MAXSIZE,nco-(co-1)*MAXSIZE),fv1));
      co = co +1;
      j =0;
      FPRINTF((file,"["));
      while ( j < MAXSIZE && i <n )
	{
	  k=0;
	  while ( k < PERLINE && i < n && j < MAXSIZE)
	    {
	      FPRINTF((file,FORMATNUM,(int)vx[i+from]));
	      if (flag == 1) 
		{ FPRINTF((file,FORMATNUM,(int) vy[i+from]));
		  k=k+2;i=i+1;j=j+2;}
	      else 
		{k=k+1;i=i+1;j=j+1;}}
	  FPRINTF((file,"\n"));
	}
      FPRINTF((file,"] dogrey "));
    }
}

/** Global variables to deal with fonts **/

#define FONTNUMBER 7
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10
integer FontsListPos[FONTNUMBER][FONTMAXSIZE];
struct SciFontInfo { integer ok;
		  char fname[20];} FontInfoTabPos[FONTNUMBER];

static char *sizePos[] = { "08" ,"10","12","14","18","24"};
static int  isizePos[] = { 8 ,10,12,14,18,24};

/** To set the current font id of font and size **/

void C2F(xsetfontPos)(fontid, fontsize, v3, v4)
     integer *fontid;
     integer *fontsize;
     integer *v3;
     integer *v4;
{ integer i,fsiz;
  i = Min(FONTNUMBER-1,Max(*fontid,0));
  fsiz = Min(FONTMAXSIZE-1,Max(*fontsize,0));
  if ( FontInfoTabPos[i].ok !=1 )
    Scistring("\n Sorry This Font is Not available ");
  else 
   {
     ScilabGCPos.FontId = i;
     ScilabGCPos.FontSize = fsiz;
     FPRINTF((file,"\n/%s findfont %d scf mul scalefont setfont",
	     FontInfoTabPos[i].fname,
	     (int) (isizePos[fsiz]*prec_fact)));
   }
}

/** To get the values id and size of the current font **/

void C2F(xgetfontPos)(verbose, font, nargs,dummy)
     integer *verbose;
     integer *font;
     integer *nargs;
     double *dummy;
{
  *nargs=2;
  font[0]= ScilabGCPos.FontId ;
  font[1] =ScilabGCPos.FontSize ;
  if (*verbose == 1) 
    {
      sciprint("\nFontId : %d ",	      ScilabGCPos.FontId );
      sciprint("--> %s at size %s pts\r\n",
	     FontInfoTabPos[ScilabGCPos.FontId].fname,
	     sizePos[ScilabGCPos.FontSize]);
    }
}


/** To set the current mark : using the symbol font of adobe **/

void C2F(xsetmarkPos)(number, size, v3, v4)
     integer *number;
     integer *size;
     integer *v3;
     integer *v4;
{ 
  ScilabGCPos.CurHardSymb =
    Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabGCPos.CurHardSymbSize = 
    Max(Min(FONTMAXSIZE-1,*size),0);
;}

/** To get the current mark id **/

void C2F(xgetmarkPos)(verbose, symb, narg,dummy)
     integer *verbose;
     integer *symb;
     integer *narg;
     double *dummy;
{
  *narg =2 ;
  symb[0] = ScilabGCPos.CurHardSymb ;
  symb[1] = ScilabGCPos.CurHardSymbSize ;
  if (*verbose == 1) 
  sciprint("\nMark : %d at size %d pts\r\n",
	  ScilabGCPos.CurHardSymb,
	  isizePos[ScilabGCPos.CurHardSymbSize]);
}

char symb_listPos[] = {
  /*
     0x2e : . alors que 0xb7 est un o plein trop gros 
     ., +,X,*,diamond(filled),diamond,triangle up,triangle down,trefle,circle*/
  (char)0x2e,(char)0x2b,(char)0xb4,(char)0xc5,(char)0xa8,
  (char)0xe0,(char)0x44,(char)0xd1,(char)0xa7,(char)0x4f};

static void C2F(displaysymbolsPos)(str, n, vx, vy)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
{
  integer fvect[1];
  fvect[0] =  ( ScilabGCPos.CurColorStatus ==1) ? ScilabGCPos.CurColor : ScilabGCPos.CurPattern ;
  if ( ScilabGCPos.CurVectorStyle !=  CoordModeOrigin)
    FPRINTF((file,"\n/absolu false def"));
  FPRINTF((file,"\nHardMark 0 16#%x put",
      Char2Int( symb_listPos[ScilabGCPos.CurHardSymb])));
  C2F(WriteGenericPos)("drawpolymark",(integer)1L,(*n)*2,vx,vy,*n,(integer)1L,fvect);
  FPRINTF((file,"\n/absolu true def"));
}



/*-------------------------------------------------------
\encadre{Check if a specified family of font exist in 
Postscript }
-------------------------------------------------------*/

void C2F(loadfamilyPos)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *name;
     integer *j;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer i ;
  for ( i = 0; i < FONTMAXSIZE ; i++)
    {
      FontsListPos[*j][i] = C2F(PosQueryFont)(name);
    }
  if  (FontsListPos[*j][0] == 0 )
	  sciprint("\n unknown font family : %s \r\n",name);
  else 
    {FontInfoTabPos[*j].ok = 1;
     strcpy(FontInfoTabPos[*j].fname,name) ;}
}

/*--------------------------------------------
\encadre{always answer ok. Must be Finished}
---------------------------------------------*/

static int C2F(PosQueryFont)(name)
     char *name;
{ 
  return(1);
}

void C2F(queryfamilyPos)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *name;
     integer *j;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i ;
  name[0]='\0';
  for (i=0;i<FONTNUMBER;i++) {
    strcat(name,FontInfoTabPos[i].fname);
    v3[i]=strlen(FontInfoTabPos[i].fname);
  }
  *j=FONTNUMBER;
}

/*------------------------END--------------------*/
