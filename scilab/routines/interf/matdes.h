#ifndef MATDES_SCI 
#define MATDES_SCI 

int check_xy _PARAMS((char *fname, char dir, int mn, int xpos, int xm, int xn, long unsigned int xl, int ypos, int ym, int yn, long unsigned int yl, int *ntics));

extern int C2F(matdes)  _PARAMS((void));
extern int scichamp  _PARAMS((char *fname,unsigned long fname_len));
extern int scicontour  _PARAMS((char *fname, unsigned long fname_len));
extern int scicontour2d  _PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d  _PARAMS((char *fname, unsigned long fname_len));
extern int scic2dex  _PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d1  _PARAMS((char *fname, unsigned long fname_len));
extern int scigeom3d  _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d_G _PARAMS(( char *fname,
			 int (*func) _PARAMS((double *,double *,double *,integer *p,integer *q,
					      double *,double *,char *,integer *,double *,integer)),
			 int (*func1)  _PARAMS((double *,double *,double *,integer *cvect,integer *p,
						integer *q,double *,double *,char *,integer *,double *,integer)),
			 int (*func2) _PARAMS((double *,double *,double *,integer *cvect,integer *p,
					       integer *q,double *,double *,char *,integer *,double *,integer)),
			 int (*func3) _PARAMS((double *,double *,double *,integer *cvect,integer *p,
					       integer *q,double *,double *,char *,integer *,double *,integer)),
			 unsigned long fname_len));

extern int sciplot2d  _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_G  _PARAMS((char *fname,int ptype,
				  int (*func) _PARAMS((char *,double *,double *,integer *,integer *, integer *,
						       char *, char *, double *,integer *,integer,integer, integer)),
				  unsigned long fname_len));

extern int scigrayplot  _PARAMS((char *fname, unsigned long fname_len));
extern int scigray1plot  _PARAMS((char *fname, unsigned long fname_len));
extern int scigray2plot  _PARAMS((char *fname, unsigned long fname_len));
extern int sci_demo  _PARAMS((char *fname, char *chaine, integer *flag));
extern int scidriver  _PARAMS((char *fname, unsigned long fname_len));
extern int scixarc  _PARAMS((char *fname, unsigned long fname_len));
extern int scixarcs  _PARAMS((char *fname, unsigned long fname_len));
extern int scixfarcs  _PARAMS((char *fname,  unsigned long fname_len));
extern int sciarrows  _PARAMS((char *fname, unsigned long fname_len));
extern int scixsegs  _PARAMS((char *fname, unsigned long fname_len));
extern int scixaxis  _PARAMS((char *fname, unsigned long fname_len));
extern int scixchange  _PARAMS((char *fname, unsigned long fname_len));
extern int entier2d  _PARAMS((integer *n, double *d__, integer *s));
extern int simple2d  _PARAMS((integer *n, double *d__, float *s));
extern int scixclea  _PARAMS((char *fname, unsigned long fname_len));
extern int scixclear  _PARAMS((char *fname, unsigned long fname_len));
extern int scixclick  _PARAMS((char *fname, unsigned long fname_len));
extern int scixend  _PARAMS((char *fname, unsigned long fname_len));
extern int scixgrid  _PARAMS((char *fname, unsigned long fname_len));
extern int scixfpoly  _PARAMS((char *fname, unsigned long fname_len));
extern int scixfpolys  _PARAMS((char *fname, unsigned long fname_len));
extern int scixget  _PARAMS((char *fname, unsigned long fname_len));
extern int scixgetg  _PARAMS((char *fname, unsigned long fname_len));
extern int scixinit  _PARAMS((char *fname, unsigned long fname_len));
extern int scixlfont  _PARAMS((char *fname, unsigned long fname_len));
extern int scixnumb  _PARAMS((char *fname, unsigned long fname_len));
extern int scixpause  _PARAMS((char *fname, unsigned long fname_len));
extern int scixpoly  _PARAMS((char *fname, unsigned long fname_len));
extern int scixpolys  _PARAMS((char *fname, unsigned long fname_len));
extern int scixselect  _PARAMS((char *fname, unsigned long fname_len));
extern int scixset  _PARAMS((char *fname, unsigned long fname_len));
extern int scixsetg  _PARAMS((char *fname, char *str, unsigned long fname_len, unsigned long str_len));
extern int scixstring  _PARAMS((char *fname, unsigned long fname_len));
extern int scixtitle  _PARAMS((char *fname, unsigned long fname_len));
extern int scixstrbi  _PARAMS((char *fname, unsigned long fname_len));
extern int scixstringl  _PARAMS((char *fname, unsigned long fname_len));
extern int scixtape  _PARAMS((char *fname, unsigned long fname_len));
extern int scixinfo  _PARAMS((char *fname, unsigned long fname_len));
extern int scixsetech  _PARAMS((char *fname, unsigned long fname_len));
extern int scixgetech  _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(sciwin)  _PARAMS((void));
extern int scifec  _PARAMS((char *fname, unsigned long fname_len));
extern int scixgetmouse  _PARAMS((char *fname, unsigned long fname_len));
extern int scixsave  _PARAMS((char *fname, unsigned long fname_len));
extern int scixload  _PARAMS((char *fname, unsigned long fname_len));
extern int scidelw  _PARAMS((char *fname, unsigned long fname_len));
extern int scixg2psofig  _PARAMS((char *fname, char *dr, unsigned long fname_len, unsigned long dr_len));
extern int scixsort  _PARAMS((char *fname, unsigned long fname_len));
extern int scissort  _PARAMS((char *fname, unsigned long fname_len));
extern int sciwinsid  _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(scigerase)  _PARAMS((void));
extern int scixgraduate  _PARAMS((char *fname, unsigned long fname_len));
extern int scixname  _PARAMS((char *fname, unsigned long fname_len));


/* NG beg */
extern int gset(char *fname, unsigned long fname_len);
extern int gget(char *fname, unsigned long fname_len);
extern int delete(char *fname, unsigned long fname_len);
extern int addcb(char *fname, unsigned long fname_len);
extern int copy(char *fname, unsigned long subwinparenttarget);
/*** 18/03/2002 ***/
extern int move(char *fname, unsigned long fname_len);
/*** 19/03/2002 ***/
extern int glue(char *fname, unsigned long fname_len);
extern int unglue(char *fname, unsigned long fname_len);
extern int drawnow(char *fname, unsigned long fname_len);
extern int drawlater(char *fname, unsigned long fname_len);
extern int scixclear(char *fname, unsigned long fname_len);
extern int scixclearsubwin(char *fname, unsigned long fname_len);
extern int scixbascsubwin(char *fname, unsigned long fname_len);


extern int draw(char *fname, unsigned long fname_len);

void Objarc (int *angle1,int *angle2,double *x,double *y,double *width,double *height,int color,int fill,long *hdl);
void Objdrawaxis (char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int subint,char *format,int font,int textcol,int ticscol,char flag,int seg);
void Objfec (double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax);


void Objfpoly (double *x,double *y,integer rempl,integer closed,long *hdl);
void Objgrayplot (double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint);
void Objmatplot (double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint);
void Objmatplot1 (double *z,integer *n1,integer *n2,double *xrect);
void Objnumb(char *fname,unsigned long fname_len,integer n,integer flag,double *x,double *y,double *angle,double *box);
void Objplot2d (int ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint);

void Objplot3d (char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox);
void Objpoly (double *x,double *y,integer n,integer closed,int mark,long *hdl);
void Objrect (double *x,double *y,double *width,double *height,int fillflag,int fillcolor,int n,long *hdl);
void Objsegs (integer *style,integer flag,integer n,double *x,double *y, double arsize);
void Objstring(char *fname,unsigned long fname_len,integer str,double x,double y,double *angle,double *box,double wy,long *hdl);
void Objtitle(char *str,int n,long *hdl);

void Xarc (char *fname,unsigned long fname_len,int *a1,int *a2,double *x,double *y,double *width,double *height);
void Xarcs (char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2);

void Xdrawaxis (char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int sub_int,char *format,int fontsize,int textcolor,int ticscolor,char flag,int seg_flag);

void Xfarcs (char *fname,unsigned long fname_len,int *vect1,int n,double *vect2);
void Xfec (double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax);

void Xfpoly (integer rempl,integer closed,double *x,double *y);
void Xfpolys (int *vect,integer val1,integer val2,integer n,integer m,double *x,double *y);
void Xgrayplot (double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint);
void Xmatplot (double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint);
void Xmatplot1 (double *z,integer *n1,integer *n2,double *xrect) ;
void Xnumb (char *fname,unsigned long fname_len,integer flag,double *x,double *y,double *angle,double *box);
void Xplot2d (double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint);
void Xplot3d (char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox);

void Xpoly (char *fname,unsigned long fname_len,int n,int close,double *x,double *y);
void Xpolys (integer *vect,integer n,integer m,double *x,double *y);
void Xrect (char *fname,unsigned long fname_len,double *x,double *y,double *width,double *height);
void Xrects (char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2);
void Xsegs (integer *style,integer flag,integer n,double *x,double *y, double arsize);
void Xstring (char *fname,unsigned long fname_len,integer str,double x,double y,double angle,double *box);
void Xtitle (char *str,int n);
/* NG beg */
#endif 
