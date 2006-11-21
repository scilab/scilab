/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifndef __PERIFIG_H__
#define __PERIFIG_H__

struct BCG  ScilabGCXfig ;
static BOOL ScilabGCXfig_is_initialized = FALSE;


 extern void  C2F(drawarcXfig)();
 extern void  C2F(fillarcsXfig)();
 extern void  C2F(drawarcsXfig)();
 extern void  C2F(fillareaXfig)();
 extern void  C2F(drawaxisXfig)();
 extern void  C2F(clearareaXfig)();
 extern void  C2F(clearwindowXfig)();
 extern void  C2F(xclickXfig)();
 extern void  C2F(xclick_anyXfig)();
 extern void  C2F(xgetmouseXfig)();
 extern void  C2F(xendXfig)();
 extern void  C2F(fillarcXfig)();
 extern void  C2F(viderbuffXfig)();
 extern void  C2F(fillrectangleXfig)();
 extern void  C2F(scilabgcgetXfig)();
 extern void  C2F(initgraphicXfig)();
 extern void  C2F(initgraphicfromscreenXfig)();
 extern void  C2F(drawpolylineXfig)();
 extern void  C2F(drawClippedPolylineXfig)();
 extern void  C2F(drawpolylinesXfig)();
 extern void  C2F(fillpolylinesXfig)();
 extern void  C2F(fillpolylineXfig)();
 extern void  C2F(drawpolymarkXfig)();
 extern void  C2F(getFontMaxSizeXfig)();
 extern void  C2F(displaynumbersXfig)();
 extern void  C2F(drawrectangleXfig)();
 extern void  C2F(drawrectanglesXfig)();
 extern void  C2F(drawsegmentsXfig)();
 extern void  C2F(xselgraphicXfig)();
 extern void  C2F(scilabgcsetXfig)();
 extern void  C2F(SetDriverXfig)();
 extern void  C2F(displaystringXfig)();
 extern void  C2F(boundingboxXfig)();
 extern void  C2F(drawarrowsXfig)(); 
 extern void  C2F(loadfamilyXfig)();
 extern void  C2F(queryfamilyXfig)() ;

void C2F(set_dash_or_color_Xfig)(integer *value, integer *v2, integer *v3, integer *v4);

void C2F(set_dash_and_color_Xfig)(integer *value, integer *v2, integer *v3, integer *v4);

void C2F(set_line_style_Xfig)(integer *value, integer *v2, integer *v3, integer *v4);

void C2F(setgccolormapXfig)(struct BCG *Xgc,integer m, double *a, integer *v3);

void C2F(getcolormapsizeXfig)(integer *v1, integer *v2, integer *v3, double *val);

void C2F(getcolormapXfig)(integer *v1, integer *v2, integer *v3, double *val);

void C2F(get_dash_or_color_Xfig)(integer *verbose, integer *value, integer *narg, double *dummy);

void C2F(get_dash_and_color_Xfig)(integer *verbose, integer *value, integer *narg, double *dummy);

void C2F(boundingboxXfig)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(fillarcsXfig_old)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(drawarcsXfig_old)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);


void C2F(drawClippedPolylineXfig)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4);

static void SetGraphicsVersion(void);

void C2F(queryfamilyXfig)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);

void C2F(getFontMaxSizeXfig)(char *str, integer * sizeMin, integer *sizeMax, integer *v1, integer *v2, integer *v3, integer *v4, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(boundingboxXfigM)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(drawarcXfig_old)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(fillarcXfig_old)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dx1, double *dx2, double *dx3, double *dx4);

void C2F(initgraphicfromscreenXfig)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4);

#endif /*__PERIFIG_H__*/

