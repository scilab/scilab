/* C code produced by gperf version 3.0.1 */
/* Command-line: gperf -C -t -k '2,3,4,$' Xcall1.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "Xcall1.gperf"

/*------------------------------------------------------------------------
 *   Graphic library for 2D and 3D plotting 
 *   Copyright (C) 1998-2001 Chancelier Jean-Philippe 
 *   jpc@cereve.enpc.fr 
 *--------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "Math.h"
#include "PloEch.h"

typedef void (f_xcall1) (char *,char *,integer *,integer *,integer *,integer *,integer *,integer *,
			       double *,double *,double *,double *,integer,integer );
typedef void (*func) (char *,char *,integer *,integer *,integer *,integer *,integer *,integer *,
			       double *,double *,double *,double *,integer,integer );
extern int C2F(sciwin) (void);
extern int version_flag();
static void GSciString (int,integer *x,integer *y,char *StrMat,integer *w,integer *h);
static void Myalloc1 (integer **xm,integer n,integer *err);
static void Myalloc (integer **xm,integer **ym, integer n, integer *err);
static void xstringb ( char *string,integer x, integer y, integer w, integer h);

static f_xcall1 xset_1, drawarc_1, fillarcs_1  ,drawarcs_1  ,fillpolyline_1  ,drawarrows_1  ;
static f_xcall1 drawaxis_1  ,cleararea_1  ,xclick_1  ,xclick_any_1  ,xgetmouse_1  ,fillarc_1  ;
static f_xcall1 fillrectangle_1  ,drawpolyline_1  ,fillpolylines_1  ,drawpolymark_1  ,displaynumbers_1  ;
static f_xcall1 drawpolylines_1  ,drawrectangle_1  ,drawrectangles_1  ,drawsegments_1  ,displaystring_1  ;
static f_xcall1 displaystringa_1  ,boundingbox_1  ,xstringb_1  ,displaystringt;
static f_xcall1 xinit_1;

static integer Ivide=0;
static double  Dvide;

#ifndef __STDC__ 
#define const 
#endif 

#line 40 "Xcall1.gperf"
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
in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  static const struct funreplace wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 45 "Xcall1.gperf"
      {"xarea",		fillpolyline_1},
      {""}, {""}, {""},
#line 74 "Xcall1.gperf"
      {"xset",		xset_1},
#line 69 "Xcall1.gperf"
      {"xrect",		drawrectangle_1},
#line 76 "Xcall1.gperf"
      {"xstart",		(func) C2F(dr)},
#line 46 "Xcall1.gperf"
      {"xarrow",		drawarrows_1},
      {""},
#line 43 "Xcall1.gperf"
      {"xarc",		drawarc_1},
#line 60 "Xcall1.gperf"
      {"xinit",		xinit_1},
#line 75 "Xcall1.gperf"
      {"xsetdr",		(func) C2F(dr)},
      {""},
#line 78 "Xcall1.gperf"
      {"xstringa",	displaystringa_1},
#line 81 "Xcall1.gperf"
      {"xstringtt",	displaystringt},
#line 44 "Xcall1.gperf"
      {"xarcs",		drawarcs_1},
#line 70 "Xcall1.gperf"
      {"xrects",		drawrectangles_1},
#line 71 "Xcall1.gperf"
      {"xreplay",	(func) C2F(dr)},
#line 79 "Xcall1.gperf"
      {"xstringb",	xstringb_1},
#line 56 "Xcall1.gperf"
      {"xget",		(func) C2F(dr)},
#line 47 "Xcall1.gperf"
      {"xaxis",		drawaxis_1},
#line 55 "Xcall1.gperf"
      {"xfrect",		fillrectangle_1},
      {""}, {""},
#line 58 "Xcall1.gperf"
      {"xgetmouse",	xgetmouse_1},
#line 53 "Xcall1.gperf"
      {"xfarc",		fillarc_1},
#line 57 "Xcall1.gperf"
      {"xgetdr",		(func) C2F(dr)},
      {""}, {""},
#line 52 "Xcall1.gperf"
      {"xend",		(func) C2F(dr)},
      {""},
#line 54 "Xcall1.gperf"
      {"xfarcs",		fillarcs_1},
      {""}, {""}, {""},
#line 48 "Xcall1.gperf"
      {"xclea",		cleararea_1},
#line 67 "Xcall1.gperf"
      {"xpause",		(func) C2F(dr)},
#line 77 "Xcall1.gperf"
      {"xstring",	displaystring_1},
      {""},
#line 51 "Xcall1.gperf"
      {"xclickany",	xclick_any_1},
#line 72 "Xcall1.gperf"
      {"xsegs",		drawsegments_1},
#line 49 "Xcall1.gperf"
      {"xclear",		(func) C2F(dr)},
#line 73 "Xcall1.gperf"
      {"xselect",	(func) C2F(dr)},
      {""}, {""},
#line 65 "Xcall1.gperf"
      {"xname",		(func) C2F(dr)},
#line 64 "Xcall1.gperf"
      {"xmarks",		drawpolymark_1},
      {""},
#line 80 "Xcall1.gperf"
      {"xstringl",	boundingbox_1},
      {""}, {""},
#line 59 "Xcall1.gperf"
      {"xgfont",		(func) C2F(dr)},
      {""}, {""}, {""}, {""},
#line 62 "Xcall1.gperf"
      {"xlines",		drawpolyline_1},
#line 63 "Xcall1.gperf"
      {"xliness",	fillpolylines_1},
      {""}, {""}, {""},
#line 61 "Xcall1.gperf"
      {"xlfont",		(func) C2F(dr)},
      {""}, {""}, {""}, {""},
#line 68 "Xcall1.gperf"
      {"xpolys",		drawpolylines_1},
      {""}, {""}, {""}, {""},
#line 50 "Xcall1.gperf"
      {"xclick",		xclick_1},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 66 "Xcall1.gperf"
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
#line 82 "Xcall1.gperf"


int C2F(dr1)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
	    integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
	    integer lx0, integer lx1)
{ 
  const struct funreplace *fr;
  fr=in_word_set(x0,strlen(x0));
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
static int Autoclear = 0;

void C2F(setautoclear)(integer *num,integer * v2,integer * v3,integer * v4)
{ 
  Autoclear = Max(0,Min(1,*num));
}

void C2F(getautoclear)(integer * verbose,integer * num,integer * narg,double *dummy)
{ 
  *narg=1;
  *num = Autoclear;
  if (*verbose == 1) 
    sciprint("\n Autoclear : %d\r\n",*num);
}

char *getFPF(void)
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
      if (strcmp(str1,"on")==0 )
	Autoclear = 1;
      else
	Autoclear = 0;
    }
  else if ( strcmp(str,"default")==0)
    {
      Autoclear =0;
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
      *len= strlen(str1);
    }
  else if ( strcmp(str,"auto clear")==0) 
    {
      if ( Autoclear == 1) 
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
  /** Warning : we must not record : wdim,wpos,colormap,window and viewport **/
  if (GetDriver()=='R' &&strcmp(str,"wdim")!=0 && strcmp(str,"wpos")!=0 && strcmp(str,"colormap") !=0
      && strcmp(str,"window") != 0 && strcmp(str,"viewport") != 0 && strcmp(str,"wresize") !=0 
      && strcmp(str,"wpdim") !=0 
      ) 
    StoreXcall1(fname,str,x1,1L,x2,1L,x3,1L,x4,1L,x5,1L,x6,1L,dx1,1L,dx2,1L,dx3,1L,dx4,1L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,angle1,1L,angle2,1L,x,1L,y,1L,width,1L,height,1L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,fillvect,*n,n,1L,&Ivide,1L,&Ivide,1L,&Ivide,1L,vects,6*(*n),dx2,1L,dx3,1L,dx4,1L);
  C2F(dr)(fname,str,xm,fillvect,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,style,*n,n,1L,&Ivide,1L,&Ivide,1L,&Ivide,1L,vects,6*(*n),dx2,1L,dx3,1L,dx4,1L);
  C2F(dr)(fname,str,xm,style,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,n,1L,v1,1L,v2,1L,closeflag,1L,&Ivide,1L,&Ivide,1L,vx,*n,vy,*n,dx3,1L,dx4,1L);
  C2F(dr)(fname,str,n,xm,ym,closeflag,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
    {
      if ( (int) *iflag== 1) 
	StoreXcall1(fname,str,style,*n,iflag,1L,n,1L,v3,1L,&Ivide,1L,&Ivide,1L,vx,*n,vy,*n,as,1L,dx4,1L);
      else 
	StoreXcall1(fname,str,style,1L,iflag,1L,n,1L,v3,1L,&Ivide,1L,&Ivide,1L,vx,*n,vy,*n,as,1L,dx4,1L);
    }
  ias=10*ias;
  C2F(dr)(fname,str,xm,ym,n,&ias,style,iflag,PD0,PD0,PD0,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
    StoreXcall1(fname,str,&Ivide,1L,nsteps,2L,&Ivide,1L,&Ivide,1L,&Ivide,1L,&Ivide,1L,alpha,1L,size,3L,initpoint,2L,&Dvide,1L);

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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,&Ivide,1L,&Ivide,1L,x,1L,y,1L,w,1L,h,1L);
  C2F(dr)(fname,str,&x1,&yy1,&w1,&h1,x7,x8,PD0,PD0,PD0,PD0,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 * click 
 *-----------------------------------------------------------------------------*/
void xclick_1(char *fname, char *str, integer *ibutton, integer *iflag, integer *istr, integer *x5, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,yy1,n=1;
  C2F(dr)(fname,str,ibutton,&x1,&yy1,iflag,istr,x7,PD0,PD0,dx3,dx4,lx0,lx1);
  C2F(echelle2d)(x,y,&x1,&yy1,&n,&n,"i2f",3L);
}
/*-----------------------------------------------------------------------------
 *  click_any
 *-----------------------------------------------------------------------------*/
void xclick_any_1(char *fname, char *str, integer *ibutton, integer *iwin, integer *iflag, integer *x5, integer *x6, integer *x7, double *x, double *y, double *dx3, double *dx4, integer lx0, integer lx1)
{ 
  integer x1,y1,n=1;
  integer verb=0,cur,na;
  C2F(dr)(fname,str,ibutton,&x1,&y1,iwin,iflag,x7,PD0,PD0,PD0,PD0,lx0,lx1);
  if (*ibutton>=0){
    C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","window",iwin,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(echelle2d)(x,y,&x1,&y1,&n,&n,"i2f",3L);
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
  C2F(echelle2d)(x,y,&x1,&yy1,&n,&n,"i2f",3L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,
	angle1,1L,angle2,1L,x,1L,y,1L,width,1L,height,1L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,
				&Ivide,1L,&Ivide,1L,x,1L,y,1L,width,1L,height,1L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,n,1L,v1,1L,v2,1L,closeflag,1L,
				&Ivide,1L,&Ivide,1L,vx,*n,vy,*n,dx3,1L,dx4,1L);
  C2F(dr)(fname,str,n,xm,ym,closeflag,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);
}

/*-----------------------------------------------------------------------------
 *  fillpolylines
 *-----------------------------------------------------------------------------*/

void fillpolylines_1(char *fname, char *str, integer *v1, integer *v2, integer *fillvect, integer *n, integer *p, integer *x8, double *vx, double *vy, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer *xm,*ym,err=0,i;
  Myalloc(&xm,&ym,(*n)*(*p),&err);
  if (err ==  1) return;
  C2F(echelle2d)(vx,vy,xm,ym,n,p,"f2i",3L);

  if (GetDriver()=='R') { 	  
  /* Code modified by polpoth 11/7/2000 (interpolated shading) */
  
     if (*v1==2) 
	StoreXcall1(fname,str,v1,1L,v2,1L,fillvect,(*n)*(*p),
	n,1L,p,1L,&Ivide,1L,vx,(*n)*(*p),vy,(*n)*(*p),dx3,1L,dx4,1L);

     else /* other cases v1==1 or v1==0 */
     
       	StoreXcall1(fname,str,v1,1L,v2,1L,fillvect,*n,
	n,1L,p,1L,&Ivide,1L,vx,(*n)*(*p),vy,(*n)*(*p),dx3,1L,dx4,1L);
  }
  if (*v1==2) {
     for (i=0 ; i< (*n) ;i++) scilab_shade(&xm[(*p)*i],&ym[(*p)*i],&fillvect[(*p)*i],*p,0);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,n,1L,v1,1L,v2,1L,
		&Ivide,1L,&Ivide,1L,&Ivide,1L,vx,(*n),vy,(*n),dx3,1L,dx4,1L);
  C2F(dr)(fname,str,n,xm,ym,x6,x7,x8,PD0,PD0,dx3,dx4,lx0,lx1);

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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,n,1L,flag,1L,x,(*n),y,(*n),z,*n,alpha,*n);
  C2F(dr)(fname,str,xm,ym,PI0,PI0,n,flag,z,alpha,PD0,PD0,lx0,lx1);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,drawvect,*n,n,1L,p,1L,&Ivide,1L,vx,(*n)*(*p),vy,(*n)*(*p),dx3,1L,dx4,1L);
  C2F(dr)(fname,str,xm,ym,drawvect,n,p,x8,PD0,PD0,dx3,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,v2,1L,v3,1L,v4,1L,&Ivide,1L,&Ivide,1L,x,1L,y,1L,w,1L,h,1L);
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
  if (GetDriver()=='R') 
	StoreXcall1(fname,str,v1,1L,fillvect,*n,n,1L,
		&Ivide,1L,&Ivide,1L,&Ivide,1L,vects,4*(*n),dx2,1L,dx3,1L,dx4,1L);
  C2F(dr)(fname,str,xm,fillvect,n,x6,x7,x8,PD0,dx2,dx3,dx4,lx0,lx1);
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
  if (GetDriver()=='R') 
    {
      if ( (int) *iflag== 1) 
	StoreXcall1(fname,str,style,*n,iflag,1L,n,1L,
		    &Ivide,1L,&Ivide,1L,&Ivide,1L,vx,(*n),vy,*n,dx3,1L,dx4,1L);
      else 
	StoreXcall1(fname,str,style,1L,iflag,1L,n,1L,
		    &Ivide,1L,&Ivide,1L,&Ivide,1L,vx,(*n),vy,*n,dx3,1L,dx4,1L);
    }
  C2F(dr)(fname,str,xm,ym,n,style,iflag,x8,PD0,PD0,dx3,dx4,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 *  displaystring
 *-----------------------------------------------------------------------------*/
void displaystring_1(char *fname, char *string, integer *v1, integer *v2, integer *v3, integer *flag, integer *x7, integer *x8, double *x, double *y, double *angle, double *dx4, integer lx0, integer lx1)
{
  integer x1,yy1;
  x1 = XDouble2Pixel(*x);
  yy1 = YDouble2Pixel(*y);
  if (GetDriver()=='R') 
	StoreXcall1(fname,string,v1,1L,v2,1L,v3,1L,flag,1L,&Ivide,1L,&Ivide,1L,x,1L,y,1L,angle,1L,dx4,1L);
  C2F(dr)(fname,string,&x1,&yy1,PI0,flag,x7,x8,angle,PD0,PD0,dx4,lx0,lx1);
}
/*-----------------------------------------------------------------------------
 *  displaystringa
 *-----------------------------------------------------------------------------*/
void displaystringa_1(char *fname, char *string, integer *ipos, integer *v2, integer *v3, integer *v4, integer *x7, integer *x8, double *dx1, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
  if (GetDriver()=='R') 
	StoreXcall1(fname,string,ipos,1L,v2,1L,v3,1L,v4,1L,&Ivide,1L,&Ivide,1L,dx1,1L,dx2,1L,dx3,1L,dx4,1L);
  switch ( *ipos )
    {
    case 1:
      xstringb(string,Cscale.WIRect1[0],Cscale.WIRect1[1],Cscale.WIRect1[2],Cscale.WIRect1[3]/6);
      break;
    case 2:
      xstringb(string,Cscale.WIRect1[0]+Cscale.WIRect1[2],Cscale.WIRect1[1]+Cscale.WIRect1[3],Cscale.WIRect1[2]/6,Cscale.WIRect1[3]/6);
      break;
    case 3:
      xstringb(string,Cscale.WIRect1[0],Cscale.WIRect1[1],Cscale.WIRect1[2]/6,Cscale.WIRect1[3]/12);
      break;
    }
}
/*-----------------------------------------------------------------------------
 * MAJ D.A
 *-----------------------------------------------------------------------------*/
void displaystringt(char *fname,char *string, integer *v1, integer *v2, integer *v3, integer *v4 , integer *x7, integer *x8, double *dx1, double *dx2, double *dx3, double *dx4, integer lx0, integer lx1)
{
  integer x, y, w, h;
  x=*v1; 
  y=*v2; 
  w=*v3; 
  h=*v4;
  xstringb (string,x,y,w,h);
}
/*-----------------------------------------------------------------------------
 * display a set of lines coded with 'line1@line2@.....@'
 *   centred in the rectangle [x,y,w=wide,h=height] 
 *-----------------------------------------------------------------------------*/

static void xstringb (char *string, integer x, integer y, integer w, integer h)
{
  char *loc,*loc1;
  loc= (char *) MALLOC( (strlen(string)+1)*sizeof(char));
  if ( loc != 0)
    {
      integer wmax=0,htot=0,x1=0,yy1=0,rect[4],i;
      strcpy(loc,string);
      loc1=strtok(loc,"@");

	  for(i=0;i<4;i++) rect[i] = 0; /* Init. to 0. for preventing Windows RunTime 'error' in debug mode F.Leray 06.04.04 */

      while ( loc1 != ( char * ) 0) 
	{  
	  C2F(dr)("xstringl",loc1,&x1,&yy1,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if ( rect[2] >= wmax ) wmax=rect[2];
	  htot += (int) (1.2*((double) rect[3]));
	  loc1=strtok((char *) 0,"@");
	}
      x1=x+ (w- wmax)/2;
      yy1=y - h + ( h - htot)/2 + rect[3];
      strcpy(loc,string);
      loc1=strtok(loc,"@");
      while ( loc1 != ( char * ) 0) 
	{  
	  double angle=0.0;
	  integer flag=0;
	  C2F(dr)("xstring",loc1,&x1,&yy1,PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
	  yy1 += (int) (1.2*((double)rect[3]));
	  loc1=strtok((char *) 0,"@");
	}
      FREE(loc);
    }
  else
    {
      Scistring("xstring : No more Place  \n");
    }
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
  if (GetDriver()=='R' && (version_flag() != 0)) 
    StoreXcall1(fname,str,fflag,1L,v2,1L,v3,1L,v4,1L,
		&Ivide,1L,&Ivide,1L,xd,1L,yd,1L,wd,1L,hd,1L);
  x = XDouble2Pixel(*xd);
  y = YDouble2Pixel(*yd);
  C2F(echelle2dl)(wd,hd,&wbox,&hbox,&n,&n,"f2i"); 
  C2F(sciwin)();
  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  size = FONTMAXSIZE;
  w = wbox +1;
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
      *xm= graphic_alloc(6,n,sizeof(integer));
      *ym= graphic_alloc(7,n,sizeof(integer));
      if ( *xm == 0 || *ym == 0 )
	{
	  Scistring("malloc: Running out of memory\n");
	  *err=1;
	}
    }
}

static void Myalloc1(integer **xm, integer n, integer *err)
{
  if ( n != 0) 
    {
      if (( *xm= graphic_alloc(6,n,sizeof(integer)))  == 0  )
	{
	  Scistring("malloc: Running out of memory\n");
	  *err=1;
	}
    }
}













