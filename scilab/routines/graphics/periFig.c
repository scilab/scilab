/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

/*----------------------BEGIN----------------------
\def\encadre#1{\paragraph{}\fbox{\begin{minipage}[t]{15cm}#1 \end{minipage}}}
\section{A Fig Driver}
---------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#if defined(THINK_C)|| defined(WIN32)
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
#endif


#include "Math.h"
#include "periFig.h"
#include "color.h"
#include "bcg.h" /* NG */

#define WHITE 7
#define BLACK 0


void C2F(Write2VectXfig)(integer *vx, integer *vy, integer n, integer flag); 
void C2F(WriteGenericXfig)(char *string, integer nobj, integer sizeobj, integer *vx, integer *vy, integer sizev, integer flag, integer *fvect);
void C2F(InitScilabGCXfig)(integer *v1, integer *v2, integer *v3, integer *v4);
void C2F(setforegroundXfig)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(ScilabGCGetorSetXfig)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1);
void C2F(setbackgroundXfig)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(set_cXfig)(integer i);
void C2F(idfromnameXfig) (char *name1, integer *num);
void C2F(getdashXfig)(integer *verbose, integer *value, integer *narg, double *dummy);

/* NG beg */
void C2F(setscilabFigureXfig)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure);
void C2F(getscilabFigureXfig)(integer *verbose, integer *x,integer *narg, double *figure);
void C2F(setscilabVersionXfig)(integer *vers, integer *v2, integer *v3, integer *v4);
void C2F(getscilabVersionXfig)(integer *verbose, integer *vers, integer *narg, double *dummy);
void C2F(setscilabxgcXfig)(integer *v1, integer *v2, integer *v3, integer *v4); 
void C2F(getscilabxgcXfig)(integer *verbose, integer *x,integer *narg, double *dummy);
/* NG end */

static void 
C2F(analyze_pointsXfig) (integer n, integer *vx, integer *vy,
			 integer onemore);
static void C2F(displaysymbolsXfig) (char *str, integer *n, integer *vx, integer *vy);
static int C2F(FigQueryFont)(char *name);
static void set_dash_or_color (int dash,int *l_style,int *style_val,int *color);

#define Char2Int(x)   ( x & 0x000000ff )
static double *vdouble = 0; /* used when a double argument is needed */
/** Global variables to deal with fonts **/

#define FONTNUMBER 7
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10
integer FontsListXfig_[FONTNUMBER][FONTMAXSIZE];
struct SciFontInfo { integer ok;
		  char fname[20];} FontInfoTabXfig_[FONTNUMBER];
/** xfig code for our fonts **/
static integer  xfig_font[]= { 12,32,0,1,2,3,0};
static char *sizeXfig_[] = { "08" ,"10","12","14","18","24"};
static integer  isizeXfig_[] = { 8,10,12,14,18,24};

#if defined(__CYGWIN32__) || defined(__MINGW32__) || defined(__GNUC__) || defined(__MSC__)
static FILE *file= (FILE *) 0;
#define FPRINTF(x) ( file != (FILE*) 0) ?  fprintf x  : 0 
#else 
static FILE *file= stdout ;
#define FPRINTF(x) fprintf x  
#endif

static void C2F(FileInitXfig) __PARAMS((void));
static void set_dash  __PARAMS((int dash,int *l_style,int *_val));
static void set_color  __PARAMS((int c,int *color));
/** Structure to keep the graphic state  **/

struct BCG  ScilabGCXfig ;


/*-----------------------------------------------------
\encadre{General routines}
-----------------------------------------------------*/

/** To select the graphic Window  **/

void C2F(xselgraphicXfig)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{}

/** End of graphic (close the file)  **/

void C2F(xendgraphicXfig)(void)
{
  if (file != stdout && file != (FILE*) 0) {
    fclose(file);
    file=stdout;
  }
}

void C2F(xendXfig)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  C2F(xendgraphicXfig)();
}


/** Clear the current graphic window     **/
/** In Fig : nothing      **/

void C2F(clearwindowXfig)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
}

/** Flush out the X11-buffer  **/

void C2F(viderbuffXfig)(void)
{}

/** To get the window size **/
/** The default fig box    **/
/** for line thickness etc \ldots **/
static int prec_fact =16;

void C2F(getwindowdimXfig)(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  *narg = 2;
  x[0]= 600*prec_fact;
  x[1]= 424*prec_fact;
  if (*verbose == 1) 
    sciprint("\n CWindow dim :%d,%d\r\n",(int)x[0],(int)x[1]);
} 

/** To change the window dimensions : do Nothing in Postscript  **/

void C2F(setwindowdimXfig)(integer *x, integer *y, integer *v3, integer *v4)
{
}

/** to get the window upper-left pointeger coordinates return 0,0 **/

void C2F(getwindowposXfig)(integer *verbose, integer *x, integer *narg, double *dummy)
{
  *narg = 2;
  x[0]= x[1]=0;
  if (*verbose == 1) 
    sciprint("\n CWindow position :%d,%d\r\n",(int)x[0],(int)x[1]);
 }

/** to set the window upper-left pointeger position (Void) **/

void C2F(setwindowposXfig)(integer *x, integer *y, integer *v3, integer *v4)
{
}


/** To generate a pause : Empty here **/

void C2F(xpauseXfig)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{}

/** Wait for mouse click in graphic window : Empty here **/

void C2F(xclickXfig)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ }

/** Wait for mouse click in any graphic window : Empty here **/

void C2F(xclick_anyXfig)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ }

void C2F(xgetmouseXfig)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ }

/** Clear a rectangle **/

void C2F(clearareaXfig)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  FPRINTF((file,"# %d %d %d %d clearzone\n",(int)*x,(int)*y,(int)*w,(int)*h));
}

/*------------------------------------------------
\encadre{Functions to modify the graphic state}
-------------------------------------------------*/

/** Select a graphic Window : Empty for Postscript **/

void C2F(setcurwinXfig)(integer *intnum, integer *v2, integer *v3, integer *v4)
{
  ScilabGCXfig.CurWindow = *intnum;
}

/** Get the id number of the Current Graphic Window **/

void C2F(getcurwinXfig)(integer *verbose, integer *intnum, integer *narg, double *dummy)
{
  *narg =1 ;
  *intnum = ScilabGCXfig.CurWindow ;
  if (*verbose == 1) 
    Scistring("\nJust one graphic page at a time ");
}

/** Set a clip zone (rectangle ) **/

void C2F(setclipXfig)(integer *x, integer *y, integer *w, integer *h)
{
  ScilabGCXfig.ClipRegionSet = 1;
  ScilabGCXfig.CurClipRegion[0]= *x;
  ScilabGCXfig.CurClipRegion[1]= *y;
  ScilabGCXfig.CurClipRegion[2]= *w;
  ScilabGCXfig.CurClipRegion[3]= *h;
  FPRINTF((file,"# %d %d %d %d setclipzone\n",(int)*x,(int)*y,(int)*w,(int)*h));
}


/** unset clip zone **/

void C2F(unsetclipXfig)(integer *v1, integer *v2, integer *v3, integer *v4)
{
  ScilabGCXfig.ClipRegionSet = 0;
  ScilabGCXfig.CurClipRegion[0]= -1;
  ScilabGCXfig.CurClipRegion[1]= -1;
  ScilabGCXfig.CurClipRegion[2]= 200000;
  ScilabGCXfig.CurClipRegion[3]= 200000;
  FPRINTF((file,"# %d %d %d %d setclipzone\n",-1,-1,200000,200000));
}

/** Get the boundaries of the current clip zone **/

void C2F(getclipXfig)(integer *verbose, integer *x, integer *narg, double *dummy)
{
  x[0] = ScilabGCXfig.ClipRegionSet;
  if ( x[0] == 1)
    {
      *narg = 5;
      x[1] =ScilabGCXfig.CurClipRegion[0];
      x[2] =ScilabGCXfig.CurClipRegion[1];
      x[3] =ScilabGCXfig.CurClipRegion[2];
      x[4] =ScilabGCXfig.CurClipRegion[3];
    }
  else *narg = 1;
  if (*verbose == 1)
    {
      if (ScilabGCXfig.ClipRegionSet == 1)
	sciprint("\nThere's a Clip Region :x:%d,y:%d,w:%d,h:%d\r\n",
		 ScilabGCXfig.CurClipRegion[0],
		 ScilabGCXfig.CurClipRegion[1],
		 ScilabGCXfig.CurClipRegion[2],
		 ScilabGCXfig.CurClipRegion[3]);
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

void C2F(absourelXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{
  if (*num == 0 )
    ScilabGCXfig.CurVectorStyle =  CoordModeOrigin;
  else 
    ScilabGCXfig.CurVectorStyle =  CoordModePrevious ;
}

/** to get information on absolute or relative mode **/

void C2F(getabsourelXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *narg = 1;
  *num = ScilabGCXfig.CurVectorStyle  ;
  if (*verbose == 1) 
    {
      if (ScilabGCXfig.CurVectorStyle == CoordModeOrigin)
	Scistring("\nTrace Absolu");
      else 
	Scistring("\nTrace Relatif");
    }
}


/** The alu function for drawing : Works only with X11 **/
/** Not in Postscript **/

void C2F(setalufunctionXfig)(char *string)
{     
  integer value;
  
  C2F(idfromnameXfig)(string,&value);
  if ( value != -1)
    {ScilabGCXfig.CurDrawFunction = value;
     FPRINTF((file,"# %d setalufunction\n",(int)value));
      }
}

/** All the possibilities : Read The X11 manual to get more informations **/

struct alinfo { 
  char *name;
  char id;
  char *info;} AluStrucXfig_[] =
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

void C2F(idfromnameXfig)(char *name1, integer *num)
{integer i;
 *num = -1;
 for ( i =0 ; i < 16;i++)
   if (strcmp(AluStrucXfig_[i].name,name1)== 0) 
     *num=AluStrucXfig_[i].id;
 if (*num == -1 ) 
   {
     Scistring("\n Use the following keys :");
     for ( i=0 ; i < 16 ; i++)
       {
	 sciprint("\nkey %s ",AluStrucXfig_[i].name);
	 sciprint("-> %s\r\n",AluStrucXfig_[i].info);
       }
   }
}


void C2F(setalufunction1Xfig)(integer *num, integer *v2, integer *v3, integer *v4)
{     
  integer value;
  value=AluStrucXfig_[Min(16,Max(0,*num))].id;
  if ( value != -1)
    {
      ScilabGCXfig.CurDrawFunction = value;
      /* to be done */
    }
}

/** To get the value of the alufunction **/

void C2F(getalufunctionXfig)(integer *verbose, integer *value, integer *narg, double *dummy)
{ 
  *narg =1 ;
  *value = ScilabGCXfig.CurDrawFunction ;
   if (*verbose ==1 ) 
     { 
       sciprint("\nThe Alufunction is %s",AluStrucXfig_[*value].name);
       sciprint("-> <%s>\r\n", AluStrucXfig_[*value].info);
     }
}

/** to set the thickness of lines :min is 1 is a possible value **/
/** give the thinest line **/

void C2F(setthicknessXfig)(integer *value, integer *v2, integer *v3, integer *v4)
{ 
  ScilabGCXfig.CurLineWidth =Max(1, *value);
  FPRINTF((file,"# %d Thickness\n",(int)Max(1L,*value)));
}

/** to get the thicknes value **/

void C2F(getthicknessXfig)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *narg =1 ;
  *value = ScilabGCXfig.CurLineWidth ;
  if (*verbose ==1 ) 
    sciprint("\nLine Width:%d\r\n", ScilabGCXfig.CurLineWidth ) ;
}
     
/*-------------------------------------------------
\encadre{To set grey level for filing areas.
  from black (*num =0 ) to white 
  you must use the get function to get the id of 
  the white pattern }
----------------------------------------------------*/

void C2F(setpatternXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{ integer i ; 
  if (  ScilabGCXfig.CurColorStatus ==1) 
    {
      i= Max(0,Min(*num-1,ScilabGCXfig.Numcolors+1));
      ScilabGCXfig.CurColor = i ;
      C2F(set_cXfig)(i);
    }
  else 
    {
      i= Max(0,Min(*num-1,GREYNUMBER-1));
      ScilabGCXfig.CurPattern = i;
      if (i ==0)
	FPRINTF((file,"# fillsolid\n"));
      else 
	FPRINTF((file,"# %d Setgray\n",(int)i));
    }
}

/** To get the id of the current pattern  **/

void C2F(getpatternXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCXfig.CurColorStatus ==1) 
    {
      *num = ScilabGCXfig.CurColor+1 ;
      if (*verbose == 1) 
	sciprint("\n Color : %d\r\n",
		 ScilabGCXfig.CurPattern);
    }
  else 
    {
      *num = ScilabGCXfig.CurPattern +1;
      if (*verbose == 1) 
	sciprint("\n Pattern : %d\r\n",
		 ScilabGCXfig.CurPattern);
    }
}

/** To get the id of the last pattern **/

void C2F(getlastXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *num = ScilabGCXfig.IDLastPattern +1 ;
  if (*verbose==1)
    sciprint("\n Id of White Pattern %d\r\n",(int)*num);
  *narg=1;
}


/** To set dash-style : **/
/**  use a table of dashes and set default dashes to **/
/**  one of the possible value. value integer **/
/**  to a strictly positive integer **/

#define MAXDASH 6
/** dot specification for line style  **/
/* solid dash dotted dash-dot dash-dot-dot dash-dot-dot-dot   */
static integer DashTab[6] =      {0,1,1,2,2,2};
static integer DashTabStyle[6] = {0,2,4,2,4,8};


/* old version of setdashXfig retained for compatibility */

void C2F(set_dash_or_color_Xfig)(integer *value, integer *v2, integer *v3, integer *v4)
{
  static integer maxdash = 6,l3 ;
  if ( ScilabGCXfig.CurColorStatus ==1) 
    {
      int i;
      i= Max(0,Min(*value-1,ScilabGCXfig.Numcolors+1));
      ScilabGCXfig.CurColor =i;
      C2F(set_cXfig)(i);
    }
  else
    {
      l3 = Max(0,Min(maxdash-1,*value-1));
      ScilabGCXfig.CurDashStyle = l3;
    }
}

void C2F(setdashXfig)(integer *value, integer *v2, integer *v3, integer *v4)
{
  static integer maxdash = 6,l3 ;

  l3 = Max(0,Min(maxdash-1,*value-1));
  ScilabGCXfig.CurDashStyle = l3;
}

void C2F(set_dash_and_color_Xfig)(integer *value, integer *v2, integer *v3, integer *v4)
{
  C2F(setdashXfig)(value, v2, v3, v4); 
  C2F(setpatternXfig)(value+6, v2, v3, v4);
}

/* style arguments sets either dash style either color */
void C2F(set_line_style_Xfig)(integer *value, integer *v2, integer *v3, integer *v4)
{
  integer j;
  if (ScilabGCXfig.CurColorStatus == 0) {
    C2F(setdashXfig)(value,PI0,PI0,PI0);
    C2F(setpatternXfig)((j=1,&j),PI0,PI0,PI0);
  }
  else {
    j= ScilabGCXfig.CurDashStyle + 1;
    C2F(setdashXfig)(&j,PI0,PI0,PI0);
    C2F(setpatternXfig)(value,PI0,PI0,PI0);
  }
}

/** to get the current dash-style **/

/* old version of getdashXfig retained for compatibility */

void C2F(get_dash_or_color_Xfig)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *narg =1 ;
  if ( ScilabGCXfig.CurColorStatus ==1) 
    {
      *value=ScilabGCXfig.CurColor + 1;
      if (*verbose == 1) sciprint("Color %d",(int)*value);
      return;
    }

  C2F(getdashXfig)(verbose, value+1, narg,dummy);
  *narg=*narg+1;
    
}

void C2F(getdashXfig)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  integer i;

  i=ScilabGCXfig.CurDashStyle;

  *narg = 3;
  value[0]=i+1;
  value[1]=DashTab[i];
  value[2]=DashTabStyle[i];
  if (*verbose ==1 ) 
    {
      sciprint("\nDash Style %d.",(int) *value);
      sciprint("%d %d ",(int)value[1],(int)value[2]);
      Scistring(">\n");
    }
 
}
void C2F(get_dash_and_color_Xfig)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  /*may be improved replacing 6 by narg */
  C2F(getdashXfig)(verbose, value, narg,dummy);
  C2F(getpatternXfig)(verbose, value+6, narg,dummy);
  *narg = 6;
}


void C2F(usecolorXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{
  integer i;
  i =  Min(Max(*num,0),1);
  if (  ScilabGCXfig.CurColorStatus != (int) i) 
    {
      if (ScilabGCXfig.CurColorStatus == 1) 
	{
	  /* je passe de Couleur a n&b */
	  /* remise des couleurs a vide */
	  ScilabGCXfig.CurColorStatus = 1;
	  C2F(setpatternXfig)((i=1,&i),PI0,PI0,PI0);
	  /* passage en n&b */
	  ScilabGCXfig.CurColorStatus = 0;
	  i= ScilabGCXfig.CurPattern+1;
	  C2F(setpatternXfig)(&i,PI0,PI0,PI0);
	  i= ScilabGCXfig.CurDashStyle+1;
	  C2F(setdashXfig)(&i,PI0,PI0,PI0);
          ScilabGCXfig.IDLastPattern = GREYNUMBER - 1;
	}
      else 
	{
	  /* je passe en couleur */
	  /* remise a zero des patterns et dash */
	  /* remise des couleurs a vide */
	  ScilabGCXfig.CurColorStatus = 0;
	  C2F(setpatternXfig)((i=1,&i),PI0,PI0,PI0);
	  C2F(setdashXfig)((i=1,&i),PI0,PI0,PI0);
	  /* passage en couleur  */
	  ScilabGCXfig.CurColorStatus = 1;
	  i= ScilabGCXfig.CurColor+1;
	  C2F(setpatternXfig)(&i,PI0,PI0,PI0);
	  ScilabGCXfig.IDLastPattern = ScilabGCXfig.Numcolors -1;
	}
    }
}


void C2F(getusecolorXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *num = ScilabGCXfig.CurColorStatus;
  if (*verbose == 1) 
    sciprint("\n Use color %d\r\n",(int)*num);
  *narg=1;
}


/*******************************************************
 * Setting the colormap
 * Attention :
 *   cette fonction n'est utilis'ee que si l''on est sous
 *   le peripherique Xfig et que l'on appelle
 *   explicitement xset("colormap",....)
 * ds le cas usuel comme cette fonction n'est pas
 * enregistree ds Rec.c elle ne doit pas etre appellee
 ******************************************************/
void setcolormapgXfig(struct  BCG *Xgc,integer *v1,integer *v2, double *a);/* NG */

void C2F(setgccolormapXfig)(struct BCG *Xgc,integer m, double *a)
{
  int i;
  Scistring("Warning : you will have to move the colors definition\n");
  Scistring(" at the top of the xfig file \n");

  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
        a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      return;
    }
  }
  Xgc->Numcolors = m;
  Xgc->IDLastPattern = m - 1;
  Xgc->NumForeground = m;
  Xgc->NumBackground = m + 1;
  for ( i=0; i < m ; i++)
    {
      unsigned short ur,ug,ub;
      ur = (unsigned short) (255.0*a[i]);
      ug = (unsigned short) (255.0*a[i+m]);
      ub = (unsigned short) (255.0*a[i+2*m]);
      FPRINTF((file,"0 %d #%02x%02x%02x \n",32+i,ur,ug,ub));
    }
  FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m,0,0,0));
  FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m+1,255,255,255));
  C2F(usecolorXfig)((i=1,&i) ,PI0,PI0,PI0);
  C2F(setalufunction1Xfig)((i=3,&i),PI0,PI0,PI0);
  C2F(setpatternXfig)((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);  
  C2F(setforegroundXfig)((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundXfig)((i=Xgc->NumForeground+2,&i),PI0,PI0,PI0);
}

void C2F(setcolormapXfig)(integer *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, double *a)
{
 
  int m;
  Scistring("Warning : you will have to move the colors definition\n");
  Scistring(" at the top of the xfig file \n");
  if (*v2 != 3 ||  *v1 < 0) {
    Scistring("Colormap must be a m x 3 array \n");
    return;
  }
  m = *v1;
  C2F(setgccolormapXfig)(&ScilabGCXfig, m, a);
}

void setcolormapgXfig(struct  BCG *Xgc,integer *m,integer *v2, double *a) /* NG */
{

  C2F(setgccolormapXfig)(Xgc,*m, a);
}

void C2F(set_cXfig)(integer i)
{
  integer j;
  j=Max(Min(i,ScilabGCXfig.Numcolors+1),0);
  ScilabGCXfig.CurColor=j;
  FPRINTF((file,"\n# %d Setcolor\n",(int)i));
}

/** set and get the number of the background or foreground */

void C2F(setbackgroundXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabGCXfig.CurColorStatus == 1) 
    {
      ScilabGCXfig.NumBackground = Max(0,Min(*num - 1,ScilabGCXfig.Numcolors+1));
    }
}

void C2F(getbackgroundXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCXfig.CurColorStatus == 1 ) 
    {
      *num = ScilabGCXfig.NumBackground + 1;
    }
  else 
    {
      *num = 1;
    }
  if (*verbose == 1) 
    sciprint("\n Background : %d\r\n",*num);
}


/** set and get the number of the background or foreground */

void C2F(setforegroundXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabGCXfig.CurColorStatus == 1) 
    {
      ScilabGCXfig.NumForeground = Max(0,Min(*num - 1,ScilabGCXfig.Numcolors+1));
    }
}

void C2F(getforegroundXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCXfig.CurColorStatus == 1 ) 
    {
      *num = ScilabGCXfig.NumForeground + 1;
    }
  else 
    {
      *num = ScilabGCXfig.IDLastPattern + 1;
    }
  if (*verbose == 1) 
    sciprint("\n Foreground : %d\r\n",*num);
}

/** set and get the number of the hidden3d color */

void C2F(sethidden3dXfig)(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabGCXfig.CurColorStatus == 1) 
    {
      /* es: Max(0,... -> Max(-1,... */
      ScilabGCXfig.NumHidden3d = Max(-1,Min(*num - 1,ScilabGCXfig.Numcolors + 1));
    }
}

void C2F(gethidden3dXfig)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCXfig.CurColorStatus == 1 ) 
    {
      *num = ScilabGCXfig.NumHidden3d + 1;
    }
  else 
    {
      *num = 1; /** the hidden3d is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Hidden3d : %d\r\n",*num);
}



/** To set the current font id of font and size **/

void C2F(xsetfontXfig)(integer *fontid, integer *fontsize, integer *v3, integer *v4)
{ integer i,fsiz;
  i = Min(FONTNUMBER-1,Max(*fontid,0));
  fsiz = Min(FONTMAXSIZE-1,Max(*fontsize,0));
  if ( FontInfoTabXfig_[i].ok !=1 )
    Scistring("\n Sorry This Font is Not available\n");
  else 
   {
     ScilabGCXfig.FontId = i;
     ScilabGCXfig.FontSize = fsiz;
     FPRINTF((file,"#/%s findfont %d scalefont setfont\n",
     	     FontInfoTabXfig_[i].fname,
	     (int)isizeXfig_[fsiz]*prec_fact));
   }
}

/** To get the values id and size of the current font **/

void C2F(xgetfontXfig)(integer *verbose, integer *font, integer *nargs, double *dummy)
{
  *nargs=2;
  font[0]= ScilabGCXfig.FontId ;
  font[1] =ScilabGCXfig.FontSize ;
  if (*verbose == 1) 
    {
      sciprint("\nFontId : %d ",ScilabGCXfig.FontId );
      sciprint("--> %s at size",
	     FontInfoTabXfig_[ScilabGCXfig.FontId].fname);
      sciprint("%s pts\r\n",sizeXfig_[ScilabGCXfig.FontSize]);
    }
}

/** To set the current mark : using the symbol font of adobe **/

void C2F(setcursymbolXfig)(integer *number, integer *size, integer *v3, integer *v4)
{ 
  ScilabGCXfig.CurHardSymb =
    Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabGCXfig.CurHardSymbSize = 
    Max(Min(FONTMAXSIZE-1,*size),0);
;}

/** To get the current mark id **/

void C2F(getcursymbolXfig)(integer *verbose, integer *symb, integer *narg, double *dummy)
{
  *narg =2 ;
  symb[0] = ScilabGCXfig.CurHardSymb ;
  symb[1] = ScilabGCXfig.CurHardSymbSize ;
  if (*verbose == 1) 
  {
    sciprint("\nMark : %d",ScilabGCXfig.CurHardSymb);
    sciprint("at size %s pts\r\n",
	  sizeXfig_[ScilabGCXfig.CurHardSymbSize]);
  }
}


/*--------------------------------------------------------
\encadre{general routines accessing the  set<> or get<>
 routines } 
-------------------------------------------------------*/

void C2F(semptyXfig)(integer *verbose, integer *v2, integer *v3, integer *v4)
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}

void C2F(gemptyXfig)(integer *verbose, integer *v2, integer *v3, double *dummy)
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}



#define NUMSETFONC 32 /* NG */

struct bgc { char *name ;
	     void  (*setfonc )() ;
	     void  (*getfonc )() ;}
  ScilabGCTabXfig_[] = {
   {"alufunction",C2F(setalufunction1Xfig),C2F(getalufunctionXfig)},
   {"background",C2F(setbackgroundXfig),C2F(getbackgroundXfig)},
   {"clipoff",C2F(unsetclipXfig),C2F(getclipXfig)},
   {"clipping",C2F(setclipXfig),C2F(getclipXfig)},
   {"color",C2F(setpatternXfig),C2F(getpatternXfig)},
   {"colormap",C2F(setcolormapXfig),C2F(gemptyXfig)},
   {"dashes",C2F(setdashXfig),C2F(getdashXfig)},
   {"default",C2F(InitScilabGCXfig), C2F(gemptyXfig)},
   {"figure",C2F(setscilabFigureXfig),C2F(getscilabFigureXfig)},/* NG */
   {"font",C2F(xsetfontXfig),C2F(xgetfontXfig)},
   {"foreground",C2F(setforegroundXfig),C2F(getforegroundXfig)},
   {"gc",C2F(semptyXfig),C2F(getscilabxgcXfig)},/* NG */
   {"gccolormap",C2F(setgccolormapXfig),C2F(gemptyXfig)},/* NG */
   {"hidden3d",C2F(sethidden3dXfig),C2F(gethidden3dXfig)},
   {"lastpattern",C2F(semptyXfig),C2F(getlastXfig)},
   {"line mode",C2F(absourelXfig),C2F(getabsourelXfig)},
   {"line style",C2F(setdashXfig),C2F(getdashXfig)},
   {"mark",C2F(setcursymbolXfig),C2F(getcursymbolXfig)},
   {"pattern",C2F(setpatternXfig),C2F(getpatternXfig)},
   {"pixmap",C2F(semptyXfig),C2F(gemptyXfig)},
   {"thickness",C2F(setthicknessXfig),C2F(getthicknessXfig)},
   {"use color",C2F(usecolorXfig),C2F(getusecolorXfig)},
   {"version",C2F(setscilabVersionXfig),C2F(getscilabVersionXfig)},/* NG */
   {"viewport",C2F(semptyXfig),C2F(gemptyXfig)},
   {"wdim",C2F(setwindowdimXfig),C2F(getwindowdimXfig)},
   {"white",C2F(semptyXfig),C2F(getlastXfig)},
   {"window",C2F(setcurwinXfig),C2F(getcurwinXfig)},
   {"wpdim",C2F(semptyXfig),C2F(gemptyXfig)},
   {"wpos",C2F(setwindowposXfig),C2F(getwindowposXfig)},
   {"wresize",C2F(semptyXfig),C2F(gemptyXfig)},
   {"wshow",C2F(semptyXfig),C2F(gemptyXfig)},
   {"wwpc",C2F(semptyXfig),C2F(gemptyXfig)},
 };



#ifdef lint

/* pour forcer lint a verifier ca */

static
test(str,flag,verbose,x1,x2,x3,x4,x5)
     char str[];
     integer flag ;
     integer  *verbose,*x1,*x2,*x3,*x4,*x5;
{ 
  double *dv;
  C2F(setalufunction1Xfig)(x1,x2,x3,x4);C2F(getalufunctionXfig)(verbose,x1,x2,dv);
  C2F(setclipXfig)(x1,x2,x3,x4);C2F(getclipXfig)(verbose,x1,x2,dv);
  C2F(setdashXfig)(x1,x2,x3,x4);C2F(getdashXfig)(verbose,x1,x2,dv);
  C2F(InitScilabGCXfig)(x1,x2,x3,x4); C2F(gemptyXfig)(verbose,x1,x2,dv);
  C2F(xsetfontXfig)(x1,x2,x3,x4);C2F(xgetfontXfig)(verbose,x1,x2,dv);
  C2F(absourelXfig)(x1,x2,x3,x4);C2F(getabsourelXfig)(verbose,x1,x2,dv);
  C2F(setcursymbolXfig)(x1,x2,x3,x4);C2F(getcursymbolXfig)(verbose,x1,x2,dv);
  C2F(setpatternXfig)(x1,x2,x3,x4);C2F(getpatternXfig)(verbose,x1,x2,dv);
  C2F(setthicknessXfig)(x1,x2,x3,x4);C2F(getthicknessXfig)(verbose,x1,x2,dv);
  C2F(usecolorXfig)(x1,x2,x3,x4);C2F(gemptyXfig)(verbose,x1,x2,dv);
  C2F(setwindowdimXfig)(x1,x2,x3,x4);C2F(getwindowdimXfig)(verbose,x1,x2,dv);
  C2F(semptyXfig)(x1,x2,x3,x4);C2F(getlastXfig)(verbose,x1,x2,dv);
  C2F(setcurwinXfig)(x1,x2,x3,x4);C2F(getcurwinXfig)(verbose,x1,x2,dv);
  C2F(setwindowposXfig)(x1,x2,x3,x4);getwindowposXfig(verbose,x1,x2,dv);
  
}

#endif 


void C2F(scilabgcgetXfig)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dx1, double *dx2, double *dx3, double *dx4)
{
  int x6=0;
  C2F(ScilabGCGetorSetXfig)(str,(integer)1L,verbose,x1,x2,x3,x4,x5,&x6,dx1);
}

void C2F(scilabgcsetXfig)(char *str, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, integer *x7, double *dx1, double *dx2, double *dx3, double *dx4)
{
 integer verbose ;
 verbose = 0 ;
 C2F(ScilabGCGetorSetXfig)(str,(integer)0L,&verbose,x2,x3,x4,x5,x6,x7,dx1);
}

void C2F(ScilabGCGetorSetXfig)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1)
{ integer i ;
  for (i=0; i < NUMSETFONC ; i++)
     {
       integer j;
       j = strcmp(str,ScilabGCTabXfig_[i].name);
       if ( j == 0 ) 
	 { if (*verbose == 1)
	     sciprint("\nGettting Info on %s\r\n",str);
	   if (flag == 1)
	     (ScilabGCTabXfig_[i].getfonc)(verbose,x1,x2,dx1);
	   else 
	     (ScilabGCTabXfig_[i].setfonc)(x1,x2,x3,x4,x5,x6,dx1);
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
void C2F(displaystringXfig)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4)
{    
  integer rect[4], font=-1,font_flag=2;
  integer verbose=0,Dnarg,Dvalue1[10];
  int pen_color;
  C2F(boundingboxXfig)(string,x,y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if (string[0]== '$') 
    {
      font=-1;
      font_flag=2;
    }
  else 
    {
      font =  xfig_font[ScilabGCXfig.FontId];
      font_flag= 4; 
    };
  C2F(getpatternXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
  

  C2F(getpatternXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
  set_color(Dvalue1[0],&pen_color);
  FPRINTF((file,"4 0 %d 0 0 %d %d %5.2f %d %5.2f %5.2f %d %d %s\\001\n",
	  pen_color,
	  (int)font,
	  (int)isizeXfig_[ScilabGCXfig.FontSize],/**prec_fact,*/
	  -(M_PI/180.0)*(*angle),
	  (int)font_flag,
	  (double) rect[3],
	  (double) rect[2],
	  (int)*x,
	  (int)*y,
	  string));
  if ( *flag == 1) 
    {
      rect[0]=rect[0]-4;rect[2]=rect[2]+6;
      C2F(drawrectangleXfig)(string,rect,rect+1,rect+2,rect+3,PI0,PI0,PD0,PD0,PD0,PD0);
    }
}

integer bsizeXfig_[6][4]= {{ 0,-7,463,9  },
		{ 0,-9,574,12 },
		{ 0,-11,674,14},
		{ 0,-12,779,15},
		{0, -15,972,19 },
		{0,-20,1341,26}};

/** To get the bounding rectangle of a string **/

void C2F(boundingboxXfig)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{integer verbose,nargs,font[2];
 verbose=0;
 C2F(xgetfontXfig)(&verbose,font,&nargs,vdouble);
 rect[0]= (int)(*x+bsizeXfig_[font[1]][0]*((double) prec_fact));
 rect[1]= (int)(*y+bsizeXfig_[font[1]][1]*((double) prec_fact));
 rect[2]= (int)(((double)prec_fact)*(bsizeXfig_[font[1]][2]/100.0)*((double)strlen(string)));
 rect[3]= (int)(bsizeXfig_[font[1]][3]*((double) prec_fact));
}

/** 
  Bounding box for marks : we have used xfig to get the  bounding boxes
  ie the correct w and h ( rect[2],rect[3]) 
**/


int symb_xw[FONTMAXSIZE][SYMBOLNUMBER]={
  {15,75,60,90,90,90,90,90,90,90},
  {30,75,75,105,75,105,105,105,75,105},
  {15,75,75,135,90,135,135,135,105,135},
  {30,105,105,135,105,165,150,165,120,150},
  {30,120,135,195,135,210,195,195,135,195},
  {45,150,150,255,195,270,255,270,195,255}
};

int symb_yh[FONTMAXSIZE][SYMBOLNUMBER]={
  {30,75,75,90,90,90,90,90,90,90},
  {30,90,90,120,105,105,105,105,105,105},
  {45,105,105,150,90,110,120,135,110,135},
  {45,120,120,150,105,165,135,160,125,150},
  {60,150,150,210,135,195,165,195,165,195},
  {90,180,195,270,180,270,230,270,195,270}
};


/** To get the bounding rectangle of a symbol 
  in fact just rect[3] is really used 
  **/


void C2F(boundingboxXfigM)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer verbose,nargs,font[2];
  verbose=0;
  C2F(xgetfontXfig)(&verbose,font,&nargs,vdouble);
  rect[0]= (int)(*x+bsizeXfig_[font[1]][0]*((double) prec_fact));
  rect[1]= (int)(*y+bsizeXfig_[font[1]][1]*((double) prec_fact));
  rect[2]= (int)(symb_xw[ScilabGCXfig.CurHardSymbSize][ScilabGCXfig.CurHardSymb]);
  rect[3]= (int)(symb_yh[ScilabGCXfig.CurHardSymbSize][ScilabGCXfig.CurHardSymb]);
}



/** Draw a single line in current style **/
/** Unused in fact **/ 

void C2F(drawlineXfig)(integer *x1, integer *yy1, integer *x2, integer *y2)
{
    FPRINTF((file,"# %d %d %d %d L\n",(int)*x1,(int)*yy1,(int)*x2,(int)*y2));
  }

/** Draw a set of segments **/
/** segments are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/**   if iflag == 1 style[i] gives the style for each segment
      if iflag == 0 (if *style >0 ) it   gives the style for all the  segment 
                    (if *style <0 ) The default style is used for all the  segment 
**/

void C2F(drawsegmentsXfig)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer NDvalue,i;
  int l_style,style_val,pen_color,fill_color,areafill;
  integer verbose=0,Dnarg,Dvalue[10],Dvalue1[10];
  /* store the current values */

  for ( i =0 ; i < *n/2 ; i++)
    {
      if ( (int) *iflag == 0) 
	NDvalue=(*style < 0) ? Dvalue[0] : *style;
      else
	NDvalue=(int) style[i];


      /** in case of min(max(,)) **/
      set_color(NDvalue,&pen_color);
      C2F(getdashXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
      set_dash(Dvalue1[0],&l_style,&style_val);

      areafill = -1;
      fill_color = WHITE;
      FPRINTF((file,"# Object : %d %s -<%d>- \n", (int)i,"segments", ScilabGCXfig.CurPattern));
      FPRINTF((file,"2 1 %d %d %d %d 0 0 -1 %d.000 0 0 0 0 0 2\n",
	      l_style,
	      ScilabGCXfig.CurLineWidth*prec_fact/16,
	      pen_color,fill_color,
	      style_val
	      ));
      FPRINTF((file," %d %d %d %d \n",
	      (int)vx[2*i], (int)vy[2*i], (int)vx[2*i+1], (int)vy[2*i+1]));
    }
}

/** Draw a set of arrows 
  if iflag == 1 style[i] gives the style for each arrow 
  if iflag == 0 *style   gives the style for all the arrows
**/

void C2F(drawarrowsXfig)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dx1, double *dx2, double *dx3, double *dx4)
{
  int i;
  int l_style,style_val,pen_color,fill_color,areafill;
  integer verbose=0,Dnarg,Dvalue[10],Dvalue1[10],NDvalue;
  /* store the current values */
  C2F(getdashXfig)(&verbose,Dvalue,&Dnarg,vdouble);
  for ( i = 0 ; i < *n/2 ; i++)
    {
      if ( (int) *iflag == 0) 
	NDvalue=(*style < 0) ? Dvalue[0] : *style;
      else
	NDvalue=(int) style[i];


      set_color(NDvalue,&pen_color);
      C2F(getdashXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
           set_dash(Dvalue1[0],&l_style,&style_val);
      /** Only draws **/
      areafill = -1;
      fill_color = WHITE;
      FPRINTF((file,"# Object : %d %s -<%d>-\n", (int)i,"arrows", ScilabGCXfig.CurPattern));
      FPRINTF((file,"2 1 %d %d %d %d 0 0 -1 %d.000 0 0 0 1 0 2\n",
	      l_style,
	      ScilabGCXfig.CurLineWidth*prec_fact/16,
	      pen_color,fill_color,
	      style_val
	      ));
      FPRINTF((file,"    0 0 %d %d %d\n",
	      (int)(1*prec_fact/16), (int)(3*prec_fact), (int) (6*prec_fact)));
      FPRINTF((file," %d %d %d %d \n",
	      (int)vx[2*i], (int)vy[2*i], (int)vx[2*i+1], (int)vy[2*i+1]));
    }
}

/** Draw one rectangle **/

void C2F(drawrectangleXfig)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i = 1;
  integer fvect[1] ;
  integer vects[4];
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;
  fvect[0] = 0;
  C2F(drawrectanglesXfig)(str,vects,fvect,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  }

/** Draw a filled rectangle **/

void C2F(fillrectangleXfig)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i = 1;
  integer vects[4],verb=0,cpat,num;
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height ; 
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  C2F(drawrectanglesXfig)(str,vects,&cpat,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);    
}

/** Draw or fill a set of rectangle **/
/** rectangles are defined by (vect[i],vect[i+1],vect[i+2],vect[i+3]) **/
/** for i=0 step 4 **/
/** (*n) : number of rectangles **/
/** fillvect[*n] : specify the action to perform fill or draw  **/
/** ( see periX11.c ) **/

void C2F(drawrectanglesXfig)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer cpat,verb,num;
  verb=0;
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericXfig)("drawbox",*n,(integer)4L,vects,vects,4*(*n),(integer)0L,fillvect);
  C2F(setpatternXfig)(&(cpat),PI0,PI0,PI0);
}


/** Draw or fill a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** fillvect[*n] : specify the action <?> **/
/** caution angle=degreangle*64          **/
/* old version no more used because it allows only full ellipse */
void C2F(fillarcsXfig_old)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer cpat,verb,num;
  verb=0;
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericXfig)("drawarc",*n,(integer)6L,vects,vects,6*(*n),(integer)0L,fillvect);
  C2F(setpatternXfig)(&(cpat),PI0,PI0,PI0);
}

void C2F(fillarcsXfig)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,pat;
  int i,i6;
  /* store the current values */
  C2F(getpatternXfig)(&verbose,&pat,&Dnarg,vdouble);
  for ( i=0 ; i < *n ; i++) 
    {
      /** to fix the style */
      C2F(setpatternXfig)(&fillvect[i],PI0,PI0,PI0);
      i6=6*i;
      C2F(fillarcXfig)(str,&(vects[i6]),&(vects[i6+1]),&(vects[i6+2]),
		      &(vects[i6+3]),&(vects[i6+4]),&(vects[i6+5]) ,dv1, dv2, dv3, dv4);

    }
  C2F(setpatternXfig)(&pat,PI0,PI0,PI0);
}


/** Draw a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/

/* Old definition no more used because it allows only full ellipse */
void C2F(drawarcsXfig_old)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10];
  /* store the current values */
  C2F(getdashXfig)(&verbose,Dvalue,&Dnarg,vdouble);
  C2F(WriteGenericXfig)("Rdrawarc",*n,(integer)6L,vects,vects,6*(*n),(integer)0L,style);
  C2F(setdashXfig)( Dvalue,PI0,PI0,PI0);
}

void C2F(drawarcsXfig)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  int i,i6;
  /* store the current values */
  C2F(get_dash_and_color_Xfig)(&verbose,Dvalue,&Dnarg,vdouble);
  for ( i=0 ; i < *n ; i++) 
    {
      /** to fix the style */
      NDvalue = style[i];
      C2F(set_line_style_Xfig)(&NDvalue,PI0,PI0,PI0);
      i6=6*i;
      C2F(drawarcXfig)(str,vects+i6,vects+(i6+1),vects+(i6+2),vects+(i6+3),
		      vects+(i6+4),vects+(i6+5) , dv1, dv2, dv3, dv4);
    }
  C2F(set_dash_and_color_Xfig)( Dvalue,PI0,PI0,PI0);
}


/** Draw a single ellipsis or part of it **/
/** caution angle=degreAngle*64          **/

/*  Old definition no more used  because it allows only full ellipse */

void C2F(drawarcXfig_old)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i =1;
  integer fvect[1] ;
  integer vects[6];
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;vects[4]= *angle1;vects[5]= *angle2;
  fvect[0] = ScilabGCXfig.IDLastPattern  +2;
  C2F(fillarcsXfig)(str,vects,fvect,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

void C2F(drawarcXfig)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer vx[365],vy[365],k,n;
  float alpha,fact=0.01745329251994330,w,h;
  integer close = 0;

  w = (*width)/2.0;
  h = (*height)/2.0;
  n=Min((*angle2/64),360);
  for (k = 0; k < n; ++k) {
    alpha=((*angle1/64)+k)*fact;
    vx[k] = *x + w*(cos(alpha)+1.0);
    vy[k] = *y + h*(-sin(alpha)+1.0);}
    
  C2F(drawpolylineXfig)(str, &n, vx, vy, &close, PI0, PI0, dv1, dv2, dv3, dv4);
}


/** Fill a single elipsis or part of it **/
/** with current pattern **/

/* Old definition commented out because it allows only full ellipse */
void C2F(fillarcXfig_old)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i =1;
  integer verb=0,cpat,num ;
  integer vects[6];
  vects[0]= *x;vects[1]= *y;vects[2]= *width;
  vects[3]= *height;vects[4]= *angle1;vects[5]= *angle2;
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  C2F(fillarcsXfig)(str,vects,&cpat,&i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
 }

void C2F(fillarcXfig)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer vx[365],vy[365],k,k0,kmax,n;
  float alpha,fact=0.01745329251994330,w,h;
  integer close = 1;

  n=Min((*angle2/64),360);

  w = (*width)/2.0;
  h = (*height)/2.0;
  k0 = 0;
  kmax = n-1;

  if (n != 360) {
  vx[0] = *x + w;
  vy[0] = *y + h;
  k0 = 1;
  kmax = n;}

  for (k = k0; k <= kmax; ++k) {
    alpha=((*angle1/64)+k)*fact;
    vx[k] = *x + w*(cos(alpha)+1.0);
    vy[k] = *y + h*(-sin(alpha)+1.0);}
  if (n != 360) {
  n++;
  vx[n] = *x + ((*width)/2.0);
  vy[n] = *y + ((*height)/2.0); 
  n++;
  }
  C2F(fillpolylineXfig)(str, &n, vx, vy, &close, PI0, PI0, dv1, dv2, dv3, dv4);
 }




/** Draw a set of  current mark centred at points defined **/
/** by vx and vy (vx[i],vy[i]) **/

void C2F(drawpolymarkXfig)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ integer keepid,keepsize,  i=1, sz=ScilabGCXfig.CurHardSymbSize;
  keepid =  ScilabGCXfig.FontId;
  keepsize= ScilabGCXfig.FontSize;
  C2F(xsetfontXfig)(&i,&sz,PI0,PI0);
  C2F(displaysymbolsXfig)(str,n,vx,vy);
  C2F(xsetfontXfig)(&keepid,&keepsize,PI0,PI0);
}

char symb_listXfig_[] = {
  /*
     0x2e : . alors que 0xb7 est un o plein trop gros 
     ., +,X,*,diamond(filled),diamond,triangle up,triangle down,trefle,circle*/
  (char)0x2e,(char)0x2b,(char)0xb4,(char)0xc5,(char)0xa8,
  (char)0xe0,(char)0x44,(char)0xd1,(char)0xa7,(char)0x4f};

static void C2F(displaysymbolsXfig)(char *str, integer *n, integer *vx, integer *vy)
{
  integer fvect[1];
  fvect[0] = 	  ScilabGCXfig.CurPattern;
  if ( ScilabGCXfig.CurVectorStyle !=  CoordModeOrigin)
    FPRINTF((file,"#/absolu false def\n"));
  FPRINTF((file,"#HardMark 0 16#%x put\n",
      Char2Int( symb_listXfig_[ScilabGCXfig.CurHardSymb])));
  C2F(WriteGenericXfig)("drawpolymark",(integer)1L,(*n)*2,vx,vy,*n,(integer)1L,fvect);
  FPRINTF((file,"#/absolu true def\n"));
}

/** Draw a set of *n polylines (each of which have (*p) points) **/
/** with lines or marks **/
/** drawvect[i] >= use a mark for polyline i **/
/** drawvect[i] < 0 use a line style for polyline i **/

void C2F(drawpolylinesXfig)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ integer verbose ,symb[2],Mnarg,Dnarg,Dvalue[10],NDvalue,i,close;
  verbose =0 ;
  /* store the current values */
  C2F(getcursymbolXfig)(&verbose,symb,&Mnarg,vdouble);
  C2F(get_dash_and_color_Xfig)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (drawvect[i] <= 0)
	{ /** on utilise la marque de numero associ\'ee **/
	  NDvalue = - drawvect[i] ;
	  C2F(setcursymbolXfig)(&NDvalue,symb+1,PI0,PI0);
          C2F(setdashXfig)(Dvalue,PI0,PI0,PI0);
	  C2F(drawpolymarkXfig)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{/** on utilise un style pointill\'e  **/
	  C2F(set_line_style_Xfig)(drawvect+i,PI0,PI0,PI0);
	  close = 0;
	  C2F(drawpolylineXfig)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,&close,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  /** back to default values **/
  C2F(set_dash_and_color_Xfig)(Dvalue,PI0,PI0,PI0);
  C2F(setcursymbolXfig)(symb,symb+1,PI0,PI0);
}

/** fill a set of polygons each of which is defined by **/
/** (*p) points (*n) is the number of polygons **/
/** the polygon is closed by the routine **/
/*  if fillvect[i] == 0 draw the boundaries with current color 
    if fillvect[i] > 0  draw the boundaries with current color 
    then fill with pattern fillvect[i]
    if fillvect[i] < 0  fill with pattern - fillvect[i]
*/


void C2F(fillpolylinesXfig)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer cpat,verb,num;
  verb=0;
  if ( ScilabGCXfig.CurVectorStyle !=  CoordModeOrigin)
    FPRINTF((file,"#/absolu false def\n"));
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  C2F(WriteGenericXfig)("drawpoly",*n,(*p)*2,vectsx,vectsy,(*p)*(*n),(integer)1L,
			fillvect);
  C2F(setpatternXfig)(&(cpat),PI0,PI0,PI0);
  FPRINTF((file,"#/absolu true def\n"));
}

/** Only draw one polygon with current line style **/
/** according to *closeflag : it's a polyline or a polygon **/
/** XXXXXX To be done Closeflag is not used **/

void C2F(drawpolylineXfig)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i=1,fvect=0;
  if (*closeflag == 1 )
    FPRINTF((file,"#/closeflag true def\n"));
  else 
    FPRINTF((file,"#/closeflag false def\n"));
  if (ScilabGCXfig.ClipRegionSet ==1 )
    {
      C2F(analyze_pointsXfig)(*n, vx, vy, *closeflag);
    }
  else 
    C2F(fillpolylinesXfig)(str,vx,vy,&fvect,&i,n,PI0,PD0,PD0,PD0,PD0);
}

/** Fill the polygon **/

void C2F(fillpolylineXfig)(char *str, integer *n, integer *vx, integer *vy, integer *closeareaflag, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  integer i =1;
  integer cpat,verb=0,num;
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  /** just fill  ==> cpat < 0 **/
  cpat = -cpat;
  C2F(fillpolylinesXfig)(str,vx,vy,&cpat,&i,n,PI0,PD0,PD0,PD0,PD0);
}
 
/*-----------------------------------------------------
\encadre{Routine for initialisation}
------------------------------------------------------*/


void C2F(initgraphicXfig)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  char string1[50];
  static integer EntryCounter = 0;
  integer fnum;
  if (EntryCounter >= 1) C2F(xendgraphicXfig)();
  strcpy(string1,string);
  file=fopen(string1,"w");
  if (file == 0) 
    {
      sciprint("Can't open file %s, I'll use stdout\r\n",string1);
      file = stdout;
    }
  if (EntryCounter == 0)
    { 
      fnum=0;      C2F(loadfamilyXfig)("Courier",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=1;      C2F(loadfamilyXfig)("Symbol",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=2;      C2F(loadfamilyXfig)("Times-Roman",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      fnum=3;      C2F(loadfamilyXfig)("Times-Italic",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      fnum=4;      C2F(loadfamilyXfig)("Times-Bold",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      fnum=5;      C2F(loadfamilyXfig)("Times-BoldItalic",&fnum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 

    }
  C2F(FileInitXfig)();
  ScilabGCXfig.CurWindow =EntryCounter;
  EntryCounter =EntryCounter +1;
}

static void C2F(FileInitXfig)(void)
{
  int m;
  integer x[2],verbose,narg;
  verbose = 0; 
  C2F(getwindowdimXfig)(&verbose,x,&narg,vdouble);
  FPRINTF((file,"#FIG 3.1\nPortrait\nCenter\nInches\n1200 2\n"));
  C2F(InitScilabGCXfig)(PI0,PI0,PI0,PI0)	;
  if (  CheckColormap(&m) == 1) 
    { 
      int i;
      float r,g,b;
      ScilabGCXfig.Numcolors = m;
      ScilabGCXfig.NumForeground = m;
      ScilabGCXfig.NumBackground = m + 1;

      if (ScilabGCXfig.CurColorStatus == 1) 
	{
	  ScilabGCXfig.IDLastPattern = ScilabGCXfig.Numcolors - 1;
	}
      for ( i=0; i < m ; i++)
	{
	  unsigned short ur,ug,ub;
	  get_r(i,&r);
	  get_g(i,&g);
	  get_b(i,&b);
	  ur = (unsigned short) (65535.0*r);
	  ug = (unsigned short) (65535.0*g);
	  ub = (unsigned short) (65535.0*b); 
	  ur = ur >> 8 ;
	  ug = ug >> 8 ;	
	  ub = ub >> 8 ; 
	  FPRINTF((file,"0 %d #%02x%02x%02x\n",32+i,ur,ug,ub));
	}
      FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m,0,0,0));
      FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m+1,255,255,255));
    }
  else 
    {
      /** the default_colors are the xfig default colors **/
      m = DEFAULTNUMCOLORS;
      ScilabGCXfig.Numcolors = m;
      ScilabGCXfig.IDLastPattern = m - 1;
      ScilabGCXfig.NumForeground = m;
      ScilabGCXfig.NumBackground = m + 1;
      FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m,0,0,0));
      FPRINTF((file,"0 %d #%02x%02x%02x \n",32+m+1,255,255,255));
    }
  FPRINTF((file,"2 2 0 0 -1 -1 0 0 -1 0.000 0 0 0 0 0 5\n"));
  FPRINTF((file," %d %d %d %d %d %d %d %d %d %d \n",
	  0,0,(int)x[0],0,(int)x[0],(int)x[1],0,(int)x[1],0,0));
}

/*--------------------------------------------------------
\encadre{Initialisation of the graphic context. Used also 
to come back to the default graphic state}
---------------------------------------------------------*/

void C2F(InitScilabGCXfig)(integer *v1, integer *v2, integer *v3, integer *v4)
{ integer i,j,col;
  ScilabGCXfig.IDLastPattern = GREYNUMBER - 1; /** bug ?? **/
  ScilabGCXfig.CurLineWidth=1 ;
  i=1;
  C2F(setthicknessXfig)(&i,PI0,PI0,PI0);
  C2F(setalufunctionXfig)("GXcopy");
  /** retirer le clipping **/
  i=j= -1;
  C2F(unsetclipXfig)(PI0,PI0,PI0,PI0);
  C2F(setdashXfig)((i=0,&i),PI0,PI0,PI0);
  C2F(xsetfontXfig)((i=2,&i),(j=1,&j),PI0,PI0);
  C2F(setcursymbolXfig)((i=0,&i),(j=0,&j),PI0,PI0);
  /** trac\'e absolu **/
  ScilabGCXfig.CurVectorStyle = CoordModeOrigin ;
  /* initialisation des pattern dash par defaut en n&b */
  ScilabGCXfig.CurColorStatus =0;
  C2F(setpatternXfig)((i=1,&i),PI0,PI0,PI0);
  C2F(setdashXfig)((i=1,&i),PI0,PI0,PI0);
  C2F(sethidden3dXfig)((i=1,&i),PI0,PI0,PI0);
  /* initialisation de la couleur par defaut */ 
  ScilabGCXfig.Numcolors = DEFAULTNUMCOLORS;
  ScilabGCXfig.CurColorStatus = 1 ;
  C2F(setpatternXfig)((i=1,&i),PI0,PI0,PI0);
  C2F(setforegroundXfig)((i=ScilabGCXfig.NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundXfig)((i=ScilabGCXfig.NumForeground+2,&i),PI0,PI0,PI0);
  C2F(sethidden3dXfig)((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic_ */
  getcolordef(&col);
  /** we force CurColorStatus to th eopposite value of col 
    to force usecolorPos to perform initialisations 
    **/
  ScilabGCXfig.graphicsversion = 0;/* NG */ /* old mode */
  ScilabGCXfig.CurColorStatus = (col == 1) ? 0: 1;
  C2F(usecolorXfig)(&col,PI0,PI0,PI0);
  if (col == 1) ScilabGCXfig.IDLastPattern = ScilabGCXfig.Numcolors - 1;
  strcpy(ScilabGCXfig.CurNumberDispFormat,"%-5.2g");
}

/*-------------------------------------------------------
\encadre{Check if a specified family of font exist in 
Postscript }
-------------------------------------------------------*/

void C2F(loadfamilyXfig)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
  integer i ;
  for ( i = 0; i < FONTMAXSIZE ; i++)
    {
      FontsListXfig_[*j][i] = C2F(FigQueryFont)(name);
    }
  if  (FontsListXfig_[*j][0] == 0 )
	  sciprint("\n unknown font family : %s\r\n",name);
  else 
    {FontInfoTabXfig_[*j].ok = 1;
     strcpy(FontInfoTabXfig_[*j].fname,name) ;}
}

void C2F(queryfamilyXfig)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer i ;
  name[0]='\0';
  for (i=0;i<FONTNUMBER;i++) {
    strcat(name,FontInfoTabXfig_[i].fname);
    v3[i]=strlen(FontInfoTabXfig_[i].fname);
  }
  *j=FONTNUMBER;
}
/*--------------------------------------------
\encadre{always answer ok. Must be Finished}
---------------------------------------------*/

static int C2F(FigQueryFont)(char *name)
{ return(1);}


/*------------------------------------------------------
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

void C2F(drawaxisXfig)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4)
{ integer i;
  int l_style,style_val,pen_color;
  double xi,yi,xf,yf;
  double cosal,sinal;
  integer verbose=0,Dnarg,Dvalue1[10];
  C2F(getdashXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
  set_dash_or_color(Dvalue1[0],&l_style,&style_val,&pen_color);
  FPRINTF((file,"# Begin Axis \n"));
  if ( *alpha == 90 )
    {cosal = 0.0 ; sinal =1.0 ;}
  else 
   {
     if ( *alpha == -90 )
       {cosal = 0.0 ; sinal = -1.0 ;}
     else 
       {
	 cosal= cos( M_PI * (*alpha)/180.0);
	 sinal= sin( M_PI * (*alpha)/180.0);
       }
   }
  for (i=0; i <= nsteps[0]*nsteps[1]; i++)
    { xi = initpoint[0]+i*size[0]*cosal;
      yi = initpoint[1]+i*size[0]*sinal;
      xf = xi - ( size[1]*sinal);
      yf = yi + ( size[1]*cosal);
      FPRINTF((file,"2 1 0 %d %d %d 0 0 -1 0.000 0 0 0 0 0 2\n",
	      ScilabGCXfig.CurLineWidth*prec_fact/16,
	      pen_color,
	      pen_color
	      ));
      FPRINTF((file," %d %d %d %d \n",(int)xi, (int)yi,  (int)  xf, (int)yf));
    }
  for (i=0; i <= nsteps[1]; i++)
    { xi = initpoint[0]+i*nsteps[0]*size[0]*cosal;
      yi = initpoint[1]+i*nsteps[0]*size[0]*sinal;
      xf = xi - ( size[1]*size[2]*sinal);
      yf = yi + ( size[1]*size[2]*cosal);
      FPRINTF((file,"2 1 0 %d %d %d 0 0 -1 0.000 0 0 0 0 0 2\n",
	      ScilabGCXfig.CurLineWidth*prec_fact/16,
	      pen_color,
	      pen_color
	      ));
      FPRINTF((file," %d %d %d %d \n", (int)xi, (int) yi, (int)xf, (int)yf));
    }
  xi = initpoint[0]; yi= initpoint[1];
  xf = initpoint[0]+ nsteps[0]*nsteps[1]*size[0]*cosal;
  yf = initpoint[1]+ nsteps[0]*nsteps[1]*size[0]*sinal;
  FPRINTF((file,"2 1 0 %d %d %d 0 0 -1 0.000 0 0 0 0 0 2\n",
	  ScilabGCXfig.CurLineWidth*prec_fact/16,
	  pen_color,
	  pen_color));
  FPRINTF((file," %d %d %d %d \n",  (int)xi,  (int)yi, (int) xf, (int)yf));
  FPRINTF((file,"# End Of Axis \n"));
}


/*-----------------------------------------------------
\encadre{Display numbers z[i] at location (x[i],y[i])
  with a slope alpha[i] (see displaystring_), if flag==1
  add a box around the string.
-----------------------------------------------------*/
void C2F(displaynumbersXfig)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4)
{ integer i ;
  char buf[20];
  for (i=0 ; i< *n ; i++)
    { 
      sprintf(buf,ScilabGCXfig.CurNumberDispFormat,z[i]);
      C2F(displaystringXfig)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0);
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


/*****************************************************************************
 * give the correct pattern for xfig 0=white-> 20=black 
 * from our pattern coding 0=black    ScilabGCXfig.IDLastPattern=white 
 *  we use xfig as follows : 
 *  when use_color == 1 we use the 32 standard colors of xfig with 20 ( full saturation )
 *  as fill area 
 *  when use_color == 0 we use the white color + areafill from 0 to 20 to generate 
 *  shades of gray 
 * XXXXXX : xset("colormap") must be implemented 
 *******************************************************************************/

#define AREAF(x) Max(0,Min(20,(int) (20.0*((double) x) /((double) GREYNUMBER -1 ))))

void set_pattern_or_color(int pat, int *areafill, int *color)
{
  if (  ScilabGCXfig.CurColorStatus == 1) 
    {
      int m;
      *color = pat-1 ; /** color value **/
      if (  CheckColormap(&m) == 1) 
	{
	  /** fix the currennt color : if a colormap is set 
	    we must have an ofset of 32 **/
	  *color += 32;
	}
      else 
	{
	  /* special case for B&white in the default colormap */
	  if ( *color == 32 ) 
	    {
	      *color = DEFAULTBLACK;
	    }
	  else if ( *color == 33 ) 
	    {
	      *color = DEFAULTWHITE;
	    }
	}
      *areafill = 20 ; /** full color saturation **/
    }
  else 
    {
      *color = WHITE ; 
      *areafill = AREAF(pat-1); /** shade of gray **/
    }
}



static void set_color(int c, int *color)
{
  int m;
  if (  ScilabGCXfig.CurColorStatus == 0) {
    *color=0;
    return;
  }
  *color = c-1 ; /** color value **/
  if (  CheckColormap(&m) == 1) 
    {
      /** fix the current color : if a colormap is set 
	  we must have an ofset of 32 **/
      *color += 32;
    }
  else 
    {
      /* special case for B&white in the default colormap */
      if ( *color == 32 ) 
	{
	  *color = DEFAULTBLACK;
	}
      else if ( *color == 33 ) 
	{
	  *color = DEFAULTWHITE;
	}
    }
}

static void set_dash(int dash, int *l_style, int *style_val)
{
  int i;
  i = Max(Min(MAXDASH -1,dash-1),0);
  *l_style = DashTab[i];
  *style_val = DashTabStyle[i];
}

static void set_dash_or_color(int dash, int *l_style, int *style_val, int *color)
{
  int j;
  if (  ScilabGCXfig.CurColorStatus == 1) 
    {
      j= ScilabGCXfig.CurDashStyle + 1;
      set_dash(j,l_style,style_val);
      set_color(dash,color);
      *l_style = 0 ;/** solid line **/
      *style_val=0;
    }
  else 
    {
      set_dash(dash,l_style,style_val);
      *color = BLACK;
    }
}


#define PERLINE 15
/** ne pas oublier le blanc aprse %d **/
#define FORMATNUM "%d "

void C2F(WriteGenericXfig)(char *string, integer nobj, integer sizeobj, integer *vx, integer *vy, integer sizev, integer flag, integer *fvect)
{ 
  integer i;
  integer verb=0,cpat,num;
  integer verbose=0,Dnarg,Dvalue[10],Dvalue1[10];
  integer lg,type=1 ;
  integer areafill,fill_color,pen_color,l_style,style_val;
  C2F(getdashXfig)(&verb,Dvalue,&Dnarg,vdouble);
  C2F(getpatternXfig)(&verb,&cpat,&num,vdouble);
  if ( nobj==0|| sizeobj==0) return;
  if ( strcmp(string,"drawpoly")==0)
    {
      for ( i =0 ; i < nobj ; i++)
	{
	  if (fvect[i] < 0 )
	   {
	     /** only fill **/

	     set_pattern_or_color( - fvect[i],&areafill,&fill_color);
	     l_style = 0;
	     style_val = 0;
	     pen_color = fill_color;
	     type = 3;
	   }
	  else if (fvect[i] == 0 )
	    {
	      /** only draws th polyline **/
	      set_color(cpat,&pen_color);
	      set_dash(Dvalue[0],&l_style,&style_val);
	      areafill=-1;
	      fill_color = WHITE;
	    }
	  else 
	    /** fill with pattern  and draw with current dash **/
	    { 
	      set_pattern_or_color(fvect[i],&areafill,&fill_color);
	      set_color(cpat,&pen_color);
	      set_dash(Dvalue[0],&l_style,&style_val);
	      /*set_dash_or_color(Dvalue[0],&l_style,&style_val,&pen_color);*/
	      type=3;
	    }
	  lg=sizeobj/2;
	  FPRINTF((file,"# Object : %d %s -<pat:%d,areafill=%d,white=%d>- \n", (int)i,string,
		  (int)fvect[i],
		  (int)areafill,
		  ScilabGCXfig.IDLastPattern));
	  FPRINTF((file,"2 %d %d %d %d %d 0 0 %d %d.00 0 0 -1 0 0 %d\n",
		   (int)type,l_style, ScilabGCXfig.CurLineWidth*prec_fact/16,
		  pen_color,fill_color,areafill,style_val, (int)lg
		  ));
	  C2F(Write2VectXfig)(&vx[i*lg],&vy[i*lg],lg,flag);
	}
    }
  else 
  if ( strcmp(string,"drawbox")==0)
    {
      for ( i =0 ; i < nobj ; i++)
	{
	  integer deb;
	  if (fvect[i] < 0  )
	    {
	      /** Only draws the rectangle **/
	      set_dash(Dvalue[0],&l_style,&style_val);
	      set_color( -fvect[i],&pen_color);
	      /*set_dash_or_color( -fvect[i],&l_style,&style_val,&pen_color);*/
	      areafill = -1;
	      fill_color = WHITE;
	    }
	  else 	  if (fvect[i] == 0  )
	    {
	      /** Only draws the rectangle **/
	      set_dash(Dvalue[0],&l_style,&style_val);
	      set_color(cpat,&pen_color);
	      /*set_dash_or_color(Dvalue[0],&l_style,&style_val,&pen_color);*/
	      areafill = -1;
	      fill_color = WHITE;
	    }
	  else 
	    {
	      /** fills the rectangle **/
	      set_pattern_or_color(fvect[i],&areafill,&fill_color);
	      pen_color = fill_color;
	      l_style = 0;
	      style_val = 0;
	      type = 3;
	    }
	  FPRINTF((file,"# Object : %d %s -<%d>- \n", (int)i,string, (int)fvect[i]));
	  FPRINTF((file,"2 2 %d %d %d %d 0 0 %d %d.000 0 0 0 0 0 5\n",
		  l_style, ScilabGCXfig.CurLineWidth*prec_fact/16,
		  pen_color,fill_color,areafill,style_val));
	  deb=i*sizeobj;
	  FPRINTF((file," %d %d %d %d %d %d %d %d %d %d \n",
		  (int)vx[deb]                , (int)vx[1+deb],
		  (int)vx[deb]+ (int)vx[2+deb], (int)vx[1+deb],
		  (int)vx[deb]+ (int)vx[2+deb], (int)vx[1+deb]+ (int)vx[3+deb],
		  (int)vx[deb]                , (int)vx[1+deb]+ (int)vx[3+deb],
		  (int)vx[deb]                , (int)vx[1+deb]));
	}
    }
  else if ( strcmp(string,"drawsegs")==0)      
    {
      /** see drawsegsXfig **/
    }
  else if ( strcmp(string,"drawarrows")==0)      
    {
      /** see drawarrowsXfig **/
    }
  else if ( strcmp(string,"drawarc")==0)      
    {
      for ( i = 0 ; i < nobj ; i++)
	{
	  if (fvect[i] > ScilabGCXfig.IDLastPattern+1 )
	    {
	      /** Only draws the arc **/
	      set_dash_or_color(Dvalue[0],&l_style,&style_val,&pen_color);
	      areafill = -1;
	      fill_color = WHITE;
	    }
	  else 
	    {
	      /** fills the arc **/
	      set_pattern_or_color(fvect[i],&areafill,&fill_color);
	      pen_color = fill_color;
	      l_style = 0;
	      style_val = 0;
	      type = 3;
	    }
	  FPRINTF((file,"# Object : %d %s -<%d>-\n", (int)i,string, (int)fvect[0]));
	  FPRINTF((file,
		  "1 2 %d %d %d %d 0 0 %d %d.000 1 0.00 %d %d %d %d %d %d %d %d \n",
		  l_style,
		  ScilabGCXfig.CurLineWidth*prec_fact/16,
		  pen_color,fill_color,areafill,style_val,
		  (int)vx[6*i]+ (int)vx[6*i+2]/2, 
		  (int)vx[6*i+1]+ (int)vx[6*i+3]/2,
		  (int)vx[6*i+2]/2, (int)vx[6*i+3]/2,
		  (int)vx[6*i]+ (int)vx[6*i+2]/2, (int)vx[6*i+1],
		  (int)vx[6*i]+ (int)vx[6*i+2]/2, (int)vx[6*i+1]));
	}
    }
  else if ( strcmp(string,"Rdrawarc")==0)      
    {
      /* store the current values */
      for ( i = 0 ; i < nobj ; i++)
	{
	  /*C2F(setdashXfig)(&fvect[i],PI0,PI0,PI0);*/

	  set_dash(Dvalue[0],&l_style,&style_val);
	  set_color(fvect[i],&pen_color);

	  /** in case of min(max(,)) **/
	  /*C2F(getdashXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
	    set_dash_or_color(Dvalue1[0],&l_style,&style_val,&pen_color);*/
	  areafill = -1;
	  fill_color = WHITE;
	  FPRINTF((file,"# Object : %d %s -<%d>-\n", (int)i,string, (int)fvect[0]));
	  FPRINTF((file,
		  "1 2 %d %d %d %d 0 0 %d %d.000 1 0.00 %d %d %d %d %d %d %d %d \n",
		  l_style,
		  (int) (ScilabGCXfig.CurLineWidth*prec_fact/16),
		  pen_color,fill_color,
		  areafill, 
		  style_val,
		  (int) (vx[6*i]+vx[6*i+2]/2), 
		  (int) (vx[6*i+1]+vx[6*i+3]/2),
		  (int) (vx[6*i+2]/2), (int) (vx[6*i+3]/2),
		  (int) (vx[6*i]+ vx[6*i+2]/2), (int)vx[6*i+1],
		  (int) (vx[6*i]+ vx[6*i+2]/2), (int)vx[6*i+1]));
	}
      /*C2F(setdashXfig)( Dvalue,PI0,PI0,PI0);*/
    }
  else if ( strcmp(string,"drawpolymark")==0)      
    {
      integer rect[4],x=0,y=0;
      C2F(getpatternXfig)(&verbose,Dvalue1,&Dnarg,vdouble);
      set_color(Dvalue1[0],&pen_color);
      l_style=0;style_val=0,
      C2F(boundingboxXfigM)("x",&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      FPRINTF((file,"# Object : %d %s -<%d>- \n", (int)0,string, (int)fvect[0]));
      for ( i =0 ; i < sizev ; i++)
	{
	  int flag = 1;
	  if ( ScilabGCXfig.ClipRegionSet == 1 ) 
	    {
	      if ( vx[i] > ScilabGCXfig.CurClipRegion[0] 
		   +ScilabGCXfig.CurClipRegion[2]
		   || vx[i] <  ScilabGCXfig.CurClipRegion[0] 
		   || vy[i] > ScilabGCXfig.CurClipRegion[1] 
		   +ScilabGCXfig.CurClipRegion[3]
		   || vy[i] < ScilabGCXfig.CurClipRegion[1] )
		flag = 0;
	    }
	  
	  /** polymarks are x-center justified sub-type =1  **/
	  if ( flag == 1) 
	  FPRINTF((file,"4 1 %d 0 0 %d %d %5.2f %d %5.2f %5.2f %d %d \\%o\\001\n",
		  pen_color,
		  32, /* Postscript font */
		  (int)isizeXfig_[ScilabGCXfig.FontSize], /**prec_fact,*/
		  0.0,
		  4,  
		  (double) rect[3],
		  (double) rect[2],
		  (int)vx[i],
		  (int)vy[i] + rect[3]/2,
		  Char2Int( symb_listXfig_[ScilabGCXfig.CurHardSymb])
		  ));

	}
    }
  else
    sciprint("Can't translate %s\r\n",string);
}


void C2F(Write2VectXfig)(integer *vx, integer *vy, integer n, integer flag)
{
  integer i,k;
  i=0;
  while( i < n)
    {
      k=0;
      while ( k < PERLINE && i < n )
	{
	  FPRINTF((file,FORMATNUM,(int) vx[i]));
	  if (flag == 1) 
	    {FPRINTF((file,FORMATNUM,(int) vy[i]));
	      k += 2;i++;}
	  else 
	    {k++;i++;}
	}
      FPRINTF((file,"\n"));
    }
}


/************************************************************
 * Clipping functions for XFig 
 ************************************************************/

static void MyDraw(integer iib, integer iif, integer *vx, integer *vy)
{
  integer fvect=0,ipoly=1;
  integer iideb;
  integer x1kn,y1kn,x2kn,y2kn;
  integer x1n,y1n,x11n,y11n,x2n,y2n,flag2=0,flag1=0;
  integer npts;
  npts= ( iib > 0) ? iif-iib+2  : iif-iib+1;
  if ( iib > 0) 
    {
      clip_line(vx[iib-1],vy[iib-1],vx[iib],vy[iib],&x1n,&y1n,&x2n,&y2n,&flag1);
    }
  clip_line(vx[iif-1],vy[iif-1],vx[iif],vy[iif],&x11n,&y11n,&x2n,&y2n,&flag2);
  /** if (C2F(store_points)(npts, &vx[Max(0,iib-1)], &vy[Max(0,iib-1)],(integer)0L)); **/
  iideb = Max(0,iib-1);
  if (iib > 0 && (flag1==1||flag1==3)) 
    {
      x1kn=vx[iideb]; y1kn=vy[iideb];
      vx[iideb]=x1n; vy[iideb]=y1n;
    }
  if (flag2==2 || flag2==3) 
    {
      x2kn=vx[iideb+npts-1]; y2kn=vy[iideb+npts-1];
      vx[iideb+npts-1]=x2n; vy[iideb+npts-1]=y2n;
    }
  C2F(fillpolylinesXfig)("MyDraw",&vx[iideb],&vy[iideb],&fvect,&ipoly,&npts,
			 PI0,PD0,PD0,PD0,PD0);
  if (iib > 0 && (flag1==1||flag1==3)) 
    {
      vx[iideb]=x1kn; vy[iideb]=y1kn;
    }
  if (flag2==2 || flag2==3) 
    {
      vx[iideb+npts-1]=x2kn; vy[iideb+npts-1]=y2kn;
    }
}

static void My2draw(integer j, integer *vx, integer *vy)
{
  /** The segment is out but can cross the box **/
  integer vxn[2],vyn[2],flag,fvect=0,ipoly=1;
  clip_line(vx[j-1],vy[j-1],vx[j],vy[j],&vxn[0],&vyn[0],&vxn[1],&vyn[1],&flag);
  if (flag == 3 ) 
  {
    integer deux=2;
    C2F(fillpolylinesXfig)("MyDraw",vxn,vyn,&fvect,&ipoly,&deux,
			 PI0,PD0,PD0,PD0,PD0);
  }
}

static void C2F(analyze_pointsXfig)(integer n, integer *vx, integer *vy, integer onemore)
{ 
  integer iib,iif,ideb=0,vxl[2],vyl[2],fvect=0,ipoly=1,deux=2;
  integer xleft, xright, ybot, ytop;
  xleft=ScilabGCXfig.CurClipRegion[0];
  xright=xleft+ScilabGCXfig.CurClipRegion[2];
  ybot=ScilabGCXfig.CurClipRegion[1];
  ytop= ybot + ScilabGCXfig.CurClipRegion[3];
  set_clip_box(xleft, xright, ybot, ytop);
  while (1) 
    { integer j;
      iib=first_in(n,ideb,vx,vy);
      if (iib == -1) 
	{ 
	  for (j=ideb+1; j < n; j++) My2draw(j,vx,vy);
	  break;
	}
      else 
      if ( iib - ideb > 1) 
	{
	  /* un partie du polygine est totalement out de ideb a iib -1 */
	  /* mais peu couper la zone */
	  for (j=ideb+1; j < iib; j++) My2draw(j,vx,vy);
	};
      iif=first_out(n,iib,vx,vy);
      if (iif == -1) {
	/* special case the polyligne is totaly inside */
	if (iib == 0) 
	  {
	    /** XXXX : if (C2F(store_points)(n,vx,vy,onemore)); **/
	    /** if (onemore == 1) n1 = n+1;else n1= n; **/
	    C2F(fillpolylinesXfig)("MyDraw",vx,vy,&fvect,&ipoly,&n,
			 PI0,PD0,PD0,PD0,PD0);
	    return ;
	  }
	else 
	  MyDraw(iib,n-1,vx,vy);
	break;
      }
      MyDraw(iib,iif,vx,vy);
      ideb=iif;
    }
  if (onemore == 1) {
    /* The polyligne is closed we consider the closing segment */
    integer x1n,y1n,x2n,y2n,flag1=0;
    vxl[0]=vx[n-1];vxl[1]=vx[0];vyl[0]=vy[n-1];vyl[1]=vy[0];
    clip_line(vxl[0],vyl[0],vxl[1],vyl[1],&x1n,&y1n,&x2n,&y2n,&flag1);
    if ( flag1==0) return ;
    if (flag1==1||flag1==3) {vxl[0]=x1n;vyl[0]=y1n;}
    if (flag1==2||flag1==3) {vxl[1]=x2n;vyl[0]=y2n;}
    C2F(fillpolylinesXfig)("MyDraw",vxl,vyl,&fvect,&ipoly,&deux,
			 PI0,PD0,PD0,PD0,PD0);
  }
}

/* NG beg */
void C2F(setscilabFigureXfig)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure)
{
 figure=(double *)ScilabGCXfig.mafigure;
}

void C2F(getscilabFigureXfig)(integer *verbose, integer *x,integer *narg, double *figure)
{   
  figure=(double *)ScilabGCXfig.mafigure;
}
void C2F(setscilabVersionXfig)(integer *vers, integer *v2, integer *v3, integer *v4)
{
  ScilabGCXfig.graphicsversion=*vers;
}

void C2F(getscilabVersionXfig)(integer *verbose, integer *vers, integer *narg, double *dummy)
{   
  *vers = ScilabGCXfig.graphicsversion;
}
void C2F(getscilabxgcXfig)(integer *verbose, integer *x,integer *narg, double *dummy)
{   
 double **XGC;
 XGC=(double **)dummy;
 *XGC= (double *)&ScilabGCXfig;
}
void C2F(setscilabxgcXfig)(integer *v1, integer *v2, integer *v3, integer *v4)
{}
/* NG end */



