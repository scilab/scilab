#ifndef MATDES_SCI 
#define MATDES_SCI 

/* Allan CORNET 18/01/2004 */
/* Gestion historique */
/*extern void LoadHistoryFunction(void); */ /*voir history.c (wsci) */
/*extern void SaveHistoryFunction(void); */ /*voir history.c (wsci) */
/*extern void ResetHistoryFunction(void); */ /*voir history.c (wsci) */
/*extern void HistoryFunction _PARAMS((char *fname)); */ /* voir history.c (wsci)*/
/* Minimize or Restore a window */
extern int ShowWindowFunction _PARAMS((char *fname)); /* wtext.c (wsci)*/
/* Allan CORNET le 20/08/03 */
/* voir wsci\wtext.c */
/* & voir xsc/x_screen.c */
extern void HomeFunction(void);
extern int ClearScreenConsole _PARAMS((char *fname));
extern int scisetposfigdim  _PARAMS((char *fname, unsigned long fname_len));
extern int scixname  _PARAMS((char *fname, unsigned long fname_len));
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
extern int scisetposfigdim  _PARAMS((char *fname, unsigned long fname_len));

/* NG beg */
extern int gset _PARAMS((char *fname, unsigned long fname_len));
extern int gget _PARAMS((char *fname, unsigned long fname_len));
extern int delete _PARAMS((char *fname, unsigned long fname_len));
extern int addcb _PARAMS((char *fname, unsigned long fname_len));
extern int copy _PARAMS((char *fname, unsigned long subwinparenttarget));
/*** 18/03/2002 ***/
extern int move _PARAMS((char *fname, unsigned long fname_len));
/*** 19/03/2002 ***/
extern int glue _PARAMS((char *fname, unsigned long fname_len));
extern int unglue _PARAMS((char *fname, unsigned long fname_len));
extern int drawnow _PARAMS((char *fname, unsigned long fname_len));
extern int drawlater _PARAMS((char *fname, unsigned long fname_len));
extern int scixclear _PARAMS((char *fname, unsigned long fname_len));
extern int scixclearsubwin _PARAMS((char *fname, unsigned long fname_len));
extern int scixbascsubwin _PARAMS((char *fname, unsigned long fname_len));

extern int draw _PARAMS((char *fname, unsigned long fname_len));

void Objarc  _PARAMS((int *angle1,int *angle2,double *x,double *y,double *width,double *height,int color,int fill,long *hdl));
void Objdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int subint,char *format,int font,int textcol,int ticscol,char flag,int seg));
void Objfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax));


void Objfpoly  _PARAMS((double *x,double *y,integer rempl,integer closed,long *hdl));
void Objgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
void Objmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
void Objmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect));
void Objnumb _PARAMS((char *fname,unsigned long fname_len,integer n,integer flag,double *x,double *y,double *angle,double *box));
void Objplot2d  _PARAMS((int ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint));

void Objplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox, integer *m1, integer *n1, integer *m2, integer *n2, integer *m3, integer *n3,integer *m3n, integer *n3n));
void Objpoly  _PARAMS((double *x,double *y,integer n,integer closed,int mark,long *hdl));
void Objrect  _PARAMS((double *x,double *y,double *width,double *height,int fillflag,int fillcolor,int n,long *hdl));
void Objsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
void Objstring _PARAMS((char *fname,unsigned long fname_len,integer str,double x,double y,double *angle,double *box,double* wh,int fill, long *hdl));
void Objtitle _PARAMS((char *str,int n,long *hdl));

void Xarc  _PARAMS((char *fname,unsigned long fname_len,int *a1,int *a2,double *x,double *y,double *width,double *height));
void Xarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));

void Xdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int sub_int,char *format,int fontsize,int textcolor,int ticscolor,char flag,int seg_flag));

void Xfarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,int n,double *vect2));
void Xfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax));

void Xfpoly  _PARAMS((integer rempl,integer closed,double *x,double *y));
void Xfpolys  _PARAMS((int *vect,integer val1,integer val2,integer n,integer m,double *x,double *y));
void Xgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
void Xmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
void Xmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect) );
void Xnumb  _PARAMS((char *fname,unsigned long fname_len,integer flag,double *x,double *y,double *angle,double *box));
void Xplot2d  _PARAMS((double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint));
void Xplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox));

void Xpoly  _PARAMS((char *fname,unsigned long fname_len,int n,int close,double *x,double *y));
void Xpolys  _PARAMS((integer *vect,integer n,integer m,double *x,double *y));
void Xrect  _PARAMS((char *fname,unsigned long fname_len,double *x,double *y,double *width,double *height));
void Xrects  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));
void Xsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
void Xstring  _PARAMS((char *fname,unsigned long fname_len,integer str,double x,double y,double angle,double *box));
void Xtitle  _PARAMS((char *str,int n));
/* NG beg */
static int get3ddata(sciPointObj *pthis); /* F.Leray Declaration here*/
static int set3ddata(sciPointObj *pobj, int *value, int *numrow, int *numcol, int colorflag, char *fname);

extern double  sciFindLogMinSPos(double *x, int n);
#endif 


