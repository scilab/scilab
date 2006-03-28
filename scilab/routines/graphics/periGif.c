/*------------------------------------------------------------------------

    SCILAB GIF Output based on GD Library from: http://www.boutell.com/gd
    Modelled after the original PostScript Driver in periPos.c

    Copyright (C) 1999, Tom Leitner, tom@finwds01.tu-graz.ac.at
    http://wiis.tu-graz.ac.at/people/tom.html

    NOTE: This needs the GD Library installed in the "gd"
    subdirectory of this directory.
    
    WARNING: The following things are not implemented yet:

         - Thick lines.

    Usage of the driver:

         driver ('GIF');
	 xinit ('test.gif');
	 xset('wdim', 800, 600);
         ......  make your drawings ....
	 xend();

    will produce a 800 by 600 GIF file called test.gif. If you omit the
    xset(wdim,...) call, the default image size is 640 by 480.
    
--------------------------------------------------------------------------*/

/*----------------------BEGIN----------------------
\def\encadre#1{\paragraph{}\fbox{\begin{minipage}[t]{15cm}#1 \end{minipage}}}
\section{A GIF Driver}
---------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
extern  char  *getenv();
#endif
#if defined(THINK_C) || defined (__MWERKS__)|| defined(WIN32)
#define CoordModePrevious 1
#define CoordModeOrigin 0
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

#define COLORMAP_WARNING "Warning: with GIF and PPM drivers, the size of colormaps should be less than %d. The resulting output may be inaccurate.\n"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#define Char2Int(x)   ( x & 0x000000ff )
#if defined(__CYGWIN32__) || defined(__MINGW32__) || defined(__GNUC__) || defined(__MSC__)|| defined(__EDG__)
static FILE *file= (FILE *) 0;
#define FPRINTF(x) ( file != (FILE*) 0) ?  fprintf x  : 0 
#else 
#define FPRINTF(x) fprintf x  
static FILE *file= stdout ;
#endif

#include "Math.h"
#include "periGif.h"
#include "color.h"
#include "../gd/gd.h"
#include "bcg.h" /* NG */


void C2F(WriteGeneric1Gif)();
void C2F(xgetmarkGif)(integer *verbose, integer *symb, integer *narg, double *dummy);
void C2F(xsetmarkGif)(integer *number, integer *size, integer *v3, integer *v4);
void C2F(xgetfontGif)(integer *verbose, integer *font, integer *nargs, double *dummy);
void C2F(xsetfontGif)(integer *fontid, integer *fontsize, integer *v3, integer *v4);
void C2F(setdashstyleGif)(integer *value, integer *xx, integer *n);
void C2F(Write2VectGif)();
void C2F(WriteGenericGif)(char *string, integer nobj, integer sizeobj, integer *vx, integer *vy, integer sizev, integer flag, integer *fvect);
void C2F(InitScilabGCGif)(integer *v1, integer *v2, integer *v3, integer *v4);
static void SetGraphicsVersion();
void C2F(setforegroundGif)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(ScilabGCGetorSetGif)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1);
void C2F(setbackgroundGif)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(set_cGif)(integer i);
void C2F(idfromnameGif) (char *name1, integer *num);
void C2F(getdashGif)(integer *verbose, integer *value, integer *narg, double *dummy);

/* NG beg */
void C2F(setscilabFigureGif)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure);
void C2F(getscilabFigureGif)(integer *verbose, integer *x,integer *narg, double *figure);
void C2F(setscilabVersionGif)(integer *vers, integer *v2, integer *v3, integer *v4);
void C2F(getscilabVersionGif)(integer *verbose, integer *vers, integer *narg, double *dummy);
void C2F(setscilabxgcGif)(integer *v1, integer *v2, integer *v3, integer *v4); 
void C2F(getscilabxgcGif)(integer *verbose, integer *x,integer *narg, double *dummy);
/* NG end */


/* static int C2F(GifQueryFont)(); */
static void C2F(displaysymbolsGif)(char *str, integer *n, integer *vx, integer *vy);
extern int ReadbdfFont();
extern void C2F(nues1)();
extern int CheckScilabXgc();


static double *vdouble = 0; /* used when a double argument is needed */

static gdImagePtr GifIm = (gdImagePtr)0;
static gdFontPtr  GifFont;
static int GifDashes[50], nGifDashes;
static int col_white;
static void FileInitGif  __PARAMS((void));
static void FileInitFromScreenGif  __PARAMS((void));
static int GifLineColor __PARAMS((void));
static int GifPatternColor __PARAMS((int pat));
static void LoadFontsGif(void);
static void ColorInitGif(void);
static void C2F(getcolormapGif) __PARAMS((integer *v1, integer *v2, integer *v3, double *val));
static void C2F(setgccolormapGif) __PARAMS((struct BCG *Xgc,integer m, double *a, integer *v3));

extern int gdImageSymb(gdImagePtr im, gdFontPtr f, int x, int y, int c, int color);
extern int versionflag;
/** Structure to keep the graphic state  **/

struct BCG  ScilabGCGif ;

static int col_index[gdMaxColors];
static int fillpolylines_closeflag = 0;

/*-----------------------------------------------------
\encadre{General routines}
-----------------------------------------------------*/

/** To select the graphic Window  **/

void C2F(xselgraphicGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if (file != stdout && file != 0 ) {
    fclose(file);
    file=stdout;
  }
  if (GifIm != (gdImagePtr)0) {
    gdImageDestroy(GifIm);
    GifIm = (gdImagePtr)0;
  }
}

/** End of graphic (close the file) **/

void GetDriverName(char *DriverName)
{    integer *v2 = NULL;
     integer *v3 = NULL;
     integer *v4 = NULL;
     integer *v5 = NULL;
     integer *v6 = NULL;
     integer *v7 = NULL;
     double *dv1 = NULL;
     double *dv2 = NULL;
     double *dv3 = NULL;
     double *dv4 = NULL;
     GetDriver1(DriverName, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4);
}

void C2F(xendgraphicGif)(void)
{
  char DriverName[10];
  integer num;
  if (file != stdout && file != (FILE*) 0 && GifIm != (gdImagePtr)0) {
    num = ScilabGCGif.NumBackground;
    gdImageChangeColor(GifIm,col_white,col_index[num]);
    gdSetBackground(GifIm,col_index[num] );
    GetDriverName(DriverName);
    if (strcmp(DriverName,"GIF")==0)
      gdImageGif(GifIm, file);
    else if (strcmp(DriverName,"PPM")==0)
      gdImagePPM(GifIm, file);
    fclose(file);
    gdImageDestroy(GifIm);
	GifIm = (gdImagePtr)0; /* F.Leray 25.08.04 */
    file=stdout;
  }
}

void C2F(xendGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  C2F(xendgraphicGif)();
}


/** Clear the current graphic window     **/
/** In GIF : nothing      **/

void C2F(clearwindowGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{

  /* F.Leray : I add a rectangle with  background color*/
  int zero = 0;
  int col_white = col_index[ScilabGCGif.Numcolors+1];

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 1 xinit must be called before any action \r\n");
    return;
  }

/*   sciprint("JE PASSE PAR ICI et col_white vaut: %d\n",col_white); */

  gdImageFilledRectangle(GifIm, zero, zero,  ScilabGCGif.CWindowWidth,  ScilabGCGif.CWindowHeight,
			 col_white);

  /* FPRINTF((file,"\n showpage")); */
  /** Sending the scale etc.. in case we want an other plot **/
  /* FileInitGif(file); */
}

/** To generate a pause : Empty here **/

void C2F(xpauseGif)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{}

/** Wait for mouse click in graphic window : Empty here **/

void C2F(xclickGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ }

void C2F(xclick_anyGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ }

void C2F(xgetmouseGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ }

/** Clear a rectangle **/

void C2F(clearareaGif)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
/*  FPRINTF((file,"\n [ %d %d %d %d ] clearzone",(int)*x,(int)*y,(int)*w,(int)*h));*/
}



/*---------------------------------------------------------------------
\section{Function for graphic context modification}
------------------------------------------------------------------------*/

/** to get the window upper-left pointeger coordinates **/

void C2F(getwindowposGif)(integer *verbose, integer *x, integer *narg, double *dummy)
{
  *narg = 2;
  x[0]= x[1]=0;
  if (*verbose == 1) 
    sciprint("\n CWindow position :%d,%d\r\n",(int)x[0],(int)x[1]);
 }

/** to set the window upper-left pointeger position (Void) **/

void C2F(setwindowposGif)(integer *x, integer *y, integer *v3, integer *v4)
{
}

/** To get the window size **/

void C2F(getwindowdimGif)(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  *narg = 2;
  x[0]= ScilabGCGif.CWindowWidth;
  x[1]= ScilabGCGif.CWindowHeight;
  if (*verbose == 1) 
    sciprint("\n CWindow dim :%d,%d\r\n",(int)x[0],(int)x[1]);
} 

/** To change the window dimensions */

void C2F(setwindowdimGif)(integer *x, integer *y, integer *v3, integer *v4)
{
  integer x2[2],verbose,narg;
  integer old_num;
  integer i,j,col,v;
  double *val = NULL;
  

  gdImagePtr GifImOld = GifIm;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 2 xinit must be called before any action \r\n");
    return;
  }
  GifIm = gdImageCreate(*x, *y);

  /*  ScilabGCGif.CWindowWidth  = *x;
      ScilabGCGif.CWindowHeight = *y; */
  
  
  /* save current colormap */
  old_num = ScilabGCGif.Numcolors;
  
  if((val=MALLOC((old_num*3)*sizeof(double)))==NULL)
    {
      sciprint("Error: can not allocate temporary coloramp\n");
      return;
    }
  
  C2F(getcolormapGif)(&v,&v,&v,val);
  /* end save current colormap */
  
  verbose = 0; 
  C2F(getwindowdimGif)(&verbose,x2,&narg,vdouble);
  
  ColorInitGif();

  InitScilabGCGif(PI0,PI0,PI0,PI0);

  ScilabGCGif.IDLastPattern = GREYNUMBER-1;
  ScilabGCGif.CurLineWidth=0 ;
  i=1;
  C2F(setthicknessGif)(&i,PI0,PI0,PI0);
  C2F(setalufunctionGif)("GXcopy");
  /** retirer le clipping **/
  i=j= -1;
  C2F(unsetclipGif)(PI0,PI0,PI0,PI0);
  C2F(xsetfontGif)((i=2,&i),(j=1,&j),PI0,PI0);
  C2F(xsetmarkGif)((i=0,&i),(j=0,&j),PI0,PI0);
  /** trac\'e absolu **/
  ScilabGCGif.CurVectorStyle = CoordModeOrigin ;
  /* initialisation des pattern dash par defaut en n&b */
  ScilabGCGif.CurColorStatus =0;
  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
  C2F(setdashGif)((i=1,&i),PI0,PI0,PI0);
  
  /* initialisation de la couleur par defaut */ 
/*   ScilabGCGif.Numcolors = DEFAULTNUMCOLORS; */
  ScilabGCGif.CurColorStatus = 1 ;
  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
  C2F(setforegroundGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundGif)((i=ScilabGCGif.NumForeground+2,&i),PI0,PI0,PI0);
  C2F(sethidden3dGif)((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic_ */
  getcolordef(&col);
  /** we force CurColorStatus to the opposite value of col 
    to force usecolorGif to perform initialisations 
    **/

/*   ScilabGCGif.mafigure = (sciPointObj *)NULL; */
/*   ScilabGCGif.graphicsversion = versionflag; /\* NG *\/ */
  ScilabGCGif.CurColorStatus = (col == 1) ? 0: 1;
  C2F(usecolorGif)(&col,PI0,PI0,PI0);
  if (col == 1) ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
  strcpy(ScilabGCGif.CurNumberDispFormat,"%-5.2g");


  SetGraphicsVersion(); /* set the graphics version using global versionflag variable */

  C2F(setpatternGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0); 
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
    }

  /* reset correct colormap */
  C2F(setgccolormapGif)(&ScilabGCGif,old_num, val, &v);
  
  FREE(val); val = (double *) NULL;
  /* end reset correct colormap */

  gdImageFilledRectangle(GifIm, 0, 0, (*x) - 1, (*y) - 1, col_white);

  gdImageCopyResized(GifIm, GifImOld, 0, 0, 0, 0, *x, *y, 
              ScilabGCGif.CWindowWidth,
              ScilabGCGif.CWindowHeight);
  gdImageDestroy(GifImOld);
  ScilabGCGif.CWindowWidth  = *x;
  ScilabGCGif.CWindowHeight = *y;
}

/** Select a graphic Window : Empty for GIF **/

void C2F(setcurwinGif)(integer *intnum, integer *v2, integer *v3, integer *v4)
{
  ScilabGCGif.CurWindow = *intnum;
}

/** Get the id number of the Current Graphic Window **/

void C2F(getcurwinGif)(integer *verbose, integer *intnum, integer *narg, double *dummy)
{
  *narg =1 ;
  *intnum = ScilabGCGif.CurWindow ;
  if (*verbose == 1) 
    Scistring("\nJust one graphic page at a time ");
}

/** Set a clip zone (rectangle ) **/

void C2F(setclipGif)(integer *x, integer *y, integer *w, integer *h)
{
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 3 xinit must be called before any action \r\n");
    return;
  }
  ScilabGCGif.ClipRegionSet = 1;
  ScilabGCGif.CurClipRegion[0]= *x;
  ScilabGCGif.CurClipRegion[1]= *y;
  ScilabGCGif.CurClipRegion[2]= *w;
  ScilabGCGif.CurClipRegion[3]= *h;
  gdSetClipping(GifIm,*x,*y,*x+*w,*y+*h); 
/*  FPRINTF((file,"\n%d %d %d %d setclipzone",(int)*x,(int)*y,(int)*w,(int)*h));*/
}

/** unset clip zone **/

void C2F(unsetclipGif)(integer *v1, integer *v2, integer *v3, integer *v4)
{
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 4 xinit must be called before any action \r\n");
    return;
  }
  ScilabGCGif.ClipRegionSet = 0;
  ScilabGCGif.CurClipRegion[0]= -1;
  ScilabGCGif.CurClipRegion[1]= -1;
  ScilabGCGif.CurClipRegion[2]= 200000;
  ScilabGCGif.CurClipRegion[3]= 200000;
  gdUnsetClipping(GifIm);
/*  FPRINTF((file,"\n%d %d %d %d setclipzone",-1,-1,200000,200000));*/
}

/** Get the boundaries of the current clip zone **/

void C2F(getclipGif)(integer *verbose, integer *x, integer *narg, double *dummy)
{
  x[0] = ScilabGCGif.ClipRegionSet;
  if ( x[0] == 1)
    {
      *narg = 5;
      x[1] =ScilabGCGif.CurClipRegion[0];
      x[2] =ScilabGCGif.CurClipRegion[1];
      x[3] =ScilabGCGif.CurClipRegion[2];
      x[4] =ScilabGCGif.CurClipRegion[3];
    }
  else *narg = 1;
  if (*verbose == 1)
    {
      if (ScilabGCGif.ClipRegionSet == 1)
	sciprint("\nThere's a Clip Region :x:%d,y:%d,w:%d,h:%d\r\n",
		 ScilabGCGif.CurClipRegion[0],
		 ScilabGCGif.CurClipRegion[1],
		 ScilabGCGif.CurClipRegion[2],
		 ScilabGCGif.CurClipRegion[3]);
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

void C2F(setabsourelGif)(integer *num, integer *v2, integer *v3, integer *v4)
{
  if (*num == CoordModeOrigin )
    ScilabGCGif.CurVectorStyle =  CoordModeOrigin;
  else 
    ScilabGCGif.CurVectorStyle =  CoordModePrevious ;
}

/** to get information on absolute or relative mode **/

void C2F(getabsourelGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *narg = 1;
  *num = ScilabGCGif.CurVectorStyle  ;
  if (*verbose == 1) 
    {
      if (ScilabGCGif.CurVectorStyle == CoordModeOrigin)
	Scistring("\nTrace Absolu");
      else 
	Scistring("\nTrace Relatif");
    }
}


void C2F(setalufunctionGif)(char *string)
{    
  integer value;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 5 xinit must be called before any action \r\n");
    return;
  }
  C2F(idfromnameGif)(string,&value);
  if ( value != -1) {
     ScilabGCGif.CurDrawFunction = value;
     gdSetAlu(GifIm, value);
/*     FPRINTF((file,"\n%% %d setalufunction",(int)value)); */
  }
}

/** All the possibilities : Read The X11 manual to get more informations **/

struct alinfo { 
  char *name;
  char id;
  char *info;} AluStrucGif[] =
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

void C2F(idfromnameGif)(char *name1, integer *num)
{integer i;
 *num = -1;
 for ( i =0 ; i < 16;i++)
   if (strcmp(AluStrucGif[i].name,name1)== 0) 
     *num=AluStrucGif[i].id;
 if (*num == -1 ) 
   {
     Scistring("\n Use the following keys :");
     for ( i=0 ; i < 16 ; i++)
       sciprint("\nkey %s -> %s\r\n",AluStrucGif[i].name,
	       AluStrucGif[i].info);
   }
}


void C2F(setalufunction1Gif)(integer *num, integer *v2, integer *v3, integer *v4)
{     
  integer value;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 6 xinit must be called before any action \r\n");
    return;
  }
  value=AluStrucGif[Min(15,Max(0,*num))].id;
  if ( value != -1)
    {
      ScilabGCGif.CurDrawFunction = value;
      gdSetAlu(GifIm, value);
    }
}

/** To get the value of the alufunction **/

void C2F(getalufunctionGif)(integer *verbose, integer *value, integer *narg, double *dummy)
{ 
  *narg =1 ;
  *value = ScilabGCGif.CurDrawFunction ;
   if (*verbose ==1 ) 
     { sciprint("\nThe Alufunction is %s -> <%s>\r\n",
	       AluStrucGif[*value].name,
	       AluStrucGif[*value].info);}
 }

integer GetAluGif(void)
{
return ScilabGCGif.CurDrawFunction;
}
/** to set the thickness of lines : 0 is a possible value **/
/** give the thinest line **/

#define Thick_prec 5

void C2F(setthicknessGif)(integer *value, integer *v2, integer *v3, integer *v4)
{ 
  ScilabGCGif.CurLineWidth =Max(0, *value);
/*  FPRINTF((file,"\n%d Thickness",(int)Max(0,*value*Thick_prec))); */
}

/** to get the thicknes value **/

void C2F(getthicknessGif)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *narg =1 ;
  *value = ScilabGCGif.CurLineWidth ;
  if (*verbose ==1 ) 
    sciprint("\nLine Width:%d\r\n",
	    ScilabGCGif.CurLineWidth ) ;
}
     

/*-------------------------------------------------
\encadre{To set grey level for filing areas.
  from black (*num =0 ) to white 
  you must use the get function to get the id of 
  the white pattern }
----------------------------------------------------*/

void C2F(setpatternGif)(integer *num, integer *v2, integer *v3, integer *v4)
{
 integer i ; 
  if ( ScilabGCGif.CurColorStatus ==1) 
    {
      i= Max(0,Min(*num-1,ScilabGCGif.Numcolors+1));
      ScilabGCGif.CurColor = i ;
      C2F(set_cGif)(i);
    }
  else 
    {
      /* used when printing from color to b&white color after GREYNUMBER 
       are translated to black */
      if ( *num-1 > GREYNUMBER -1 ) 
	i=0;
      else 
	i= Max(0,Min(*num-1,GREYNUMBER-1));
      ScilabGCGif.CurPattern = i;
/*      if (i ==0)
	FPRINTF((file,"\nfillsolid"));
      else 
	FPRINTF((file,"\n%d Setgray",(int)i)); */
    }
}

/** To get the id of the current pattern  **/

void C2F(getpatternGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 

  *narg=1;
  if ( ScilabGCGif.CurColorStatus ==1) 
    {
      *num = ScilabGCGif.CurColor +1 ;
      if (*verbose == 1) 
	sciprint("\n Color : %d\r\n",*num);
    }
  else 
    {
      *num = ScilabGCGif.CurPattern +1 ;
      if (*verbose == 1) 
	sciprint("\n Pattern : %d\r\n",*num);
    }
}

/** To get the id of the last pattern **/

void C2F(getlastGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *num = ScilabGCGif.IDLastPattern +1 ;
  if (*verbose==1) 
    sciprint("\n Id of White Pattern %d\r\n",(int)*num);
  *narg=1;
}

/** To set dash-style : **/
/**  use a table of dashes and set default dashes to **/
/**  one of the possible value. value pointeger **/
/**  to a strictly positive integer **/
#define MAXDASH 6


static integer DashTabGif[MAXDASH][4] = {
  {2,5,2,5}, {7,2,7,2},  {2,2,2,2}, {7,2,2,2},
  {11,3,2,3}, {11,3,5,3}};

/* old version of setdashGif retained for compatibility */
void C2F(set_dash_or_color_Gif)(integer *value, integer *v2, integer *v3, integer *v4)
{
  static integer maxdash = 6, l2=4,l3 ;

  if ( ScilabGCGif.CurColorStatus == 1) 
    {
      int i;
      i= Max(0,Min(*value-1,ScilabGCGif.Numcolors+1));
      ScilabGCGif.CurColor =i;
      C2F(set_cGif)(i);
    }
  else 
    {
      l3 = Max(0,Min(maxdash - 1,*value - 1));
      C2F(setdashstyleGif)(&l3,DashTabGif[l3],&l2);
      ScilabGCGif.CurDashStyle = l3;
    }
}
void C2F(setdashGif)(integer *value, integer *v2, integer *v3, integer *v4)
{
  static integer maxdash = 6, l2=4,l3 ;
  l3 = Max(0,Min(maxdash - 1,*value - 1));
  C2F(setdashstyleGif)(&l3,DashTabGif[l3],&l2);
  ScilabGCGif.CurDashStyle = l3;
}

static int GifLineColor (void)
{
    int i, c = col_index[ScilabGCGif.CurColor];

    if (GifIm == (gdImagePtr)0 ) {
      sciprint(" 7 xinit must be called before any action \r\n");
      return 0;
    }
    if (c < 0) c = 0;
    if (ScilabGCGif.CurDashStyle == 0) return c;
    for (i = 0; i < nGifDashes; i++) {
        if (GifDashes[i] != gdTransparent) GifDashes[i] = c;
    }
    gdImageSetStyle(GifIm, GifDashes, nGifDashes);
    return gdStyled;
}

static int GifPatternColor(int pat)
{
    int c = col_index[pat - 1];
    if (c < 0) c = 0;
    return c;
}

/** To change The Gif-default dash style **/
/** if *value == 0, use a solid line, if *value != 0 **/
/** the dash style is specified by the xx vector of n values **/
/** xx[3]={5,3,7} and *n == 3 means :  5white 3 void 7 white \ldots **/

void C2F(setdashstyleGif)(integer *value, integer *xx, integer *n)
{
  int i, j, cn, c1, c = col_index[ScilabGCGif.CurColor];

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 8 xinit must be called before any action \r\n");
    return;
  }
  if (*value != 0) {
      cn = 0;
      c1 = c;
      for (i = 0; i < *n; i++) {
          for (j = 0; j < xx[i]; j++) {
              GifDashes[cn] = c1;
              cn++;
          }
          if (c1 != gdTransparent) c1 = gdTransparent;
          else                     c1 = c;
      }
      nGifDashes = cn;
      gdImageSetStyle(GifIm, GifDashes, nGifDashes);
  } else {
      nGifDashes = 0;
  }
}
static void C2F(set_dash_and_color_Gif)(integer *value, integer *v2, integer *v3, integer *v4)
{
  C2F(setdashGif)(value, v2, v3, v4);
  C2F(setpatternGif)(value+6, v2, v3, v4);
}

void C2F(set_line_style_Gif)(integer *value, integer *v2, integer *v3, integer *v4)
{
  integer j;
  if (ScilabGCGif.CurColorStatus == 0)
    C2F(setdashGif)(value,PI0,PI0,PI0);
  else {
    j= ScilabGCGif.CurDashStyle + 1;
    C2F(setdashGif)(&j,PI0,PI0,PI0);
    C2F(setpatternGif)(value,PI0,PI0,PI0);
  }
}


/** to get the current dash-style **/
/* old version of getdashGif retained for compatibility */
void C2F(get_dash_or_color_Gif)(integer *verbose, integer *value, integer *narg, double *dummy)
{integer i ;
 *narg =1 ;
 if ( ScilabGCGif.CurColorStatus ==1) 
   {
     *value= ScilabGCGif.CurColor + 1;
     if (*verbose == 1) sciprint("Color %d",(int)*value);
     return;
   }
 *value=ScilabGCGif.CurDashStyle+1;
 if ( *value == 1) 
   { if (*verbose == 1) Scistring("\nLine style = Line Solid");}
 else 
   {
     value[1]=4;
     *narg = value[1]+2;
     for ( i =0 ; i < value[1]; i++) value[i+2]=DashTabGif[*value-1][i];
     if (*verbose ==1 ) 
       {
	 sciprint("\nDash Style %d:<",(int)*value);
	 for ( i =0 ; i < value[1]; i++)
	   sciprint("%d ",(int)value[i+2]);
	 Scistring(">\n");
       }
   }
}

void C2F(getdashGif)(integer *verbose, integer *value, integer *narg, double *dummy)
{integer i ;
 *narg =1 ;
 *value=ScilabGCGif.CurDashStyle+1;
 if ( *value == 1) 
   { if (*verbose == 1) Scistring("\nLine style = Line Solid");}
 else 
   {
     value[1]=4;
     *narg = value[1]+2;
     for ( i =0 ; i < value[1]; i++) value[i+2]=DashTabGif[*value-1][i];
     if (*verbose ==1 ) 
       {
	 sciprint("\nDash Style %d:<",(int)*value);
	 for ( i =0 ; i < value[1]; i++)
	   sciprint("%d ",(int)value[i+2]);
	 Scistring(">\n");
       }
   }
}
void C2F(get_dash_and_color_Gif)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  /*may be improved replacing 6 by narg */
  C2F(getdashGif)(verbose, value, narg,dummy);
  C2F(getpatternGif)(verbose, value+6, narg,dummy);
  *narg = 6;
}

void C2F(usecolorGif)(integer *num, integer *v2, integer *v3, integer *v4)
{
  integer i;
  i =  Min(Max(*num,0),1);
  if ( ScilabGCGif.CurColorStatus != (int)i)
    {
      if (ScilabGCGif.CurColorStatus == 1) 
	{
	  /* je passe de Couleur a n&b */
	  /* remise des couleurs a vide */
	  ScilabGCGif.CurColorStatus = 1;
	  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
	  /* passage en n&b */
	  ScilabGCGif.CurColorStatus = 0;
	  i= ScilabGCGif.CurPattern+1;
	  C2F(setpatternGif)(&i,PI0,PI0,PI0);
	  i= ScilabGCGif.CurDashStyle+1;
	  C2F(setdashGif)(&i,PI0,PI0,PI0);
	  ScilabGCGif.IDLastPattern = GREYNUMBER - 1;
	}
      else 
	{
	  /* je passe en couleur */
	  /* remise a zero des patterns et dash */
	  /* remise des couleurs a vide */
	  ScilabGCGif.CurColorStatus = 0;
	  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
	  C2F(setdashGif)((i=1,&i),PI0,PI0,PI0);
	  /* passage en couleur  */
	  ScilabGCGif.CurColorStatus = 1;
	  i= ScilabGCGif.CurColor+1;
	  C2F(setpatternGif)(&i,PI0,PI0,PI0);
	  ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
	}
    }
}


void C2F(getusecolorGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *num = ScilabGCGif.CurColorStatus;
  if (*verbose == 1) 
    sciprint("\n Use color %d\r\n",(int)*num);
  *narg=1;
}


/*******************************************************
 * Setting the colormap 
 * WARNING 
 * -------
 *   This function is only used when the GIF driver is on 
 *   and xset('colormap',..) is used 
 *   (i.e driver('Gif');xset('colormap',....) 
 *   In the usual case (i.e when someone exports a graphic 
 *   which is displayed in a window) only the graphics 
 *   recorded commands are replayed and xset('colormap') belongs 
 *   to the non-recorded Scilab graphic commands 
 *   
 *   Only the <<current colormap>> of the window is translated 
 *   to GIF when the GIF file is opened 
 *   ( see  if (  CheckColormap(&m) == 1) in FileInt) 
 ******************************************************/
void setcolormapgGif(struct  BCG *Xgc,integer *v1,integer *v2, double *a, integer *v3);/* NG */

void C2F(setgccolormapGif)(struct BCG *Xgc,integer m, double *a, integer *v3)
{
  int i,j,r,g,b,c,ierr,m1,mm;
  double *cmap;
  int *ind,i1;
  int old_white,old_black;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 9 xinit must be called before any action \r\n");
    *v3 = 1;
    return;
  }
  
  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      *v3 = 1;
      return;
    }
  }
  old_white=col_index[Xgc->Numcolors+1];
  old_black=col_index[Xgc->Numcolors];
  /* deallocate old colors*/
  for ( i=0; i < GifIm->colorsTotal; i++) 
    gdImageColorDeallocate(GifIm, i);
  for ( i=0;i < Xgc->Numcolors+2; i++) 
    col_index[i] = -1;

  if (m>gdMaxColors-3) 
  {
    /* reduce the number of colors */
    m1 = gdMaxColors-2;
    sciprint(COLORMAP_WARNING,gdMaxColors-2) ;
    if ( (cmap = (double*) MALLOC(3*m1 * sizeof(double)))== NULL) {
      Scistring("Not enough memory\n");
      *v3 = 1;
      return;
    }
    if ( (ind = (int*) MALLOC(m * sizeof(int)))== NULL) {
      Scistring("Not enough memory\n");
      FREE(cmap);
      *v3 = 1;
      return;
    }

    C2F(nues1)(a,&m,cmap,&m1,ind,&ierr);
    /* create new colormap */
    Xgc->Numcolors = m;
    for ( i=0; i < Xgc->Numcolors; i++) {
      i1 = ind[i] - 1;
      r=(int)(cmap[i1] * 255);
      g=(int)(cmap[i1 + m1] * 255);
      b=(int)(cmap[i1 + 2*m1] * 255);
      if (r==255 && g==255 && b==255) {
	/* move white a little to distinguish it from the background */
	r=254;g=254;b=254; }
      /*c = gdImageColorExact(GifIm, r,g,b);
        if (c == -1)*/
      c = gdImageColorAllocate(GifIm,r,g,b);
      col_index[i] = c;
    }
    FREE(ind);
    FREE(cmap);
  }
  else {
    /* create new colormap */
    Xgc->Numcolors = m;
    for ( i=0; i < Xgc->Numcolors; i++) {
      r=(int)(a[i] * 255);
      g=(int)(a[i + m] * 255);
      b=(int)(a[i + 2*m] * 255);
      if (r==255&&g==255&&b==255) {
	/* move white a little to distinguish it from the background */
	r=254;g=254;b=254; }
      /*c = gdImageColorExact(GifIm, r,g,b);
	if (c == -1)*/
      c = gdImageColorAllocate(GifIm,r,g,b);
      col_index[i] = c;
    }
  }
  /* adding white and black color at the end */
  /*c = gdImageColorExact(GifIm, 0,0,0);
  if (c == -1) */
  c = gdImageColorAllocate(GifIm,0,0,0);
  col_index[m]=c;
  /*c = gdImageColorExact(GifIm, 255,255,255);
  if (c == -1) */
  c = gdImageColorAllocate(GifIm,255,255,255);
  col_index[m+1]=c;
  col_white=c;

  Xgc->IDLastPattern = m - 1;
  Xgc->NumForeground = m;
  Xgc->NumBackground = m + 1;


  C2F(usecolorGif)((i=1,&i) ,PI0,PI0,PI0);
  C2F(setalufunction1Gif)((i=3,&i),PI0,PI0,PI0);
  C2F(setpatternGif)((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);  

  /* next line added to change the pixel color index for pixel painted with 
     the default white and black*/

  for (i=0;i<Xgc->CWindowWidth;i++) {
    for (j=0;j<Xgc->CWindowHeight;j++) {
      c=gdImageGetPixel(GifIm, i, j);
      if(c==old_white)
	gdImageSetPixel(GifIm, i, j,col_index[m+1]);
      else if (c==old_black)
	gdImageSetPixel(GifIm, i, j,col_index[m]);
    }
  }

  /* -------------------------------------------------------- */
  /* store the colormap for the get command too */
  /* F.Leray 04.10.05 */
  FREE(Xgc->Red);
  FREE(Xgc->Green);
  FREE(Xgc->Blue);
  
  /* don't forget black and white */
  mm = m;
  if (!(Xgc->Red = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    return;
  }
  if (!(Xgc->Green = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(Xgc->Red);
    return;
  }
  if (!(Xgc->Blue = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(Xgc->Red);
    FREE(Xgc->Green);
    return;
  }

  for(i=0;i<m;i++){
    Xgc->Red[i] = (float)a[i];
    Xgc->Green[i] = (float)a[i+m];
    Xgc->Blue[i] = (float)a[i+2*m]; 
  }
  /* -------------------------------------------------------- */


  /* next line added by bruno (7 dec 2004) to (badly) correct the bug 1112 */
   /* gdImageFilledRectangle(GifIm, 0, 0, Xgc->CWindowWidth-1, Xgc->CWindowHeight-1, col_white);*/
  /* Note: Fabrice has also added some code in clearwindowgif function but this doesn't */
  /* correct the problem for the old graphic mode (rmk: this added line seems to correct  */
  /* the bug for both mode) */
}

/* add *v3 (OUT) to know if colormap allocation has succeeded: */
/* 0: succeed */
/* 1: failed */
/* NG beg*/

void C2F(setcolormapGif)(integer *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, double *a)
{
  int m;

  *v3 = 0;

  if (*v2 != 3 ||  *v1 < 0 ) {
    Scistring("Colormap must be a m x 3 array \n");
    *v3 = 1;
    return;
  }
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 10 xinit must be called before any action \r\n");
    *v3 = 1;
    return;
  }
  m = *v1;

  C2F(setgccolormapGif)(&ScilabGCGif,m, a, v3);
}

void C2F(getcolormapsizeGif)(integer *v1, integer *v2, integer *v3, double *val)
{
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 10.1 xinit must be called before any action \r\n");
    return;
  }
  
  *v2 = ScilabGCGif.Numcolors;
}

/* F.Leray 22.11.05: get the current colormap */

void C2F(getcolormapGif)(integer *v1, integer *v2, integer *v3, double *val)
{
  int m;
  int i;
  *v3 = 0;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 10.1 xinit must be called before any action \r\n");
    return;
  }
  
  m = ScilabGCGif.Numcolors;

  for (i = 0; i < m; i++) {
    val[i] = (double)ScilabGCGif.Red[i];
    val[i+m] = (double)ScilabGCGif.Green[i];
    val[i+2*m] = (double)ScilabGCGif.Blue[i];
  }
}


/* NG beg */
void setcolormapgGif(struct  BCG *Xgc,integer *m,integer *v2, double *a, integer *v3) /* NG */
{
  C2F(setgccolormapGif)(Xgc,*m, a, v3);
}
/* NG end */

/** 
  Initial (default) colormap 
**/

static void ColorInitGif(void)
{
  int m,i,r,g,b,c;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 11 xinit must be called before any action \r\n");
    return;
  }

  m = DEFAULTNUMCOLORS;
  ScilabGCGif.Numcolors = m;
  for ( i=0; i < ScilabGCGif.Numcolors; i++) {
    r = default_colors[3 * i];
    g = default_colors[3 * i + 1];
    b = default_colors[3 * i + 2];
    if (r==255&&g==255&&b==255) {
      /* move white a little to distinguish it from the background */
      r=254;g=254;b=254; }
    col_index[i] = gdImageColorAllocate(GifIm,r,g,b);
  }
  /* add black and white at the end of the colormap */
  /*c = gdImageColorExact(GifIm, 0,0,0);
    if (c == -1) */
  c = gdImageColorAllocate(GifIm,0,0,0);
  col_index[m]=c;
  /*c = gdImageColorExact(GifIm, 255,255,255);
    if (c == -1) */
  c = gdImageColorAllocate(GifIm,255,255,255);
  col_index[m+1]=c;
  col_white=col_index[m+1];
  ScilabGCGif.NumForeground = m;
  ScilabGCGif.NumBackground = m + 1;
  C2F(setpatternGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0); 
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
    }

}


void C2F(set_cGif)(integer i)
{
  integer j;
  j=Max(Min(i,ScilabGCGif.Numcolors+1),0);
/*  FPRINTF((file,"\n%d Setcolorcolormap",(int)j)); */
}


/** set and get the number of the background or foreground */

void C2F(setbackgroundGif)(integer *num, integer *v2, integer *v3, integer *v4)
{
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.NumBackground = Max(0,Min(*num - 1,ScilabGCGif.Numcolors + 2));
    }
}

void C2F(getbackgroundGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCGif.CurColorStatus == 1 ) 
    {
      *num = ScilabGCGif.NumBackground + 1;
    }
  else 
    {
      *num = 1;
    }
  if (*verbose == 1) 
    sciprint("\n Background : %d\r\n",*num);
}


/** set and get the number of the background or foreground */

void C2F(setforegroundGif)(integer *num, integer *v2, integer *v3, integer *v4)
{
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.NumForeground = Max(0,Min(*num - 1,ScilabGCGif.Numcolors + 1));
    }
}

void C2F(getforegroundGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCGif.CurColorStatus == 1 ) 
    {
      *num = ScilabGCGif.NumForeground + 1;
    }
  else 
    {
      *num = 1; /** the foreground is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Foreground : %d\r\n",*num);
}



/** set and get the number of the hidden3d color */

void C2F(sethidden3dGif)(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      /* e Segre: Max(0,... -> Max(-1,... */
      /* S Mottelet: Max(-1,... -> Max(-2,...  to take into account the value -1 */
      ScilabGCGif.NumHidden3d = Max(-2,Min(*num - 1,ScilabGCGif.Numcolors + 1));
    }
}

void C2F(gethidden3dGif)(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabGCGif.CurColorStatus == 1 ) 
    {
      *num = ScilabGCGif.NumHidden3d + 1;
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

void C2F(semptyGif)(integer *v1, integer *v2, integer *v3, integer *v4)
{
  
}
void C2F(setwwhowGif)(integer *verbose, integer *v2, integer *v3, integer *v4)
{
/*  FPRINTF((file,"\n%% SPLIT HERE")); */
}

void C2F(gemptyGif)(integer *verbose, integer *v2, integer *v3, double *dummy)
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}

#define NUMSETFONC 32

/** Table in lexicographic order **/

struct bgc { char *name ;
	     void  (*setfonc )() ;
	     void (*getfonc )() ;}
  ScilabGCTabGif[] = {
    {"alufunction",C2F(setalufunction1Gif),C2F(getalufunctionGif)},
    {"background",C2F(setbackgroundGif),C2F(getbackgroundGif)},
    {"clipoff",C2F(unsetclipGif),C2F(getclipGif)},
    {"clipping",C2F(setclipGif),C2F(getclipGif)},
    {"cmap_size",C2F(semptyGif),C2F(getcolormapsizeGif)},
    {"color",C2F(setpatternGif),C2F(getpatternGif)},
    {"colormap",C2F(setcolormapGif),C2F(getcolormapGif)},
    {"dashes",C2F(set_dash_or_color_Gif),C2F(get_dash_or_color_Gif)},
    {"default",InitScilabGCGif, C2F(gemptyGif)},
    {"figure",C2F(setscilabFigureGif),C2F(getscilabFigureGif)},/* NG */
    {"font",C2F(xsetfontGif),C2F(xgetfontGif)},
    {"foreground",C2F(setforegroundGif),C2F(getforegroundGif)},
    {"gc",C2F(semptyGif),C2F(getscilabxgcGif)},/* NG */
    {"gccolormap",C2F(setgccolormapGif),C2F(gemptyGif)},/* NG */
    {"hidden3d",C2F(sethidden3dGif),C2F(gethidden3dGif)},
    {"lastpattern",C2F(semptyGif),C2F(getlastGif)},
    {"line mode",C2F(setabsourelGif),C2F(getabsourelGif)},
    {"line style",C2F(setdashGif),C2F(getdashGif)},
    {"mark",C2F(xsetmarkGif),C2F(xgetmarkGif)},
    {"pattern",C2F(setpatternGif),C2F(getpatternGif)},
    {"pixmap",C2F(semptyGif),C2F(gemptyGif)},
    {"thickness",C2F(setthicknessGif),C2F(getthicknessGif)},
    {"use color",C2F(usecolorGif),C2F(getusecolorGif)},
    {"version",C2F(setscilabVersionGif),C2F(getscilabVersionGif)},/* NG */
    {"viewport",C2F(semptyGif),C2F(gemptyGif)},
    {"wdim",C2F(setwindowdimGif),C2F(getwindowdimGif)},
    {"white",C2F(semptyGif),C2F(getlastGif)},
    {"window",C2F(setcurwinGif),C2F(getcurwinGif)},
    {"wpdim",C2F(semptyGif),C2F(gemptyGif)},
    {"wpos",C2F(setwindowposGif),C2F(getwindowposGif)},
    {"wresize",C2F(semptyGif),C2F(gemptyGif)},
    {"wshow",C2F(setwwhowGif),C2F(gemptyGif)},
    {"wwpc",C2F(semptyGif),C2F(gemptyGif)}
 };

#ifdef lint

/* pour forcer linteger a verifier ca */

static  test(str,flag,verbose,x1,x2,x3,x4,x5)
     char str[];
     integer flag ;
     integer  *verbose,*x1,*x2,*x3,*x4,*x5;
{ 
  double *dv;
  C2F(setalufunction1Gif)(x1,x2,x3,x4);C2F(getalufunctionGif)(verbose,x1,x2,dv);
  C2F(setclipGif)(x1,x2,x3,x4);C2F(getclipGif)(verbose,x1,x2,dv);
  C2F(setdashGif)(x1,x2,x3,x4);C2F(getdashGif)(verbose,x1,x2,dv);
  InitScilabGCGif(x1,x2,x3,x4); C2F(gemptyGif)(verbose,x1,x2,dv);
  C2F(xsetfontGif)(x1,x2,x3,x4);C2F(xgetfontGif)(verbose,x1,x2,dv);
  C2F(setabsourelGif)(x1,x2,x3,x4);C2F(getabsourelGif)(verbose,x1,x2,dv);
  C2F(xsetmarkGif)(x1,x2,x3,x4);C2F(xgetmarkGif)(verbose,x1,x2,dv);
  C2F(setpatternGif)(x1,x2,x3,x4);C2F(getpatternGif)(verbose,x1,x2,dv);
  C2F(setthicknessGif)(x1,x2,x3,x4);C2F(getthicknessGif)(verbose,x1,x2,dv);
  C2F(usecolorGif)(x1,x2,x3,x4);C2F(gemptyGif)(verbose,x1,x2,dv);
  C2F(setwindowdimGif)(x1,x2,x3,x4);C2F(getwindowdimGif)(verbose,x1,x2,dv);
  C2F(semptyGif)(x1,x2,x3,x4);C2F(getlastGif)(verbose,x1,x2,dv);
  C2F(setcurwinGif)(x1,x2,x3,x4);C2F(getcurwinGif)(verbose,x1,x2,dv);
  C2F(setwindowposGif)(x1,x2,x3,x4);getwindowposGif(verbose,x1,x2,dv);
}

#endif 


void C2F(scilabgcgetGif)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dv1, double *dv2, double *dv3, double *dv4)
{
  int x6=0;
  C2F(ScilabGCGetorSetGif)(str,(integer)1L,verbose,x1,x2,x3,x4,x5,&x6,dv1);
}

void C2F(scilabgcsetGif)(char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1, double *dv2, double *dv3, double *dv4)
{
 integer verbose ;
 verbose = 0 ;
 C2F(ScilabGCGetorSetGif)(str,(integer)0L,&verbose,x1,x2,x3,x4,x5,x6,dv1);}

void C2F(ScilabGCGetorSetGif)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1)
{ integer i ;
  for (i=0; i < NUMSETFONC ; i++)
     {
       integer j;
       j = strcmp(str,ScilabGCTabGif[i].name);
       if ( j == 0 ) 
	 { if (*verbose == 1)
	     sciprint("\nGetting Info on %s\r\n",str);
	   if (flag == 1)
	     (ScilabGCTabGif[i].getfonc)(verbose,x1,x2,dv1);
	   else 
	     (ScilabGCTabGif[i].setfonc)(x1,x2,x3,x4,x5,x6,dv1);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("\nUnknow GIF operator <%s>\r\n",str);
	       *x1=1;*x2=0;
	       return;
	     }
	 }
     }
  sciprint("\n Unknow GIF operator <%s>\r\n",str);
  *x1=1;*x2=0;
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
void C2F(DispStringAngleGif)(integer *x0, integer *yy0, char *string, double *angle)
{
  int i;
  integer x,y, rect[4];
  double sina ,cosa,l;
  char str1[2];
  integer verbose, Dnarg,Dvalue[10],j;
  verbose =0 ;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 12 xinit must be called before any action \r\n");
    return;
  }

  str1[1]='\0';
  x= *x0;
  y= *yy0;
  sina= sin(*angle * M_PI/180.0);
  cosa= cos(*angle * M_PI/180.0);
  str1[0]=string[0];
  C2F(boundingboxGif)(str1,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);

  C2F(getdashGif)(&verbose,Dvalue,&Dnarg,vdouble);
  C2F(setdashGif)((j=1,&j),PI0,PI0,PI0);
  for ( i = 0 ; i < (int)strlen(string); i++)
    { 
      str1[0]=string[i];
      gdImageString(GifIm, GifFont, x, y - rect[3], (unsigned char *)str1,
		    GifLineColor());
      C2F(boundingboxGif)(str1,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);

      if ( cosa <= 0.0 && i < (int)strlen(string)-1)
	{ char str2[2];
	  /** si le cosinus est negatif le deplacement est a calculer **/
	  /** sur la boite du caractere suivant **/
	  str2[1]='\0';str2[0]=string[i+1];
	  C2F(boundingboxGif)(str2,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      if ( Abs(cosa) >= 1.e-8 )
	{
	  if ( Abs(sina/cosa) <= Abs(((double)rect[3])/((double)rect[2])))
	    l = Abs(rect[2]/cosa);
	  else 
	    l = Abs(rect[3]/sina);
	}
      else 
	l = Abs(rect[3]/sina);
      x +=  (integer)(cosa*l*1.1);
      y +=  (integer)(sina*l*1.1);
    }
  C2F(setdashGif)(Dvalue,PI0,PI0,PI0);
}

void C2F(displaystringGif)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4)
{     
  integer rect[4],x1=0,y1=0;

  integer verbose, Dnarg,Dvalue[10],j;
  verbose =0 ;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 13 xinit must be called before any action \r\n");
    return;
  }
  if ( Abs(*angle) <= 0.1) {
    C2F(boundingboxGif)(string,&x1,&y1,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    C2F(getdashGif)(&verbose,Dvalue,&Dnarg,vdouble);
    C2F(setdashGif)((j=1,&j),PI0,PI0,PI0);
    gdImageString(GifIm, GifFont, *x, *y - rect[3], (unsigned char*) string,
		  GifLineColor());
    C2F(setdashGif)(Dvalue,PI0,PI0,PI0);
  }
  else if ( Abs(*angle + 90) <= 0.1 )   /* added by Bruno */
    {
    C2F(boundingboxGif)(string,&x1,&y1,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    C2F(getdashGif)(&verbose,Dvalue,&Dnarg,vdouble);
    C2F(setdashGif)((j=1,&j),PI0,PI0,PI0);
    /* a voir (peut etre enlever le -rect[2] qui doit centrer en vertical alors
       que ce n'est pas attendu) */
    gdImageStringUp(GifIm, GifFont, *x - rect[3], *y - rect[2], (unsigned char*) string,
		  GifLineColor());
    C2F(setdashGif)(Dvalue,PI0,PI0,PI0);
    }
  else 
    C2F(DispStringAngleGif)(x,y,string,angle);
}


void C2F(boundingboxGif)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  int k,width;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 14 xinit must be called before any action \r\n");
    return;
  }
  width = 0;
  for (k=0;k< (int) strlen(string);k++) 
    width += gdCharWidth(GifFont, string[k]);
  rect[0]= (int)(*x);
  rect[1]= (int)(*y);
  rect[2]= width;
  rect[3]= GifFont->h;
}

/** Draw a single line in current style **/

void C2F(drawlineGif)(integer *xx1, integer *yy1, integer *x2, integer *y2)
{
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 15 xinit must be called before any action \r\n");
    return;
  }
  gdImageThickLine(GifIm, *xx1, *yy1, *x2, *y2, GifLineColor(),
		   Max(1,ScilabGCGif.CurLineWidth));
}

/** Draw a set of segments **/
/** segments are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/

void C2F(drawsegmentsGif)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  int i;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 16 xinit must be called before any action \r\n");
    return;
  }


  /* store the current values */


  C2F(get_dash_and_color_Gif)(&verbose,Dvalue,&Dnarg,vdouble);
  if ((int)  *iflag == 0 )
    {
      /** all segments have the same color or dash style */

      NDvalue= (*style < 1) ? Dvalue[0] : *style;
      C2F(set_dash_or_color_Gif)(&NDvalue,PI0,PI0,PI0);
      for ( i=0 ; i < *n/2 ; i++) {
          gdImageThickLine(GifIm, vx[2*i], vy[2*i],
			   vx[2*i + 1], vy[2*i + 1],
			   GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
      }
    }
  else
    {
      for ( i=0 ; i < *n/2 ; i++) 
	{
	  integer NDvalue;
	  NDvalue = style[i];
	  C2F(setpatternGif)(&NDvalue,PI0,PI0,PI0);
          gdImageThickLine(GifIm, vx[2*i], vy[2*i],
			   vx[2*i + 1], vy[2*i + 1],
			   GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
	}
    }
  C2F(set_dash_and_color_Gif)( Dvalue,PI0,PI0,PI0);
}

/** Draw a set of arrows **/
/** arrows are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/** n is the size of vx and vy **/
/** as is 10*arsize (arsize) the size of the arrow head in pixels **/

void C2F(drawarrowsGif)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer verbose=0,Dnarg,Dvalue[10],NDvalue,i;
  double cos20=cos(20.0*M_PI/180.0);
  double sin20=sin(20.0*M_PI/180.0);
  integer polyx[4],polyy[4]; 

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 17 xinit must be called before any action \r\n");
    return;
  }

  C2F(get_dash_and_color_Gif)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i < *n/2 ; i++)
    { 
      double dx,dy,norm;
      if ( (int) *iflag == 1) 
	NDvalue = style[i];
      else 
	NDvalue=(*style < 1) ?  Dvalue[0] : *style;
      C2F(set_line_style_Gif)(&NDvalue,PI0,PI0,PI0);

     dx=( vx[2*i+1]-vx[2*i]);
      dy=( vy[2*i+1]-vy[2*i]);
      norm = sqrt(dx*dx+dy*dy);
      if ( Abs(norm) >  SMDOUBLE ) 
	{ integer nn=1,p=3;
	  dx=(*as/10.0)*dx/norm;dy=(*as/10.0)*dy/norm;
	  polyx[0]= polyx[3]=vx[2*i+1];/* correction bug 607 (bruno 18 nov 2004) */
	  polyx[1]= inint(polyx[0]  - cos20*dx -sin20*dy );
	  polyx[2]= inint(polyx[0]  - cos20*dx + sin20*dy);
	  polyy[0]= polyy[3]=vy[2*i+1];/* correction bug 607 (bruno 18 nov 2004) */
	  polyy[1]= inint(polyy[0] + sin20*dx -cos20*dy) ;
	  polyy[2]= inint(polyy[0] - sin20*dx - cos20*dy) ;
	  C2F(fillpolylinesGif)("v",polyx,polyy,&NDvalue, &nn,&p,PI0,PD0,PD0,PD0,PD0);
	  }

      gdImageThickLine(GifIm, vx[2*i], vy[2*i],(int)
		       (vx[2*i + 1]-dx*cos20), (int)(vy[2*i + 1]-dy*cos20),
		       GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
 
    }
  C2F(set_dash_and_color_Gif)( Dvalue,PI0,PI0,PI0);
}


/** Draw one rectangle **/

/** Draw or fill a set of rectangle **/
/** rectangles are defined by (vect[i],vect[i+1],vect[i+2],vect[i+3]) **/
/** for i=0 step 4 **/
/** (*n) : number of rectangles **/
/** fillvect[*n] : specify the action (see periX11.c) **/

void C2F(drawrectanglesGif)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  int i,cpat,verb=0,num,cd[10],thick;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 18 xinit must be called before any action \r\n");
    return;
  }
  C2F(getpatternGif)(&verb,&cpat,&num,vdouble);
  C2F(get_dash_and_color_Gif)(&verb,cd,&num,vdouble);
  thick = Max(1,ScilabGCGif.CurLineWidth);
  for (i = 0; i < *n; i++) {
    int x, y, w, h;
    x = vects[4 * i];
    y = vects[4 * i + 1];
    w = vects[4 * i + 2];
    h = vects[4 * i + 3];
    if ( fillvect[i] < 0 ) {
      int dash = - fillvect[i];
      C2F(set_line_style_Gif)(&dash,PI0,PI0,PI0);
      gdImageThickRectangle(GifIm, x, y, x + w, y + h, GifLineColor(),thick);
    }
    else if ( fillvect[i] == 0 ) 
      gdImageThickRectangle(GifIm, x, y, x + w, y + h, GifLineColor(),thick);
    else
      gdImageFilledRectangle(GifIm, x, y, x + w, y + h,
			     GifPatternColor(abs(fillvect[i])));
  }
  C2F(set_dash_and_color_Gif)(cd,PI0,PI0,PI0);
}

void C2F(drawrectangleGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 19 xinit must be called before any action \r\n");
    return;
  }
  gdImageRectangle(GifIm, *x, *y, *x + *width, *y + *height, GifLineColor());
}

/** Draw a filled rectangle **/

void C2F(fillrectangleGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 

  integer cpat,verb=0,num;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 20 xinit must be called before any action \r\n");
    return;
  }
  C2F(getpatternGif)(&verb,&cpat,&num,vdouble);
  gdImageFilledRectangle(GifIm, *x, *y, *x + *width, *y + *height,
                         GifPatternColor(cpat));
}
/** Draw or fill a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** fillvect[*n] : specify the action <?> **/
/** caution angle=degreangle*64          **/

void C2F(fillarcsGif)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,pat;
  int i,i6;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 21 xinit must be called before any action \r\n");
    return;
  }
  /* store the current values */
  C2F(getpatternGif)(&verbose,&pat,&Dnarg,vdouble);
  for ( i=0 ; i < *n ; i++) 
    {
      /** to fix the style */
      C2F(setpatternGif)(&fillvect[i],PI0,PI0,PI0);
      i6=6*i;
      C2F(fillarcGif)(str,&(vects[i6]),&(vects[i6+1]),&(vects[i6+2]),
		      &(vects[i6+3]),&(vects[i6+4]),&(vects[i6+5]) ,dv1, dv2, dv3, dv4);

    }
  C2F(setpatternGif)(&pat,PI0,PI0,PI0);
}

/** Draw a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/

void C2F(drawarcsGif)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  int i,i6;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 22 xinit must be called before any action \r\n");
    return;
  }
  /* store the current values */
  C2F(get_dash_and_color_Gif)(&verbose,Dvalue,&Dnarg,vdouble);
  for ( i=0 ; i < *n ; i++) 
    {
      /** to fix the style */
      NDvalue = style[i];
      C2F(set_line_style_Gif)(&NDvalue,PI0,PI0,PI0);
      i6=6*i;
      C2F(drawarcGif)(str,vects+i6,vects+(i6+1),vects+(i6+2),vects+(i6+3),
		      vects+(i6+4),vects+(i6+5) , dv1, dv2, dv3, dv4);
    }
  C2F(set_dash_and_color_Gif)( Dvalue,PI0,PI0,PI0);
}


/** Draw a single ellipsis or part of it **/
/** caution angle=degreAngle*64          **/

void C2F(drawarcGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer vx[365],vy[365],k,n;
  float alpha,fact= (float) 0.01745329251994330,w,h;
  integer close = 0;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 23 xinit must be called before any action \r\n");
    return;
  }

  w = (float) ((*width)/2.0);
  h = (float) ((*height)/2.0);
  n=Min((*angle2/64),360);
  for (k = 0; k < n; ++k) {
    alpha=((*angle1/64)+k)*fact;
    vx[k] = (integer) (*x + w*(cos(alpha)+1.0));
    vy[k] = (integer) (*y + h*(-sin(alpha)+1.0));}
  
  /* Fix bug 1737 : if n==360 we want a closed circle */
  if(n == 360) close = 1;
  
  C2F(drawpolylineGif)(str, &n, vx, vy, &close, PI0, PI0, dv1, dv2, dv3, dv4);
}

/** Fill a single elipsis or part of it **/
/** with current pattern **/

void C2F(fillarcGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer vx[365],vy[365],k,k0,kmax,n;
  float alpha,fact= (float) 0.01745329251994330,w,h;
  integer close = 1;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 24 xinit must be called before any action \r\n");
    return;
  }
  n=Min((*angle2/64),360);

  w = (float) ((*width)/2.0);
  h = (float) ((*height)/2.0);
  k0 = 0;
  kmax = n-1;

  if (n != 360) {
  vx[0] =  (integer) (*x + w);
  vy[0] =  (integer) (*y + h);
  k0 = 1;
  kmax = n;}

  for (k = k0; k <= kmax; ++k) {
    alpha=((*angle1/64)+k)*fact;
    vx[k] =  (integer) (*x + w*(cos(alpha)+1.0));
    vy[k] =  (integer) (*y + h*(-sin(alpha)+1.0));}
  if (n != 360) {
  n++;
  vx[n] = (integer) (*x + ((*width)/2.0));
  vy[n] = (integer) (*y + ((*height)/2.0)); 
  n++;
  }
  C2F(fillpolylineGif)(str, &n, vx, vy, &close, PI0, PI0, dv1, dv2, dv3, dv4);
 }

/*--------------------------------------------------------------
\encadre{Filling or Drawing Polylines and Polygons}
---------------------------------------------------------------*/

/** Draw a set of *n polylines (each of which have (*p) points) **/
/** with lines or marks **/
/** drawvect[i] >= use a mark for polyline i **/
/** drawvect[i] < 0 use a line style for polyline i **/

void C2F(drawpolylinesGif)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ integer verbose ,symb[2],Mnarg,Dnarg,Dvalue[10],NDvalue,i,j,close;
  verbose =0 ;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 25 xinit must be called before any action \r\n");
    return;
  }
  /* store the current values */
  C2F(xgetmarkGif)(&verbose,symb,&Mnarg,vdouble);
  C2F(get_dash_and_color_Gif)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (drawvect[i] <= 0)
	{ /** on utilise la marque de numero associ\'ee **/
	  NDvalue = - drawvect[i];
	  C2F(setdashGif)((j=1,&j),PI0,PI0,PI0);

	  C2F(xsetmarkGif)(&NDvalue,symb+1,PI0,PI0);
	  
	  C2F(drawpolymarkGif)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  C2F(setdashGif)(Dvalue,PI0,PI0,PI0);
	}
      else
	{/** on utilise un style pointill\'e  **/

	  C2F(set_line_style_Gif)(drawvect+i,PI0,PI0,PI0);
	  close = 0;
	  C2F(drawpolylineGif)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,&close,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  /** back to default values **/
  C2F(set_dash_and_color_Gif)(Dvalue,PI0,PI0,PI0);
  C2F(xsetmarkGif)(symb,symb+1,PI0,PI0);
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

void C2F(fillpolylinesGif)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer n1,i,j,o;

  gdPoint *points;
  integer c,thick;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 26 xinit must be called before any action \r\n");
    return;
  }
  n1 = *p;
  if (fillpolylines_closeflag) n1++;
  points = (gdPoint*) MALLOC(n1 * sizeof(gdPoint));
  if (points == (gdPoint*) NULL) return;
  for (j = 0; j < *n; j++) {
      o = j * (*p);
      for (i = 0; i < *p; i++) {
          points[i].x = vectsx[o + i];
          points[i].y = vectsy[o + i];
      }
      if (fillpolylines_closeflag) {
          points[*p].x = vectsx[o];
          points[*p].y = vectsy[o];
      }
      if (fillvect[j] != 0) {
	gdImageFilledPolygon(GifIm, points, n1,
			     GifPatternColor(abs(fillvect[j])));
      }
      if (fillvect[j] >= 0) {
	c = GifLineColor();
	thick = Max(1,ScilabGCGif.CurLineWidth);
	gdImagePolyLine(GifIm, &(vectsx[o]), &(vectsy[o]),
			*p,c,thick,fillpolylines_closeflag);
      }
  }
  FREE(points);
}

/** Only draw one polygon with current line style **/
/** according to *closeflag : it's a polyline or a polygon **/

void C2F(drawpolylineGif)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer thick,n1;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 27 xinit must be called before any action \r\n");
    return;
  }
  thick = Max(1,ScilabGCGif.CurLineWidth);
  n1 = *n;
  gdImagePolyLine(GifIm,vx,vy,n1,GifLineColor(),thick,*closeflag);

}

/** Fill the polygon **/

void C2F(fillpolylineGif)(char *str, integer *n, integer *vx, integer *vy, integer *closeareaflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer i =1;
  integer cpat,verb=0,num;
  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 28 xinit must be called before any action \r\n");
    return;
  }
  C2F(getpatternGif)(&verb,&cpat,&num,vdouble); 
  /** just fill  ==> cpat < 0 **/
  cpat = -cpat;
  fillpolylines_closeflag = *closeareaflag;
  C2F(fillpolylinesGif)(str,vx,vy,&cpat,&i,n,PI0,PD0,PD0,PD0,PD0);
}

/** Draw a set of  current mark centred at points defined **/
/** by vx and vy (vx[i],vy[i]) **/

void C2F(drawpolymarkGif)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer keepid,keepsize,i=1,sz=ScilabGCGif.CurHardSymbSize;

  keepid =  ScilabGCGif.FontId;
  keepsize= ScilabGCGif.FontSize;
  C2F(xsetfontGif)(&i,&sz,PI0,PI0);
  C2F(displaysymbolsGif)(str,n,vx,vy);
  C2F(xsetfontGif)(&keepid,&keepsize,PI0,PI0);
}

/*-----------------------------------------------------
\encadre{Routine for initialisation}
------------------------------------------------------*/

void C2F(initgraphicGif)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  char string1[256];
  static integer EntryCounter = 0;
  integer  x[2],verbose=0,narg;
  double dummy;
  *v3=0;
  if (EntryCounter >= 1) 
  {
      C2F(xendgraphicGif)();
  }
  strncpy(string1,string,256);

  file=fopen(string1,"wb");
  if (file == 0) 
    {
      /* sciprint("Can't open file %s\n",string1);*/
      *v3=1;
      return;
    }
  x[0]=640;
  x[1]=480;

  if (CheckScilabXgc()) { 
    C2F(getwindowdim)(&verbose, x, &narg,&dummy);
  }
  
  LoadFontsGif();
  GifIm = gdImageCreate(x[0], x[1]);

  /*GifFont = gdFontSmall;*/
  ScilabGCGif.CWindowWidth  = x[0];
  ScilabGCGif.CWindowHeight = x[1];
  FileInitGif();
  ScilabGCGif.CurWindow =EntryCounter;
  EntryCounter =EntryCounter +1;
  
  gdImageFilledRectangle(GifIm, 0, 0, x[0]-1, x[1]-1, col_white); 

}

static void FileInitGif(void)
{
/*   int m,r,g,b,c,i; */
/*   float R,G,B; */
/*   double *bigcmap,*cmap; */
/*   int *ind,m1,ierr,i1; */
  int i;
  integer x[2],verbose,narg;

  verbose = 0; 
  C2F(getwindowdimGif)(&verbose,x,&narg,vdouble);
  ColorInitGif();
  InitScilabGCGif(PI0,PI0,PI0,PI0);
  SetGraphicsVersion(); /* set the graphics version using global versionflag variable */
  


/*   if (  CheckColormap(&m) == 1) { /\* a previously defined colormap *\/ */

/*     /\* deallocate old colors*\/ */
/*     for ( i=0; i < GifIm->colorsTotal; i++)  */
/*       gdImageColorDeallocate(GifIm, i); */
/*     for ( i=0;i < ScilabGCGif.Numcolors+2; i++)  */
/*       col_index[i] = -1; */

/*     if (m>gdMaxColors-3) {/\* reduce the number of colors *\/ */
/*       if ( (bigcmap = (double*) MALLOC(3*m * sizeof(double)))== NULL) { */
/* 	Scistring("Not enough memory\n"); */
/* 	return; */
/*       } */
/*       for ( i=0; i < m; i++) { /\* get the previously defined colormap *\/ */
/* 	get_r(i,&R); */
/* 	get_g(i,&G); */
/* 	get_b(i,&B); */
/*         bigcmap[i] = R; */
/* 	bigcmap[i + m] = G; */
/* 	bigcmap[i + 2 * m] = B; */
/*       } */
/*       m1 = gdMaxColors-2; */
/*       if ( (cmap = (double*) MALLOC(3*m1 * sizeof(double)))== NULL) { */
/* 	Scistring("Not enough memory\n"); */
/* 	FREE(bigcmap); */
/* 	return; */
/*       } */
/*       if ( (ind = (int*) MALLOC(m * sizeof(int)))== NULL) { */
/* 	Scistring("Not enough memory\n"); */
/* 	FREE(bigcmap); */
/* 	FREE(cmap); */
/* 	return; */
/*       } */

/*       C2F(nues1)(bigcmap,&m,cmap,&m1,ind,&ierr); /\* compute new colormap *\/ */
/*       /\* create new colormap *\/ */
/*       ScilabGCGif.Numcolors = m; */
/*       for ( i=0; i < ScilabGCGif.Numcolors; i++) { */
/* 	i1 = ind[i] - 1; */
/* 	r=(int)(cmap[i1] * 255); */
/* 	g=(int)(cmap[i1 + m1] * 255); */
/* 	b=(int)(cmap[i1 + 2 * m1] * 255); */
/* 	if (r==255&&g==255&&b==255) { */
/* 	  /\* move white a little to distinguish it from the background *\/ */
/* 	  r=254;g=254;b=254; } */
/* 	/\*	c = gdImageColorExact(GifIm, r,g,b); */
/* 		if (c == -1)*\/ */
/* 	c = gdImageColorAllocate(GifIm,r,g,b); */
/* 	col_index[i] = c; */
/*       } */
/*       FREE(ind); */
/*       FREE(cmap); */
/*       FREE(bigcmap); */
/*     } */
/*     else { */
/*       /\* create new color map *\/ */
/*       ScilabGCGif.Numcolors = m; */
/*       for ( i=0; i < ScilabGCGif.Numcolors; i++) { */
/* 	get_r(i,&R); */
/* 	get_g(i,&G); */
/* 	get_b(i,&B); */
/* 	r = (int)(R*255); */
/* 	g = (int)(G*255); */
/* 	b = (int)(B*255); */
/* 	/\*c = gdImageColorExact(GifIm, r,g,b); */
/* 	  if (c == -1)*\/ */
/* 	c = gdImageColorAllocate(GifIm,r,g,b); */
/* 	col_index[i] = c; */
/*       } */
/*     } */
/*     /\* add black and white at the end of the colormap *\/ */
/*     /\*c = gdImageColorExact(GifIm, 0,0,0); */
/*       if (c == -1) *\/ */
/*       c = gdImageColorAllocate(GifIm,0,0,0); */
/*     col_index[m]=c; */
/*     /\*c = gdImageColorExact(GifIm, 255,255,255); */
/*       if (c == -1) *\/ */
/*     c = gdImageColorAllocate(GifIm,255,255,255); */
/*     col_index[m+1]=c; */
/*     col_white=col_index[m+1]; */
/*     ScilabGCGif.NumForeground = m; */
/*     ScilabGCGif.NumBackground = m + 1; */
/*   } */



  C2F(setpatternGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0); 
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
    }

}

/*--------------------------------------------------------
\encadre{Initialisation of the graphic context. Used also 
to come back to the default graphic state}
---------------------------------------------------------*/


void InitScilabGCGif(integer *v1, integer *v2, integer *v3, integer *v4)
{ integer i,j,col;
  ScilabGCGif.IDLastPattern = GREYNUMBER-1;
  ScilabGCGif.CurLineWidth=0 ;
  i=1;
  C2F(setthicknessGif)(&i,PI0,PI0,PI0);
  C2F(setalufunctionGif)("GXcopy");
  /** retirer le clipping **/
  i=j= -1;
  C2F(unsetclipGif)(PI0,PI0,PI0,PI0);
  C2F(xsetfontGif)((i=2,&i),(j=1,&j),PI0,PI0);
  C2F(xsetmarkGif)((i=0,&i),(j=0,&j),PI0,PI0);
  /** trac\'e absolu **/
  ScilabGCGif.CurVectorStyle = CoordModeOrigin ;
  /* initialisation des pattern dash par defaut en n&b */
  ScilabGCGif.CurColorStatus =0;
  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
  C2F(setdashGif)((i=1,&i),PI0,PI0,PI0);

  /* initialisation de la couleur par defaut */ 
  ScilabGCGif.Numcolors = DEFAULTNUMCOLORS;
  ScilabGCGif.CurColorStatus = 1 ;
  C2F(setpatternGif)((i=1,&i),PI0,PI0,PI0);
  C2F(setforegroundGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackgroundGif)((i=ScilabGCGif.NumForeground+2,&i),PI0,PI0,PI0);
  C2F(sethidden3dGif)((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic_ */
  getcolordef(&col);
  /** we force CurColorStatus to the opposite value of col 
    to force usecolorGif to perform initialisations 
    **/

  ScilabGCGif.mafigure = (sciPointObj *)NULL;
/*   ScilabGCGif.graphicsversion = versionflag; /\* NG *\/ */
  ScilabGCGif.CurColorStatus = (col == 1) ? 0: 1;
  C2F(usecolorGif)(&col,PI0,PI0,PI0);
  if (col == 1) ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
  strcpy(ScilabGCGif.CurNumberDispFormat,"%-5.2g");
}


static void SetGraphicsVersion()
{  
  ScilabGCGif.graphicsversion = versionflag; /* NG */
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
void C2F(drawaxisGif)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4)
{ integer i;
  double xi,yi,xf,yf;
  double cosal,sinal;

  if (GifIm == (gdImagePtr)0 ) {
    sciprint(" 29 xinit must be called before any action \r\n");
    return;
  }
  cosal= cos( (double)M_PI * (*alpha)/180.0);
  sinal= sin( (double)M_PI * (*alpha)/180.0);
  for (i=0; i <= nsteps[0]*nsteps[1]; i++)
    {
      if (( i % nsteps[0]) != 0)
	{
	  xi = initpoint[0]+i*size[0]*cosal;
	  yi = initpoint[1]+i*size[0]*sinal;
	  xf = xi - ( size[1]*sinal);
	  yf = yi + ( size[1]*cosal);
	  gdImageThickLine(GifIm,inint(xi),inint(yi),inint(xf),inint(yf), 
                      GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
	}
    }
  for (i=0; i <= nsteps[1]; i++)
    { xi = initpoint[0]+i*nsteps[0]*size[0]*cosal;
      yi = initpoint[1]+i*nsteps[0]*size[0]*sinal;
      xf = xi - ( size[1]*size[2]*sinal);
      yf = yi + ( size[1]*size[2]*cosal);
      gdImageThickLine(GifIm,inint(xi),inint(yi),inint(xf),inint(yf), 
                  GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
    }

  xi = initpoint[0]; yi= initpoint[1];
  xf = initpoint[0]+ nsteps[0]*nsteps[1]*size[0]*cosal;
  yf = initpoint[1]+ nsteps[0]*nsteps[1]*size[0]*sinal;
  gdImageThickLine(GifIm, inint(xi),inint(yi),inint(xf),inint(yf),
              GifLineColor(),Max(1,ScilabGCGif.CurLineWidth));
}


/*-----------------------------------------------------
\encadre{Display numbers z[i] at location (x[i],y[i])
  with a slope alpha[i] (see displaystring_), if flag==1
  add a box around the string.
-----------------------------------------------------*/
void C2F(displaynumbersGif)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4)
{ integer i ;
  char buf[20];
  for (i=0 ; i< *n ; i++)
    { 
      sprintf(buf,ScilabGCGif.CurNumberDispFormat,z[i]);
      C2F(displaystringGif)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0) ;
    }
}

/** Global variables to deal with fonts **/

#define FONTNUMBER 11
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10


static char *sizeGif[] = { "08" ,"10","12","14","18","24"};
static int  isizeGif[FONTMAXSIZE] = { 8 ,10,12,14,18,24 };

/* FontsList : stockage des structures des fonts 
   la font i a la taille fsiz se trouve ds 
   FontsList[i][fsiz]->fid
*/
gdFont FontListGif[FONTNUMBER][FONTMAXSIZE];


/* Dans FontInfoTabGif : on se garde des information sur les 
   fonts la fonts i a pour nom fname et ok vaut 1 si 
   elle a ete chargee ds le serveur 
   c'est loadfamilyGif qui se charge de charger une font a diverses 
   taille ds le serveur.
*/

struct FontInfo { integer ok;
		  char fname[100];
		} FontInfoTabGif[FONTNUMBER];

typedef  struct  {
  char *alias;
  char *name;
  }  FontAlias;

/** ce qui suit marche sur 75dpi ou 100dpi **/

static FontAlias fonttab[] ={
  {"courR", "-adobe-courier-medium-r-normal--*-%s0-*-*-m-*-iso8859-1"},
  {"symb", "-adobe-symbol-medium-r-normal--*-%s0-*-*-p-*-adobe-fontspecific"},
  {"timR", "-adobe-times-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"timI", "-adobe-times-medium-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"timB", "-adobe-times-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"timBI", "-adobe-times-bold-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"helvR", "-adobe-helvetica-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"helvO", "-adobe-helvetica-medium-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"helvB", "-adobe-helvetica-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"helvBO","-adobe-helvetica-bold-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {(char *) NULL,( char *) NULL}
};

/** To set the current font id of font and size **/

void C2F(xsetfontGif)(integer *fontid, integer *fontsize, integer *v3, integer *v4)
{ integer i,fsiz;
  i = Min(FONTNUMBER-1,Max(*fontid,0));
  fsiz = Min(FONTMAXSIZE-1,Max(*fontsize,0));
  if ( FontInfoTabGif[i].ok !=1 )
    { 
      if (i != FONTNUMBER-1 )  /* a voir ... */
	{
	  C2F(loadfamilyGif)(fonttab[i].alias,&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
	  sciprint(" The Font Id %d is not affected: use default font (Times)\r\n",(int)i);
	  i = 2; /* celle-ci est préchargée lors de l'init */
	}
    }
  ScilabGCGif.FontId = i;
  ScilabGCGif.FontSize = fsiz;
  GifFont = &(FontListGif[i][fsiz]);
}

/** To get the values id and size of the current font **/

void C2F(xgetfontGif)(integer *verbose, integer *font, integer *nargs, double *dummy)
{
  *nargs=2;
  font[0]= ScilabGCGif.FontId ;
  font[1] =ScilabGCGif.FontSize ;
  if (*verbose == 1) 
    {
      sciprint("\nFontId : %d ",	      ScilabGCGif.FontId );
      sciprint("--> %s at size %s pts\r\n",
	     "GifFont",
	     sizeGif[ScilabGCGif.FontSize]);
    }
}

/** To set the current mark : using the symbol font of adobe **/

void C2F(xsetmarkGif)(integer *number, integer *size, integer *v3, integer *v4)
{ 
  ScilabGCGif.CurHardSymb = Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabGCGif.CurHardSymbSize = Max(Min(FONTMAXSIZE-1,*size),0);
;}

/** To get the current mark id **/

void C2F(xgetmarkGif)(integer *verbose, integer *symb, integer *narg, double *dummy)
{
  *narg =2 ;
  symb[0] = ScilabGCGif.CurHardSymb ;
  symb[1] = ScilabGCGif.CurHardSymbSize ;
  if (*verbose == 1) 
  sciprint("\nMark : %d at size %d pts\r\n",
	  ScilabGCGif.CurHardSymb,
	  isizeGif[ScilabGCGif.CurHardSymbSize]);
}

char symb_listGif[] = {
  /*
     0x2e : . alors que 0xb7 est un o plein trop gros 
     ., +,X,*,diamond(filled),diamond,triangle up,triangle down,trefle,circle*/
  (char)0x2e,(char)0x2b,(char)0xb4,(char)0xc5,(char)0xa8,
  (char)0xe0,(char)0x44,(char)0xd1,(char)0xa7,(char)0x4f};

static void C2F(displaysymbolsGif)(char *str, integer *n, integer *vx, integer *vy)
{
  int col, i, c;
  /*
   if (GifIm == (gdImagePtr)0 ) {
      sciprint("xinit must be called before any action \r\n");
      return 0;
    }
	*/ /* Normally, this part should stand here too no?? F.Leray 26.07.04 */
  col = ( ScilabGCGif.CurColorStatus ==1) ? ScilabGCGif.CurColor : ScilabGCGif.CurPattern ;
  for (i = 0; i < *n; i++) {
      c = Char2Int(symb_listGif[ScilabGCGif.CurHardSymb]);
      gdImageSymb(GifIm, GifFont, vx[i], vy[i], c,GifLineColor());
      /*            sz = isizeGif[ScilabGCGif.CurHardSymbSize];
		    gdImageChar(GifIm, GifFont, vx[i], vy[i]-sz, c,GifLineColor());*/
  }
}

/*-------------------------------------------------------
\encadre{Check if a specified family of font exist in GIF }
-------------------------------------------------------*/

void C2F(loadfamilyGif)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  FILE *ff;
  char fname[200];
  int i,ierr;
  char *SciPath;
  gdFontPtr Font;

  /** test if it is an alias font name **/
  if ( strchr(name,'%') != (char *) NULL)  /* no apriori it is an X11 font name */
    {
      sciprint("only alias font name are supported by gif driver\n");
      return;
    }
  SciPath=getenv("SCI");
  if (SciPath==NULL)
    {
      Scistring("The SCI environment variable is not set\n");
      return;
    }
  fname[0]=0;

  if (FontInfoTabGif[*j].ok == 1) { /* Font number already used */
    if (strcmp(FontInfoTabGif[*j].fname,name)!=0) { /* by a different font */
      /* unload this font */
      FontInfoTabGif[*j].ok = 0;
      for (i=0;i<FONTMAXSIZE;i++) {
	Font = &(FontListGif[*j][i]);
	  if (Font != NULL) FREE(Font->data);
      }
    }
  }
  if (FontInfoTabGif[*j].ok == 0) {
    for (i=0;i<FONTMAXSIZE;i++) {
      sprintf(fname,"%s/imp/giffonts/75dpi/%s%s.bdf",
	      SciPath,name,sizeGif[i]);
      ff=fopen(fname,"r");
      if (ff == 0) 
	{
	  sciprint("Can't open font file %s\n",fname);
	  return;
	}
      ierr=ReadbdfFont(ff,&(FontListGif[*j][i]),fname);
      fclose(ff);
      if  (ierr==1) {
	sciprint("\n Cannot allocate memory for font : %s%s\n",name,sizeGif[i]);
	return;
      }
      if  (ierr==2) {
	sciprint("\n Font not found: %s%s\n",name,sizeGif[i]);
	return;
      }
      

      FontInfoTabGif[*j].ok = 1;
      strcpy(FontInfoTabGif[*j].fname,name);
    }
  }
}

void C2F(queryfamilyGif)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer i ;
  name[0]='\0';
  for (i=0;i<FONTNUMBER;i++) {
    v3[i]=strlen(FontInfoTabGif[i].fname);
    strcat(name,FontInfoTabGif[i].fname);
  }
  *j=FONTNUMBER;
}


/*------------------------END--------------------*/
static void LoadFontsGif(void)
{
  int i;

  /*CourR */
  i = 0;
  C2F(loadfamilyGif)("courR",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 

  /*Symb */
  i = 1;
  C2F(loadfamilyGif)("symb",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 

  /*TimR */
  i = 2;
  C2F(loadfamilyGif)("timR",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
 
  /*timI */
  /* i = 3;
     C2F(loadfamilyGif)("timI",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/

  /*timB */
  /* i = 4;
  C2F(loadfamilyGif)("timB",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/

 /*timBI */
  /* i = 5;
     C2F(loadfamilyGif)("timBI",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);*/

}

/* NG beg */
void C2F(setscilabFigureGif)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure)
{
 figure=(double *)ScilabGCGif.mafigure;
}

void C2F(getscilabFigureGif)(integer *verbose, integer *x,integer *narg, double *figure)
{   
  figure=(double *)ScilabGCGif.mafigure;
}
void C2F(setscilabVersionGif)(integer *vers, integer *v2, integer *v3, integer *v4)
{
  ScilabGCGif.graphicsversion=*vers;
}

void C2F(getscilabVersionGif)(integer *verbose, integer *vers, integer *narg, double *dummy)
{   
  *vers = ScilabGCGif.graphicsversion;
}
void C2F(getscilabxgcGif)(integer *verbose, integer *x,integer *narg, double *dummy)
{   
 double **XGC;
 XGC=(double **)dummy;
 *XGC= (double *)&ScilabGCGif;
}
void C2F(setscilabxgcGif)(integer *v1, integer *v2, integer *v3, integer *v4)
{}
/* NG end */



/* 2 routines used only by a call to xinitfromscreen to perform the colormap selection */
/* directly from the screen */

void C2F(initgraphicfromscreenGif)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  char string1[256];
  static integer EntryCounter = 0;
  integer  x[2],verbose=0,narg;
  double dummy;
  *v3=0;
  if (EntryCounter >= 1) 
  {
      C2F(xendgraphicGif)();
  }
  strncpy(string1,string,256);

  file=fopen(string1,"wb");
  if (file == 0) 
    {
      /* sciprint("Can't open file %s\n",string1);*/
      *v3=1;
      return;
    }
  x[0]=640;
  x[1]=480;

  if (CheckScilabXgc()) { 
    C2F(getwindowdim)(&verbose, x, &narg,&dummy);
  }
  
  LoadFontsGif();
  GifIm = gdImageCreate(x[0], x[1]);

  /*GifFont = gdFontSmall;*/
  ScilabGCGif.CWindowWidth  = x[0];
  ScilabGCGif.CWindowHeight = x[1];
  FileInitFromScreenGif();
  ScilabGCGif.CurWindow =EntryCounter;
  EntryCounter =EntryCounter +1;
  
  gdImageFilledRectangle(GifIm, 0, 0, x[0]-1, x[1]-1, col_white); 

}

static void FileInitFromScreenGif(void)
{
  int m,r,g,b,c,i;
  float R,G,B;
  double *bigcmap,*cmap;
  int *ind,m1,ierr,i1;

  integer x[2],verbose,narg;

  verbose = 0; 
  C2F(getwindowdimGif)(&verbose,x,&narg,vdouble);
  ColorInitGif();
  InitScilabGCGif(PI0,PI0,PI0,PI0);
  SetGraphicsVersion(); /* set the graphics version using global versionflag variable */
  
  if (  CheckColormap(&m) == 1) { /* a previously defined colormap */

    /* deallocate old colors*/
    for ( i=0; i < GifIm->colorsTotal; i++) 
      gdImageColorDeallocate(GifIm, i);
    for ( i=0;i < ScilabGCGif.Numcolors+2; i++) 
      col_index[i] = -1;

    if (m>gdMaxColors-3) {/* reduce the number of colors */
      sciprint(COLORMAP_WARNING,gdMaxColors-2) ;
      if ( (bigcmap = (double*) MALLOC(3*m * sizeof(double)))== NULL) {
	Scistring("Not enough memory\n");
	return;
      }
      for ( i=0; i < m; i++) { /* get the previously defined colormap */
	get_r(i,&R);
	get_g(i,&G);
	get_b(i,&B);
        bigcmap[i] = R;
	bigcmap[i + m] = G;
	bigcmap[i + 2 * m] = B;
      }
      m1 = gdMaxColors-2;
      if ( (cmap = (double*) MALLOC(3*m1 * sizeof(double)))== NULL) {
	Scistring("Not enough memory\n");
	FREE(bigcmap);
	return;
      }
      if ( (ind = (int*) MALLOC(m * sizeof(int)))== NULL) {
	Scistring("Not enough memory\n");
	FREE(bigcmap);
	FREE(cmap);
	return;
      }

      C2F(nues1)(bigcmap,&m,cmap,&m1,ind,&ierr); /* compute new colormap */
      /* create new colormap */
      ScilabGCGif.Numcolors = m;
      for ( i=0; i < ScilabGCGif.Numcolors; i++) {
	i1 = ind[i] - 1;
	r=(int)(cmap[i1] * 255);
	g=(int)(cmap[i1 + m1] * 255);
	b=(int)(cmap[i1 + 2 * m1] * 255);
	if (r==255&&g==255&&b==255) {
	  /* move white a little to distinguish it from the background */
	  r=254;g=254;b=254; }
	/*	c = gdImageColorExact(GifIm, r,g,b);
		if (c == -1)*/
	c = gdImageColorAllocate(GifIm,r,g,b);
	col_index[i] = c;
      }
      FREE(ind);
      FREE(cmap);
      FREE(bigcmap);
    }
    else {
      /* create new color map */
      ScilabGCGif.Numcolors = m;
      for ( i=0; i < ScilabGCGif.Numcolors; i++) {
	get_r(i,&R);
	get_g(i,&G);
	get_b(i,&B);
	r = (int)(R*255);
	g = (int)(G*255);
	b = (int)(B*255);
	/*c = gdImageColorExact(GifIm, r,g,b);
	  if (c == -1)*/
	c = gdImageColorAllocate(GifIm,r,g,b);
	col_index[i] = c;
      }
    }
    /* add black and white at the end of the colormap */
    /*c = gdImageColorExact(GifIm, 0,0,0);
      if (c == -1) */
      c = gdImageColorAllocate(GifIm,0,0,0);
    col_index[m]=c;
    /*c = gdImageColorExact(GifIm, 255,255,255);
      if (c == -1) */
    c = gdImageColorAllocate(GifIm,255,255,255);
    col_index[m+1]=c;
    col_white=col_index[m+1];
    ScilabGCGif.NumForeground = m;
    ScilabGCGif.NumBackground = m + 1;
  }
  C2F(setpatternGif)((i=ScilabGCGif.NumForeground+1,&i),PI0,PI0,PI0); 
  if (ScilabGCGif.CurColorStatus == 1) 
    {
      ScilabGCGif.IDLastPattern = ScilabGCGif.Numcolors - 1;
    }

}
