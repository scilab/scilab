/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * exported function : int C2F(xloadplots) (fname1,lvx)
 * --------------------------------------------------------------------------*/

#include <string.h> 

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef macintosh
#	include "types.h"
#else /* not macintosh */
#       ifndef VMS
#	ifndef __ABSC__
#   	include <sys/types.h>	/* for <netinet/in.h> on some systems */
#       endif
#   	if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
#          include <netinet/in.h>	/* for htonl() */
#   	endif
#	endif
#endif /* not macintosh */

#ifdef WIN32 
#include "../xdr/rpc/types.h"
#include "../xdr/rpc/xdr.h"
#else 
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#include <stdio.h>
#include "Math.h"
#include "Rec.h"

#define assert(ex) {if (!(ex)){ sciprint("Graphic Load/Save Error \r\n");return(0);}}

extern void  xdrstdio_create();

static char *SciF_version;
static int LoadVectC(),LoadLI(),LoadVectLI(), LoadVectF(),LoadC(),LoadD();
static int LoadTPlot();
static int LoadVectS   __PARAMS((char ***nx));

static int LoadPlot  __PARAMS((void)); 
static int LoadSciAxis  __PARAMS((void)); 
static int LoadGrid __PARAMS((void)); 
static int LoadParam3D __PARAMS((void)); 
static int LoadParam3D1 __PARAMS((void)); 
static int LoadPlot3D __PARAMS((void)); 
static int LoadFac3D __PARAMS((void)); 
static int LoadFec __PARAMS((void)); 
static int LoadContour __PARAMS((void)); 
static int LoadContour2D __PARAMS((void)); 
static int LoadGray __PARAMS((void)); 
static int LoadChamp __PARAMS((void)); 
static int LoadXcall1 __PARAMS((void)); 
static int LoadEch __PARAMS((void)); 

extern void C2F(syncexec)(char * str, int *ns, int *ierr, int *seq);
extern int version_flag();

/*---------------------------------------------------------------------
 * reload a xcall1 statement 
 *---------------------------------------------------------------------------*/

static int LoadXcall1()
{
  struct xcall1_rec *lplot ;
  lplot= ((struct xcall1_rec *) MALLOC(sizeof(struct xcall1_rec)));
  if (lplot != NULL)
    {
      if (LoadVectC(&(lplot->fname)) == 0) return(0);
      if (LoadVectC(&(lplot->string)) == 0) return(0);
      if (LoadLI(&lplot->n1 ) == 0) return(0);
      if (LoadLI(&lplot->n2 ) == 0) return(0);
      if (LoadLI(&lplot->n3 ) == 0) return(0);
      if (LoadLI(&lplot->n4 ) == 0) return(0);
      if (LoadLI(&lplot->n5 ) == 0) return(0);
      if (LoadLI(&lplot->n6 ) == 0) return(0);
      if (LoadLI(&lplot->ndx1 ) == 0) return(0);
      if (LoadLI(&lplot->ndx2 ) == 0) return(0);
      if (LoadLI(&lplot->ndx3 ) == 0) return(0);
      if (LoadLI(&lplot->ndx4 ) == 0) return(0);
      if (LoadVectLI(&(lplot->x1)) == 0) return(0);
      if (LoadVectLI(&(lplot->x2)) == 0) return(0);
      if (LoadVectLI(&(lplot->x3)) == 0) return(0);
      if (LoadVectLI(&(lplot->x4)) == 0) return(0);
      if (LoadVectLI(&(lplot->x5)) == 0) return(0);
      if (LoadVectLI(&(lplot->x6)) == 0) return(0);
      if (LoadVectF(&(lplot->dx1)) == 0) return(0);
      if (LoadVectF(&(lplot->dx2)) == 0) return(0);
      if (LoadVectF(&(lplot->dx3)) == 0) return(0);
      if (LoadVectF(&(lplot->dx4)) == 0) return(0);
      if (Store("xcall1",(char *) lplot) == 0) return(0);
    }
  else 
    {
      Scistring("\nLoad Plot (xcall1): No more place \n");
      return(0);
    }
  return(1);
}

/*---------------------------------------------------------------------
 * scales 
 *---------------------------------------------------------------------------*/

static int LoadEch()
{
  struct scale_rec *lplot;
  lplot= ((struct scale_rec *) MALLOC(sizeof(struct scale_rec)));
  if (lplot != NULL)
    {
      if (LoadC(lplot->logflag) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->Wrect)) == 0) return(0);
      if (LoadVectF(&(lplot->Frect)) == 0) return(0);
      if (LoadVectF(&(lplot->Frect_kp))   == 0) return(0);
      if (Store("scale",(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (Ech): No more place \n");
      return(0);
    }
  return(1);
}

static int LoadNEch()
{
  struct nscale_rec *lplot;
  lplot= ((struct nscale_rec *) MALLOC(sizeof(struct nscale_rec)));
  if (lplot != NULL)
    {
      if (LoadC(lplot->logflag) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectC(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->Wrect)) == 0) return(0);
      if (LoadVectF(&(lplot->Frect)) == 0) return(0);
      if (LoadVectF(&(lplot->Arect)) == 0) return(0);
      if (LoadVectF(&(lplot->Frect_kp))   == 0) return(0);
      if (Store("nscale",(char *) lplot) == 0) return(0);
    }
  else 
    {
      Scistring("\nLoad Plot (Ech): No more place \n");
      return(0);
    }
  return(1);
}

/*---------------------------------------------------------------------
 * 2D plots  
 *---------------------------------------------------------------------------*/

static int LoadPlot()
{
  integer n=0, nstyle;
  struct plot2d_rec *lplot;
  lplot= ((struct plot2d_rec *) MALLOC(sizeof(struct plot2d_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectC(&(lplot->xf)) == 0) return(0);
      if (lplot->n1==1 ) nstyle= lplot->n1+1;else nstyle= lplot->n1;
      switch (lplot->xf[0])
	{
	case 'g': n=(lplot->n1)*(lplot->n2);break;
	case 'e': n=0;break;
	case 'o': n=(lplot->n2);break;
	}
      if ( n != 0) 
	{
	  if (LoadVectF(&(lplot->x)) == 0) return(0);
	}
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectLI(&(lplot->style)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectLI(&(lplot->aint)) == 0) return(0);
      if (LoadVectLI(&(lplot->aint_kp)) == 0) return(0);
      if (Store("plot2d",(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (plot): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 * xgrid 
 *---------------------------------------------------------------------------*/

static int LoadSciAxis()
{
  char type[3] ;
  struct sciaxis_rec *lplot;
  if ((lplot= ((struct sciaxis_rec *) MALLOC(sizeof(struct sciaxis_rec)))) == NULL) 
    {
      Scistring("\nLoad Plot (grid): No more place \n");
      return(0);
    }
  if ( LoadC(type)== 0) return(0);
  if ( LoadLI(&lplot->nx)==0) return(0);
  if ( LoadLI(&lplot->ny)==0) return(0);
  if ( LoadLI(&lplot->subtics)==0) return(0);
  if ( LoadLI(&lplot->fontsize)==0) return(0);
  if ( LoadLI(&lplot->textcolor)==0) return(0);
  if ( LoadLI(&lplot->ticscolor)==0) return(0);
  if ( LoadLI(&lplot->seg_flag)==0) return(0);
  if ( LoadLI(&lplot->f_l)==0) return(0);
  if (LoadVectC(&(lplot->name)) == 0) return(0);
  if ( lplot->f_l == 1) 
    {
      if ( LoadVectC( &(lplot->format)) == 0) return(0);
    }
  else
    lplot->format =0;
  if ( LoadVectF(&lplot->x) == 0) return(0);
  if ( LoadVectF(&lplot->y) == 0) return(0);
  if ( LoadVectS(&lplot->str) == 0) return(0);
  
  lplot->pos =  type[0] ;
  lplot->xy_type =  type[1];
  lplot->logflag  = type[2];
  if (Store("axis",(char *) lplot) == 0) return(0);
  return(1);
}

/*---------------------------------------------------------------------
 * xgrid 
 *---------------------------------------------------------------------------*/

static int LoadGrid()
{ 
  struct xgrid_rec *lplot ;
  lplot= ((struct xgrid_rec *) MALLOC(sizeof(struct xgrid_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->style) == 0) return(0);
      if (LoadVectC(&(lplot->name))== 0) return(0);
      if (Store("xgrid",(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (grid): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 * param3d 
 *---------------------------------------------------------------------------*/

static int LoadParam3D()
{
  struct param3d_rec *lplot;
  lplot= ((struct param3d_rec *) MALLOC(sizeof(struct param3d_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n) == 0) return(0);
      if (LoadD(&lplot->teta) ==0) return(0);
      if (LoadD(&lplot->alpha) ==0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectLI(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->bbox))== 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else
    {
      Scistring("\nLoad Plot (param3d): No more place \n");
      return(0);
    }
  return(1);
}

static int LoadParam3D1()
{
  struct param3d1_rec *lplot;
  lplot= ((struct param3d1_rec *) MALLOC(sizeof(struct param3d1_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->m) == 0) return(0);
      if (LoadLI(&lplot->n) == 0) return(0);
      if (LoadLI(&lplot->iflag) == 0) return(0);
      if (lplot->iflag == 1) 
	if (LoadVectLI(&(lplot->colors)) == 0) return(0);
      if (LoadD(&lplot->teta) ==0) return(0);
      if (LoadD(&lplot->alpha) ==0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectLI(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->bbox))== 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else
    {
      Scistring("\nLoad Plot (param3d1): No more place \n");
      return(0);
    }
  return(1);
}

/*---------------------------------------------------------------------
 * plot3d 
 *---------------------------------------------------------------------------*/

static int LoadPlot3D()
{
  struct plot3d_rec *lplot ;
  lplot= ((struct plot3d_rec *) MALLOC(sizeof(struct plot3d_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->p) == 0) return(0);
      if (LoadLI(&lplot->q) == 0) return(0);
      if (LoadD(&lplot->teta) == 0) return(0);
      if (LoadD(&lplot->alpha) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectLI(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->bbox))== 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (plot3d): No more place \n");
      return(0);
    }
  return(1);
}

/*---------------------------------------------------------------------
 * fac3d 
 *---------------------------------------------------------------------------*/

static int LoadFac3D()
{
  struct fac3d_rec *lplot;
  lplot= ((struct fac3d_rec *) MALLOC(sizeof(struct fac3d_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->p) == 0) return(0);
      if (LoadLI(&lplot->q) == 0) return(0);
      if (LoadD(&lplot->teta) == 0) return(0);
      if (LoadD(&lplot->alpha) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if ( strcmp(lplot->name,"fac3d2")==0) 
	if ( LoadVectLI(&(lplot->cvect)) == 0) return(0);
      if ( strcmp(lplot->name,"fac3d3")==0) /****** added by polpoth 4/5/2000 ******/ 
	if ( LoadVectLI(&(lplot->cvect)) == 0) return(0); /****** entry added by polpoth 4/5/2000 ******/
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectLI(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->bbox))== 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (fac3d): No more place \n");
      return(0);
    }
  return(1);
}

/*---------------------------------------------------------------------
 * fec 
 *---------------------------------------------------------------------------*/

/* pas compatibility */
static int LoadFec()
{
  struct fec_rec *lplot;
  lplot= ((struct fec_rec *) MALLOC(sizeof(struct fec_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->Nnode) == 0) return(0);
      if (LoadLI(&lplot->Ntr) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->triangles)) == 0) return(0);
      if (LoadVectF(&(lplot->func) ) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint)) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->legend))  == 0) return(0);
      if ((lplot->zminmax = (double *)  MALLOC(2*sizeof(double))) == 0) return (0);
      if ((lplot->colminmax = (int *)  MALLOC(2*sizeof(int))) == 0) return (0);
      if ((lplot->colout = (int *)  MALLOC(2*sizeof(int))) == 0) return (0);
      lplot->zminmax[0] = lplot->zminmax[1]= 0.0;
      lplot->colminmax[0] = lplot->colminmax[1]=1;
      lplot->colout[0] = lplot->colout[1]=-1;
      lplot->with_mesh = 0;
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
    }
  else 
    {
      Scistring("\nLoad Plot (fec): No more place \n");
      return(0);
    }
  return(1);
}

static int LoadFec_n()
{
  struct fec_rec *lplot;
  lplot= ((struct fec_rec *) MALLOC(sizeof(struct fec_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->Nnode) == 0) return(0);
      if (LoadLI(&lplot->Ntr) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->triangles)) == 0) return(0);
      if (LoadVectF(&(lplot->func) ) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectF(&(lplot->zminmax)) == 0) return(0);    /* added by bruno */
      if (LoadVectLI(&(lplot->colminmax)) == 0) return(0); /* added by bruno */
      if (LoadVectLI(&(lplot->colout)) == 0) return(0); /* added by bruno */
      if (LoadLI(&lplot->with_mesh) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint)) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->legend))  == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (fec): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 * contour 
 *---------------------------------------------------------------------------*/

static int LoadContour()
{
  struct contour_rec *lplot;
  lplot= ((struct contour_rec *) MALLOC(sizeof(struct contour_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadLI(&lplot->nz) == 0) return(0);
      if (LoadLI(&lplot->flagnz) == 0) return(0);
      if (lplot->flagnz != 0) 
	{
	  if (LoadVectF(&(lplot->zz))== 0) return(0);
	}
      if (LoadD(&lplot->teta)== 0) return(0);
      if (LoadD(&lplot->alpha)== 0) return(0);
      if (LoadD(&lplot->zlev)== 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectLI(&(lplot->flag)) == 0) return(0);
      if (LoadVectF(&(lplot->bbox)) == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (contour): No more place \n");
      return(0);
    }
  return(1);
}


static int LoadContour2D()
{
  struct contour2d_rec *lplot;
  lplot= ((struct contour2d_rec *) MALLOC(sizeof(struct contour2d_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadLI(&lplot->nz) == 0) return(0);
      if (LoadLI(&lplot->flagnz) == 0) return(0);
      if (lplot->flagnz != 0) 
	{
	  if (LoadVectF(&(lplot->zz))== 0) return(0);
	}
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectLI(&(lplot->style)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectC(&(lplot->legend)) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectLI(&(lplot->aint)) == 0) return(0);
      if (LoadVectLI(&(lplot->aint_kp)) == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (contour): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 * gray plot 
 * ---------------------------------------------------------------------------*/

static int LoadGray()
{
 
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint))  == 0) return(0);
      if (LoadVectLI(&(lplot->aaint_kp)) == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (gray): No more place \n");
      return(0);
    }
  return(1);
}

static int LoadGray1()
{
 
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      lplot->x = lplot->y = NULL; /* unused in Gray1 */
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectF(&(lplot->brect)) == 0) return(0);
      if (LoadVectF(&(lplot->brect_kp)) == 0) return(0);
      if (LoadVectLI(&(lplot->aaint))  == 0) return(0);
      if (LoadVectLI(&(lplot->aaint_kp)) == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (gray): No more place \n");
      return(0);
    }
  return(1);
}

static int LoadGray2()
{
 
  struct gray_rec_2 *lplot;
  lplot= ((struct gray_rec_2 *) MALLOC(sizeof(struct gray_rec_2)));
  if (lplot != NULL)
    {
      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->z)) == 0) return(0);
      if (LoadVectF(&(lplot->xrect)) == 0) return(0);
      if (Store(lplot->name,(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (gray): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 * champ 
 * ---------------------------------------------------------------------------*/

static int LoadChamp()
{
  struct champ_rec *lplot;
  lplot= ((struct champ_rec *) MALLOC(sizeof(struct champ_rec)));
  if (lplot != NULL)
    {

      if (LoadLI(&lplot->n1) == 0) return(0);
      if (LoadLI(&lplot->n2) == 0) return(0);
      if (LoadD(&lplot->arfact) == 0) return(0);
      if (LoadVectC(&(lplot->name)) == 0) return(0);
      if (LoadVectF(&(lplot->x)) == 0) return(0);
      if (LoadVectF(&(lplot->y)) == 0) return(0);
      if (LoadVectF(&(lplot->fx)) == 0) return(0);
      if (LoadVectF(&(lplot->fy)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag)) == 0) return(0);
      if (LoadVectC(&(lplot->strflag_kp)) == 0) return(0);
      if (LoadVectF(&(lplot->vrect)) == 0) return(0);
      if (LoadVectF(&(lplot->vrect_kp))== 0) return(0);
      if (Store("champ",(char *) lplot) == 0) return(0);
      
    }
  else 
    {
      Scistring("\nLoad Plot (champ): No more place \n");
      return(0);
    }
  return(1);
}


/*---------------------------------------------------------------------
 *  code for reload operations 
 *---------------------------------------------------------------------------*/

static char RFname[128];
static FILE *RF ;
static XDR rxdrs[1] ;
static u_int rcount ;
static u_int rszof ;

/** special case for colormap **/

static int LoadColormap()
{
  double *table; double x;
  int m,i,trois=3;
  int succeed;
  if (LoadLI(&m) == 0) return(0);
  table  = (double *)  MALLOC(3*m*sizeof(double));
  if ( table == NULL) return(0);
  /**  assert( xdr_vector(rxdrs, (char *) table,3*m,sizeof(float), (xdrproc_t) xdr_float)) ;
  for ( i = 3*m-1 ; i >= 0 ; i--) 
  table[i]= ((float *) table)[i]; **/

  for ( i = 0 ; i < 3*m  ; i++ ) 
    {
      assert( xdr_vector(rxdrs, (char *) &x,1,sizeof(double), (xdrproc_t) xdr_double)) ;
      table[i]=x;
      /** sciprint("loading %f\r\n",table[i]); **/
    }
  C2F(dr)("xset","colormap",&m,&trois,&succeed,PI0,PI0,PI0,table,PD0,PD0,PD0,0L,0L);
  FREE(table);
  return(1);
}


typedef  struct  {
  char *name;
  int  (*Load)();} LoadTable;

static LoadTable LoadCTable[] ={
  {"axis",LoadSciAxis},
  {"champ",LoadChamp},
  {"colormap",LoadColormap},
  {"contour",LoadContour},
  {"contour2",LoadContour2D},
  {"fac3d",LoadFac3D},
  {"fac3d1",LoadFac3D},
  {"fac3d2",LoadFac3D},
  {"fac3d3",LoadFac3D}, /****** entry added by polpoth 4/5/2000 ******/
  {"fec",LoadFec},
  {"fec_n",LoadFec_n},
  {"gray",LoadGray},
  {"gray1",LoadGray1}, 
  {"gray2",LoadGray2}, 
  {"nscale",LoadNEch},
  {"param3d",LoadParam3D},
  {"param3d1",LoadParam3D1},
  {"plot2d",LoadPlot},
  {"plot3d",LoadPlot3D},
  {"plot3d1",LoadPlot3D},
  {"scale",LoadEch},
  {"xcall1",LoadXcall1},
  {"xgrid",LoadGrid},
  {(char *)NULL,NULL}};

#ifdef __MSC__
#define __STDC__
#endif 

int C2F(xloadplots)(fname1,lvx)
     char *fname1;
     integer lvx;
{
  integer verb=0,cur,na;
  char name[4],*type;
  strncpy(RFname,fname1,128);
#ifdef __STDC__
  RF = fopen(RFname,"rb") ;
#else
  RF = fopen(RFname,"r") ;
#endif
  if( RF == NULL)
    {
      sciprint("fopen failed\r\n") ;
      return(0);
    }
  xdrstdio_create(rxdrs, RF, XDR_DECODE) ;

  /*if ( LoadVectC(&SciF_version) == 0 )  replaced by the following line to avoid error message*/
  xdr_vector(rxdrs,(char *) &rszof,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int);
  SciF_version = (char *)  MALLOC(rszof);
  if (( version_flag() == 0||SciF_version == NULL) || (xdr_opaque(rxdrs, SciF_version,rszof)==0))
    {
      char temp[256];
      integer ierr,seq=1;
      fclose(RF);
      sprintf(temp,"%%xload('%s')",fname1);
      na=strlen(temp);
      C2F(syncexec)(temp,&na,&ierr,&seq);
      if(ierr != 0)  sciprint("Wrong plot file : %s\n\n",fname1);
      return(0);
    }

  if ( strncmp(SciF_version,"SciG",4) != 0 )
    {
      sciprint("Not a save graphics file: %s\n\n",fname1);
      return(0);
    }
  if ( strcmp(SciF_version,"SciG1.0") != 0 && 
       strcmp(SciF_version,"SciG1.1") != 0 )
    {
      sciprint("Wrong version of saved graphics %s : %s\n\n",
	       SciF_version,fname1);
      return(0);
    }


  while (LoadVectC(&type) != 0 && strcmp(type,"endplots") != 0) 
    {
      if (LoadTPlot(type) == 0) break;
    }
  assert(fflush((FILE *)rxdrs->x_private) != EOF) ; 
  assert(fclose(RF) != EOF) ;
  /** we plot the Loaded graphics **/
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R')
    C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  CPixmapResize1();
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xreplay","v",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(0);
}

static int LoadTPlot(type)
     char *type;
{
  int i=0;
  while (LoadCTable[i].name != (char *) NULL)
    {
      int j;
      j = strcmp(type,LoadCTable[i].name);
      if ( j == 0 ) 
	{ 
	  return( (*(LoadCTable[i].Load))());
	}
      else 
	{ 
	  if ( j <= 0)
	    {
	      sciprint("Unknown Plot type <%s>\r\n",type);
	      return(0);
	    }
	  else i++;
	}
    }
  return(0);
}

static int LoadD(x)
     double *x;
{
  rszof = sizeof(double) ;
  rcount = (u_int) 1;
  assert( xdr_vector(rxdrs, (char *)x, rcount, rszof, (xdrproc_t) xdr_double)) ;
  return(1);
}

static int LoadLI(ix)
     integer *ix;
{
  rszof = sizeof(int) ;
  rcount = (u_int)1;
  assert( xdr_vector(rxdrs, (char *)ix, rcount, rszof, (xdrproc_t) xdr_int)) ;
  return(1);
}
	
static int LoadC(c)
     char *c;
{
  assert( xdr_vector(rxdrs,(char *) &rszof,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;
  assert( xdr_opaque(rxdrs,c,rszof));
  return(1);
}
      
/**
static int LoadVectI(nx)
     int **nx;
{ 
  rszof = sizeof(int) ;
  assert( xdr_vector(rxdrs,(char *) &rcount,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;      
  *nx = (int *)  MALLOC(rcount*sizeof(int));
  if ( *nx == NULL) return(0);
  assert( xdr_vector(rxdrs, (char *)*nx, rcount, rszof, (xdrproc_t) xdr_int)) ;
  return(1);
}

**/

static int LoadVectLI(nx)
     integer **nx;
{ 
  /** Attention integer peut etre un long int **/
  rszof = sizeof(int) ;
  assert( xdr_vector(rxdrs,(char *) &rcount,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;
  *nx = (integer *)  MALLOC(rcount*sizeof(integer));
  if ( *nx == NULL) return(0);
  assert( xdr_vector(rxdrs, (char *)*nx, rcount, rszof, (xdrproc_t) xdr_int)) ;
  return(1);
}

static int    LoadVectF(nx)
     double **nx;
{
  rszof = sizeof(double) ;
  assert( xdr_vector(rxdrs,(char *) &rcount,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;
  *nx = (double *)  MALLOC(rcount*sizeof(double));
  if ( *nx == NULL) return(0);
  assert( xdr_vector(rxdrs, (char *) *nx, rcount, rszof, (xdrproc_t) xdr_double)) ;
  return(1);
}

static int LoadVectC(nx)
     char **nx;
{
  assert( xdr_vector(rxdrs,(char *) &rszof,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;
  *nx = (char *)  MALLOC(rszof);
  if ( *nx == NULL) return(0);
  assert( xdr_opaque(rxdrs, *nx,rszof));
  return(1);
}

static int LoadVectS(nx) 
     char ***nx ;
{
  int i;
  char **loc;
  assert( xdr_vector(rxdrs,(char *) &rcount,(u_int)1,(u_int) sizeof(u_int), (xdrproc_t) xdr_u_int)) ;
  if ( rcount == 0) { *nx = 0 ; return 1;} 
  if (( loc = (char **) MALLOC( (rcount+1)* sizeof(char*))) == NULL) return 0;
  for ( i=0; i < (int) rcount ; i++) 
    {
      if ( LoadVectC(&loc[i]) == 0 ) return 0;
    }
  loc[rcount]= 0;
  *nx = loc ;
  return 1;
}

