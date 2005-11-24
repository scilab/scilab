/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifndef __PERIPOS_H__
#define __PERIPOS_H__

 extern void  C2F(drawarcPos)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillarcsPos)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawarcsPos)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillpolylinePos)(char *str, integer *n, integer *vx, integer *vy, integer *closeareaflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawaxisPos)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4);
 extern void  C2F(clearareaPos)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(clearwindowPos)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xclickPos)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xclick_anyPos)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xgetmousePos)(char *str, integer *ibutton, integer *xx1, integer *yy1, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xendPos)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillarcPos)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillrectanglePos)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(scilabgcgetPos)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(initgraphicPos)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(initgraphicfromscreenPos)(char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *); 
 extern void  C2F(drawpolylinePos)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(fillpolylinesPos)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawpolylinesPos)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawpolymarkPos)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(displaynumbersPos)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4);
 extern void  C2F(xpausePos)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawrectanglePos)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawrectanglesPos)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawsegmentsPos)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(xselgraphicPos)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(scilabgcsetPos)(char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(SetDriverPos)();
 extern void  C2F(displaystringPos)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4);
 extern void  C2F(boundingboxPos)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(drawarrowsPos)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dv1, double *dv2, double *dv3, double *dv4); 
 extern void  C2F(loadfamilyPos)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
 extern void  C2F(queryfamilyPos)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
/*----------------------------------END---------------------------*/

#endif /*__PERIPOS_H__*/

