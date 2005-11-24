/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 *    SCILAB GIF Output based on GD Library from: http://www.boutell.com/gd
 *   Modelled after the original PostScript Driver in periPos.c
 *   Copyright (C) 1999, Tom Leitner, tom@finwds01.tu-graz.ac.at
 *--------------------------------------------------------------------------*/
#ifndef __PERIGIF_H__
#define __PERIGIF_H__

 extern void  C2F(drawarcGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillarcsGif)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawarcsGif)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillpolylineGif)(char *str, integer *n, integer *vx, integer *vy, integer *closeareaflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawaxisGif)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4);
 extern void  C2F(clearareaGif)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(clearwindowGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xclickGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xclick_anyGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xgetmouseGif)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xendGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillarcGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillrectangleGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(scilabgcgetGif)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(initgraphicGif)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(initgraphicfromscreenGif)(char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *); 
 extern void  C2F(drawpolylineGif)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillpolylinesGif)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawpolylinesGif)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawpolymarkGif)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(displaynumbersGif)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4);
 extern void  C2F(xpauseGif)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawrectangleGif)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawrectanglesGif)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawsegmentsGif)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xselgraphicGif)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(scilabgcsetGif)(char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(SetDriverGif)();
 extern void  C2F(displaystringGif)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4);
 extern void  C2F(boundingboxGif)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawarrowsGif)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dv1, double *dv2, double *dv3, double *dv4); 
 extern void  C2F(loadfamilyGif)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(queryfamilyGif)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
#endif /* __PERIGIF_H__ */


