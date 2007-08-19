/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#ifndef __PERIWIN_H__
#define __PERIWIN_H__


 extern void C2F(drawarc)();
 extern void C2F(fillarcs)();
 extern void C2F(drawarcs)();
 extern void C2F(fillpolyline)();
 extern void C2F(drawaxis)();
 extern void C2F(cleararea)();
 extern void C2F(clearwindow)();
 extern void C2F(xclick)();
 extern void C2F(xclick_any)();
 extern void C2F(xgetmouse)();
 extern void C2F(xend)();
 extern void C2F(fillarc)();
 extern void C2F(viderbuff)();
 extern void C2F(fillrectangle)();
 extern void C2F(MissileGCget)();
 extern void C2F(initgraphic)();
 extern void C2F(drawpolyline)();
 extern void C2F(drawClippedPolyline)();
 extern void C2F(fillpolylines)();
  extern void C2F(drawpolymark)();
 extern void C2F(setpopupname)();
 extern void C2F(getFontMaxSize)();
  extern void C2F(drawrectangle)();
 extern void C2F(drawrectangles)();
 extern void C2F(drawsegments)();
 extern void C2F(xselgraphic)();
 extern void C2F(MissileGCset)();
 extern void C2F(SetDriver)(char *x0, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6, integer * v7,double * dv1,double * dv2,double * dv3,double * dv4);
 extern void C2F(displaystring)();
 extern void C2F(boundingbox)();
 extern void C2F(drawarrows)();
 extern void C2F(loadfamily)();
 extern void  C2F(queryfamily)();
 extern void C2F(xinfo)();
extern void C2F(drawpolylines)();
extern void C2F(displaynumbers)();
 
/** set and get the number of the background or foreground */
void C2F(setforeground)( integer * num, integer * v2, integer * v3, integer * v4) ;
void C2F(setbackground)( integer * num, integer * v2, integer * v3, integer * v4) ;
void C2F(xsetfont)( integer * fontid, integer * fontsize, integer * v3, integer * v4 ) ;
void C2F(xgetfont)( integer * verbose, integer * font, integer * nargs, double * dummy) ;
void C2F(xsetmark)( integer * number, integer * size, integer * v3, integer * v4) ;
void C2F(xgetmark)( integer * verbose,  integer *symb, integer * narg, double * dummy) ;

 extern void C2F(getwins)( integer * Num, integer Ids[] , integer * flag) ;

 extern void getcolordef( integer * screenc ) ;
 extern void setcolordef( integer   screenc ) ;

extern int SwitchWindow( integer * intnum ) ;

 void C2F(getwins)( integer * Num, integer Ids[] , integer * flag) ;

 void wininfo(char *fmt,...) ;

void deletePoints( void ) ;

/*-------------------------------------------------------------------------------*/
int   MaybeSetWinhdc( void ) ;
void  ReleaseWinHdc( void ) ;

#ifdef _MSC_VER
static int flag_DO; /* F.Leray 16.02.04 flag global pour la fonction recursive DrawObj*/
#endif
/*-------------------------------------------------------------------------------*/

void set_clip_box( integer xxleft ,
                   integer xxright,
                   integer yybot  ,
                   integer yytop  ) ;

void clip_line( integer   x1  ,
                integer   yy1 ,
                integer   x2  ,
                integer   y2  ,
                integer * x1n ,
                integer * yy1n,
                integer * x2n ,
                integer * y2n ,
                integer * flag ) ;


int CheckColormap( int * m ) ;

void get_r( int i, float * r ) ;
void get_g( int i, float * g ) ;
void get_b( int i, float * b ) ;


void C2F(getwindowdim)( integer * verbose, integer * x, integer * narg, double * dummy ) ;

int C2F(sedeco)( int * flag ) ;

int C2F(store_points)( integer n, integer * vx, integer * vy, integer onemore) ;
int C2F(AllocVectorStorage)( void ) ;

/*--------------------------------------------------------------------------------------------*/

void SetWinhdc( void ) ;

void SciMouseCapture( void ) ;
void SciMouseRelease( void ) ;

void DeleteSGWin( integer intnum ) ;

void CPixmapResize1( void ) ;

void SciG_Font_Printer(int scale) ;

/**
 * Utilise le ScilabXgc courant pour reinitialiser le gc XWindow
 * cela est utilis'e quand on change de fenetre graphique
 */

void ResetScilabXgc( void ) ;
void SciG_Font(void)  ;
void wininfo(char *fmt,...) ;

void C2F(DispStringAngle)( integer * x0, integer * yy0, char * string, double * angle) ;

void CPixmapResize1( void ) ;

/*--------------------------------------------------------------------------------------------*/


#endif /* __PERIWIN_H__ */


