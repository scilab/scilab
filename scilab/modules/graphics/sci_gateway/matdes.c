/*-----------------------------------------------------------------------------------*/
/* Graphic interface */
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY 2003 INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "bcg.h"
#include "stack-c.h"
#include "math_graphics.h"
#include "Graphics.h"
#include "PloEch.h"
#include "CloneObjects.h"
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "intcommongraphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "sci_demo.h"
#include "XsetXgetParameters.h"
#include "Xcall1.h"
#include "gsort.h"


#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/

extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
extern int IsAScalar(int RhsNumber);
extern int C2F(deletewin)(integer *number);
#if _MSC_VER
extern int Interface_XS2BMP(int figurenum,char *filename); /* wgraph.c */
extern int Interface_XS2EMF(int figurenum,char *filename);
#endif
extern void setposfig(integer *i,integer *j);
extern void Objdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int subint,char *format,int font,int textcol,int ticscol,char flag,int seg,int nb_tics_labels));
extern void Xdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int sub_int,char *format,int fontsize,int textcolor,int ticscolor,char flag,int seg_flag));
extern void Objfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax,integer *ExtremesCol, BOOL with_mesh, BOOL flagNax));
extern void Objnumb _PARAMS((char *fname,unsigned long fname_len,integer n,integer flag,double *x,double *y,double *angle,double *box));
extern void Xfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax,integer *ColOut, BOOL with_mesh));
extern void Xfpoly  _PARAMS((integer rempl,integer closed,double *x,double *y));
extern void Xfpolys  _PARAMS((int *vect,integer val1,integer val2,integer n,integer m,double *x,double *y));
extern void Xnumb  _PARAMS((char *fname,unsigned long fname_len,integer flag,double *x,double *y,double *angle,double *box));
extern void Objfpoly  _PARAMS((double *x,double *y,integer rempl,integer *closed,long *hdl, int v1));
extern void seteventhandler(int *win_num,char *name,int *ierr);
extern void ShowScales(void);
extern void Xpolys  _PARAMS((integer *vect,integer n,integer m,double *x,double *y));
extern void Xrect  _PARAMS((char *fname,unsigned long fname_len,double *x,double *y,double *width,double *height));
extern void Objrect  _PARAMS((double *x,double *y,double *width,double *height,int *foreground,int *background,BOOL isfilled,BOOL isline,int n,long *hdl,BOOL flagxtring));
extern int zoom_box(double *bbox,int *x_pixel, int *y_pixel);
extern void unzoom_one_axes(sciPointObj *psousfen);
extern void Xrects  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));
extern void Xstring  _PARAMS((char *fname,unsigned long fname_len,integer str,double x,double y,double angle,double *box));
extern void Xtitle  _PARAMS((char *str,int n));
extern void Objstring _PARAMS((char ** fname,int nbRow,int nbCol,double x,double y,
			       double *angle,double *box, BOOL autoSize, double* userSize, long *hdl, BOOL centerPos,
			       int *foreground,int *background,BOOL isboxed,BOOL isline,
                               BOOL isfilled, sciTextAlignment alignment));
extern void Xpoly  _PARAMS((char *fname,unsigned long fname_len,int n,int close,double *x,double *y));
extern void Objpoly  _PARAMS((double *x,double *y,integer n,integer closed,int mark,long *hdl));
extern void Xsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
extern void Objsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
extern void Xfarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,int n,double *vect2));
extern void Xarc  _PARAMS((char *fname,unsigned long fname_len,int *a1,int *a2,double *x,double *y,double *width,double *height));
extern void Xarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));
extern void Objmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect));
extern void Xgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
extern void Objarc  _PARAMS((int *angle1,int *angle2,double *x,double *y,double *width,double *height,int *foreground,int *background, BOOL isfilled, BOOL isline, long *hdl));
extern void Xmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect) );
extern void Xmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
extern void Objmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint, BOOL flagNax));
extern void Objgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint, BOOL flagNax));
extern void Xplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox));
extern void Objplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,double *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox, integer *m1, integer *n1, integer *m2, integer *n2, integer *m3, integer *n3,integer *m3n, integer *n3n));
extern void Xplot2d  _PARAMS((double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint));
extern void Objplot2d  _PARAMS((int ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint, BOOL flagNax));
/*-----------------------------------------------------------------------------------*/
int sciseteventhandler(char *fname,unsigned long fname_len);
int C2F(scigerase)();
int scicontour2d_G(char *fname,int (*func) (),unsigned long fname_len);
int sciplot2d1_G(char *fname, int ptype,
		 int (*func) __PARAMS((char *,double *,double *,integer *,integer *,
				       integer *,char *,char *,double *,integer *,
				       integer,integer,integer)),unsigned long fname_len);
int sciplot3d_G(char *fname,
		int (*func) __PARAMS((double *,double *,double *,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func2) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func3) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		unsigned long fname_len);
int scichamp_G(char *fname,int (*func) (),unsigned long fname_len);
int scisetposfig(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
char *pmodes[] =
  { 
    "clear", 
    "and" ,
    "andReverse" ,
    "copy", 
    "andInverted" ,
    "noop" ,
    "xor" ,
    "or" ,
    "nor" ,
    "equiv" ,
    "invert" ,
    "orReverse" ,
    "copyInverted" ,
    "orInverted" ,
    "nand" ,
    "set" ,
  };



int cf_type=1; /* used by gcf to determine if current figure is a graphic (1) or a tclsci (0) one */

static integer one = 1, zero = 0;
static char error_message[70];
static int * Style;

static double *Rect ; 

static char *Strf ; 

static char *Legend ; 

static double *Zminmax ; 

static int *Nax;
static BOOL flagNax = FALSE; /* F.Leray : to know weither or not we use Nax */

static int *Colminmax;

static int *ColOut;

static BOOL WithMesh;

static char *Logflags; 
static char logflags[3];

extern int xinitxend_flag;

extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;

/*-----------------------------------------------------------------------------------*/
/* Graphic subroutines interface */
/*-----------------------------------------------------------------------------------*/





/*-----------------------------------------------------------------------------------*/
/**
 * Method called for relocating handles. The input should contains a vector of handles
 * which will be relocated and a single handle which is the new parent.
 */
int sciRelocateHandle( char * fname, unsigned long fname_len )
{
  int handleCol ;
  int handleRow ;
  int nbHandle ;
  int handleStkIndex ;
  int parentCol ;
  int parentRow ;
  int parentStkIndex ;
  int outIndex ;
  int i ;
  unsigned long * handleArray = NULL ;

  /* the function should be called with relocate_handle( handle, parent_handle ) */
  CheckRhs(2,2) ;
  CheckLhs(0,1) ;

  
  GetRhsVar( 1, "h", &handleRow, &handleCol, &handleStkIndex ) ;
  nbHandle = handleRow * handleCol ;
  GetRhsVar( 2, "h", &parentRow, &parentCol, &parentStkIndex ) ;

  if ( parentCol * parentRow != 1 )
  {
    Scierror(999,"%s :Handles must be relocated under a single parent.\r\n",fname);
    return 0 ;
  }
  
  /* create an array of handles */
  handleArray = MALLOC( nbHandle * sizeof( unsigned long ) ) ;
  if ( handleArray == NULL )
  {
    Scierror(999,"Memory full, aborting operation.\r\n",fname);
    return 0 ;
  }

  for ( i = 0 ; i < nbHandle ; i++ )
  {
    handleArray[i] = (unsigned long) *hstk( handleStkIndex + i ) ;
  }
  
  if ( sciRelocateHandles( handleArray          ,
                           handleRow * handleCol,
                           (unsigned long) *hstk( parentStkIndex ) ) != 0 )
  {
    return 0 ;
  }
  FREE( handleArray ) ;
  CreateVar( Rhs + 1, "h", &handleCol, &handleRow, &outIndex ) ;
  *hstk(outIndex) = *hstk(handleStkIndex) ;
  LhsVar(1) = Rhs + 1 ;
  return 0 ;

}
/*-----------------------------------------------------------------------------------*/
/**
 * This method is called to swap two handles position in the hierarchy.
 * The input should be two single handle which will be swaped.
 */
int sciSwapHandles( char * fname, unsigned long fname_len )
{
  int firstHdlCol  ;
  int firstHdlRow  ;
  int secondHdlCol ;
  int secondHdlRow ;
  int firstHdlStkIndex  ;
  int secondHdlStkIndex ;

  CheckRhs( 2, 2 ) ;
  CheckLhs( 0, 1 ) ;

  GetRhsVar( 1, "h", &firstHdlRow, &firstHdlCol, &firstHdlStkIndex ) ;
  GetRhsVar( 2, "h", &secondHdlRow, &secondHdlCol, &secondHdlStkIndex ) ;

  if ( firstHdlRow * firstHdlCol != 1 || secondHdlRow * secondHdlCol != 1 )
  {
    Scierror(999,"%s : Routine can only swap two single handles.\r\n",fname);
    return 0 ;
  }

  /* get the two handles and swap them */
  swapHandles( (unsigned long) *hstk( firstHdlStkIndex  ),
               (unsigned long) *hstk( secondHdlStkIndex ) ) ;
  LhsVar(1) = 0 ;
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
