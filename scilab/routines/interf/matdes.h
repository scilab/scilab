#ifndef MATDES_SCI 
#define MATDES_SCI 

int check_xy __PARAMS((char *fname, char dir, int mn, int xpos, int xm, int xn, long unsigned int xl, int ypos, int ym, int yn, long unsigned int yl, int *ntics));

extern int C2F(matdes)  __PARAMS((void));
extern int scichamp  __PARAMS((char *fname,unsigned long fname_len));
extern int scicontour  __PARAMS((char *fname, unsigned long fname_len));
extern int scicontour2d  __PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d  __PARAMS((char *fname, unsigned long fname_len));
extern int scic2dex  __PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d1  __PARAMS((char *fname, unsigned long fname_len));
extern int scigeom3d  __PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d_G __PARAMS(( char *fname,
			 int (*func) __PARAMS((double *,double *,double *,integer *p,integer *q,
					      double *,double *,char *,integer *,double *,integer)),
			 int (*func1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,
						integer *q,double *,double *,char *,integer *,double *,integer)),
			 int (*func2) __PARAMS((double *,double *,double *,integer *cvect,integer *p,
					       integer *q,double *,double *,char *,integer *,double *,integer)),
			 int (*func3) __PARAMS((double *,double *,double *,integer *cvect,integer *p,
					       integer *q,double *,double *,char *,integer *,double *,integer)),
			 unsigned long fname_len));

extern int sciplot2d  __PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_G  __PARAMS((char *fname,
				  int (*func) __PARAMS((char *,double *,double *,integer *,integer *, integer *,
						       char *, char *, double *,integer *,integer,integer, integer)),
				  unsigned long fname_len));

extern int scigrayplot  __PARAMS((char *fname, unsigned long fname_len));
extern int scigray1plot  __PARAMS((char *fname, unsigned long fname_len));
extern int scigray2plot  __PARAMS((char *fname, unsigned long fname_len));
extern int sci_demo  __PARAMS((char *fname, char *chaine, integer *flag));
extern int scidriver  __PARAMS((char *fname, unsigned long fname_len));
extern int scixarc  __PARAMS((char *fname, unsigned long fname_len));
extern int scixarcs  __PARAMS((char *fname, unsigned long fname_len));
extern int scixfarcs  __PARAMS((char *fname,  unsigned long fname_len));
extern int sciarrows  __PARAMS((char *fname, unsigned long fname_len));
extern int scixsegs  __PARAMS((char *fname, unsigned long fname_len));
extern int scixaxis  __PARAMS((char *fname, unsigned long fname_len));
extern int scixchange  __PARAMS((char *fname, unsigned long fname_len));
extern int entier2d  __PARAMS((integer *n, double *d__, integer *s));
extern int simple2d  __PARAMS((integer *n, double *d__, float *s));
extern int scixclea  __PARAMS((char *fname, unsigned long fname_len));
extern int scixclear  __PARAMS((char *fname, unsigned long fname_len));
extern int scixclick  __PARAMS((char *fname, unsigned long fname_len));
extern int scixend  __PARAMS((char *fname, unsigned long fname_len));
extern int scixgrid  __PARAMS((char *fname, unsigned long fname_len));
extern int scixfpoly  __PARAMS((char *fname, unsigned long fname_len));
extern int scixfpolys  __PARAMS((char *fname, unsigned long fname_len));
extern int scixget  __PARAMS((char *fname, unsigned long fname_len));
extern int scixgetg  __PARAMS((char *fname, unsigned long fname_len));
extern int scixinit  __PARAMS((char *fname, unsigned long fname_len));
extern int scixlfont  __PARAMS((char *fname, unsigned long fname_len));
extern int scixnumb  __PARAMS((char *fname, unsigned long fname_len));
extern int scixpause  __PARAMS((char *fname, unsigned long fname_len));
extern int scixpoly  __PARAMS((char *fname, unsigned long fname_len));
extern int scixpolys  __PARAMS((char *fname, unsigned long fname_len));
extern int scixselect  __PARAMS((char *fname, unsigned long fname_len));
extern int scixset  __PARAMS((char *fname, unsigned long fname_len));
extern int scixsetg  __PARAMS((char *fname, char *str, unsigned long fname_len, unsigned long str_len));
extern int scixstring  __PARAMS((char *fname, unsigned long fname_len));
extern int scixtitle  __PARAMS((char *fname, unsigned long fname_len));
extern int scixstrbi  __PARAMS((char *fname, unsigned long fname_len));
extern int scixstringl  __PARAMS((char *fname, unsigned long fname_len));
extern int scixtape  __PARAMS((char *fname, unsigned long fname_len));
extern int scixinfo  __PARAMS((char *fname, unsigned long fname_len));
extern int scixsetech  __PARAMS((char *fname, unsigned long fname_len));
extern int scixgetech  __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(sciwin)  __PARAMS((void));
extern int scifec  __PARAMS((char *fname, unsigned long fname_len));
extern int scixgetmouse  __PARAMS((char *fname, unsigned long fname_len));
extern int scixsave  __PARAMS((char *fname, unsigned long fname_len));
extern int scixload  __PARAMS((char *fname, unsigned long fname_len));
extern int scidelw  __PARAMS((char *fname, unsigned long fname_len));
extern int scixg2psofig  __PARAMS((char *fname, char *dr, unsigned long fname_len, unsigned long dr_len));
extern int scixsort  __PARAMS((char *fname, unsigned long fname_len));
extern int scissort  __PARAMS((char *fname, unsigned long fname_len));
extern int sciwinsid  __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(scigerase)  __PARAMS((void));
extern int scixgraduate  __PARAMS((char *fname, unsigned long fname_len));
extern int scixname  __PARAMS((char *fname, unsigned long fname_len));

#endif 
