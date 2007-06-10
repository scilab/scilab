#include "ObjectStructure.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "Xcall1.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"

#include "MALLOC.h" /* MALLOC */

/*------------------------------------------------------------------------
 *   Graphic library for 2D and 3D plotting 
 *   Copyright (C) 1998-2001 Chancelier Jean-Philippe 
 *   jpc@cereve.enpc.fr 
 *--------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "math_graphics.h"
#include "PloEch.h"
#include "sciprint.h"
#include "clipping.h"

typedef void (f_xcall1) (char *,char *,integer *,integer *,integer *,integer *,integer *,integer *,
			       double *,double *,double *,double *,integer,integer );
typedef void (*func) (char *,char *,integer *,integer *,integer *,integer *,integer *,integer *,
			       double *,double *,double *,double *,integer,integer );
extern int C2F(sciwin) (void);
extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);

static void GSciString (int,integer *x,integer *y,char *StrMat,integer *w,integer *h);
static void Myalloc1 (integer **xm,integer n,integer *err);
static void Myalloc (integer **xm,integer **ym, integer n, integer *err);

static f_xcall1 xset_1, drawarc_1, fillarcs_1  ,drawarcs_1  ,fillpolyline_1  ,drawarrows_1  ;
static f_xcall1 drawaxis_1  ,cleararea_1  ,xclick_1  ,xclick_any_1  ,xgetmouse_1  ,fillarc_1  ;
static f_xcall1 fillrectangle_1  ,drawpolyline_1  ,fillpolylines_1  ,drawpolymark_1  ,displaynumbers_1  ;
static f_xcall1 drawpolylines_1  ,drawrectangle_1  ,drawrectangles_1  ,drawsegments_1  ,displaystring_1  ;
static f_xcall1 boundingbox_1  ,xstringb_1 ;
static f_xcall1 xinit_1;


#ifndef __STDC__ 
#define const 
#endif 


#ifdef _MSC_VER
extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);
#endif

const struct funreplace * in_word_set (register const char *str, register unsigned int len);
void xclick_2(char *fname, char *str, integer *ibutton, integer *iflag, integer *istr, integer *x1, integer *yy1, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1);
void xgetmouse2(char *fname, char *str, integer *ibutton, integer *iflag, integer *x1, integer *yy1, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1);



struct funreplace { char *name; f_xcall1 *action; };

#define TOTAL_KEYWORDS 39
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 10
#define MAX_HASH_VALUE 114
/* maximum key range = 105, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115,   0,   5,   5,
       10,   0,  20,  25, 115,   0, 115,  35,  35,  35,
       15,  10,  15, 115,   5,  10,   0,  25, 115,   1,
       15,   0, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
      115, 115, 115, 115, 115, 115
    };
  return len + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct funreplace *
in_word_set (register const char *str, register unsigned int len)
{
  static const struct funreplace wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"xarea",		fillpolyline_1},
      {""}, {""}, {""},
      {"xset",		xset_1},
      {"xrect",		drawrectangle_1},
      {"xstart",		(func) C2F(dr)},
      {"xarrow",		drawarrows_1},
      {""},
      {"xarc",		drawarc_1},
      {"xinit",		xinit_1},
      {"xsetdr",		(func) C2F(dr)},
      {""},
      {""},
      {""},
      {"xarcs",		drawarcs_1},
      {"xrects",		drawrectangles_1},
      {"xreplay",	(func) C2F(dr)},
      {"xstringb",	xstringb_1},
      {"xget",		(func) C2F(dr)},
      {"xaxis",		drawaxis_1},
      {"xfrect",		fillrectangle_1},
      {""}, {""},
      {"xgetmouse",	xgetmouse_1},
      {"xfarc",		fillarc_1},
      {"xgetdr",		(func) C2F(dr)},
      {""}, {""},
      {"xend",		(func) C2F(dr)},
      {""},
      {"xfarcs",		fillarcs_1},
      {""}, {""}, {""},
      {"xclea",		cleararea_1},
      {""},
      {"xstring",	displaystring_1},
      {""},
      {"xclickany",	xclick_any_1},
      {"xsegs",		drawsegments_1},
      {"xclear",		(func) C2F(dr)},
      {"xselect",	(func) C2F(dr)},
      {""}, {""},
      {"xname",		(func) C2F(dr)},
      {"xmarks",		drawpolymark_1},
      {""},
      {"xstringl",	boundingbox_1},
      {""}, {""},
      {"xgfont",		(func) C2F(dr)},
      {""}, {""}, {""}, {""},
      {"xlines",		drawpolyline_1},
      {"xliness",	fillpolylines_1},
      {""}, {""}, {""},
      {"xlfont",		(func) C2F(dr)},
      {""}, {""}, {""}, {""},
      {"xpolys",		drawpolylines_1},
      {""}, {""}, {""}, {""},
      {"xclick",		xclick_1},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
      {"xnum",		displaynumbers_1}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}


int C2F(dr1)( char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
	            integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
	            integer lx0, integer lx1)
{ 
  const struct funreplace *fr;
  fr=in_word_set(x0,(unsigned long)strlen(x0));
  if ( fr != NULL ) 
    {
      (*(fr->action))(x0,x1,x2,x3,x4,x5,x6,x7,dx1,dx2,dx3,dx4,lx0,lx1);
    }
  else 
    {
      sciprint("\n Unknow X operator <%s>\r\n",x0);
    }
  return(0);
}

/**************************************************
 * Global values which are set at this level and 
 * not redirected to each driver
 **************************************************/

static char FPF[32]={'\0'};

char * getFPF(void)
{
  return(FPF);
}

int C2F(xsetg)(char * str,char * str1,integer lx0,integer lx1)
{
  if ( strcmp(str,"fpf") == 0) 
    {
      strncpy(FPF,str1,32);
    }
  else if ( strcmp(str,"auto clear")==0) 
    {
      sciPointObj * subwin = sciGetFirstTypedSelectedSon(sciGetCurrentFigure(), SCI_SUBWIN);
      if (strcmp(str1,"on")==0 )
      {
	sciSetAddPlot( subwin,FALSE);
      }
      else{
	sciSetAddPlot( subwin,TRUE);
      }
    }
  else if ( strcmp(str,"default")==0)
    {
      FPF[0]='\0';
    }
  else 
    {
      sciprint("xset(arg,<string>): Unrecognized arg: %s\r\n",str);
    }
  return 0;
}

int C2F(xgetg)( char * str, char * str1, integer * len,integer  lx0,integer lx1)
{
  if ( strcmp(str,"fpf") == 0) 
  {
    strncpy(str1,FPF,32);
    *len= (integer)strlen(str1);
  }
  else if ( strcmp(str,"auto clear")==0) 
  {
    int autoclear;
    sciPointObj * subwin = sciGetFirstTypedSelectedSon( sciGetCurrentFigure(), SCI_SUBWIN ) ;
    autoclear = !(sciGetAddPlot(subwin));
    if (autoclear == 1) 
    {
      strncpy(str1,"on",2);
      *len=2;
    }
    else 
    {
      strncpy(str1,"off",3);
      *len=3;
    }
  }
  return 0;
}

/*-----------------------------------------------------------------------------
 *  xset_1 
 *-----------------------------------------------------------------------------*/

void xinit_1(char *fname, char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
   C2F(dr)(fname,str,x1,x2,x3,x4,x5,x6,dx1,dx2,dx3,dx4,lx0,lx1);
}


/*-----------------------------------------------------------------------------
 *  xset_1 
 *-----------------------------------------------------------------------------*/

void xset_1(char *fname, char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{

  if (strcmp(str,"clipping")==0)
    {
      /** and clipping is special its args are floats **/
      integer xx1,yy1,w1,h1,n=1;
      C2F(echelle2d)(dx1,dx2,&xx1,&yy1,&n,&n,"f2i",3L);
      C2F(echelle2dl)(dx3,dx4,&w1,&h1,&n,&n,"f2i");
      C2F(dr)(fname,str,&xx1,&yy1,&w1,&h1,x5,x6,dx1,dx2,dx3,dx4,lx0,lx1);
    }
  else if (strcmp(str,"clipping-p")==0) 
    {
      /* clip given in pixel */
      C2F(dr)(fname,"clipping",x1,x2,x3,x4,x5,x6,dx1,dx2,dx3,dx4,lx0,lx1);
    }
  else if (strcmp(str,"clipgrf")==0)
    {
      /** getting clip rect **/
      frame_clip_on();
    }
  else
    {
      C2F(dr)(fname,str,x1,x2,x3,x4,x5,x6,dx1,dx2,dx3,dx4,lx0,lx1);
    }
}

/*-----------------------------------------------------------------------------
 *  drawarc_1
 *-----------------------------------------------------------------------------*/

void drawarc_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *angle1, integer *angle2, double *x, double *y, double *width, double *height, integer lx0, integer lx1)
{ 
  integer x1,yy1,w1,h1,n=1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  C2F(echelle2dl)(width,height,&w1,&h1,&n,&n,"f2i"); 
  C2F(dr)(fname,str,&x1,&yy1,&w1,&h1,angle1,angle2,PD0,PD0,PD0,PD0,lx0,lx1);
}

/*-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------*/

void fillarcs_1(char *fname, char *str, integer *v1, integer *fillvect, integer *n, integer *x6, integer *x7, integer *x8, double *vects, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,err=0,n2;
  Myalloc1(&xm,6*(*n),&err);
  if (err ==  1) return;
  C2F(ellipse2d)(vects,xm,(n2=6*(*n),&n2),"f2i");
  C2F(dr)(fname,str,xm,fillvect,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
  FREE(xm) ;
}

/*-----------------------------------------------------------------------------
 *  
 *-----------------------------------------------------------------------------*/

void drawarcs_1(char *fname, char *str, integer *v1, integer *style, integer *n, integer *x6, integer *x7, integer *x8, double *vects, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,err=0,n2;
  Myalloc1(&xm,6*(*n),&err);
  if (err ==  1) return;
  C2F(ellipse2d)(vects,xm,(n2=6*(*n),&n2),"f2i");
  C2F(dr)(fname,str,xm,style,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
  FREE(xm) ;
}
/*-----------------------------------------------------------------------------
 *  
 *-----------------------------------------------------------------------------*/

void fillpolyline_1(char *fname, char *str, integer *n, integer *v1, integer *v2, integer *closeflag, integer *x7, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,n2=1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,&n2,"f2i",3L);
  C2F(dr)(fname,str,n,xm,ym,closeflag,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}

/*-----------------------------------------------------------------------------
 *  arrows
 *-----------------------------------------------------------------------------*/

void drawarrows_1(char *fname, char *str, integer *style, integer *iflag, integer *n, integer *v3, integer *x7, integer *x8, double *vx, double *vy, double *as, double *dx4, integer lx0, integer lx1)
{ 
  integer *xm,*ym,err=0,n2=1,n1=1,ias,ias1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,&n2,"f2i",3L);
  /* is as < 0 --> not set */
  if ( *as < 0.0 ) 
    {
      int i;
      double Mnorm=0.0;
      for (i=0 ; i < *n/2 ; i++)
	{ 
	  double dx,dy;
	  dx=( vx[2*i+1]-vx[2*i]);
	  dy=( vy[2*i+1]-vy[2*i]);
	  Mnorm += sqrt(dx*dx+dy*dy);
	}
      if ( *n != 0) 
	Mnorm /= (*n/2);
      *as = Mnorm/5.0;
    }
  C2F(echelle2dl)(as,as,&ias,&ias1,&n1,&n1,"f2i"); 
  ias=10*ias;
  C2F(dr)(fname,str,xm,ym,n,&ias,style,iflag,PD0,PD0,PD0,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}

/*-----------------------------------------------------------------------------
 * axis 
 *-----------------------------------------------------------------------------*/

void drawaxis_1(char *fname, char *str, integer *v1, integer *nsteps, integer *v2, integer *v3, integer *x7, integer *x8, double *alpha, double *size, double *initpoint, double *dx4, integer lx0, integer lx1)
{
  integer initpoint1[2],alpha1;
  double size1[3];
  alpha1=inint( *alpha);
  C2F(axis2d)(alpha,initpoint,size,initpoint1,size1);

  C2F(dr)(fname,str,&alpha1,nsteps,PI0,initpoint1,x7,x8,size1,PD0,PD0,PD0,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 *  cleararea
 *-----------------------------------------------------------------------------*/

void cleararea_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *x7, integer *x8, double *x, double *y, double *w, double *h, integer lx0, integer lx1)
{
  integer x1,yy1,w1,h1,n=1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  C2F(echelle2dl)(w,h,&w1,&h1,&n,&n,"f2i"); 
  C2F(dr)(fname,str,&x1,&yy1,&w1,&h1,x7,x8,PD0,PD0,PD0,PD0,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 * click 
 *-----------------------------------------------------------------------------*/
void xclick_1(char *fname, char *str, integer *ibutton, integer *iflag, integer *istr, integer *x5, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,yy1,n=1;
  C2F(dr)(fname,str,ibutton,&x1,&yy1,iflag,istr,x7,PD0,PD0,dx3,dx4,lx0,lx1);
  if (*ibutton != -100) C2F(echelle2d)(x,y,&x1,&yy1,&n,&n,"i2f",3L);
}
/*-----------------------------------------------------------------------------
 * click2  returns also the value in pixels (usefull for 3d zoom)
 *-----------------------------------------------------------------------------*/
void xclick_2(char *fname, char *str, integer *ibutton, integer *iflag, integer *istr, integer *x1, integer *yy1, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer n=1;
  C2F(dr)(fname,str,ibutton,x1,yy1,iflag,istr,x7,PD0,PD0,dx3,dx4,lx0,lx1);
  if (*ibutton != -100) C2F(echelle2d)(x,y,x1,yy1,&n,&n,"i2f",3L);
}
/*-----------------------------------------------------------------------------
 *  click_any
 *-----------------------------------------------------------------------------*/
void xclick_any_1(char *fname, char *str, integer *ibutton, integer *iwin, integer *iflag, integer *x5, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,y1_,n=1;
  integer verb=0,cur,na;
  C2F(dr)(fname,str,ibutton,&x1,&y1_,iwin,iflag,x7,PD0,PD0,PD0,PD0,lx0,lx1);
  if (*ibutton>=0){
    C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","window",iwin,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(echelle2d)(x,y,&x1,&y1_,&n,&n,"i2f",3L);
    C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
}

/*-----------------------------------------------------------------------------
 *   xgetmouse
 *-----------------------------------------------------------------------------*/
void xgetmouse_1(char *fname, char *str, integer *ibutton, integer *iflag, integer *v2, integer *x5, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,yy1,n=1;
  C2F(dr)(fname,str,ibutton,&x1,&yy1,iflag,x6,x7,PD0,PD0,dx3,dx4,lx0,lx1);
  if (*ibutton != -100) C2F(echelle2d)(x,y,&x1,&yy1,&n,&n,"i2f",3L);
}

/*-----------------------------------------------------------------------------
 *   xgetmouse2 returns also the value in pixels (usefull for 3d zoom)
 *-----------------------------------------------------------------------------*/
void xgetmouse2(char *fname, char *str, integer *ibutton, integer *iflag, integer *x1, integer *yy1, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer n=1;
  C2F(dr)(fname,str,ibutton,x1,yy1,iflag,x6,x7,PD0,PD0,dx3,dx4,lx0,lx1);
  
  if (*ibutton != -100) C2F(echelle2d)(x,y,x1,yy1,&n,&n,"i2f",3L);
}

/*-----------------------------------------------------------------------------
 *   fillarc
 *-----------------------------------------------------------------------------*/

void fillarc_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *angle1, integer *angle2, double *x, double *y, double *width, double *height, integer lx0, integer lx1)
{ 
  integer x1,yy1,w1,h1,n=1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  C2F(echelle2dl)(width,height,&w1,&h1,&n,&n,"f2i"); 
  C2F(dr)(fname,str,&x1,&yy1,&w1,&h1,angle1,angle2,PD0,PD0,PD0,PD0,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 *  fillrectangle
 *-----------------------------------------------------------------------------*/

void fillrectangle_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *x7, integer *x8, double *x, double *y, double *width, double *height, integer lx0, integer lx1)
{ 
  integer x1,yy1,w1,h1,n=1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  C2F(echelle2dl)(width,height,&w1,&h1,&n,&n,"f2i"); 
  C2F(dr)(fname,str,&x1,&yy1,&w1,&h1,x7,x8,PD0,PD0,PD0,PD0,lx0,lx1);
}


/*-----------------------------------------------------------------------------
 *  drawpolyline
 *-----------------------------------------------------------------------------*/
void drawpolyline_1(char *fname, char *str, integer *n, integer *v1, integer *v2, integer *closeflag, integer *x7, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,n2=1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,&n2,"f2i",3L);
  C2F(dr)(fname,str,n,xm,ym,closeflag,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}

/*-----------------------------------------------------------------------------
 *  fillpolylines
 *-----------------------------------------------------------------------------*/

void fillpolylines_1(char *fname, char *str, integer *v1, integer *v2, integer *fillvect, integer *n, integer *p, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,i,j;
  integer *px,*py;
  Myalloc(&xm,&ym,(*n)*(*p),&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,p,"f2i",3L);


  if (*v1==2) {
    px=(integer *)MALLOC(((*p)+1)*sizeof(integer));
    py=(integer *)MALLOC(((*p)+1)*sizeof(integer));
	if (px==NULL || py==NULL) {
		sciprint("fillpolylines_1 : malloc No more Place\n");
		return;
	}    
    for (i=0 ; i< (*n) ;i++) {
        for (j=0; j<(*p); j++) {
        	px[j]=xm[(*p)*i+j];
        	py[j]=ym[(*p)*i+j];
		}
        px[*p]=px[0];     	         
        py[*p]=py[0];     	 
        scilab_shade(px,py,&fillvect[(*p)*i],*p,(fillvect[(*p)*i]));
     }
     FREE(px);
     FREE(py);
  }
  else C2F(dr)(fname,str,xm,ym,fillvect,n,p,x8,PD0,PD0,dx3,dx4,lx0,lx1);

  /* end of code modified by polpoth 11/7/2000 */


}
/*-----------------------------------------------------------------------------
 *  drawpolymark
 *-----------------------------------------------------------------------------*/
void drawpolymark_1(char *fname, char *str, integer *n, integer *v1, integer *v2, integer *x6, integer *x7, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,n2=1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,&n2,"f2i",3L);
  C2F(dr)(fname,str,n,xm,ym,x6,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}

/*-----------------------------------------------------------------------------
 *  displaynumbers
 *-----------------------------------------------------------------------------*/

void displaynumbers_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *n, integer *flag, double *x, double *y, double *z, double *alpha, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,n2=1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(x,y,xm,ym,n,&n2,"f2i",3L);
  C2F(dr)(fname,str,xm,ym,PI0,PI0,n,flag,z,alpha,PD0,PD0,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}

/*-----------------------------------------------------------------------------
 *   drawpolylines
 *-----------------------------------------------------------------------------*/

void drawpolylines_1(char *fname, char *str, integer *v1, integer *v2, integer *drawvect, integer *n, integer *p, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0;
  Myalloc(&xm,&ym,(*n)*(*p),&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,p,"f2i",3L);
  C2F(dr)(fname,str,xm,ym,drawvect,n,p,x8,PD0,PD0,dx3,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}
/*-----------------------------------------------------------------------------
 *   drawrectangle
 *-----------------------------------------------------------------------------*/
void drawrectangle_1(char *fname, char *str, integer *v1, integer *v2, integer *v3, integer *v4, integer *x7, integer *x8, double *x, double *y, double *w, double *h, integer lx0, integer lx1)
{
  integer xm[4],n2=4;
  double vect[4];
  vect[0]=*x;vect[1]=*y;vect[2]=*w;vect[3]=*h;
  C2F(rect2d)(vect,xm,&n2,"f2i");
  C2F(dr)(fname,str,xm,xm+1,xm+2,xm+3,x7,x8,PD0,PD0,PD0,PD0,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 *   drawrectangles
 *-----------------------------------------------------------------------------*/
void drawrectangles_1(char *fname, char *str, integer *v1, integer *fillvect, integer *n, integer *x6, integer *x7, integer *x8, double *vects, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,err=0,n2;
  Myalloc1(&xm,4*(*n),&err);
  if (err ==  1) return;
  C2F(rect2d)(vects,xm,(n2=4*(*n),&n2),"f2i");
  C2F(dr)(fname,str,xm,fillvect,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
  FREE(xm) ;
}
/*-----------------------------------------------------------------------------
 *  drawsegments
 *-----------------------------------------------------------------------------*/
void drawsegments_1(char *fname, char *str, integer *style, integer *iflag, integer *n, integer *x6, integer *x7, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer *xm,*ym,err=0,n2=1;
  Myalloc(&xm,&ym,*n,&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,&n2,"f2i",3L);
  C2F(dr)(fname,str,xm,ym,n,style,iflag,x8,PD0,PD0,dx3,dx4,lx0,lx1);
  FREE(xm) ;
  FREE(ym) ;
}
/*-----------------------------------------------------------------------------
 *  displaystring
 *-----------------------------------------------------------------------------*/
void displaystring_1(char *fname, char *string, integer *v1, integer *v2, integer *v3, integer *flag, integer *x7, integer *x8, double *x, double *y, double *angle, double *dx4, integer lx0, integer lx1)
{
  integer x1,yy1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  C2F(dr)(fname,string,&x1,&yy1,PI0,flag,x7,x8,angle,PD0,PD0,dx4,lx0,lx1);
}

/*-----------------------------------------------------------------------------
 *  boundingbox_1
 *  To get the bounding rectangle of a string
 *-----------------------------------------------------------------------------*/

void boundingbox_1(char *fname, char *string, integer *v1, integer *v2, integer *v3, integer *x6, integer *x7, integer *x8, double *x, double *y, double *rect, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,yy1,n=1,rect1[4];

  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);

  C2F(dr)(fname,string,&x1,&yy1,rect1,x6,x7,x8,PD0,PD0,PD0,dx4,lx0,lx1);
  

  C2F(echelle2d)(rect,rect+1,rect1,rect1+1,&n,&n,"i2f",3L);
  C2F(echelle2dl)(rect+2,rect+3,rect1+2,rect1+3,&n,&n,"i2f");

}

/*-----------------------------------------------------------------------------
 * a string in a bounded box : with font size change to fit into the 
 * specified box (only works with driver which properly estimate string sizes)
 *-----------------------------------------------------------------------------*/

#define FONTMAXSIZE 6

void xstringb_1(char *fname, char *str, integer *fflag, integer *v2, integer *v3, integer *v4, integer *x7, integer *x8, double *xd, double *yd, double *wd, double *hd, integer lx0, integer lx1)
{
  integer x,y,w,h,wbox,hbox,size,n=1;
  integer fontid[2],narg,verbose=0;
  x = XDouble2Pixel(*xd);
  y = YDouble2Pixel(*yd);
  C2F(echelle2dl)(wd,hd,&wbox,&hbox,&n,&n,"f2i"); 
  C2F(sciwin)();
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  size = FONTMAXSIZE;
  w = wbox + 1;
  h = hbox + 1;

  if ( *fflag == 1 ) 
    {
      while ( (w > wbox || h > hbox) && size >=0  ) 
	{
	  size--;
	  C2F(dr)("xset","font",fontid,&size,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  GSciString(0,&x,&y,str,&w,&h);
	}
    }
  else 
    GSciString(0,&x,&y,str,&w,&h);
  x = (int) (x +  (wbox - w)/2.0);
  y = (int) (y -  (hbox - h)/2.0); 
  GSciString(1,&x,&y,str,&w,&h);
  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}


/**********************************
 * StrMat = 'xxxxZxxxxZxxx....' Z = \n 
 * find the enclosing rectangle for drawing 
 * the string StrMat 
 * and the string is Drawn if Dflag ==1 ;
 **********************************/

void GSciString(int Dflag, integer *x, integer *y, char *StrMat, integer *w, integer *h)
{
  char *p = StrMat,*p1,*p2,*plast;
  integer yi=*y;
  integer wc =0;
  char name[4];
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  p1 = plast = p+ strlen(p);
  while (1) 
    {
      integer logrect[4];
      double angle=0.0;
      /* integer flag=1; SS 10/1/03*/
      integer flag=0;
      p2 =p1 ; *p1 = '\0';
      while ( p1 != p && *p1 != '\n' ) 
	p1--;
      if ( Dflag == 1) 
	C2F(dr)("xstring",( p1 == p ) ? p1 : p1 +1,
		x,&yi,PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstringl",
	      ( p1 == p ) ? p1 : p1 +1,
	      x,&yi,logrect,PI0,PI0,PI0,
	      PD0,PD0,PD0,PD0,0L,0L);	
      if ( p2 != plast) 	*p2 = '\n';
      wc = Max( wc , logrect[2]);
      if ( p == p1 ) 
	{
	  yi=yi- logrect[3];
	  break;
	}	
      else 
	{
	  yi=(int) (yi-1.2*logrect[3]);
	}
    }
  *w = wc ;
  *h = *y - yi;
}

/*-----------------------------------------------------------------------------
 * Utilities : Allocation 
 *-----------------------------------------------------------------------------*/

static void Myalloc(integer **xm, integer **ym, integer n, integer *err)
{
  if ( n != 0) 
    {
      *xm= MALLOC( n * sizeof(integer) ) ;
      *ym= MALLOC( n * sizeof(integer) ) ;
      if ( *xm == 0 || *ym == 0 )
	{
	  sciprint("malloc: Running out of memory\n");
	  *err=1;
	}
    }
}

static void Myalloc1(integer **xm, integer n, integer *err)
{
  if ( n != 0) 
    {
      if (( *xm= MALLOC( n * sizeof(integer)))  == 0  )
	{
	  sciprint("malloc: Running out of memory\n");
	  *err=1;
	}
    }
}













