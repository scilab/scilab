/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * save graphic code in a file  
 * --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include "Math.h"
#include "Rec.h"

extern void  xdrstdio_create();

static int SaveTPlot __PARAMS((char *type, char *plot));
static int SaveD  __PARAMS((double x));
static int SaveLI __PARAMS((integer ix));
static int SaveC __PARAMS((char *c, integer lc));
static int SaveVectLI __PARAMS((integer *nx, integer l));
static int SaveVectF __PARAMS((double *nx, integer l)); 
static int SaveVectC __PARAMS((char *nx, int l));
static int SaveVectS   __PARAMS((char **nx));
static int SaveColormap __PARAMS((void));
static int SaveEch  __PARAMS((char *)); 
static int SavePlot  __PARAMS((char *)); 
static int SaveSciAxis  __PARAMS((char *)); 
static int SaveGrid  __PARAMS((char *)); 
static int SaveParam3D  __PARAMS((char *)); 
static int SaveParam3D1  __PARAMS((char *)); 
static int SavePlot3D  __PARAMS((char *)); 
static int SaveFac3D  __PARAMS((char *)); 
static int SaveFec  __PARAMS((char *)); 
static int SaveContour  __PARAMS((char *)); 
static int SaveXcall1  __PARAMS((char *)); 
static int SaveContour2D  __PARAMS((char *)); 
static int SaveGray  __PARAMS((char *)); 
static int SaveChamp  __PARAMS((char *)); 

/*---------------------------------------------------------------------
 * save plots using xdr 
 * exproted function int C2F(xsaveplots) __PARAMS((integer *winnumber,char *,integer lxv)); 
 *---------------------------------------------------------------------------*/

static int SaveXcall1(char *plot)
{
  struct xcall1_rec *lplot = (struct xcall1_rec *) plot;
  if ( SaveVectC((lplot->fname),((int)strlen(lplot->fname))+1) == 0) return(0);
  if ( SaveVectC((lplot->string),((int)strlen(lplot->string))+1) == 0) return(0);
  if ( SaveLI(lplot->n1 )== 0) return(0);
  if ( SaveLI(lplot->n2 )== 0) return(0);
  if ( SaveLI(lplot->n3 )== 0) return(0);
  if ( SaveLI(lplot->n4 )== 0) return(0);
  if ( SaveLI(lplot->n5 )== 0) return(0);
  if ( SaveLI(lplot->n6 )== 0) return(0);
  if ( SaveLI(lplot->ndx1 )== 0) return(0);
  if ( SaveLI(lplot->ndx2 )== 0) return(0);
  if ( SaveLI(lplot->ndx3 )== 0) return(0);
  if ( SaveLI(lplot->ndx4 )== 0) return(0);
  if ( SaveVectLI((lplot->x1),lplot->n1) == 0) return(0);
  if ( SaveVectLI((lplot->x2),lplot->n2) == 0) return(0);
  if ( SaveVectLI((lplot->x3),lplot->n3) == 0) return(0);
  if ( SaveVectLI((lplot->x4),lplot->n4) == 0) return(0);
  if ( SaveVectLI((lplot->x5),lplot->n5) == 0) return(0);
  if ( SaveVectLI((lplot->x6),lplot->n6) == 0) return(0);
  if ( SaveVectF((lplot->dx1),lplot->ndx1) == 0) return(0);
  if ( SaveVectF((lplot->dx2),lplot->ndx2) == 0) return(0);
  if ( SaveVectF((lplot->dx3),lplot->ndx3) == 0) return(0);
  if ( SaveVectF((lplot->dx4),lplot->ndx4) == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * scale 
 * ---------------------------------------------------------------------------*/

static int SaveEch(char *plot)
{
  struct scale_rec *lplot = (struct scale_rec *) plot;
  if ( SaveC(lplot->logflag,2L)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->Wrect),4L) == 0) return(0);
  if ( SaveVectF((lplot->Frect),4L) == 0) return(0);
  if ( SaveVectF((lplot->Frect_kp),4L) == 0) return(0);
  return(1);
}

static int SaveNEch(char *plot)
{
  struct nscale_rec *lplot = (struct nscale_rec *) plot;
  if ( SaveC(lplot->logflag,2L)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectC((lplot->flag),((int)strlen(lplot->flag))+1) == 0) return(0);
  if ( SaveVectF((lplot->Wrect),4L) == 0) return(0);
  if ( SaveVectF((lplot->Frect),4L) == 0) return(0);
  if ( SaveVectF((lplot->Arect),4L) == 0) return(0);
  if ( SaveVectF((lplot->Frect_kp),4L) == 0) return(0);
  return(1);
}
  
/*---------------------------------------------------------------------
 * plot2d 
 *---------------------------------------------------------------------------*/

static int SavePlot(char *plot)
{
  integer n=0, nstyle;
  struct plot2d_rec *lplot = (struct plot2d_rec *) plot;
  if (lplot->n1==1 ) nstyle= lplot->n1+1;else nstyle= lplot->n1;
  switch (lplot->xf[0])
    {
    case 'g': n=(lplot->n1)*(lplot->n2);break;
    case 'e': n=0;break;
    case 'o': n=(lplot->n2);break;
    }
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectC((lplot->xf),((int)strlen(lplot->xf))+1) == 0) return(0);
  if ( ((n == 0) ? 1 : SaveVectF((lplot->x),n)) == 0) return(0);
  if ( SaveVectF((lplot->y),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectLI((lplot->style),nstyle) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->legend),((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectF((lplot->brect),4L) == 0) return(0);
  if ( SaveVectF((lplot->brect_kp),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aint),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aint_kp),4L) == 0) return(0);
  return(1);
}
  
/*---------------------------------------------------------------------
 * axis 
 *---------------------------------------------------------------------------*/

static int SaveSciAxis(char *plot)
{
  struct sciaxis_rec *lplot = (struct sciaxis_rec *) plot;
  char type[3] ;
  type[0] = lplot->pos;
  type[1] = lplot->xy_type;
  type[2] = lplot->logflag;
  if ( SaveC(type,3L)== 0) return(0);
  if ( SaveLI(lplot->nx)==0) return(0);
  if ( SaveLI(lplot->ny)==0) return(0);
  if ( SaveLI(lplot->subtics)==0) return(0);
  if ( SaveLI(lplot->fontsize)==0) return(0);
  if ( SaveLI(lplot->textcolor)==0) return(0);
  if ( SaveLI(lplot->ticscolor)==0) return(0);
  if ( SaveLI(lplot->seg_flag)==0) return(0);
  if ( SaveLI(lplot->f_l)==0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( lplot->f_l == 1) { if ( SaveVectC((lplot->format),((int)strlen(lplot->format))+1) == 0) return(0);} 
  if ( SaveVectF(lplot->x,lplot->nx) == 0) return(0);
  if ( SaveVectF(lplot->y,lplot->ny) == 0) return(0);
  if ( SaveVectS(lplot->str) == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * xgrid 
 *---------------------------------------------------------------------------*/

static int SaveGrid(char *plot)
{
  struct xgrid_rec *lplot = (struct xgrid_rec *) plot;
  if ( SaveLI(lplot->style)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1)== 0) return(0);
  return(1);
}


/*---------------------------------------------------------------------
 * param3d 
 *---------------------------------------------------------------------------*/

static int SaveParam3D(char *plot)
{
  struct param3d_rec *lplot = (struct param3d_rec *) plot;
  if ( SaveLI(lplot->n)== 0) return(0);
  if ( SaveD(lplot->teta)== 0) return(0);
  if ( SaveD(lplot->alpha)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->n) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->n) == 0) return(0);
  if ( SaveVectF((lplot->z),lplot->n) == 0) return(0);
  if ( SaveVectC((lplot->legend), ((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectLI((lplot->flag),3L) == 0) return(0);
  if ( SaveVectF((lplot->bbox),6L)== 0) return(0);
  return(1);
}

static int SaveParam3D1(char *plot)
{
  struct param3d1_rec *lplot = (struct param3d1_rec *) plot;
  if ( SaveLI(lplot->m)== 0) return(0);
  if ( SaveLI(lplot->n)== 0) return(0);
  if ( SaveLI(lplot->iflag)== 0) return(0);
  if ( lplot->iflag == 1) 
    if ( SaveVectLI((lplot->colors),lplot->n) == 0) return(0);
  if ( SaveD(lplot->teta)== 0) return(0);
  if ( SaveD(lplot->alpha)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->n*lplot->m) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->n*lplot->m) == 0) return(0);
  if ( SaveVectF((lplot->z),lplot->n*lplot->m) == 0) return(0);
  if ( SaveVectC((lplot->legend), ((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectLI((lplot->flag),3L) == 0) return(0);
  if ( SaveVectF((lplot->bbox),6L)== 0) return(0);
  return(1);
}


/*---------------------------------------------------------------------
 * plot3d  
 *---------------------------------------------------------------------------*/

static int SavePlot3D(char *plot)
{
  struct plot3d_rec *lplot = (struct plot3d_rec *) plot;
  if ( SaveLI(lplot->p)== 0) return(0);
  if ( SaveLI(lplot->q)== 0) return(0);
  if ( SaveD(lplot->teta)== 0) return(0);
  if ( SaveD(lplot->alpha)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->p) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->q) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->p)*(lplot->q)) == 0) return(0);
  if ( SaveVectC((lplot->legend), ((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectLI((lplot->flag),3L) == 0) return(0);
  if ( SaveVectF((lplot->bbox),6L)== 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * fac3d 
 *---------------------------------------------------------------------------*/

static int SaveFac3D(char *plot)
{
  struct fac3d_rec *lplot = (struct fac3d_rec *) plot;
  if ( SaveLI(lplot->p)== 0) return(0);
  if ( SaveLI(lplot->q)== 0) return(0);
  if ( SaveD(lplot->teta)== 0) return(0);
  if ( SaveD(lplot->alpha)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( strcmp(lplot->name,"fac3d2")==0) 
    if ( SaveVectLI((lplot->cvect),(lplot->q)) == 0) return(0);
  if ( strcmp(lplot->name,"fac3d3")==0) { /****** added by polpoth 4/5/2000 ******/
    if ( SaveVectLI((lplot->cvect),(lplot->p)*(lplot->q)) == 0) return(0); /****** added by polpoth 4/5/2000 ******/
  }
  if ( SaveVectF((lplot->x),(lplot->p)*(lplot->q)) == 0) return(0);
  if ( SaveVectF((lplot->y),(lplot->p)*(lplot->q)) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->p)*(lplot->q)) == 0) return(0);
  if ( SaveVectC((lplot->legend), ((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectLI((lplot->flag),3L) == 0) return(0);
  if ( SaveVectF((lplot->bbox),6L)== 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * fec 
 *---------------------------------------------------------------------------*/

static int SaveFec(char *plot)
{
  struct fec_rec *lplot = (struct fec_rec *) plot;
  if ( SaveLI(lplot->Nnode)== 0) return(0);
  if ( SaveLI(lplot->Ntr)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->Nnode) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->Nnode) == 0) return(0);
  if ( SaveVectF((lplot->triangles),(lplot->Ntr)*5) == 0) return(0);
  if ( SaveVectF((lplot->func),lplot->Nnode ) == 0) return(0);
  if ( SaveVectF((lplot->brect),4L) == 0) return(0);
  if ( SaveVectF((lplot->brect_kp),4L) == 0) return(0);
  if ( SaveVectF((lplot->zminmax),2L) == 0) return(0);   /* added by bruno */
  if ( SaveVectLI((lplot->colminmax),2L) == 0) return(0);/* ============== */
  if ( SaveVectLI((lplot->aaint),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aaint_kp),4L) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->legend),((int)strlen(lplot->legend))+1)  == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * contour 
 *---------------------------------------------------------------------------*/

static int SaveContour(char *plot)
{
  struct contour_rec *lplot = (struct contour_rec *) plot;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveLI(lplot->nz)== 0) return(0);
  if ( SaveLI(lplot->flagnz)== 0) return(0);
  if (lplot->flagnz != 0) 
    if ( SaveVectF((lplot->zz),lplot->nz) == 0) return(0);
  if ( SaveD(lplot->teta)== 0) return(0);
  if ( SaveD(lplot->alpha)== 0) return(0);
  if ( SaveD(lplot->zlev)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->n1) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->n2) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectC((lplot->legend), ((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectLI((lplot->flag),3L) == 0) return(0);
  if ( SaveVectF((lplot->bbox),6L) == 0) return(0);
  return(1);
}

static int SaveContour2D(char *plot)
{
  integer nstyle;
  struct contour2d_rec *lplot = (struct contour2d_rec *) plot;
  if (lplot->n1==1 ) nstyle= lplot->n1+1;else nstyle= lplot->n1;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveLI(lplot->nz)== 0) return(0);
  if ( SaveLI(lplot->flagnz)== 0) return(0);
  if (lplot->flagnz != 0) 
    if ( SaveVectF((lplot->zz),lplot->nz) == 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->n1) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->n2) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectLI((lplot->style),nstyle) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->legend),((int)strlen(lplot->legend))+1) == 0) return(0);
  if ( SaveVectF((lplot->brect),4L) == 0) return(0);
  if ( SaveVectF((lplot->brect_kp),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aint),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aint_kp),4L) == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * xgray 
 *---------------------------------------------------------------------------*/

static int SaveGray(char *plot)
{
  struct gray_rec *lplot = (struct gray_rec *) plot;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),lplot->n1) == 0) return(0);
  if ( SaveVectF((lplot->y),lplot->n2) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectF((lplot->brect),4L) == 0) return(0);
  if ( SaveVectF((lplot->brect_kp),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aaint),4L)  == 0) return(0);
  if ( SaveVectLI((lplot->aaint_kp),4L) == 0) return(0);
  return(1);
}

static int SaveGray1(char *plot)
{
  struct gray_rec *lplot = (struct gray_rec *) plot;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectF((lplot->brect),4L) == 0) return(0);
  if ( SaveVectF((lplot->brect_kp),4L) == 0) return(0);
  if ( SaveVectLI((lplot->aaint),4L)  == 0) return(0);
  if ( SaveVectLI((lplot->aaint_kp),4L) == 0) return(0);
  return(1);
}

static int SaveGray2(char *plot)
{
  struct gray_rec_2 *lplot = (struct gray_rec_2 *) plot;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->z),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectF((lplot->xrect),4L) == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * champ 
 *---------------------------------------------------------------------------*/

static int SaveChamp(char *plot)
{
  struct champ_rec *lplot = (struct champ_rec *) plot;
  if ( SaveLI(lplot->n1)== 0) return(0);
  if ( SaveLI(lplot->n2)== 0) return(0);
  if ( SaveD(lplot->arfact)== 0) return(0);
  if ( SaveVectC((lplot->name),((int)strlen(lplot->name))+1) == 0) return(0);
  if ( SaveVectF((lplot->x),(lplot->n1)) == 0) return(0);
  if ( SaveVectF((lplot->y),(lplot->n2)) == 0) return(0);
  if ( SaveVectF((lplot->fx),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectF((lplot->fy),(lplot->n1)*(lplot->n2)) == 0) return(0);
  if ( SaveVectC((lplot->strflag),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectC((lplot->strflag_kp),((int)strlen(lplot->strflag))+1) == 0) return(0);
  if ( SaveVectF((lplot->vrect),4L) == 0) return(0);
  if ( SaveVectF((lplot->vrect_kp),4L)== 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * Saving  
 *---------------------------------------------------------------------------*/

#ifdef macintosh
#	include "types.h"
#else /* not macintosh */
#       ifndef VMS
#	ifndef __ABSC__
#   	include <sys/types.h>	/* for <netinet/in.h> on some systems */
#	endif
#   	if (!defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__) 
#          include <netinet/in.h>	/* for htonl() */
#   	endif
#	endif
#endif /* not macintosh */


#define assert(ex) {if (!(ex)){ sciprint("Graphic Load/Save Error \r\n");return(0);}} 

#ifdef WIN32 
#include "../xdr/rpc/types.h"
#include "../xdr/rpc/xdr.h"
#else 
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

static char fname[128];
static FILE *F ;
static XDR xdrs[1] ;
static u_int count ;
static u_int szof ;

typedef  struct  {
  char *name;
  int  (*Save)();} SaveTable;

static SaveTable SaveCTable[] ={
  {"axis", SaveSciAxis},
  {"champ",SaveChamp},
  {"contour",SaveContour},
  {"contour2",SaveContour2D},
  {"fac3d",SaveFac3D},
  {"fac3d1",SaveFac3D},
  {"fac3d2",SaveFac3D},
  {"fac3d3",SaveFac3D},/****** entry added by polpoth 4/5/2000 ******/
  {"fec_n",SaveFec},
  {"gray",SaveGray},
  {"gray1",SaveGray1},
  {"gray2",SaveGray2},
  {"nscale",SaveNEch},
  {"param3d",SaveParam3D},
  {"param3d1",SaveParam3D1},
  {"plot2d",SavePlot},
  {"plot3d",SavePlot3D},
  {"plot3d1",SavePlot3D},
  {"scale",SaveEch},
  {"xcall1",SaveXcall1},
  {"xgrid",SaveGrid},
  {(char *)NULL,NULL}};

#ifdef __MSC__
#define __STDC__
#endif 

extern int version_flag(void); /* NG */

int C2F(xsaveplots)(integer *winnumber, char *fname1, integer lxv)
{
  static char endplots[]={"endplots"};
  static char scig[]={"SciG1.1"};
  struct listplot *list;
  int verb=0,cur,na;


  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",winnumber,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (version_flag() == 0) {
    sciprint("New style plot save is not yet implemented\n");
    C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  else {

#ifdef lint 
  *unused;
#endif
  strncpy(fname,fname1,128);
#ifdef __STDC__
  F = fopen(fname,"wb") ;
#else
  F = fopen(fname,"w") ;
#endif
  if( F == NULL)
    {
      sciprint("fopen failed\r\n") ;
      return(0);
    }
  xdrstdio_create(xdrs, F, XDR_ENCODE) ;
  SaveVectC(scig,((int)strlen(scig))+1) ;
  list=ListPFirst;
  if ( SaveColormap() == 0) 
    {
      sciprint("save: saving colormap failed\r\n") ;
      return(0);
    }
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	if ( SaveTPlot(list->type,list->theplot) == 0)
	  break;
      list =list->ptrplot;
    }
  SaveVectC(endplots,((int)strlen(endplots))+1) ;
  assert(fflush((FILE *)xdrs->x_private) != EOF) ; 
  assert(fclose(F) != EOF) ;
  }
  return(0);
}

static int SaveTPlot(char *type, char *plot)
{
  int i=0;
  while (SaveCTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,SaveCTable[i].name);
       if ( j == 0 ) 
	 { 
	   if (SaveVectC(type,((int)strlen(type))+1) == 0) return(0);
	   return( (*(SaveCTable[i].Save))(plot));
	 }
       else 
	 { if ( j <= 0)
	     {
	       /** sciprint("Unknown Plot type <%s>\r\n",type); **/
	       return(0);
	     }
	   else { i++; }
	 }
     }
  return(0);
}

static int SaveD(double x)
{
  szof = sizeof(double) ;
  count = 1;
  assert( xdr_vector(xdrs, (char *) &x, count, szof, (xdrproc_t) xdr_double)) ;
  return(1);
}

static int SaveF(float x)
{
  double z=x;
  SaveD(z);
  /** sciprint("saving %f\r\n",z); **/
  /** 
  szof = sizeof(float) ;
  count = 1;
  assert( xdr_vector(xdrs, (char *) &x, count, szof, (xdrproc_t) xdr_float)) ;
  sciprint("saving %f\r\n",x);
  **/
  return(1);
}

static int SaveLI(integer ix)
{
  szof = sizeof(int) ;
  count = 1;
  assert( xdr_vector(xdrs, (char *)&ix, count, szof, (xdrproc_t) xdr_int)) ;
  return(1);
}

static int SaveC(char *c, integer lc)
{
  szof = lc*sizeof(char);
  assert( xdr_vector(xdrs,(char *) &szof,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  assert( xdr_opaque(xdrs,c,szof));
  return(1);
}

/**  unused 

static int SaveVectI(nx,l)
     int *nx;
     integer l;
{ 
  int nx1=1;
  szof = sizeof(int) ;
  count = (int) l;
  assert( xdr_vector(xdrs,(char *) &count,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  if ( nx == (int *) NULL && l == (integer) 1)
    {
      assert( xdr_vector(xdrs, (char *)&nx1, count, szof, (xdrproc_t) xdr_int));
    }
  else 
    {
      assert( xdr_vector(xdrs, (char *)nx, count, szof, (xdrproc_t) xdr_int)) ;
    }
  return(1);
}

**/

static int SaveVectLI(integer *nx, integer l)
{ 
  integer nx1=1;
  /** Attention integer peut etre un long int **/
  szof = sizeof(int) ;
  count = (int) l;
  assert( xdr_vector(xdrs,(char *) &count,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  if ( nx == (integer  *) NULL && l == (integer) 1)
    {
      assert( xdr_vector(xdrs, (char *)&nx1, count, szof, (xdrproc_t) xdr_int)) ;
    }
  else
    {
      assert( xdr_vector(xdrs, (char *)nx, count, szof, (xdrproc_t) xdr_int)) ;
    }
  return(1);
}

static int SaveVectF(double *nx, integer l)
{
  double nx1=0.0;
  szof = sizeof(double) ;
  count = (int) l;
  assert( xdr_vector(xdrs,(char *) &count,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  if ( nx == (double  *) NULL && l == (integer) 1)
    { assert( xdr_vector(xdrs, (char *)&nx1, count, szof, (xdrproc_t) xdr_double)) ; } 
  else
    { assert( xdr_vector(xdrs, (char *)nx, count, szof, (xdrproc_t) xdr_double)) ; } 
  return(1);
}

static int SaveVectC(char *nx, int l)
{ 
  char nx1='1';
  szof = l*sizeof(char);
  assert( xdr_vector(xdrs,(char *) &szof,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  if ( nx == (char  *) NULL && l == (integer) 1)
    { assert( xdr_opaque(xdrs, &nx1,szof)); } 
  else 
    { assert( xdr_opaque(xdrs, nx,szof)); }
  return(1);
}

static int SaveVectS(char **nx)
{
  int scount = 0,i;
  if (nx != NULL) { while ( nx[scount] != NULL) scount++;} 
  /* save the number of strings */
  count = scount;
  assert( xdr_vector(xdrs,(char *) &count,(unsigned)1,(unsigned) sizeof(unsigned), (xdrproc_t) xdr_u_int)) ;
  /* now save each string */ 
  for ( i=0; i < scount ; i++) 
    {
      if ( SaveVectC( nx[i], strlen(nx[i])+1) == 0 ) return 0;
    }
  return 1;
}

/** save the colormap if necessary **/

static int SaveColormap(void)
{
  int m;
  /** If the X window exists we check its colormap **/
  if (  CheckColormap(&m) == 1) 
    { 
      int i;
      float r,g,b;
      if ( SaveVectC("colormap",((int)strlen("colormap"))+1) == 0) return(0);
      if ( SaveLI(m)== 0) return(0);
      for ( i=0; i < m ; i++)
	{
	  get_r(i,&r);
	  if ( SaveF(r) == 0) return(0);
	}
      for ( i=0; i < m ; i++) 
	{
	  get_g(i,&g);
	  if ( SaveF(g) == 0) return(0);
	}
      for ( i=0; i < m; i++)
	{
	  get_b(i,&b);
	  if ( SaveF(b) == 0) return(0);
	}
    }
  return(1);
}

