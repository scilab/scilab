/*-------------BEGIN--------------------------------------------------------
---------------------------------------------------------------------------*/
#ifndef __PERIX11_H__
#define __PERIX11_H__
#include "bcg.h"

 extern void XDroutine( int npts ) ;

 extern void C2F(drawarc)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(fillarcs)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawarcs)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(fillpolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawaxis)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4);
 extern void C2F(cleararea)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(clearwindow)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xclick)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iflag, integer *istr, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xclick_any)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iwin, integer *iflag, integer *istr, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xgetmouse)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xend)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(fillarc)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(viderbuff)();
 extern void C2F(fillrectangle)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(MissileGCget)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(initgraphic)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawpolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawClippedPolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4) ;
 extern void C2F(fillpolylines)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawpolylines)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawpolymark)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(getFontMaxSize)(char *str, integer *sizeMin, integer *sizeMax, integer *v1, integer *v2, integer *v3, integer *v4, double *dx1, double *dx2, double *dx3, double *dx4) ;
 extern void C2F(displaynumbers)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4);
 extern void C2F(drawrectangle)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawrectangles)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawsegments)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xselgraphic)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(MissileGCset)(char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(SetDriver)(char *x0, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6, integer * v7,double * dv1,double * dv2,double * dv3,double * dv4);
 extern void C2F(displaystring)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4);
 extern void C2F(boundingbox)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(drawarrows)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(loadfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(queryfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(xinfo)(char *message, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void C2F(setpopupname)(char *x0, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4, integer lx0); 

void deletePoints( void ) ;

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


void getcolordef( integer * screenc ) ;
void setcolordef( int screenc ) ;

void get_r( int i, float * r ) ;
void get_g( int i, float * g ) ;
void get_b( int i, float * b ) ;

void C2F(getwindowdim)( integer * verbose, integer * x, integer * narg, double * dummy ) ;

int C2F(sedeco)( int * flag ) ;

void CPixmapResize1(void) ;

void CPixmapResize(int x, int y) ;

void SwitchWindow(integer *intnum) ;

int C2F(store_points)(integer n, integer *vx, integer *vy, integer onemore) ;

void C2F(xendgraphic)(void) ;

void SciClick(integer *ibutton, integer *x1, integer *yy1, integer *iflag, int getmouse, int getrelease, int dyn_men, char *str, integer *lstr) ;

void set_c(integer col) ;

int get_pixel(int i) ;

void C2F(drawline)(integer *x1, integer *yy1, integer *x2, integer *y2) ;

void fill_grid_rectangles(integer *x, integer *y, double *z, integer n1, integer n2) ;

void fill_grid_rectangles1(integer *x, integer *y, double *z, integer n1, integer n2) ;

void DeleteWindowToList(integer num) ;

void DeleteSGWin(integer intnum) ;

void wininfo(char *format,...) ;

void C2F(bitmap)(char *string, integer w, integer h) ;

int C2F(CurSymbXOffset)(void) ;

int C2F(CurSymbYOffset)(void) ;

integer first_in(integer n, integer ideb, integer *vx, integer *vy) ;

integer first_out(integer n, integer ideb, integer *vx, integer *vy) ;

/*---------------------END------------------------------*/
#endif /* __PERIX11_H__ */


