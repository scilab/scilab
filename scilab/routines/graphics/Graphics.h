/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#ifndef SCIG_PROTO
#define SCIG_PROTO

#include "../machine.h"

/* Other functions */

extern void sciprint __PARAMS((char *fmt,...));
extern void sciprint_nd __PARAMS((char *fmt,...));
extern void Scistring __PARAMS((char *str));

/*Actions.c */

typedef int (*Scig_handler) __PARAMS((int));
extern Scig_handler set_scig_handler __PARAMS(( Scig_handler f));
extern Scig_handler scig_handler;

extern int C2F(xg2psofig) __PARAMS((char *,integer *,integer *,integer *,char *,long int,long int)); 
extern int scig_2dzoom __PARAMS((integer )); 
extern void scig_unzoom __PARAMS((integer )); 
extern void scig_replay __PARAMS((integer )); 
extern void scig_expose __PARAMS((integer )); 
extern int scig_3drot __PARAMS((integer )); 
extern void scig_sel __PARAMS((integer )); 
extern void scig_raise __PARAMS((integer )); 
extern void scig_loadsg __PARAMS((int ,char *)); 
extern void scig_resize __PARAMS((integer )); 
extern void scig_resize_pixmap __PARAMS((integer )); 
extern void scig_erase __PARAMS((integer )); 
extern int scig_tops __PARAMS((integer ,integer colored,char *bufname,char *river)); 

/* Alloc.c */

void *graphic_alloc __PARAMS((int indice,int n,unsigned int size));
void graphic_alloc_info __PARAMS((void));
void graphic_alloc_free __PARAMS((void));

/* Axes.c */

extern void Sci_Axis __PARAMS((char,char,double *,int *,double *,int*,char *str[],int subtics,char *format,
			      int fontsize,int textcolor,int fontstyle,int ticscolor,char logflag,int seg_flag , int axisbuild_flag));
extern void sci_axis __PARAMS((char,char,double *,int *,double *,int*,char *str[],int subtics,char *format,
			      int fontsize,int textcolor,int fontstyle,int ticscolor,char logflag,int seg_flag));
extern void axis_draw  __PARAMS(( char strflag[]));
extern void axis_draw2  __PARAMS(( char strflag[]));
extern void DrawAxesBackground __PARAMS((void));

extern void ChoixFormatE  __PARAMS((char *fmt,double xmin,double xmax,double xpas)); 
extern void ChoixFormatE1  __PARAMS((char *fmt,double *x,integer nx)); 
extern void C2F(aplot)  __PARAMS((integer *,double *,double *,double *,double *,integer *,integer *,char *)); 

/* Champ.c */

extern int C2F(champ)  __PARAMS((double *,double *,double *,double *,integer *,integer *,char *,double *,double *,integer)); 
extern int C2F(champ1)  __PARAMS((double *,double *,double *,double *,integer *,integer *,char *,double *,double *,integer)); 
extern void champg  __PARAMS((char *,integer ,double *,double *,double *,double *,integer *,integer *,char *,double *,double *,integer)); 
	
	/* Contour.c */

extern int C2F(getconts) __PARAMS((double **x,double **y,integer *mm,integer *n));
extern int C2F(contour)  __PARAMS((double *,double *,double *,integer *,integer *,integer *,integer *,double *,double *,double *,char *,integer *,double *,double *,integer)); 
extern int C2F(contour2)  __PARAMS((double *,double *,double *,integer *,integer *,integer *,integer *,double *,integer *,char *,char *,double *,integer *,integer,integer)); 
extern int C2F(contourif)  __PARAMS((double *,double *,double *,integer *,integer *,integer *,integer *,double *,integer *)); 

	/* FeC.c */

extern int C2F(fec)  __PARAMS((double *,double *,double *triangles,double *func,integer *Nnode,integer *Ntr,char *,char *,double *,integer *, double *, integer *, integer *, integer with_mesh, integer flagNax, integer,integer)); 

/* extern int C2F(fec)  __PARAMS((double *,double *,double *triangles,double *func,integer *Nnode,integer *Ntr,char *,char *,double *,integer *,integer,integer));  */


/* Gray.c */

extern int C2F(xgray)  __PARAMS((double *,double *,double *,integer *,integer *,char *,double *,integer *,integer flagNax,long int l1)); 

extern int C2F(xgray1) __PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint,integer flagNax,long int l1));

extern int C2F(xgray2) __PARAMS((double *z,integer *n1,integer *n2,double *xrect));


/* Math.c */

extern double Mini  __PARAMS((double *vect,integer));
extern double Maxi  __PARAMS((double *vect,integer));

	/* Plo2d.c */

extern int C2F(xgrid)  __PARAMS((integer *)); 
extern void AxisDraw  __PARAMS((double *,integer *,integer *,integer *,integer *,double,double,double,double,char *,char *)); 
extern void FrameBounds  __PARAMS((char *,double *,double *,integer *,integer *,integer *,char *,double *,double *,integer *,integer *)); 
extern void Legends  __PARAMS(( integer *,integer *,char *)); 
extern int C2F(plot2d)  __PARAMS((double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *,integer,integer)); 

/* Plo2d1.c */

extern int CheckxfParam  __PARAMS((char *)); 
extern int C2F(plot2d1)  __PARAMS((char *,double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *,integer,integer,integer)); 

	/* Plo2d2.c */

extern int C2F(plot2d2)  __PARAMS((char *,double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *,integer,integer,integer)); 

/* Plo2d3.c */

extern int C2F(plot2d3)  __PARAMS((char *,double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *,integer,integer,integer )); 

/* Plo2d4.c */

extern int C2F(plot2d4)  __PARAMS((char *,double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *,integer,integer,integer)); 

/* Plo2dEch.c */

extern void update_frame_bounds  __PARAMS((int cflag,char *xf,double *x,double *y,integer *n1,integer *n2,integer *aaint,char *strflag,double *brect));

void frame_clip_on  __PARAMS((void));
void frame_clip_off  __PARAMS((void));
extern int C2F(xechelle2d) __PARAMS((double x[],integer x1[],integer *n1,char dir[],integer lstr));
extern int C2F(yechelle2d) __PARAMS((double x[],integer x1[],integer *n1,char dir[],integer lstr));

extern void set_scale __PARAMS((char flag[],double subwin[],double frame_values[],int aaint[],char logflag[],double axis[]));
extern int C2F(graduate)  __PARAMS((double *,double *,double *,double *,integer *,integer *,integer *,integer *,integer *)); 

/* Theticks F.Leray 21.09.04 */
extern int C2F(theticks) __PARAMS((double *xminv, double *xmaxv, double *grads, int *ngrads));
extern int TheTicks( double *xminv, double * xmaxv, double * grads, int * ngrads);
extern int GradEqual(const double * grads,const int *ngrads);

extern void Cscale2default  __PARAMS((void)); 


extern void set_window_scale_with_default  __PARAMS((integer i));
extern int get_window_scale  __PARAMS((integer,double * )); 
extern void del_window_scale  __PARAMS((integer)); 

extern void Scale2D  __PARAMS((integer,double *,integer *,integer *,double *,double *,double *,double *,char *));
extern int C2F(setscale2d)  __PARAMS((double *,double *,char *,integer)); 
extern int C2F(Nsetscale2d)  __PARAMS((double *,double *,double *,char *,integer)); 
extern int getscale2d  __PARAMS((double *,double *,char *,double *)); 
extern void get_margin_in_pixel(integer *);
extern int C2F(echelle2d)  __PARAMS((double *,double *,integer *,integer *,integer *,integer *, char *,integer)); 
extern void C2F(echelle2dl)  __PARAMS((double *,double *,integer *,integer *,integer *,integer *,char *)); 
extern void C2F(ellipse2d)  __PARAMS((double *,integer *,integer *,char *)); 
extern void C2F(rect2d)  __PARAMS((double *,integer *,integer *,char *)); 
extern void C2F(axis2d)  __PARAMS((double *,double *,double *,integer *,double *)); 
extern int zoom  __PARAMS((void)); 
extern int zoom_get_rectangle    __PARAMS((double bbox[], int *x_pixel, int *y_pixel));

extern void unzoom  __PARAMS((void)); 
extern void Gr_Rescale  __PARAMS((char *,double *,integer *,integer *,integer *,integer *)); 
extern void Gr_Rescale2  __PARAMS((char *,double *,integer *,integer *,integer *,integer *)); /* F.Leray 20.04.04 */
extern void C2F(aplot1)  __PARAMS((double *,integer *,integer *,integer *,integer *npx,integer *npy,char *logflag,double scx,double scy,double xofset,double yofset)); 

	/* Plo3d.c */

int shade __PARAMS(( integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag));
void GetEch3d1  __PARAMS(( double (*m1)[3],double *,double *,double *,double *));
void GetEch3d   __PARAMS((void));
extern void AxesStrings  __PARAMS((integer,integer *,integer *,integer *,char *,double *)); 
extern void MaxiInd  __PARAMS((double *,integer,integer *,double)); 
extern void UpNext  __PARAMS((integer ,integer *,integer *)); 
extern void DownNext  __PARAMS((integer ,integer *,integer *)); 
extern void TDAxis  __PARAMS((integer flag,double FPval,double LPval,integer *nax,integer *FPoint,integer *LPoint,integer *Ticsdir)); 
extern void C2F(TDdrawaxis)  __PARAMS((double ,double FPval,double LPval,integer *nax,integer *FPoint,integer *LPoint,integer *Ticsdir)); 
extern void BBoxToval  __PARAMS((double *,double *,double *,integer ,double *)); 
extern int I3dRotation  __PARAMS((void)); 
extern int DPoints1  __PARAMS((integer *polyx,integer *polyy,integer *fill,integer whiteid,double zmin,double zmax,double *,double *,double *,integer i,integer j,integer jj1,integer *p,integer dc,integer fg)); 
extern int DPoints  __PARAMS((integer *polyx,integer *polyy,integer *fill,integer whiteid,double zmin,double zmax,double *,double *,double *,integer i,integer j,integer jj1,integer *p,integer dc,integer fg)); 
extern int C2F(plot3d)  __PARAMS((double *,double *,double *,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer)); 
extern int C2F(plot3d1)  __PARAMS((double *,double *,double *,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer)); 
extern int C2F(fac3d)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer)); 
extern int C2F(fac3d1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer)); 
extern int C2F(fac3d2)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer));
extern int C2F(fac3d3)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *teta,double *,char *,integer *,double *,integer));
extern int C2F(param3d)  __PARAMS((double *,double *,double *,integer *,double *teta,double *,char *,integer *,double *,integer)); 
extern int C2F(param3d1)  __PARAMS((double *,double *,double *,integer *,integer *,integer *,integer *colors,double *teta,double *,char *,integer *,double *,integer )); 
extern int C2F(box3d)  __PARAMS((double *,double *,double *)); 
extern int C2F(geom3d)  __PARAMS((double *,double *,double *,integer *n)); 
extern void SetEch3d  __PARAMS((double *,double *,double *,double *,double *teta,double *)); 
extern void SetEch3d1  __PARAMS((double *,double *,double *,double *,double *teta,double *,integer flag)); 
extern void DrawAxis  __PARAMS((double *,double *,integer *Indices,integer style)); 
extern void Convex_Box  __PARAMS((double *,double *,integer *,integer *,char *,integer *,double *)); 


/* Rec.c */ 

extern void SCPlots  __PARAMS((char *unused,integer *winnumber,integer *,double *,integer *,int,int *,double *)); 
extern int Check3DPlots  __PARAMS((char *unused,integer *winnumber)); 
extern int EchCheckSCPlots  __PARAMS((char *unused,integer *winnumber)); 
extern void Tape_ReplayUndoScale  __PARAMS((char *unused,integer *winnumber)); 
extern void Tape_ReplayNewScale  __PARAMS((char *unused,integer *winnumber,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void Tape_ReplayNewScale1  __PARAMS((char *unused,integer *winnumber,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void Tape_ReplayNewAngle  __PARAMS((char *unused,integer *winnumber,integer *,integer *,integer *,integer *,integer *,double *theta,double *,double *,double *)); 
 extern void Tape_Replay_Show  __PARAMS((char *unused,integer *winnumber,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void Tape_Replay  __PARAMS((char *unused,integer *winnumber,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void UseColorFlag  __PARAMS((int flag)); 
 extern int  Store  __PARAMS((char *type,char *plot)); 
 extern void CleanPlots  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void StoreXcall1  __PARAMS((char *,char *,integer *,integer,integer *,integer ,integer *,integer ,integer *,integer ,integer *,integer ,integer *,integer ,double *,integer ,double *,integer ,double *,integer ,double *,integer )); 

 extern void StoreEch  __PARAMS((char *,double *WRect,double *,char *logflag)); 
 extern void StoreNEch  __PARAMS((char *,char *flag,double *,double *,double *,char *logflag)); 
 extern void StorePlot  __PARAMS((char *,char *f,double *,double *,integer *,integer *,integer *,char *,char *,double *,integer *aint)); 
 extern void StoreGrid  __PARAMS((char *,integer *)); 
 extern void StoreParam3D  __PARAMS((char *,double *,double *,double *,integer *,double *teta,double *,char *,integer *,double *)); 
 extern void StoreParam3D1  __PARAMS((char *,double *,double *,double *,integer *,integer *,integer *,integer *colors,double *teta,double *,char *,integer *,double *)); 
 extern void StorePlot3D  __PARAMS((char *,double *,double *,double *,integer *p,integer *q,double *teta,double *,char *,integer *,double *)); 
 extern void StoreFac3D  __PARAMS((char *,double *,double *,double *,integer *cvect,integer *p,integer *q,double *teta,double *,char *,integer *,double *)); 

 extern void StoreFec  __PARAMS((char *,double *,double *,double *triangles,double *func,integer *Nnode,integer *Ntr,char *,char *,double *,integer *, double *, integer *, integer *, int)); 

/* old  extern void StoreFec  __PARAMS((char *,double *,double *,double *triangles,double *func,integer *Nnode,integer *Ntr,char *,char *,double *,integer *)); */
 extern void StoreContour  __PARAMS((char *,double *,double *,double *,integer *,integer *,integer *,integer *,double *,double *teta,double *,char *,integer *,double *,double *zlev)); 
 extern void StoreContour2D  __PARAMS((char *,double *,double *,double *,integer *,integer *,integer *,integer *,double *,integer *,char *,char *,double *,integer *)); 
 extern void StoreGray  __PARAMS((char *,double *,double *,double *,integer *,integer *,char *,double *,integer *)); 
 extern void StoreGray1  __PARAMS((char *,double *,integer *,integer *,char *,double *,integer *)); 
 extern void StoreGray2  __PARAMS((char *,double *,integer *,integer *,double *)); 
 extern void StoreChamp  __PARAMS((char *,double *,double *,double *,double *,integer *,integer *,char *,double *,double *)); 
 extern void StoreXgc  __PARAMS((integer )); 
extern void StoreSciAxis __PARAMS((char *name,char pos,char xy_type,double *x,int *nx,double *y,int *ny,char **str,int subtics,char *format,int fontsize,int textcolor, int fontstyle,int ticscolor,char logflag,int seg_flag));

/* RecLoad.c  */

extern int C2F(xloadplots)  __PARAMS((char *,integer lvx)); 

/* RecSave.c */ 

extern int C2F(xsaveplots)  __PARAMS((integer *winnumber,char *,integer lxv)); 

/* Xcall.c */

extern int C2F(dr)  __PARAMS((char *,char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *,integer ,integer )); 
extern void C2F(SetDriver)  __PARAMS((char * ,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 

extern void GetDriver1  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 

extern char GetDriver  __PARAMS((void)); 
extern int GetDriverId __PARAMS((void)); 
extern int C2F(inttest) __PARAMS((int *)); 

/* Xcall1 */

extern int C2F(xsetg) __PARAMS((char *str,char *str1,integer lx0,integer lx1));
extern int C2F(xgetg) __PARAMS((char *str,char *str1,integer *len,integer lx0,integer lx1));
extern char *getFPF __PARAMS((void));
extern int C2F(dr1)  __PARAMS((char * ,char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *,integer ,integer )); 

/* periFig.c */

extern void C2F(getcursymbolXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(semptyXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(gemptyXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(scilabgcgetXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(scilabgcsetXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(ScilabGCGetorSetXfig)  __PARAMS((char *,integer flag,integer *,integer *,integer *,integer *,integer *,integer *,integer *,double *)); 
extern void C2F(displaystringXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *angle,double *,double *,double *)); 
extern void C2F(boundingboxXfig)  __PARAMS((char *,integer *,integer *,integer *rect,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawlineXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(drawsegmentsXfig)  __PARAMS((char *,integer *,integer *vy,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarrowsXfig)  __PARAMS((char *,integer *,integer *vy,integer *,integer *as,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawrectangleXfig)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(fillrectangleXfig)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawrectanglesXfig)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(fillarcsXfig)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarcsXfig)  __PARAMS((char *,integer *vects,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarcXfig)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
extern void C2F(xselgraphicXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(fillarcXfig)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
extern void C2F(drawpolymarkXfig)  __PARAMS((char *,integer *,integer *,integer *vy,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawpolylinesXfig)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *rawvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
extern void C2F(fillpolylinesXfig)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *fillvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
extern void C2F(drawpolylineXfig)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xendgraphicXfig)  __PARAMS((void)); 
extern void C2F(fillpolylineXfig)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeareaflag,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(initgraphicXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xendXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 

extern void C2F(InitScilabGCXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(loadfamilyXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawaxisXfig)  __PARAMS((char *,integer *,integer *nsteps,integer *,integer *initpoint,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(clearwindowXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(displaynumbersXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void set_pattern_or_color __PARAMS((int pat,int *areafill,int *color)); 
/* extern void set_dash_or_color  __PARAMS((int dash,int *l_style,int *_val,int *color)); */
extern void C2F(viderbuffXfig)  __PARAMS((void)); 
extern void C2F(WriteGenericXfig)  __PARAMS((char *,integer nobj,integer sizeobj,integer *,integer *vy,integer sizev,integer flag,integer *fvect)); 
extern void C2F(getwindowdimXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(Write2VectXfig)  __PARAMS((integer *,integer *vy,integer,integer flag)); 
extern void C2F(setwindowdimXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(getwindowposXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(setwindowposXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(xpauseXfig)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xclickXfig)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xclick_anyXfig)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xgetmouseXfig)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(clearareaXfig)  __PARAMS((char *,integer *,integer *,integer *w,integer *h,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(setcurwinXfig)  __PARAMS((integer *intnum,integer *,integer *,integer *)); 
extern void C2F(getcurwinXfig)  __PARAMS((integer *,integer *intnum,integer *,double *)); 
extern void C2F(setclipXfig)  __PARAMS((integer *,integer *,integer *w,integer *h)); 
extern void C2F(unsetclipXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(getclipXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(absourelXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getabsourelXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(setalufunctionXfig)  __PARAMS((char *)); 
extern void C2F(idfromnameXfig)  __PARAMS((char *,integer *num)); 
extern void C2F(setalufunction1Xfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getalufunctionXfig)  __PARAMS((integer *,integer *value,integer *,double *)); 
extern void C2F(setthicknessXfig)  __PARAMS((integer *value,integer *,integer *,integer *)); 
extern void C2F(getthicknessXfig)  __PARAMS((integer *,integer *value,integer *,double *)); 
extern void C2F(setpatternXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getpatternXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(getlastXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(setdashXfig)  __PARAMS((integer *value,integer *,integer *,integer *)); 
extern void C2F(getdashXfig)  __PARAMS((integer *,integer *value,integer *,double *)); 
extern void C2F(usecolorXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getusecolorXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(setcolormapXfig)  __PARAMS((integer *,integer *,integer *,integer *,integer *,integer *,double *a)); 
extern void C2F(set_cXfig)  __PARAMS((integer i)); 
extern void C2F(setbackgroundXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getbackgroundXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(setforegroundXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(getforegroundXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(sethidden3dXfig)  __PARAMS((integer *num,integer *,integer *,integer *)); 
extern void C2F(gethidden3dXfig)  __PARAMS((integer *,integer *num,integer *,double *)); 
extern void C2F(xsetfontXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 
extern void C2F(xgetfontXfig)  __PARAMS((integer *,integer *,integer *,double *)); 
extern void C2F(setcursymbolXfig)  __PARAMS((integer *,integer *,integer *,integer *)); 

/* periPos.c */

extern void ColorInit  __PARAMS((void)); 
extern void C2F(gemptyPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(scilabgcgetPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(scilabgcsetPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(ScilabGCGetorSetPos)  __PARAMS((char *,integer flag,integer *,integer *,integer *,integer *,integer *,integer *,integer *,double *)); 
 extern void C2F(displaystringPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *angle,double *,double *,double *)); 
 extern void C2F(boundingboxPos)  __PARAMS((char *,integer *,integer *,integer *rect,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawlinePos)  __PARAMS((integer *x1,integer *,integer *,integer *)); 
 extern void C2F(drawsegmentsPos)  __PARAMS((char *,integer *,integer *vy,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarrowsPos)  __PARAMS((char *,integer *,integer *vy,integer *,integer *as,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xselgraphicPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawrectanglesPos)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawrectanglePos)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillrectanglePos)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillarcsPos)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarcsPos)  __PARAMS((char *,integer *vects,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarcPos)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
 extern void C2F(fillarcPos)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
 extern void C2F(xendgraphicPos)  __PARAMS((void)); 
 extern void C2F(drawpolylinesPos)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *rawvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillpolylinesPos)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *fillvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
 extern void C2F(xendPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawpolylinePos)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillpolylinePos)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeareaflag,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawpolymarkPos)  __PARAMS((char *,integer *,integer *,integer *vy,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(initgraphicPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void InitScilabGCPos  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(clearwindowPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawaxisPos)  __PARAMS((char *,integer *,integer *nsteps,integer *,integer *initpoint,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(displaynumbersPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(WriteGenericPos)  __PARAMS((char *,integer nobj,integer sizeobj,integer *,integer *vy,integer sizev,integer flag,integer *fvect)); 
 extern void C2F(WriteGeneric1Pos)  __PARAMS((char *,integer nobjpos,integer objbeg,integer sizeobj,integer *,integer *vy,integer flag,integer *fvect)); 
 extern void C2F(xpausePos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(Write2VectPos)  __PARAMS((integer *,integer *vy,integer from,integer,char *,integer flag,integer fv)); 
 extern void C2F(xsetfontPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(xgetfontPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(xsetmarkPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(xgetmarkPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(xclickPos)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(loadfamilyPos)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xclick_anyPos)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xgetmousePos)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(clearareaPos)  __PARAMS((char *,integer *,integer *,integer *w,integer *h,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(getwindowposPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setwindowposPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(getwindowdimPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setwindowdimPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(setcurwinPos)  __PARAMS((integer *intnum,integer *,integer *,integer *)); 
 extern void C2F(getcurwinPos)  __PARAMS((integer *,integer *intnum,integer *,double *)); 
 extern void C2F(setclipPos)  __PARAMS((integer *,integer *,integer *w,integer *h)); 
 extern void C2F(unsetclipPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(getclipPos)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setabsourelPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getabsourelPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setalufunctionPos)  __PARAMS((char *)); 
 extern void C2F(idfromnamePos)  __PARAMS((char *,integer *num)); 
 extern void C2F(setalufunction1Pos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getalufunctionPos)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(setthicknessPos)  __PARAMS((integer *value,integer *,integer *,integer *)); 
 extern void C2F(getthicknessPos)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(setpatternPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getpatternPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(getlastPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setdashPos)  __PARAMS((integer *value,integer *,integer *,integer *)); 
 extern void C2F(setdashstylePos)  __PARAMS((integer *value,integer *x,integer *n)); 
 extern void C2F(getdashPos)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(usecolorPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getusecolorPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setcolormapPos)  __PARAMS((integer *,integer *,integer *,integer *,integer *,integer *,double *a)); 
 extern void C2F(set_cPos)  __PARAMS((integer i)); 
 extern void C2F(setbackgroundPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getbackgroundPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setforegroundPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getforegroundPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(sethidden3dPos)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(gethidden3dPos)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(semptyPos)  __PARAMS((integer *,integer *,integer *,integer *)); 
	
	/* periGif.c */

extern void C2F(gemptyGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(scilabgcgetGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(scilabgcsetGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(ScilabGCGetorSetGif)  __PARAMS((char *,integer flag,integer *,integer *,integer *,integer *,integer *,integer *,integer *,double *)); 
 extern void C2F(displaystringGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *angle,double *,double *,double *)); 
 extern void C2F(boundingboxGif)  __PARAMS((char *,integer *,integer *,integer *rect,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawlineGif)  __PARAMS((integer *x1,integer *,integer *,integer *)); 
 extern void C2F(drawsegmentsGif)  __PARAMS((char *,integer *,integer *vy,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarrowsGif)  __PARAMS((char *,integer *,integer *vy,integer *,integer *as,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xselgraphicGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawrectanglesGif)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawrectangleGif)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillrectangleGif)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillarcsGif)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarcsGif)  __PARAMS((char *,integer *vects,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawarcGif)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
 extern void C2F(fillarcGif)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
 extern void C2F(xendgraphicGif)  __PARAMS((void)); 
 extern void C2F(drawpolylinesGif)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *rawvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillpolylinesGif)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *fillvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
 extern void C2F(xendGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawpolylineGif)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(fillpolylineGif)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeareaflag,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawpolymarkGif)  __PARAMS((char *,integer *,integer *,integer *vy,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(initgraphicGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void InitScilabGCGif  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(clearwindowGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(drawaxisGif)  __PARAMS((char *,integer *,integer *nsteps,integer *,integer *initpoint,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(displaynumbersGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(WriteGenericGif)  __PARAMS((char *,integer nobj,integer sizeobj,integer *,integer *vy,integer sizev,integer flag,integer *fvect)); 
 extern void C2F(WriteGeneric1Gif)  __PARAMS((char *,integer nobjpos,integer objbeg,integer sizeobj,integer *,integer *vy,integer flag,integer *fvect)); 
 extern void C2F(xpauseGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(Write2VectGif)  __PARAMS((integer *,integer *vy,integer from,integer,char *,integer flag,integer fv)); 
 extern void C2F(xsetfontGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(xgetfontGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(xsetmarkGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(xgetmarkGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(xclickGif)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(loadfamilyGif)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xclick_anyGif)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(xgetmouseGif)  __PARAMS((char *,integer *ibutton,integer *x1,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(clearareaGif)  __PARAMS((char *,integer *,integer *,integer *w,integer *h,integer *,integer *,double *,double *,double *,double *)); 
 extern void C2F(getwindowposGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setwindowposGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(getwindowdimGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setwindowdimGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(setcurwinGif)  __PARAMS((integer *intnum,integer *,integer *,integer *)); 
 extern void C2F(getcurwinGif)  __PARAMS((integer *,integer *intnum,integer *,double *)); 
 extern void C2F(setclipGif)  __PARAMS((integer *,integer *,integer *w,integer *h)); 
 extern void C2F(unsetclipGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
 extern void C2F(getclipGif)  __PARAMS((integer *,integer *,integer *,double *)); 
 extern void C2F(setabsourelGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getabsourelGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setalufunctionGif)  __PARAMS((char *)); 
 extern void C2F(idfromnameGif)  __PARAMS((char *,integer *num)); 
 extern void C2F(setalufunction1Gif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getalufunctionGif)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(setthicknessGif)  __PARAMS((integer *value,integer *,integer *,integer *)); 
 extern void C2F(getthicknessGif)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(setpatternGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getpatternGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(getlastGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setdashGif)  __PARAMS((integer *value,integer *,integer *,integer *)); 
 extern void C2F(setdashstyleGif)  __PARAMS((integer *value,integer *x,integer *n)); 
 extern void C2F(getdashGif)  __PARAMS((integer *,integer *value,integer *,double *)); 
 extern void C2F(usecolorGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getusecolorGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setcolormapGif)  __PARAMS((integer *,integer *,integer *,integer *,integer *,integer *,double *a)); 
 extern void C2F(set_cGif)  __PARAMS((integer i)); 
 extern void C2F(setbackgroundGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getbackgroundGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(setforegroundGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(getforegroundGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(sethidden3dGif)  __PARAMS((integer *num,integer *,integer *,integer *)); 
 extern void C2F(gethidden3dGif)  __PARAMS((integer *,integer *num,integer *,double *)); 
 extern void C2F(semptyGif)  __PARAMS((integer *,integer *,integer *,integer *)); 
	
	/* periX11.c */

#ifndef WIN32 

extern void SciClick __PARAMS((integer *ibutton,integer *x1,integer *yy1,integer *iflag,int getmouse,
		       int dyn_men,int getrelease,char *str,integer * lstr));
extern void fill_grid_rectangles __PARAMS(( integer *x,  integer *y,  double *z, integer n1, integer n2));
extern void fill_grid_rectangles1 __PARAMS(( integer *x,  integer *y,  double *z, integer n1, integer n2));
extern void C2F(getwindowdim) __PARAMS((integer *verbose, integer *x, integer *narg, double *dummy));
extern int GetWinsMaxId __PARAMS((void));
extern void SwitchWindow __PARAMS((integer *intnum));
extern int C2F(sedeco)  __PARAMS((int *)); 
extern void C2F(set_default_colormap)  __PARAMS((void)); 
extern int get_pixel  __PARAMS((int i)); 
extern int CheckColormap  __PARAMS((int *m)); 
extern void get_r  __PARAMS((int i,float *r)); 
extern void get_g  __PARAMS((int i,float *g)); 
extern void get_b  __PARAMS((int i,float *b)); 
extern void C2F(MissileGCget)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(MissileGCset)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(MissileGCGetorSet)  __PARAMS((char *,integer flag,integer *,integer *,integer *,integer *,integer *,integer *,integer *,double *)); 
extern void C2F(displaystring)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *angle,double *,double *,double *)); 
extern void C2F(DispStringAngle)  __PARAMS((integer *,integer *,char *,double *angle)); 
extern void C2F(boundingbox)  __PARAMS((char *,integer *,integer *,integer *rect,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawline)  __PARAMS((integer *,integer *,integer *,integer *)); 
/**  extern int XgcFreeColors  __PARAMS((struct BCG *gc));  **/
extern void C2F(drawsegments)  __PARAMS((char *,integer *,integer *vy,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarrows)  __PARAMS((char *,integer *,integer *vy,integer *,integer *as,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawrectangles)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawrectangle)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(fillrectangle)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(fillarcs)  __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarcs)  __PARAMS((char *,integer *vects,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawarc)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
extern void C2F(fillarc)  __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *)); 
extern void C2F(drawpolylines)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *rawvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
extern void C2F(fillpolylines)  __PARAMS((char *,integer *vectsx,integer *vectsy,integer *fillvect,integer *,integer *p,integer *,double *,double *,double *,double *)); 
extern void C2F(drawpolyline)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *)); 
extern void CPixmapResize  __PARAMS((int x,int y)); 
extern void C2F(fillpolyline)  __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawpolymark)  __PARAMS((char *,integer *,integer *,integer *vy,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void DeleteSGWin  __PARAMS((integer intnum)); 
extern void DeleteWindowToList  __PARAMS((integer num)); 
extern void C2F(getwins)  __PARAMS((integer *Num,integer *,integer *)); 
extern void set_c  __PARAMS((integer i)); 
extern void C2F(initgraphic)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void CPixmapResize1  __PARAMS((void)); 
extern void C2F(xinfo)  __PARAMS((char *message,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xselgraphic)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(drawaxis)  __PARAMS((char *,integer *,integer *nsteps,integer *,integer *initpoint,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xendgraphic)  __PARAMS((void)); 
extern void C2F(displaynumbers)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xend)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(bitmap)  __PARAMS((char *,integer,integer)); 
extern void C2F(loadfamily)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(clearwindow)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern int C2F(CurSymbXOffset)  __PARAMS((void)); 
extern int C2F(CurSymbYOffset)  __PARAMS((void)); 
extern int C2F(store_points)  __PARAMS((integer,integer *,integer *vy,integer onemore)); 
extern int C2F(AllocVectorStorage)  __PARAMS((void)); 
extern void set_clip_box __PARAMS((integer xxleft,integer xxright,integer yybot,integer yytop)); 
extern void clip_line  __PARAMS((integer,integer,integer ,integer,integer *,integer *,integer *,integer *,integer *)); 
extern void C2F(xpause)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xclick)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xclick_any)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *iwin,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xgetmouse_test)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(xgetmouse)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(cleararea)  __PARAMS((char *,integer *,integer *,integer *w,integer *h,integer *,integer *,double *,double *,double *,double *)); 
extern void C2F(Recenter_GW)  __PARAMS((void)); 
extern void getcolordef  __PARAMS((integer *));
extern void setcolordef  __PARAMS((integer ));
extern void wininfo  __PARAMS((char *fmt,...));

#else 

/*  periWin.c */

extern void SciClick __PARAMS((integer *ibutton,integer *x1,integer *yy1,integer *iflag,int getmouse,int dyn_men,int release,char *str,integer * lstr));
extern void SciG_Font_Printer(int scale);
extern void SciG_Font(void) ;
extern void CleanFonts();
extern void SciMouseCapture();
extern void SciMouseRelease();
extern void SetWinhdc  __PARAMS((void));  
extern int MaybeSetWinhdc  __PARAMS((void));  
extern void ReleaseWinHdc  __PARAMS((void));  
/**   extern void SetGHdc  __PARAMS((HDC lhdc,int width,int height));  **/
/**  extern int XgcAllocColors  __PARAMS((struct BCG *gc,int m));   **/
/**   extern int XgcFreeColors  __PARAMS((struct BCG *gc));  **/
  extern void C2F(pixmapclear)  __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(show)  __PARAMS((integer *,integer *,integer *,integer *));  
  extern void CPixmapResize  __PARAMS((int x,int y));  
  extern void CPixmapResize1  __PARAMS((void));  
  extern void C2F(xselgraphic)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xendgraphic)  __PARAMS((void));  
  extern void C2F(xend)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(clearwindow)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xpause)  __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xclick)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xclick_any)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *iwin,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xgetmouse)  __PARAMS((char *,integer *ibutton,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(cleararea)  __PARAMS((char *,integer *,integer *,integer *w,integer *h,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(getwindowpos)  __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(setwindowpos)  __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(getwindowdim)  __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(setwindowdim)  __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(setcurwin)  __PARAMS((integer *intnum,integer *,integer *,integer *));  
  extern int SwitchWindow  __PARAMS((integer *intnum));  
  extern void C2F(getcurwin) __PARAMS((integer *,integer *intnum,integer *,double *));  
  extern void C2F(setclip) __PARAMS((integer *,integer *,integer *w,integer *h));  
  extern void C2F(unsetclip) __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(getclip) __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(setabsourel) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getabsourel) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(setalufunction) __PARAMS((char *));  
  extern void C2F(setalufunction1) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getalufunction) __PARAMS((integer *,integer *value,integer *,double *));  
  extern void C2F(setthickness) __PARAMS((integer *value,integer *,integer *,integer *));  
  extern void C2F(getthickness) __PARAMS((integer *,integer *value,integer *,double *));  
  extern void C2F(CreatePatterns) __PARAMS((void));  
  extern void C2F(setpattern) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getpattern) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(getlast) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(setdash) __PARAMS((integer *value,integer *,integer *,integer *));  
  extern void C2F(getdash) __PARAMS((integer *,integer *value,integer *,double *));  
  extern void C2F(usecolor) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getusecolor) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(setpixmapOn) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getpixmapOn) __PARAMS((integer *,integer *value,integer *,double *));  
  extern int C2F(sedeco) __PARAMS((int *));  
  extern void set_default_colormap  __PARAMS((void));  
  extern void C2F(setcolormap) __PARAMS((integer *,integer *,integer *,integer *,integer *,integer *,double *a));  
  extern void C2F(getcolormap) __PARAMS((integer *,integer *num,integer *,double *val));  
  extern void C2F(setbackground) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getbackground) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(setforeground) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(getforeground) __PARAMS((integer *,integer *num,integer *,double *));  
  extern void C2F(sethidden3d) __PARAMS((integer *num,integer *,integer *,integer *));  
  extern void C2F(gethidden3d) __PARAMS((integer *,integer *num,integer *,double *));  
  extern int CheckColormap  __PARAMS((int *m));  
  extern void get_r  __PARAMS((int i,float *r));  
  extern void get_g  __PARAMS((int i,float *g));  
  extern void get_b  __PARAMS((int i,float *b));  
  extern void C2F(sempty) __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(gempty) __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(MissileGCget) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(MissileGCset) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(MissileGCGetorSet) __PARAMS((char *,integer flag,integer *,integer *,integer *,integer *,integer *,integer *,integer *,double *));  
  extern void C2F(displaystring) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *angle,double *,double *,double *));  
  extern void C2F(DispStringAngle) __PARAMS((integer *,integer *y0,char *,double *angle));  
  extern void C2F(boundingbox) __PARAMS((char *,integer *,integer *,integer *rect,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawline) __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(drawsegments) __PARAMS((char *,integer *,integer *vy,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawarrows) __PARAMS((char *,integer *,integer *vy,integer *,integer *as,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawrectangles) __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawrectangle) __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(fillrectangle) __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(fillarcs) __PARAMS((char *,integer *vects,integer *fillvect,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawarcs) __PARAMS((char *,integer *vects,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawarc) __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *));  
  extern void C2F(fillarc) __PARAMS((char *,integer *,integer *,integer *width,integer *height,integer *angle1,integer *angle2,double *,double *,double *,double *));  
  extern void C2F(drawpolylines) __PARAMS((char *,integer *vectsx,integer *vectsy,integer *rawvect,integer *,integer *p,integer *,double *,double *,double *,double *));  
  extern void C2F(fillpolylines) __PARAMS((char *,integer *vectsx,integer *vectsy,integer *fillvect,integer *,integer *p,integer *,double *,double *,double *,double *));  
  extern void C2F(drawpolyline) __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(fillpolyline) __PARAMS((char *,integer *,integer *,integer *vy,integer *closeflag,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(drawpolymark) __PARAMS((char *,integer *,integer *,integer *vy,integer *,integer *,integer *,double *,double *,double *,double *));  
/**  extern struct BCG *AddNewWindowToList  __PARAMS((void));  **/
/**  extern struct BCG *AddNewWindow  __PARAMS((WindowList **listptr));  **/
  extern void DeleteSGWin  __PARAMS((integer intnum));  
  extern void DeleteWindowToList  __PARAMS((integer num));  
  extern void C2F(getwins) __PARAMS((integer *,integer *,integer *));  
  extern void set_c  __PARAMS((integer i));  
  extern void C2F(initgraphic) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(xinfo) __PARAMS((char *message,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void wininfo  __PARAMS((char *fmt,...));
  extern void getcolordef  __PARAMS((integer *creenc));  
  extern void setcolordef  __PARAMS((integer screenc));  
  extern void ResetScilabXgc  __PARAMS((void));  
  extern void C2F(drawaxis) __PARAMS((char *,integer *,integer *nsteps,integer *,integer *initpoint,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(displaynumbers) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
  extern void C2F(bitmap) __PARAMS((char *,integer,integer));  
  extern void C2F(xsetfont) __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(xgetfont) __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(xsetmark) __PARAMS((integer *,integer *,integer *,integer *));  
  extern void C2F(xgetmark) __PARAMS((integer *,integer *,integer *,double *));  
  extern void C2F(loadfamily) __PARAMS((char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *));  
/**   extern void SciMakeFont  __PARAMS((char *,int size,HFONT *hfont));  **/
  extern int C2F(CurSymbXOffset) __PARAMS((void));  
  extern int C2F(CurSymbYOffset) __PARAMS((void));  
  extern int C2F(store_points) __PARAMS((integer,integer *,integer *vy,integer onemore));  
  extern int C2F(AllocVectorStorage) __PARAMS((void));  
  extern void set_clip_box  __PARAMS((integer xxleft,integer xxright,integer yybot,integer yytop));  
  extern void clip_line  __PARAMS((integer,integer,integer ,integer,integer *,integer *,integer *,integer *, integer *));  




#endif /* ifndef WIN32 */

extern integer first_in __PARAMS(( integer,integer,integer *,integer*));
extern integer first_out __PARAMS(( integer,integer,integer *,integer*));

extern double computeGridMinGap( double gridX[], double gridY[], int nbRow, int nbCol ) ;
extern int computeArrowColor( double gridX[], double gridY[], int nbRow, int nbCol, int index ) ;
extern double getLongestVector( double vectX[], double vectY[], int nbRow, int nbCol, double scx, double scy ) ;

#endif /** SCIG_PROTO **/

