/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Graphic High Level Recording function 
 *---------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "Math.h"
#include "Rec.h"
#include "PloEch.h"

static void GReplay __PARAMS((char *type,char *plot));
static void ShowPlot __PARAMS((char *type,char *plot));
static void SCPlot __PARAMS((char *type,char * plot,integer * flag,double * bbox,integer * aaint,
			    int undo,int *bbox1,double *subwin,int win_num));
static void UnSCPlot __PARAMS(( char *type, char *plot));
static void CleanPlot __PARAMS(( char *type, char *plot));
static void NAPlot __PARAMS(( char *type, char *plot, double *alpha,double *theta,integer *iflag,integer *flag,double *bbox));
static void Tape_Replay1 __PARAMS(( struct listplot *list, integer winnumber));
static int MaybeCopyVect3dPLI  __PARAMS((integer *,integer **,integer *,int l)); 
/*static int CopyVectG  __PARAMS((char **pstr,char *,integer,char type)); */
static int MaybeCopyVectLI  __PARAMS((char *,integer **,integer *,int l)); 
/*static int CopyVectI  __PARAMS((int **,int *,integer )); */
static int CopyVectLI  __PARAMS((integer **,integer *,int )); 
static int CopyVectF  __PARAMS((double **,double *,integer )); 
static int CopyVectC  __PARAMS((char **,char *,int )); 
static int CopyVectS  __PARAMS((char ***,char **)); 


static int curwin(void)
{
  integer verbose=0,narg,winnum;
  C2F(dr)("xget","window",&verbose,&winnum,&narg ,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return(winnum);
}

/*---------------------------------------------------------------------
 * xcall1  
 *---------------------------------------------------------------------------*/

void StoreXcall1(char *fname, char *string, integer *x1, integer n1, integer *x2, integer n2, integer *x3, integer n3, integer *x4, integer n4, integer *x5, integer n5, integer *x6, integer n6, double *dx1, integer ndx1, double *dx2, integer ndx2, double *dx3, integer ndx3, double *dx4, integer ndx4)
{
  int debug=0;
  struct xcall1_rec *lplot;
  if (debug)
    {
      sciprint("Inside StoreXcall1 [%s],[%s]\n",fname,string);
      if ( (x1) != (integer *) 0){
	sciprint("Argument-1, taille %ld \n",(long int)n1);
	if ( (x2) != (integer *) 0){
	  sciprint("Argument-2, taille %ld \n",(long int)n2);
	  if ( (x3) != (integer *) 0){
	    sciprint("Argument-3, taille %ld \n",(long int)n3);
	    if ( (x4) != (integer *) 0){
	      sciprint("Argument-4, taille %ld \n",(long int)n4);
	      if ( (x5) != (integer *) 0){
		sciprint("Argument-5, taille %ld \n",(long int)n5);
		if ( (x6) != (integer *) 0){
		  sciprint("Argument-6, taille %ld \n",(long int)n6);
		}
	      }
	    }
	  }
	}
      }
      sciprint("That's over\n");
    }
  lplot= ((struct xcall1_rec *) MALLOC(sizeof(struct xcall1_rec)));
  if (lplot != NULL)
    {
      /** On initialise les champs a zero car CopyVect peut ne rien faire 
	si certains champs sont vides **/
      lplot->x1=(integer *) 0;
      lplot->x2=(integer *) 0;
      lplot->x3=(integer *) 0;
      lplot->x4=(integer *) 0;
      lplot->x5=(integer *) 0;
      lplot->x6=(integer *) 0;
      lplot->dx1=(double *) 0;
      lplot->dx2=(double *) 0;
      lplot->dx3=(double *) 0;
      lplot->dx4=(double *) 0;

      lplot->fname=(char *) 0;
      lplot->string=(char *) 0;
      lplot->n1 = n1;
      lplot->n2 = n2;
      lplot->n3 = n3;
      lplot->n4 = n4;
      lplot->n5 = n5;
      lplot->n6 = n6;
      lplot->ndx1 = ndx1;
      lplot->ndx2 = ndx2;
      lplot->ndx3 = ndx3;
      lplot->ndx4 = ndx4;
      if (
	  CopyVectC(&(lplot->fname),fname,((int)strlen(fname))+1) &&
	  CopyVectC(&(lplot->string),string,((int)strlen(string))+1) &&
	  CopyVectLI(&(lplot->x1),x1,(int) n1) &&
	  CopyVectLI(&(lplot->x2),x2,(int) n2) &&
	  CopyVectLI(&(lplot->x3),x3,(int) n3) &&
	  CopyVectLI(&(lplot->x4),x4,(int) n4) &&
	  CopyVectLI(&(lplot->x5),x5,(int) n5) &&
	  CopyVectLI(&(lplot->x6),x6,(int) n6) &&
	  CopyVectF(&(lplot->dx1),dx1,ndx1) &&
	  CopyVectF(&(lplot->dx2),dx2,ndx2) &&
	  CopyVectF(&(lplot->dx3),dx3,ndx3) &&
	  CopyVectF(&(lplot->dx4),dx4,ndx4) 
	  ) 
	{
	  Store("xcall1",(char *) lplot);
	  return;}
    }
  Scistring("\nStore Plot (xcall1): No more place \n");
}

  
/*---------------------------------------------------------------------
 * xsetech 
 *---------------------------------------------------------------------------*/

void StoreEch(char *name, double *WRect, double *FRect, char *logflag)
{
  struct scale_rec *lplot;
  lplot= ((struct scale_rec *) MALLOC(sizeof(struct scale_rec)));
  if (lplot != NULL)
    {
      lplot->logflag[0]=logflag[0];
      lplot->logflag[1]=logflag[1];
      if ( 
	  CopyVectC(&(lplot->name),name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->Wrect),WRect,4L) &&
	  CopyVectF(&(lplot->Frect),FRect,4L) &&
	  CopyVectF(&(lplot->Frect_kp),FRect,4L) 
	  ) 
	{
	  Store("scale",(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storeEch): No more place \n");
}

void StoreNEch(char *name, char *flag, double *WRect, double *ARect, double *FRect, char *logflag)
{
  struct nscale_rec *lplot;
  lplot= ((struct nscale_rec *) MALLOC(sizeof(struct nscale_rec)));
  if (lplot != NULL)
    {
      lplot->logflag[0]=logflag[0];
      lplot->logflag[1]=logflag[1];
      if ( 
	  CopyVectC(&(lplot->name),name,((int)strlen(name))+1) &&
	  CopyVectC(&(lplot->flag),flag,((int)strlen(flag))+1) &&
	  CopyVectF(&(lplot->Wrect),WRect,4L) &&
	  CopyVectF(&(lplot->Frect),FRect,4L) &&
	  CopyVectF(&(lplot->Arect),ARect,4L) &&
	  CopyVectF(&(lplot->Frect_kp),FRect,4L) 
	  ) 
	{
	  Store("nscale",(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storeEch): No more place \n");
}

  
/*---------------------------------------------------------------------
 * 2D plots 
 *---------------------------------------------------------------------------*/

void StorePlot(char *name, char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aint)
{
  int nstyle,n1n2;
  struct plot2d_rec *lplot;
  lplot= ((struct plot2d_rec *) MALLOC(sizeof(struct plot2d_rec)));
  if ( *n1==1) nstyle= *n1+1;else nstyle= *n1;
  if (lplot != NULL)
    {
      integer n=0;
      switch (xf[0])
	{
	case 'g': n=(*n1)*(*n2);break;
	case 'e': n=0;break;
	case 'o': n=(*n2);break;
	}
      lplot->n1= *n1;
      lplot->n2= *n2;
      /* to be sure that lplot is corectly initialized */
      lplot->x = lplot->y = NULL;
      n1n2=(*n1)*(*n2);
      if ( 
	  CopyVectC(&(lplot->name),name,((int)strlen(name))+1) &&
	  CopyVectC(&(lplot->xf),xf,((int)strlen(xf))+1) &&
	  ((n == 0) ? 1 : CopyVectF(&(lplot->x),x,n)) &&
	  ((n1n2==0)? 1 : CopyVectF(&(lplot->y),y,n1n2)) &&
	  CopyVectLI(&(lplot->style),style,nstyle) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->legend),legend,((int)strlen(legend))+1) && 
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aint),aint,4) &&
	  CopyVectLI(&(lplot->aint_kp),aint,4) 
	  ) 
	{
	  Store("plot2d",(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storeplot): No more place \n");
}

/*---------------------------------------------------------------------
 * axis  
 *---------------------------------------------------------------------------*/

void StoreSciAxis(char *name, char pos, char xy_type, double *x, int *nx, double *y, int *ny, char **str, int subtics, char *format, int fontsize, int textcolor, int ticscolor, char logflag, int seg_flag)
{
  struct sciaxis_rec *lplot = ((struct sciaxis_rec *) MALLOC(sizeof(struct sciaxis_rec)));
  if (lplot == NULL)
    {
      Scistring("\nRunning out of memory in Store plots\n");
      return ;
    }
  lplot->pos= pos;
  lplot->xy_type = xy_type;
  lplot->nx = *nx;
  lplot->ny = *ny;
  lplot->subtics = subtics ;
  lplot->fontsize = fontsize;
  lplot->textcolor = textcolor;
  lplot->ticscolor = ticscolor;
  lplot->logflag = logflag ;
  lplot->seg_flag = seg_flag ;
  lplot->f_l = ((format == NULL) ? 0: 1);
  lplot->format = NULL;
  if ( 
      CopyVectC(&(lplot->name),name,((int)strlen(name))+1) &&
      ((format == NULL) ? 1 : CopyVectC(&(lplot->format),format,((int)strlen(format))+1)) &&
      CopyVectF(&(lplot->x),x,*nx) &&
      CopyVectF(&(lplot->y),y,*ny) &&
      CopyVectS(&(lplot->str),str))
    {
      Store("axis",(char *) lplot);
    }
}
  
/*---------------------------------------------------------------------
 * xgrid 
 *---------------------------------------------------------------------------*/

void StoreGrid(char *name, integer *style)
{
  struct xgrid_rec *lplot;
  lplot= ((struct xgrid_rec *) MALLOC(sizeof(struct xgrid_rec)));
  if (lplot != NULL)
    {
      lplot->style = *style;
      if ( CopyVectC(&(lplot->name),name,((int)strlen(name))+1) )
	{
	  Store("xgrid",(char *) lplot);
	  return;
	}
    }
  Scistring("\n Store (storegrid): No more place \n");
}

/*---------------------------------------------------------------------
 * param3d 
 *---------------------------------------------------------------------------*/

void StoreParam3D(char *name, double *x, double *y, double *z, integer *n, double *teta, double *alpha, char *legend, integer *flag, double *bbox)
{
  struct param3d_rec *lplot;
  lplot= ((struct param3d_rec *) MALLOC(sizeof(struct param3d_rec)));
  if (lplot != NULL)
    {
      lplot->n= *n;
      lplot->teta= *teta;
      lplot->alpha= *alpha;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*n) &&
	  CopyVectF(&(lplot->y), y,*n) &&
	  CopyVectF(&(lplot->z), z,*n) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L)
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storeparam3d): No more place \n");
}

void StoreParam3D1(char *name, double *x, double *y, double *z, integer *m, integer *n, integer *iflag, integer *colors, double *teta, double *alpha, char *legend, integer *flag, double *bbox)
{
  struct param3d1_rec *lplot;
  lplot= ((struct param3d1_rec *) MALLOC(sizeof(struct param3d1_rec)));
  if (lplot != NULL)
    {
      lplot->n= *n;
      lplot->m= *m;
      lplot->iflag= *iflag;
      lplot->teta= *teta;
      lplot->alpha= *alpha;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*m*(*n)) &&
	  CopyVectF(&(lplot->y), y,*m*(*n)) &&
	  CopyVectF(&(lplot->z), z,*m*(*n)) &&
	  MaybeCopyVect3dPLI(iflag,&(lplot->colors), colors, *n) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L)
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;
	}
    }
  Scistring("\n Store Plot (storeparam3d): No more place \n");
}

/*---------------------------------------------------------------------
 * plot3d 
 *---------------------------------------------------------------------------*/

void StorePlot3D(char *name, double *x, double *y, double *z, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox)
{
  struct plot3d_rec *lplot;
  lplot= ((struct plot3d_rec *) MALLOC(sizeof(struct plot3d_rec)));
  if (lplot != NULL)
    {
      lplot->p= *p;
      lplot->q= *q;
      lplot->teta= *teta;
      lplot->alpha= *alpha;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*p) &&
	  CopyVectF(&(lplot->y), y,*q) &&
	  CopyVectF(&(lplot->z), z,(*p)*(*q)) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L)
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storeplot3d): No more place \n");
}

/*---------------------------------------------------------------------
 * fac3d 
 *---------------------------------------------------------------------------*/

void StoreFac3D(char *name, double *x, double *y, double *z, integer *cvect, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox)
{
  struct fac3d_rec *lplot;
  lplot= ((struct fac3d_rec *) MALLOC(sizeof(struct fac3d_rec)));
  if (lplot != NULL)
    {
      lplot->p= *p;
      lplot->q= *q;
      lplot->teta= *teta;
      lplot->alpha= *alpha;
      
      /********* Beginning of added code by polpoth 4/5/2000 *********/
      
      if ( strcmp(name,"fac3d3")==0) {
          if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,(*p)*(*q)) &&
	  CopyVectF(&(lplot->y), y,(*p)*(*q)) &&
	  CopyVectF(&(lplot->z), z,(*p)*(*q)) &&
	  CopyVectLI(&(lplot->cvect),cvect,(*p)*(*q)) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;
	 }
      
      } else /**************** End of added code  *******************/

      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,(*p)*(*q)) &&
	  CopyVectF(&(lplot->y), y,(*p)*(*q)) &&
	  CopyVectF(&(lplot->z), z,(*p)*(*q)) &&
	  MaybeCopyVectLI(name,&(lplot->cvect),cvect,(*q)) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L)
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storefac3d): No more place \n");
}

/*---------------------------------------------------------------------
 *fec 
 *---------------------------------------------------------------------------*/

void StoreFec(char *name, double *x, double *y, double *triangles, double *func, integer *Nnode, integer *Ntr, char *strflag, char *legend, double *brect, integer *aaint, double *zminmax, integer *colminmax)
{
  struct fec_rec *lplot;
  lplot= ((struct fec_rec *) MALLOC(sizeof(struct fec_rec)));
  if (lplot != NULL)
    {
      lplot->Nnode= *Nnode;
      lplot->Ntr= *Ntr;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*Nnode) &&
	  CopyVectF(&(lplot->y), y,*Nnode) &&
	  CopyVectF(&(lplot->triangles), triangles,(*Ntr)*5) &&
	  CopyVectF(&(lplot->func), func,*Nnode ) && 
	  CopyVectF(&(lplot->brect), brect,4L) &&
	  CopyVectF(&(lplot->zminmax), zminmax,2L) &&       /* entry added by Bruno */
	  CopyVectLI(&(lplot->colminmax), colminmax,2L) &&   /*     idem             */
	  CopyVectF(&(lplot->brect_kp), brect,4L) &&
	  CopyVectLI(&(lplot->aaint), aaint,4) &&
	  CopyVectLI(&(lplot->aaint_kp), aaint,4) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->legend),legend,((int)strlen(legend))+1)  
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storefec): No more place \n");
}


/*---------------------------------------------------------------------
 * contour 
 *---------------------------------------------------------------------------*/

void StoreContour(char *name, double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, double *teta, double *alpha, char *legend, integer *flag, double *bbox, double *zlev)
{
  struct contour_rec *lplot;
  lplot= ((struct contour_rec *) MALLOC(sizeof(struct contour_rec)));
  if (lplot != NULL)
    { int res=1;
      lplot->n1= *n1;
      lplot->n2= *n2;
      lplot->nz= *nz;
      lplot->flagnz= *flagnz;
      if (*flagnz != 0)
	res= CopyVectF(&(lplot->zz), zz,*nz);
      else
	lplot->zz= (double *) 0;
      lplot->teta= *teta;
      lplot->alpha= *alpha;
      lplot->zlev= *zlev;
      if ( 
	  res &&
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*n1) &&
	  CopyVectF(&(lplot->y), y,*n2) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->legend), legend, ((int)strlen(legend))+1) && 
	  CopyVectLI(&(lplot->flag), flag,3) &&
	  CopyVectF(&(lplot->bbox), bbox,6L)
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storecontour): No more place \n");
}

void StoreContour2D(char *name, double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, integer *style, char *strflag, char *legend, double *brect, integer *aint)
{
  struct contour2d_rec *lplot;
  int nstyle;
  if ( *n1==1) nstyle= *n1+1;else nstyle= *n1;
  lplot= ((struct contour2d_rec *) MALLOC(sizeof(struct contour2d_rec)));
  if (lplot != NULL)
    { int res=1;
      lplot->n1= *n1;
      lplot->n2= *n2;
      lplot->nz= *nz;
      lplot->flagnz= *flagnz;
      if (*flagnz != 0)
	res= CopyVectF(&(lplot->zz),zz,*nz);
      else
	lplot->zz= (double *) 0;
      if ( 
	  res &&
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*n1) &&
	  CopyVectF(&(lplot->y), y,*n2) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectLI(&(lplot->style),style,nstyle) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->legend),legend,((int)strlen(legend))+1) && 
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aint),aint,4) &&
	  CopyVectLI(&(lplot->aint_kp),aint,4) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storecontour): No more place \n");
}


/*---------------------------------------------------------------------
 * grayplots Matplot 
 *---------------------------------------------------------------------------*/

void StoreGray(char *name, double *x, double *y, double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint)
{
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x), x,*n1) &&
	  CopyVectF(&(lplot->y), y,*n2) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aaint),aaint,4)  &&
	  CopyVectLI(&(lplot->aaint_kp),aaint,4) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storegray): No more place \n");
}

/** For matrices  z(i,j) **/

void StoreGray1(char *name, double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint)
{
  struct gray_rec *lplot;
  lplot= ((struct gray_rec *) MALLOC(sizeof(struct gray_rec)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      lplot->x = NULL;
      lplot->y = NULL;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectF(&(lplot->brect),brect,4L) &&
	  CopyVectF(&(lplot->brect_kp),brect,4L) &&
	  CopyVectLI(&(lplot->aaint),aaint,4)  &&
	  CopyVectLI(&(lplot->aaint_kp),aaint,4) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storegray): No more place \n");
}



void StoreGray2(char *name, double *z, integer *n1, integer *n2, double *xrect)
{
  struct gray_rec_2 *lplot;
  lplot= ((struct gray_rec_2 *) MALLOC(sizeof(struct gray_rec_2)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      if ( 
	  CopyVectC(&(lplot->name), name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->z), z,(*n1)*(*n2)) &&
	  CopyVectF(&(lplot->xrect),xrect,4L) 
	  ) 
	{
	  Store(name,(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storegray): No more place \n");
}


/*---------------------------------------------------------------------
 * champ champ1 
 *---------------------------------------------------------------------------*/

void StoreChamp(char *name, double *x, double *y, double *fx, double *fy, integer *n1, integer *n2, char *strflag, double *vrect, double *arfact)
{
  struct champ_rec *lplot;
  lplot= ((struct champ_rec *) MALLOC(sizeof(struct champ_rec)));
  if (lplot != NULL)
    {
      lplot->n1= *n1;
      lplot->n2= *n2;
      lplot->arfact= *arfact;
      if ( 
	  CopyVectC(&(lplot->name),name,((int)strlen(name))+1) &&
	  CopyVectF(&(lplot->x),x,(*n1)) &&
	  CopyVectF(&(lplot->y),y,(*n2)) &&
	  CopyVectF(&(lplot->fx),fx,(*n1)*(*n2)) &&
	  CopyVectF(&(lplot->fy),fy,(*n1)*(*n2)) &&
	  CopyVectC(&(lplot->strflag),strflag,((int)strlen(strflag))+1) &&
	  CopyVectC(&(lplot->strflag_kp),strflag,((int)strlen(strflag))+1) &&
	  CopyVectF(&(lplot->vrect),vrect,4L) &&
	  CopyVectF(&(lplot->vrect_kp),vrect,4L)
	  ) 
	{
	  Store("champ",(char *) lplot);
	  return;}
    }
  Scistring("\n Store Plot (storechamp): No more place \n");
}

/*---------------------------------------------------------------------
 * Utilities 
 *   for object copy 
 *---------------------------------------------------------------------------*/
/*
static int CopyVectI(nx, x, l)
     int **nx;
     int *x;
     integer l;
{ 
  int i;
  if ( x != ( int *) 0) 
    {
      *nx = (int *)  MALLOC(l*sizeof(int));
      if ( *nx == NULL) return(0);
      for ( i=0 ; i < l ; i++) (*nx)[i]= x[i];
    }
  return(1);
}
*/
static int CopyVectLI(integer **nx, integer *x, int l)
{ 
  int i;
  if ( x != (integer *) 0) 
    {
      *nx = (integer *)  MALLOC(l*sizeof(integer));
      if ( *nx == NULL) return(0);
      for ( i=0 ; i < l ; i++) (*nx)[i]= x[i];
    }
  return(1);
}

static int CopyVectF(double **nx, double *x, integer l)
{
  int i;
  if ( x != (double *) 0) 
    {
      *nx = (double *)  MALLOC(l*sizeof(double));
      if ( *nx == NULL) return(0);
      for ( i=0 ; i < l ; i++) (*nx)[i]= x[i];
    }
  return(1);
}

static int CopyVectC(char **nx, char *x, int l)
{
  int i;
  if ( x != (char *) 0) 
    {
      *nx = (char *)  MALLOC(l*sizeof(char));
      if ( *nx == NULL) return(0);
      for ( i=0 ; i < l ; i++) (*nx)[i]= x[i];
    }
  return(1);
}

static int CopyVectS(char ***hstr, char **str)
{
  /** x is a null terminated string vector */
  if ( str != 0) 
    {
      char **loc;
      int count =0,i;
      while ( str[count] != NULL) count++;
      if ((loc = (char **) MALLOC( (count+1)*sizeof(char*))) == NULL) return 0;
      for ( i=0 ; i < count ; i++)
	{
	  if ( CopyVectC( &loc[i], str[i],strlen(str[i])+1) == 0) return 0;
	}
      loc[count]=0;
      *hstr = loc;
    }
  else
    {
      *hstr= NULL;
    }
  return(1);
}
/*
static int CopyVectG(pstr, str, n, type)
     char **pstr;
     char *str;
     integer n;
     char type;
{
  if ( str != (char *) 0)
    {
      switch ( type)
	{
	case 'f' : return(CopyVectF((double **) pstr,(double *)str,n));
	case 'i' : return(CopyVectI((int **) pstr,(int *)str,n));
	}
    }
  return(1);
}
*/

static int MaybeCopyVect3dPLI(integer *iflag, integer **nx, integer *x, int l)
{
  if ( *iflag == 1 ) 
    return( CopyVectLI(nx,x,l));
  else
    return(1);
}

static int MaybeCopyVectLI(char *name, integer **nx, integer *x, int l)
{
  if (strcmp(name,"fac3d2")==0)
    return( CopyVectLI(nx,x,l));
  else
    return(1);
}

/*---------------------------------------------------------------------
 *  
 *---------------------------------------------------------------------------*/

struct listplot *ListPFirst = NULL ;

/*-------------------------------------------------------------------------
 * StoreXgc is to be called after CleanPlots 
 * to restore in recorded list graphic context elements 
 *---------------------------------------------------------------------------*/

void StoreXgc(integer winnumber)
{
  integer i,win,col;
  integer fontid[2],narg,verbose=0;

  C2F(dr)("xget","window",&verbose,&win,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( win != winnumber) 
      C2F(dr)("xset","window",&winnumber,&narg,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F(dr)("xget","font",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","mark",&verbose,fontid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","mark",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  C2F(dr)("xget","thickness",&verbose,&i,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","thickness",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  C2F(dr)("xget","line mode",&verbose,&i,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","line mode",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F(dr)("xget","alufunction",&verbose,&i,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","alufunction",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* pour le clipping on l'enleve */

  C2F(dr1)("xset","clipoff",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    
  C2F(dr)("xget","use color",&verbose,&col,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /**  It seams not a good idea to send back use color on the recorded  commands 
    see Actions.c (scig_tops) 
    C2F(dr1)("xset","use color",&col,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  **/

  if (col == 0) 
    {
      integer xz[10];
      C2F(dr)("xget","pattern",&verbose,&i,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr1)("xset","pattern",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xget","dashes",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr1)("xset","dashes",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  else 
    {
      C2F(dr)("xget","pattern",&verbose,&i,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr1)("xset","pattern",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  if ( win != winnumber) 
      C2F(dr)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

/*-------------------------------------------------------------------------
 * Show recorded elements  
 *---------------------------------------------------------------------------*/

void Tape_Replay_Show(char *unused, integer *winnumber, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  struct listplot *list;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	    ShowPlot(list->type,list->theplot);
      list =list->ptrplot;
    }
}

static void Show3D(char *plot)
{
  struct plot3d_rec *theplot=(struct plot3d_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowFac3D(char *plot)
{
  struct fac3d_rec *theplot =(struct fac3d_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowFec(char *plot)
{
  struct fec_rec *theplot =(struct fec_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowContour(char *plot)
{
  struct contour_rec *theplot =(struct contour_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowContour2D(char *plot)
{
  struct contour2d_rec *theplot =(struct contour2d_rec *) plot;
  sciprint("%s \r\n",theplot->name);

}

static void ShowGray(char *plot)
{
  struct gray_rec *theplot =(struct gray_rec *) plot;
  sciprint("%s \r\n",theplot->name);

}

static void ShowGray1(char *plot)
{
  struct gray_rec *theplot =(struct gray_rec *) plot;
  sciprint("%s \r\n",theplot->name);

}

static void ShowGray2(char *plot)
{
  struct gray_rec_2 *theplot = (struct gray_rec_2 *) plot;
  sciprint("%s \r\n",theplot->name);

}

static void ShowParam3D(char *plot)
{
  struct param3d_rec *theplot =(struct param3d_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowParam3D1(char *plot)
{
  struct param3d1_rec *theplot =(struct param3d1_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void Show2D(char *plot)
{
  struct plot2d_rec *theplot =(struct plot2d_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowSciAxis(char *plot)
{
  struct sciaxis_rec *theplot = (struct sciaxis_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowGrid(char *plot)
{
  struct xgrid_rec *theplot =(struct xgrid_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowEch(char *plot)
{
  struct scale_rec *theplot=(struct scale_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowNEch(char *plot)
{
  struct nscale_rec *theplot=(struct nscale_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

static void ShowX1(char *plot)
{
  struct xcall1_rec *theplot;
  theplot=(struct xcall1_rec *) plot;
  sciprint("%s %s\r\n",theplot->fname,theplot->string);
}

static void ShowChamp(char *plot)
{
  struct champ_rec *theplot;
  theplot=(struct champ_rec *) plot;
  sciprint("%s \r\n",theplot->name);
}

typedef  struct  {
  char *name;
  void  (*Show) __PARAMS((char *));
} ShowTable;

static void shfnvoid(char *plot) {} 

static ShowTable ShTable[] ={
  {"axis", ShowSciAxis},
  {"champ",ShowChamp},
  {"contour",ShowContour},
  {"contour2",ShowContour2D},
  {"fac3d",ShowFac3D},
  {"fac3d1",ShowFac3D},
  {"fac3d2",ShowFac3D},
  {"fac3d3",ShowFac3D}, /****** entry added by polpoth 4/5/2000 ******/
  {"fec",ShowFec},
  {"fec_n",ShowFec},
  {"gray",ShowGray},
  {"gray1",ShowGray1},
  {"gray2",ShowGray2},
  {"nscale",ShowNEch},
  {"param3d",ShowParam3D},
  {"param3d1",ShowParam3D1},
  {"plot2d",Show2D},
  {"plot3d",Show3D},
  {"plot3d1",Show3D},
  {"scale",ShowEch},
  {"xcall1",ShowX1},
  {"xgrid",ShowGrid},
  {(char *)NULL,shfnvoid}
};

static void ShowPlot(char *type, char *plot)
{
  int i=0;
  while ( ShTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,ShTable[i].name);
       if ( j == 0 ) 
	 { 
	   (*(ShTable[i].Show))(plot);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("Unknown Plot type <%s>\r\n",type);
	       return;
	     }
	   else i++;
	 }
     }
  sciprint("Unknown Plot type <%s>\r\n",type);
}

/*-------------------------------------------------------------------------
 * Delete all recorded graphics associated to window winnumber 
 * Then [1] current graphic context is stored back in the list 
 *      [2] the scale flag is reset to zero (for preventing auto scale to use it) 
 *---------------------------------------------------------------------------*/

void CleanPlots(char *unused, integer *winnumber, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{
  struct listplot *list,*list1;     
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber)
	{
	  if (list->theplot != NULL) {
	    CleanPlot(list->type,list->theplot);
	    FREE(list->theplot);}
	  FREE(list->type);
	  if (list->previous != NULL)
	    (list->previous)->ptrplot=list->ptrplot;
	  else
	    ListPFirst=list->ptrplot;
	  if (list->ptrplot != NULL) 
	    (list->ptrplot)->previous=list->previous;
	  list1=list;
	  list =list->ptrplot;
	  FREE((char *) list1);
	}
      else 
	list=list->ptrplot;
    }
  /* graphic context back in the list */
  StoreXgc(*winnumber);
  /* we remove scales in window number i */
  del_window_scale(*winnumber);
  /* if *winnumber is also the current window we reset Cscale to default value */
  if ( *winnumber == curwin() ) 
    Cscale2default();
  else 
    set_window_scale_with_default(*winnumber);
}

static void Clean3D(char *plot)
{
  struct plot3d_rec *theplot;
  theplot=(struct plot3d_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->legend);FREE(theplot->flag);
  FREE(theplot->bbox);
}


static void CleanFac3D(char *plot)
{
  struct fac3d_rec *theplot;
  theplot=(struct fac3d_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->legend);FREE(theplot->flag);
  FREE(theplot->bbox);
}


static void CleanFec(char *plot)
{
  struct fec_rec *theplot;
  theplot=(struct fec_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->triangles);  FREE(theplot->func);
  FREE(theplot->legend);
  FREE(theplot->strflag);  FREE(theplot->strflag_kp);
  FREE(theplot->brect);   FREE(theplot->brect_kp); 
  FREE(theplot->aaint);  FREE(theplot->aaint_kp);
  FREE(theplot->zminmax); FREE(theplot->colminmax);  /* added by bruno */

}

static void CleanContour(char *plot)
{
  struct contour_rec *theplot;
  theplot=(struct contour_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->zz);FREE(theplot->legend);FREE(theplot->flag);
  FREE(theplot->bbox);
}

static void CleanContour2D(char *plot)
{
  struct contour2d_rec *theplot;
  theplot=(struct contour2d_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->zz);
  FREE(theplot->style);FREE(theplot->strflag);
  FREE(theplot->legend);FREE(theplot->brect);FREE(theplot->aint);   
  FREE(theplot->strflag_kp);
  FREE(theplot->brect_kp);FREE(theplot->aint_kp);   
}

static void CleanGray(char *plot)
{
  struct gray_rec *theplot;
  theplot=(struct gray_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);
  FREE(theplot->strflag);
  FREE(theplot->brect);FREE(theplot->aaint);   
  FREE(theplot->strflag_kp);
  FREE(theplot->brect_kp);FREE(theplot->aaint_kp);   
}

static void CleanGray2(char *plot)
{
  struct gray_rec_2 *theplot;
  theplot=(struct gray_rec_2 *) plot;
  FREE(theplot->name);
  FREE(theplot->z);
  FREE(theplot->xrect);
}

static void CleanParam3D(char *plot)
{
  struct param3d_rec *theplot;
  theplot=(struct param3d_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->legend);FREE(theplot->flag);
  FREE(theplot->bbox);
}

static void CleanParam3D1(char *plot)
{
  struct param3d1_rec *theplot;
  theplot=(struct param3d1_rec *) plot;
  FREE(theplot->name);FREE(theplot->x);FREE(theplot->y);
  FREE(theplot->z);FREE(theplot->legend);FREE(theplot->flag);
  if ( theplot->iflag == 1 ) 
    FREE(theplot->colors);
  FREE(theplot->bbox);
}

static void Clean2D(char *plot)
{
  struct plot2d_rec *theplot;
  theplot=(struct plot2d_rec *) plot;
  FREE(theplot->name);    
  if ( theplot->xf[0] != 'e') FREE(theplot->x);
  FREE(theplot->xf);FREE(theplot->y);     
  FREE(theplot->style);FREE(theplot->strflag);
  FREE(theplot->legend);FREE(theplot->brect);FREE(theplot->aint);   
  FREE(theplot->strflag_kp);
  FREE(theplot->brect_kp);FREE(theplot->aint_kp);   
}

static void CleanSciAxis(char *plot)
{
  int count = 0;
  struct sciaxis_rec *theplot = (struct sciaxis_rec *) plot;
  FREE(theplot->name);
  if (theplot->f_l == 1) FREE(theplot->format);
  FREE(theplot->x);
  FREE(theplot->y);
  if ( theplot->str != 0)
    { 
      while ( theplot->str[count] != 0) { FREE(theplot->str[count]); count++;} 
      FREE(theplot->str);
    }
}

static void CleanGrid(char *plot)
{
  struct xgrid_rec *theplot;
  theplot=(struct xgrid_rec *) plot;
  FREE(theplot->name);    
}

static void CleanEch(char *plot)
{
  struct scale_rec *theplot;
  theplot=(struct scale_rec *) plot;
  FREE(theplot->name);    
  FREE(theplot->Wrect);
  FREE(theplot->Frect);     
  FREE(theplot->Frect_kp);     
}

static void CleanNEch(char *plot)
{
  struct nscale_rec *theplot;
  theplot=(struct nscale_rec *) plot;
  FREE(theplot->name);    
  FREE(theplot->flag);    
  FREE(theplot->Wrect);
  FREE(theplot->Frect);     
  FREE(theplot->Arect);     
  FREE(theplot->Frect_kp);     
}


static void CleanX1(char *plot)
{
  struct xcall1_rec *theplot;
  theplot=(struct xcall1_rec *) plot;
  FREE(theplot->fname);    
  FREE(theplot->string);
  FREE(theplot->x1);
  FREE(theplot->x2);
  FREE(theplot->x3);
  FREE(theplot->x4);
  FREE(theplot->x5);
  FREE(theplot->x6);
  FREE(theplot->dx1);
  FREE(theplot->dx2);
  FREE(theplot->dx3);
  FREE(theplot->dx4);
}

static void CleanChamp(char *plot)
{
  struct champ_rec *theplot;
  theplot=(struct champ_rec *) plot;
  FREE(theplot->name);    
  FREE(theplot->x);FREE(theplot->y);     
  FREE(theplot->fx);FREE(theplot->fy);     
  FREE(theplot->strflag);FREE(theplot->vrect);
  FREE(theplot->strflag_kp);FREE(theplot->vrect_kp);
}

typedef  struct  {
  char *name;
  void  (*clean) __PARAMS((char *));} CleanTable;

static void fnvoid (char *plot) {}

static CleanTable CTable[] ={
  {"axis", CleanSciAxis},
  {"champ",CleanChamp},
  {"contour",CleanContour},
  {"contour2",CleanContour2D},
  {"fac3d",CleanFac3D},
  {"fac3d1",CleanFac3D},
  {"fac3d2",CleanFac3D},
  {"fac3d3",CleanFac3D}, /****** entry added by polpoth 4/5/2000 ******/
  {"fec",CleanFec},
  {"fec_n",CleanFec},
  {"gray",CleanGray},
  {"gray1",CleanGray}, /** same for gray and gray1 **/
  {"gray2",CleanGray2}, 
  {"nscale",CleanNEch},
  {"param3d",CleanParam3D},
  {"param3d1",CleanParam3D1},
  {"plot2d",Clean2D},
  {"plot3d",Clean3D},
  {"plot3d1",Clean3D},
  {"scale",CleanEch},
  {"xcall1",CleanX1},
  {"xgrid",CleanGrid},
  {(char *)NULL,fnvoid}};

static void CleanPlot(char *type, char *plot)
{
  int i=0;
  while ( CTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,CTable[i].name);
       if ( j == 0 ) 
	 { 
	   (*(CTable[i].clean))(plot);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("Unknown Plot type <%s>\r\n",type);
	       return;
	     }
	   else i++;
	 }
     }
  sciprint("Unknown Plot type <%s>\r\n",type);
}

/*-------------------------------------------------------------------------
 * Change les angles alpha theta dans tous les plot3d stockes 
 *change  aussi flag et box suivant la valeur de iflag.
 *iflag est de longueur [4] si iflag[i] != 0 cela veut dire qu'il faut changer le 
 *flag[i] en utilisant celui de l'argument flag.
 *iflag[4] sert a dire s'il faut ou pas changer bbox 
 *---------------------------------------------------------------------------*/

static void NAPlots(char *unused, integer *winnumber, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  struct listplot *list;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	    NAPlot(list->type,list->theplot,theta,alpha,iflag,flag,bbox);
      list =list->ptrplot;
    }
}

static void NA3D(char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  int i;
  struct plot3d_rec *theplot;
  theplot=(struct plot3d_rec *) plot;
  theplot->teta=*theta;
  theplot->alpha=*alpha;
  for (i=0 ; i< 3 ; i++) 
      if (iflag[i]!=0) theplot->flag[i] = flag[i];
  if ( iflag[3] != 0) 
    for ( i= 0 ; i < 6 ; i++ ) 
            theplot->bbox[i] = bbox[i];
}

static void NAFac3D(char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  int i;
  struct fac3d_rec *theplot;
  theplot=(struct fac3d_rec *) plot;
  theplot->teta=*theta;
  theplot->alpha=*alpha;
  for (i=0 ; i< 3 ; i++) 
      if (iflag[i]!=0) theplot->flag[i] = flag[i];
  if ( iflag[3] != 0) 
    for ( i= 0 ; i < 6 ; i++ ) 
            theplot->bbox[i] = bbox[i];
}

static void NAContour(char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  int i;
  struct contour_rec *theplot;
  theplot=(struct contour_rec *) plot;
  theplot->teta=*theta;
  theplot->alpha=*alpha;
  for (i=0 ; i< 3 ; i++) 
      if (iflag[i]!=0) theplot->flag[i] = flag[i];
  if ( iflag[3] != 0) 
    for ( i= 0 ; i < 6 ; i++ ) 
            theplot->bbox[i] = bbox[i];
}

static void NAParam3D(char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  int i;
  struct param3d_rec *theplot;
  theplot=(struct param3d_rec *) plot;
  theplot->teta=*theta;
  theplot->alpha=*alpha;
  for (i=0 ; i< 3 ; i++) 
      if (iflag[i]!=0) theplot->flag[i] = flag[i];
  if ( iflag[3] != 0) 
    for ( i= 0 ; i < 6 ; i++ ) 
            theplot->bbox[i] = bbox[i];
}

static void NAParam3D1(char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{
  int i;
  struct param3d1_rec *theplot;
  theplot=(struct param3d1_rec *) plot;
  theplot->teta=*theta;
  theplot->alpha=*alpha;
  for (i=0 ; i< 3 ; i++) 
      if (iflag[i]!=0) theplot->flag[i] = flag[i];
  if ( iflag[3] != 0) 
    for ( i= 0 ; i < 6 ; i++ ) 
            theplot->bbox[i] = bbox[i];
}


static void navoid (char *plot, double *theta, double *alpha, integer *iflag, integer *flag, double *bbox)
{}


typedef  struct  {
  char *name;
  void (*NA) __PARAMS((char *,double *,double *,integer *,integer*,double *));
}
NATable;

static NATable NACTable[] ={
    {"contour",NAContour},
    {"fac3d",NAFac3D},
    {"fac3d1",NAFac3D},
    {"fac3d2",NAFac3D},
    {"fac3d3",NAFac3D}, /****** entry added by polpoth 4/5/2000 ******/
    {"param3d",NAParam3D},
    {"param3d1",NAParam3D1},
    {"plot3d",NA3D},
    {"plot3d1",NA3D},
    {(char *)NULL,navoid}};

static void NAPlot(char *type, char *plot, double *alpha, double *theta, integer *iflag, integer *flag, double *bbox)
{
  int i=0;
  while ( NACTable[i].name != (char *) NULL)
    {
      int j;
      j = strcmp(type,NACTable[i].name);
      if ( j == 0 ) 
	{ 
	  (*(NACTable[i].NA))(plot,alpha,theta,iflag,flag,bbox);
	  return;}
      else 
	{ if ( j <= 0)
	  {
	    /** sciprint("Unknown Plot type <%s>\r\n",type); **/
	    return;
	  }
	else i++;
	}
    }
}

/*--------------------------------------------------
 * change the scale in all the recorded graphics 
 *   if flag[0]== 1  bbox is changed 
 *   if flag[1]== 1  aaint is changed 
 *   if undo = 1 then the work can be undone (with unzoom)
 *   else undo = 1 unzoom cannot be performed 
 *   if subwin == NULL 
 *       => we must find the subwin asscoiated to bbox1
 *--------------------------------------------------*/

void SCPlots(char *unused, integer *winnumber, integer *flag, double *bbox, integer *aaint, int undo, int *bbox1, double *subwin)
{
  struct listplot *list;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	SCPlot(list->type,list->theplot,flag,bbox,aaint,undo,bbox1,subwin,*winnumber);
      list =list->ptrplot;
    }
}

/** change the plot flag in order to use bbox **/ 

static void SC2DChangeFlag(char *str)
{
  return; /* Added by POLPOTH09042001 on Apr  9 08:59:10 MET DST 2001 */
  if ( str[1] == '2' ||  str[1] == '1'  || str[1] == '6' ) 
    str[1] = '5';
  else if ( str[1] == '4' ) 
    str[1] = '3';
}
  
static void SC2D(char *plot, integer *flag, double *bbox, integer *aaint, int undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct plot2d_rec *theplot;
  theplot=(struct plot2d_rec *) plot;
  /* on passe en mode autoscale 5= on utilise bbox et on `regradue' dessus */
  SC2DChangeFlag( theplot->strflag);
  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1)
	{
	  theplot->brect[i]=bbox[i];
	  if (undo == 0)   theplot->brect_kp[i]=bbox[i];
	}
      if (flag[1]==1)  theplot->aint[i]=aaint[i];
    }
}

static void SCContour2D(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct contour2d_rec *theplot;
  theplot=(struct contour2d_rec *) plot;
  /* on passe en mode autoscale 5= on utilise bbox et on `regradue' dessus */
  SC2DChangeFlag( theplot->strflag);
  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1)
	{
	  theplot->brect[i]=bbox[i];
	  if (undo == 0)   theplot->brect_kp[i]=bbox[i];
	}
      if (flag[1]==1)  theplot->aint[i]=aaint[i];
    }
}

static void SCgray(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct gray_rec *theplot;
  theplot=(struct gray_rec *) plot;
  /* on passe en mode autoscale 5= on utilise bbox et on `regradue' dessus */
  SC2DChangeFlag( theplot->strflag);
  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1) 
	{
	  theplot->brect[i]=bbox[i];
	  if (undo == 0)   theplot->brect_kp[i]=bbox[i];
	}
      if (flag[1]==1)  theplot->aaint[i]=aaint[i];
    }
}

static void SCchamp(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct champ_rec *theplot;
  theplot=(struct champ_rec *) plot;
  /* on passe en mode autoscale 5= on utilise bbox et on `regradue' dessus */
  SC2DChangeFlag( theplot->strflag);
  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1)
	{
	  theplot->vrect[i]=bbox[i];
	  if (undo == 0)   theplot->vrect_kp[i]=bbox[i];
	}
    }
}

static void SCfec(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct fec_rec *theplot;
  theplot=(struct fec_rec *) plot;
  /* on passe en mode autoscale 5= on utilise bbox et on `regradue' dessus */
  SC2DChangeFlag( theplot->strflag);
  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1)  
	{
	  theplot->brect[i]=bbox[i];
	  if (undo == 0)   theplot->brect_kp[i]=bbox[i];
	}	  
      if (flag[1]==1)  theplot->aaint[i]=aaint[i];
    }
}

/* here we deal with subwin changes */

static void SCEch(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct scale_rec *theplot;
  theplot =   (struct scale_rec *) plot;
  
  if (bbox1 != NULL) 
    {
      /* we are trying to change the scale */
      int verbose=0,wdim[2],narg;
      C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      /* check if bbox1 is inside current subwin */
      if ( theplot->Wrect[0]*wdim[0] <= bbox1[0] 
	   && (theplot->Wrect[0]+theplot->Wrect[2])*wdim[0] >= bbox1[2] 
	   && theplot->Wrect[1]*wdim[1] <= bbox1[1] 
	   && (theplot->Wrect[1]+theplot->Wrect[3])*wdim[1] >= bbox1[3] )
	{
	  /* bbox1 is inside the subwindow 
	   * we must change bbox to the subwindow scale 
	   */
	  flag[0]=1;
	  /* localy change the scale */
	  set_scale("tttftf",theplot->Wrect,theplot->Frect,NULL,theplot->logflag,NULL);
	  bbox[0] = XPixel2Double(bbox1[0]);
	  bbox[1] = YPixel2Double(bbox1[1]);
	  bbox[2] = XPixel2Double(bbox1[2]);
	  bbox[3] = YPixel2Double(bbox1[3]);
	}
      else 
	flag[0]=0;
    }
  else if ( subwin != NULL) 
    {
      if (    Abs(theplot->Wrect[0] - subwin[0]) < 1.e-8
	   && Abs(theplot->Wrect[1] - subwin[1]) < 1.e-8
	   && Abs(theplot->Wrect[2] - subwin[2]) < 1.e-8
	   && Abs(theplot->Wrect[3] - subwin[3]) < 1.e-8 )
	{
	  /* we are switching to the good subwindow */
	  /* sciprint("ech : je suis ds la bonne subwin [%f,%f,%f,%f]\r\n",
		   subwin[0],subwin[1],subwin[2],subwin[3]);
	  */
	  flag[0] = 1;
	}
      else 
	{
	  /* sciprint("ech : je suis pas ds la bonne subwin [%f,%f,%f,%f]\r\n",
		   subwin[0],subwin[1],subwin[2],subwin[3]);
	  */
	  flag[0] = 0;
	}
    }

  for ( i = 0 ; i < 4 ; i++)
    {
      if (flag[0]==1) 
	{
	  theplot->Frect[i]=bbox[i];
	  if (undo == 0)   theplot->Frect_kp[i]=bbox[i];
	}
    }
}

static void SCNEch(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  struct nscale_rec *theplot;
  theplot =   (struct nscale_rec *) plot;
  if (bbox1 != NULL) 
    {
      /* we are trying to change the scale with a zoom rect */
      /* specified in bbox1 in pixel */
      /* 1- check if nscale contains a subwin definition */
      if ( theplot->flag[1] == 't' ) 
	{
	  int verbose=0,wdim[2],narg;
	  flag[0]=0; 
	  C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  /* check if bbox1 is inside current subwin */
	  if (theplot->Wrect[0]*wdim[0] <= bbox1[0] && (theplot->Wrect[0]+theplot->Wrect[2])*wdim[0] >= bbox1[2] 
	      && theplot->Wrect[1]*wdim[1] <= bbox1[1] && (theplot->Wrect[1]+theplot->Wrect[3])*wdim[1] >= bbox1[3] )
	    {
	      /** we extract the associated scale **/
	      get_window_scale(win_num,theplot->Wrect);
	      /* bbox1 is inside the subwindow 
	       * we must change bbox to the subwindow scale 
	       */
	      bbox[0] = XPixel2Double(bbox1[0]);
	      bbox[1] = YPixel2Double(bbox1[1]);
	      bbox[2] = XPixel2Double(bbox1[2]);
	      bbox[3] = YPixel2Double(bbox1[3]);
	      /* sciprint("je trouve un bbox ds [%f %f %f %f ] ds [%f,%f,%f,%f] log=[%c,%c]\r\n",
		       bbox[0],bbox[1],bbox[2],bbox[3],
		       theplot->Wrect[0],theplot->Wrect[1],theplot->Wrect[2],theplot->Wrect[3],
		       Cscale.logflag[0],		       Cscale.logflag[1]);
	      */
	      /* flag is changed ==> next recorded plots will use bbox **/
	      flag[0]=1;
	    }
	}
    }
  else if ( subwin != NULL) 
    {
      /* check if subwin is the same as theplot->Wrect */
      if ( theplot->flag[1] == 't' ) 
	{ 
	  if( Abs(theplot->Wrect[0] - subwin[0]) < 1.e-8
	      && Abs(theplot->Wrect[1] - subwin[1]) < 1.e-8
	      && Abs(theplot->Wrect[2] - subwin[2]) < 1.e-8
	      && Abs(theplot->Wrect[3] - subwin[3]) < 1.e-8 )
	    {
	      /* we are switching to the good subwindow */
	      /* sciprint("Nech : je suis ds la bonne subwin [%f,%f,%f,%f]\r\n",
		 subwin[0],subwin[1],subwin[2],subwin[3]);
	      */
	      flag[0] = 1;
	    }
	  else
	    flag[0] = 0;
	}
      else 
	{
	  flag[0] = 0;
	  /* sciprint("Nech: je suis pas ds la bonne subwin \r\n"); */
	}
    }
  if (flag[0]==1) 
    for ( i = 0 ; i < 4 ; i++)
    {
      theplot->Frect[i]=bbox[i];
      if (undo == 0)   theplot->Frect_kp[i]=bbox[i];
    }
}

/*** code added by ES 21/5/2002 ****/
static void SCContour(char *plot, integer *flag, double *b1, integer *aaint,
                       integer undo, int *bbox1, double *subwin, int win_num)
{
  int i, j;
  double xmin, xmax, ymin, ymax, zmin, zmax, xp, yp, x,y,z;
  struct contour_rec *theplot;
  theplot =   (struct contour_rec *) plot;
  if (bbox1 != NULL) 
  {
   b1[0] = XPixel2Double(bbox1[0]);
   b1[1] = YPixel2Double(bbox1[1]);
   b1[2] = XPixel2Double(bbox1[2]);
   b1[3] = YPixel2Double(bbox1[3]);
/*   sciprint("I'm trying to zoom a 3d contour \r\n"); */
/*  sciprint(" zoom area: x={%f:%f};  y={%f:%f}\r\n",b1[0],b1[2],b1[1],b1[3]); */
   xmin=Maxi(theplot->x,theplot->n1); xmax=Mini(theplot->x,theplot->n1);
   ymin=Maxi(theplot->y,theplot->n1); ymax=Mini(theplot->y,theplot->n2);
   zmin=Maxi(theplot->z,theplot->n1*theplot->n2);
   zmax=Mini(theplot->z,theplot->n1*theplot->n2);
   for (i=0; i < theplot->n1; i++)
    for (j=0; j < theplot->n2; j++)
     {
      x=theplot->x[i]; y=theplot->y[j]; z=theplot->z[j*theplot->n1 + i]; 
      xp=TRX(x,y,z); yp=TRY(x,y,z); 
      if(xp >= b1[0] && xp <= b1[2] && yp >= b1[1] && yp <= b1[3])
       {
        if(x < xmin) xmin=x; if(x > xmax) xmax=x;
        if(y < ymin) ymin=y; if(y > ymax) ymax=y;
        if(z < zmin) zmin=z; if(z > zmax) zmax=z;
       }
     }
   if (xmax > xmin) {theplot->bbox[0]=xmin; theplot->bbox[1]=xmax;}
   if (ymax > ymin) {theplot->bbox[2]=ymin; theplot->bbox[3]=ymax;}
   if (zmax > zmin) {theplot->bbox[4]=zmin; theplot->bbox[5]=zmax;}
   if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1;
  }
}

static void SCFac3d(char *plot, integer *flag, double *b1, integer *aaint,
                    integer undo, int *bbox1, double *subwin, int win_num)
{
  int i;
  double xmin, xmax, ymin, ymax, zmin, zmax, xp, yp, x,y,z;
  struct fac3d_rec *theplot;
  theplot =   (struct fac3d_rec *) plot;
  if (bbox1 != NULL) 
  {
   b1[0] = XPixel2Double(bbox1[0]);
   b1[1] = YPixel2Double(bbox1[1]);
   b1[2] = XPixel2Double(bbox1[2]);
   b1[3] = YPixel2Double(bbox1[3]);
/*   sciprint("I'm trying to zoom a 3d fac-plot \r\n");*/
/*  sciprint(" zoom area: x={%f:%f};  y={%f:%f}\r\n",b1[0],b1[2],b1[1],b1[3]); */
   xmin=Maxi(theplot->x,theplot->p*theplot->q);
   xmax=Mini(theplot->x,theplot->p*theplot->q);
   ymin=Maxi(theplot->y,theplot->p*theplot->q);
   ymax=Mini(theplot->y,theplot->p*theplot->q);
   zmin=Maxi(theplot->z,theplot->p*theplot->q);
   zmax=Mini(theplot->z,theplot->p*theplot->q);
   for (i=0; i < theplot->p*theplot->q; i++)
     {
      x=theplot->x[i]; y=theplot->y[i]; z=theplot->z[i]; 
      xp=TRX(x,y,z); yp=TRY(x,y,z); 
      if(xp >= b1[0] && xp <= b1[2] && yp >= b1[1] && yp <= b1[3])
       {
        if(x < xmin) xmin=x; if(x > xmax) xmax=x;
        if(y < ymin) ymin=y; if(y > ymax) ymax=y;
        if(z < zmin) zmin=z; if(z > zmax) zmax=z;
       }
     }
   if (xmax > xmin) {theplot->bbox[0]=xmin; theplot->bbox[1]=xmax;}
   if (ymax > ymin) {theplot->bbox[2]=ymin; theplot->bbox[3]=ymax;}
   if (zmax > zmin) {theplot->bbox[4]=zmin; theplot->bbox[5]=zmax;}
   if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1; 
  }
}

static void SCParam3d(char *plot, integer *flag, double *b1, integer *aaint,
                       integer undo, int *bbox1, double *subwin, int win_num)
{
  int i; double xmin, xmax, ymin, ymax, zmin, zmax, xp, yp, x,y,z;
  struct param3d_rec *theplot;
  theplot =   (struct param3d_rec *) plot;
  if (bbox1 != NULL) 
  {
   b1[0] = XPixel2Double(bbox1[0]);
   b1[1] = YPixel2Double(bbox1[1]);
   b1[2] = XPixel2Double(bbox1[2]);
   b1[3] = YPixel2Double(bbox1[3]);
/*  sciprint(" zoom area: x={%f:%f};  y={%f:%f}\r\n",b1[0],b1[2],b1[1],b1[3]); */
   xmin=Maxi(theplot->x,theplot->n); xmax=Mini(theplot->x,theplot->n);
   ymin=Maxi(theplot->y,theplot->n); ymax=Mini(theplot->y,theplot->n);
   zmin=Maxi(theplot->z,theplot->n); zmax=Mini(theplot->z,theplot->n);
/* search the min and max x, y, z of the points which got projected into the bbox1 */
   xmin=theplot->bbox[1]; xmax=theplot->bbox[0];
   ymin=theplot->bbox[3]; ymax=theplot->bbox[2];
   zmin=theplot->bbox[5]; zmax=theplot->bbox[4];
   for (i=0; i < theplot->n; i++)
     {
      x=theplot->x[i]; y=theplot->y[i]; z=theplot->z[i]; 
      xp=TRX(x,y,z); yp=TRY(x,y,z); 
/*      sciprint("%f %f \r\n",xp,yp); */
      if(xp >= b1[0] && xp <= b1[2] && yp >= b1[1] && yp <= b1[3])
       {
/*        sciprint("**\r\n");  */
        if(x < xmin) xmin=x; if(x > xmax) xmax=x;
        if(y < ymin) ymin=y; if(y > ymax) ymax=y;
        if(z < zmin) zmin=z; if(z > zmax) zmax=z;
       }
     }
   if (xmax > xmin) {theplot->bbox[0]=xmin; theplot->bbox[1]=xmax;}
   if (ymax > ymin) {theplot->bbox[2]=ymin; theplot->bbox[3]=ymax;}
   if (zmax > zmin) {theplot->bbox[4]=zmin; theplot->bbox[5]=zmax;}
/*   sciprint("trying to zoom a 3d param3d plot: x= [%f:%f], y=[%f:%f], z=[%f:%f]\r\n",xmin, xmax, ymin, ymax, zmin, zmax);
*/
   if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1; 
   }
}

static void SCParam3d1(char *plot, integer *flag, double *b1, integer *aaint, 
                       integer undo, int *bbox1, double *subwin, int win_num)
{
  int i; 
  double xmin, xmax, ymin, ymax, zmin, zmax, xp, yp, x,y,z;
  struct param3d1_rec *theplot;
  theplot =   (struct param3d1_rec *) plot;
  if (bbox1 != NULL) 
  {
   b1[0] = XPixel2Double(bbox1[0]);
   b1[1] = YPixel2Double(bbox1[1]);
   b1[2] = XPixel2Double(bbox1[2]);
   b1[3] = YPixel2Double(bbox1[3]);
/*   sciprint("I'm trying to zoom a param3d-1 plot  \r\n");*/
/*  sciprint(" zoom area: x={%f:%f};  y={%f:%f}\r\n",b1[0],b1[2],b1[1],b1[3]); */
   xmin=Maxi(theplot->x,theplot->n*theplot->m);
   xmax=Mini(theplot->x,theplot->n*theplot->m);
   ymin=Maxi(theplot->y,theplot->n*theplot->m);
   ymax=Mini(theplot->y,theplot->n*theplot->m);
   zmin=Maxi(theplot->z,theplot->n*theplot->m);
   zmax=Mini(theplot->z,theplot->n*theplot->m);
   for (i=0; i < theplot->n*theplot->m; i++)
     {
      x=theplot->x[i]; y=theplot->y[i]; z=theplot->z[i]; 
      xp=TRX(x,y,z); yp=TRY(x,y,z); 
      if(xp >= b1[0] && xp <= b1[2] && yp >= b1[1] && yp <= b1[3])
       {
        if(x < xmin) xmin=x; if(x > xmax) xmax=x;
        if(y < ymin) ymin=y; if(y > ymax) ymax=y;
        if(z < zmin) zmin=z; if(z > zmax) zmax=z;
       }
     }
   if (xmax > xmin) {theplot->bbox[0]=xmin; theplot->bbox[1]=xmax;}
   if (ymax > ymin) {theplot->bbox[2]=ymin; theplot->bbox[3]=ymax;}
   if (zmax > zmin) {theplot->bbox[4]=zmin; theplot->bbox[5]=zmax;}
   if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1;
/*   sciprint("I'm trying to zoom a 3d param3d1 plot \r\n"); */
  } 
}

static void SCPlot3d(char *plot, integer *flag, double *b1, integer *aaint, 
                     integer undo, int *bbox1, double *subwin, int win_num)
{
  int i, j;
  double xmin, xmax, ymin, ymax, zmin, zmax, xp, yp, x,y,z;
  struct plot3d_rec *theplot;
  theplot =   (struct plot3d_rec *) plot;
  if (bbox1 != NULL) 
  {
/*   sciprint("I'm trying to zoom a 3d plot\r\n"); */
   b1[0] = XPixel2Double(bbox1[0]);
   b1[1] = YPixel2Double(bbox1[1]);
   b1[2] = XPixel2Double(bbox1[2]);
   b1[3] = YPixel2Double(bbox1[3]);
/*  sciprint(" zoom area: x={%f:%f};  y={%f:%f}\r\n",b1[0],b1[2],b1[1],b1[3]); */
   xmin=Maxi(theplot->x,theplot->p); xmax=Mini(theplot->x,theplot->p);
   ymin=Maxi(theplot->y,theplot->q); ymax=Mini(theplot->y,theplot->q);
   zmin=Maxi(theplot->z,theplot->q*theplot->p);
   zmax=Mini(theplot->z,theplot->q*theplot->p);
   for (i=0; i < theplot->p; i++)
    for (j=0; j < theplot->q; j++)
     {
      x=theplot->x[i]; y=theplot->y[j]; z=theplot->z[j*theplot->p + i]; 
      xp=TRX(x,y,z); yp=TRY(x,y,z); 
      if(xp >= b1[0] && xp <= b1[2] && yp >= b1[1] && yp <= b1[3])
       {
        if(x < xmin) xmin=x; if(x > xmax) xmax=x;
        if(y < ymin) ymin=y; if(y > ymax) ymax=y;
        if(z < zmin) zmin=z; if(z > zmax) zmax=z;
/*        sciprint("%i,%i  (%f,%f,%f)-->(%f,%f)\r\n",i,j,x,y,z,xp,yp); */
       }
     }
   if (xmax > xmin) {theplot->bbox[0]=xmin; theplot->bbox[1]=xmax;}
   if (ymax > ymin) {theplot->bbox[2]=ymin; theplot->bbox[3]=ymax;}
   if (zmax > zmin) {theplot->bbox[4]=zmin; theplot->bbox[5]=zmax;}
   if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1; 
/*   sciprint("theplotflags: %i %i %i\r\n", theplot->flag[0], theplot->flag[1],
             theplot->flag[2]);
   sciprint("flags: %i %i %\r\n", flag[0], flag[1],flag[2]); */
/*   sciprint(" new bbox:: x=%f:%f; y=%f:%f; z=%f:%f\r\n",
              theplot->bbox[0],theplot->bbox[1],theplot->bbox[2],
              theplot->bbox[3],theplot->bbox[4],theplot->bbox[5]); */ 
  }
}
/*** end code added by ES ****/

static void SCvoid(char *plot, integer *flag, double *bbox, integer *aaint, integer undo, int *bbox1, double *subwin, int win_num)
{}

typedef  struct  {
  char *name;
  void  (*SC) __PARAMS((char *,integer *,double *,integer *,integer ,int *,double *,int));
} SCTable;

/* modified by ES: changed SCvoid with  SCNe3d for 3d plots */
static SCTable SCCTable[] ={
  {"axis",  SCvoid},
  {"champ", SCchamp},
  {"contour",SCContour},
  {"contour2",SCContour2D},
  {"fac3d",SCFac3d},
  {"fac3d1",SCFac3d},
  {"fac3d2",SCFac3d},
  {"fac3d3",SCFac3d}, /****** entry added by polpoth 4/5/2000 (modif by ES 21/5/2002)******/
  {"fec", SCfec},
  {"fec_n", SCfec},
  {"gray", SCgray},
  {"gray1",SCgray},
  {"gray2",SCvoid },
  {"nscale",SCNEch},
  {"param3d",SCParam3d},
  {"param3d1",SCParam3d1},
  {"plot2d",SC2D},
  {"plot3d",SCPlot3d},
  {"plot3d1",SCPlot3d},
  {"scale",SCEch},
  {"xcall1",SCvoid},
  {"xgrid",SCvoid},
  {(char *)NULL,SCvoid}};

static void SCPlot(char *type, char *plot, integer *flag, double *bbox, integer *aaint, int undo, int *bbox1, double *subwin, int win_num)
{
  int i=0;
  while ( SCCTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,SCCTable[i].name);
       if ( j == 0 ) 
	 { 
	   (*(SCCTable[i].SC))(plot,flag,bbox,aaint,undo,bbox1,subwin,win_num);
	   return;
	 }
       else 
	 { if ( j <= 0)
	     {
	       sciprint("Unknow Plot type <%s>\r\n",type);
	       return;
	     }
	   else i++;
	 }
     }
  sciprint("Unknow Plot type <%s>\r\n",type);
}

/*--------------------------------------------------------------------
 * scale undo : used in unzoom 
 *--------------------------------------------------------------------*/

static void UnSCPlots(char *unused, integer *winnumber)
{
  struct listplot *list;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	    UnSCPlot(list->type,list->theplot);
      list =list->ptrplot;
    }
}

static void UnSC2D(char *plot)
{
  int i;
  struct plot2d_rec *theplot;
  theplot=(struct plot2d_rec *) plot;
  strcpy( theplot->strflag,theplot->strflag_kp);
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->brect[i]=theplot->brect_kp[i];
      theplot->aint[i]=theplot->aint_kp[i];
    }
}
static void UnSCContour2D(char *plot)
{
  int i;
  struct contour2d_rec *theplot;
  theplot=(struct contour2d_rec *) plot;
  strcpy( theplot->strflag,theplot->strflag_kp);
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->brect[i]=theplot->brect_kp[i];
      theplot->aint[i]=theplot->aint_kp[i];
    }
}

static void UnSCgray(char *plot)
{
  int i;
  struct gray_rec *theplot;
  theplot=(struct gray_rec *) plot;
  strcpy( theplot->strflag,theplot->strflag_kp);
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->brect[i]=theplot->brect_kp[i];
      theplot->aaint[i]=theplot->aaint_kp[i];
    }
}

static void UnSCchamp(char *plot)
{
  int i;
  struct champ_rec *theplot;
  theplot=(struct champ_rec *) plot;
  strcpy( theplot->strflag,theplot->strflag_kp);
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->vrect[i]=theplot->vrect_kp[i];
    }
}

static void UnSCfec(char *plot)
{
  int i;
  struct fec_rec *theplot;
  theplot=(struct fec_rec *) plot;
  strcpy( theplot->strflag,theplot->strflag_kp);
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->brect[i]=theplot->brect_kp[i];
      theplot->aaint[i]=theplot->aaint_kp[i];
    }
}

static void UnSCEch(char *plot)
{
  int i;
  struct scale_rec *theplot;
  theplot =   (struct scale_rec *) plot;
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->Frect[i]=theplot->Frect_kp[i];
    }
}

static void UnSCNEch(char *plot)
{
  int i;
  struct nscale_rec *theplot;
  theplot =   (struct nscale_rec *) plot;
  for ( i = 0 ; i < 4 ; i++)
    {
      theplot->Frect[i]=theplot->Frect_kp[i];
    }
}

/*** code added by ES 21/5/2002 ****/
static void UnSCContour(char *plot)
{
  int i;
  struct contour_rec *theplot;
  theplot =   (struct contour_rec *) plot;
  {
   theplot->bbox[0]=Mini(theplot->x,theplot->n1); theplot->bbox[1]=Maxi(theplot->x,theplot->n1); 
   theplot->bbox[2]=Mini(theplot->y,theplot->n2); theplot->bbox[3]=Maxi(theplot->y,theplot->n2); 
   theplot->bbox[4]=Mini(theplot->z,theplot->n1*theplot->n2); 
   theplot->bbox[5]=Maxi(theplot->z,theplot->n1*theplot->n2); 
/*   sciprint("I'm trying to unzoom a 3d contour plot \r\n");*/
  }
}

static void UnSCFac3D(char *plot)
{
  int i;
  struct fac3d_rec *theplot;
  theplot =   (struct fac3d_rec *) plot;
  {
   theplot->bbox[0]=Mini(theplot->x,theplot->p*theplot->q); 
   theplot->bbox[1]=Maxi(theplot->x,theplot->p*theplot->q); 
   theplot->bbox[2]=Mini(theplot->y,theplot->p*theplot->q); 
   theplot->bbox[3]=Maxi(theplot->y,theplot->p*theplot->q); 
   theplot->bbox[4]=Mini(theplot->z,theplot->p*theplot->q); 
   theplot->bbox[5]=Maxi(theplot->z,theplot->p*theplot->q); 
/*   sciprint("I'm trying to unzoom a 3d fac plot \r\n"); */
  }
}

static void UnSCParam3D(char *plot)
{
  int i;
  struct param3d_rec *theplot;
  theplot =   (struct param3d_rec *) plot;
  {
   theplot->bbox[0]=Mini(theplot->x,theplot->n); theplot->bbox[1]=Maxi(theplot->x,theplot->n); 
   theplot->bbox[2]=Mini(theplot->y,theplot->n); theplot->bbox[3]=Maxi(theplot->y,theplot->n); 
   theplot->bbox[4]=Mini(theplot->z,theplot->n); theplot->bbox[5]=Maxi(theplot->z,theplot->n); 
/*   sciprint("I'm trying to unzoom a 3d param3d plot \r\n");*/
  }
}

static void UnSCParam3D1(char *plot)
{
  int i;
  struct param3d1_rec *theplot;
  theplot =   (struct param3d1_rec *) plot;
  {
   theplot->bbox[0]=Mini(theplot->x,theplot->n*theplot->m); 
   theplot->bbox[1]=Maxi(theplot->x,theplot->n*theplot->m); 
   theplot->bbox[2]=Mini(theplot->y,theplot->n*theplot->m); 
   theplot->bbox[3]=Maxi(theplot->y,theplot->n*theplot->m); 
   theplot->bbox[4]=Mini(theplot->z,theplot->n*theplot->m); 
   theplot->bbox[5]=Maxi(theplot->z,theplot->n*theplot->m); 
/*   sciprint("I'm trying to unzoom a 3d param3d1 plot \r\n");*/
  }
}

static void UnSCPlot3D(char *plot)
{
  int i;
  struct plot3d_rec *theplot;
  theplot =   (struct plot3d_rec *) plot;
  {

   theplot->bbox[0]=Mini(theplot->x,theplot->p); theplot->bbox[1]=Maxi(theplot->x,theplot->p); 
   theplot->bbox[2]=Mini(theplot->y,theplot->q); theplot->bbox[3]=Maxi(theplot->y,theplot->q); 
   theplot->bbox[4]=Mini(theplot->z,theplot->p*theplot->q); 
   theplot->bbox[5]=Maxi(theplot->z,theplot->p*theplot->q); 
/*   sciprint("I'm trying to unzoom a 3d plot \r\n"); */
/*  for ( i = 0 ; i < 6 ; i++)  theplot->bbox[i] =  Cscale.bbox1[i]; */
/*  if (theplot->flag[1]>0) theplot->flag[1]=2*Cscale.metric3d-1; */
/*   sciprint("theplotflags: %i %i %i\r\n", theplot->flag[0], theplot->flag[1],
             theplot->flag[2]); */
  }
}
/*** end code added by ES ****/

static void UnSCvoid(char *plot)
{}

typedef  struct  {
  char *name;
  void  (*UnSC) __PARAMS((char *));} UnSCTable;

/* modified by ES: changed UnSCvoid with  UnSCxxx3d for 3d plots */
static UnSCTable UnSCCTable[] ={
  {"axis", UnSCvoid},
  {"champ", UnSCchamp},
  {"contour",UnSCContour},
  {"contour2",UnSCContour2D},
  {"fac3d",UnSCFac3D},
  {"fac3d1",UnSCFac3D},
  {"fac3d2",UnSCFac3D},
  {"fac3d3",UnSCFac3D}, /****** entry added by polpoth 4/5/2000 (modif. by ES)******/
  {"fec", UnSCfec},
  {"fec_n", UnSCfec},
  {"gray", UnSCgray},
  {"gray1", UnSCgray},
  {"gray2", UnSCvoid},
  {"nscale",UnSCNEch},
  {"param3d",UnSCParam3D},
  {"param3d1",UnSCParam3D1},
  {"plot2d",UnSC2D},
  {"plot3d",UnSCPlot3D},
  {"plot3d1",UnSCPlot3D},
  {"scale",UnSCEch},
  {"xcall1",UnSCvoid},
  {"xgrid",UnSCvoid},
  {(char *)NULL,fnvoid}
};

static void UnSCPlot(char *type, char *plot)
{
  int i=0;
  while ( UnSCCTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,UnSCCTable[i].name);
       if ( j == 0 ) 
	 { 
	   (*(UnSCCTable[i].UnSC))(plot);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("Unknow Plot type <%s>\r\n",type);
	       return;
	     }
	   else i++;
	 }
     }
  sciprint("Unknow Plot type <%s>\r\n",type);
}

/*-------------------------------------------------------
 * checks if recorded list contains 3d graphics 
 *-------------------------------------------------------*/

int Check3DPlots(char *unused, integer *winnumber)
{
  struct listplot *list;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	{
	  if ( strcmp(list->type,"fac3d") == 0 || strcmp(list->type,"fac3d1")==0 
	      || strcmp(list->type,"fac3d2")==0 
	      || strcmp(list->type,"contour")==0 || strcmp(list->type,"param3d")==0
	       || strcmp(list->type,"param3d1")==0 
	      || strcmp(list->type,"plot3d")==0 || strcmp(list->type,"plot3d1")==0 
	      ||  strcmp(list->type,"fac3d3")==0)  /******  added by polpoth 4/5/2000 ******/
	    return(1);
	}
      list =list->ptrplot;
    }
  return(0);
}

/*-------------------------------------------------------
 * checks if xsetech was used : no more used 
 *-------------------------------------------------------*/

int EchCheckSCPlots(char *unused, integer *winnumber)
{
  struct listplot *list;
  int res=0 ;
#ifdef lint 
  *unused;
#endif
  list=ListPFirst;
  while (list)
    {
      if (list->window == *winnumber && list->theplot != NULL) 
	{
	  if ( strcmp(list->type,"scale") == 0 ) 
	    res++;
	}
      list =list->ptrplot;
    }
  return(res);
}

/*---------------------------------------------------------------------
 *  restore scales (unzoom) and redraw stored graphics 
 *---------------------------------------------------------------------------*/

void Tape_ReplayUndoScale(char *unused, integer *winnumber)
{ 
  UnSCPlots(unused,winnumber);
  Tape_Replay("v",winnumber,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/*---------------------------------------------------------------------
 * used when zooming: replay with a new scale 
 * the new scale is coded in bbox=[xmin,ymin,xmax,ymax] 
 * the problem is a bit complex if we have manu subwindows 
 *---------------------------------------------------------------------------*/

void Tape_ReplayNewScale(char *unused, integer *winnumber, integer *flag, integer *v1, integer *aaint, integer *v6, integer *v7, double *bbox, double *dx2, double *dx3, double *dx4)
{ 
  /** get the bounding box in pixel */
  int bbox1[4];
  bbox1[0]= XDouble2Pixel(bbox[0]);
  bbox1[1]= YDouble2Pixel(bbox[1]);
  bbox1[2]= XDouble2Pixel(bbox[2]);
  bbox1[3]= YDouble2Pixel(bbox[3]);
  SCPlots(unused,winnumber,flag,bbox,aaint,1,bbox1,NULL);
  Tape_Replay("v",winnumber,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/*---------------------------------------------------------------------
 * replay with a new Scale but undo is impossible 
 * used for automatic scales 
 *---------------------------------------------------------------------------*/

void Tape_ReplayNewScale1(char *unused, integer *winnumber, integer *flag, integer *v1, integer *aaint, integer *v6, integer *v7, double *bbox, double *dx2, double *dx3, double *dx4)
{ 
  /* here we want to change bbox but only for recorded graphics 
   * which are on the same subwin as the current one 
   * and we do not want this operation to be undone ==> undo =0 
   */
  SCPlots(unused,winnumber,flag,bbox,aaint,0,NULL,Cscale.subwin_rect);
  Tape_Replay("v",winnumber,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/*---------------------------------------------------------------------
 *  changes theta, alpha  flag and bbox 
 *  then redraw recorded graphics 
 *---------------------------------------------------------------------------*/

extern char GetDriver(void);

void Tape_ReplayNewAngle(char *unused, integer *winnumber, integer *v1, integer *v2, integer *iflag, integer *flag, integer *v3, double *theta, double *alpha, double *bbox, double *dx4)
{ 
  NAPlots(unused,winnumber,theta,alpha,iflag,flag,bbox);
  Tape_Replay("v",winnumber,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
}

/*---------------------------------------------------------------------
 * redraw stored graphics 
 *---------------------------------------------------------------------------*/

void Tape_Replay(char *unused, integer *winnumber, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dx1, double *dx2, double *dx3, double *dx4)
{ 
#ifdef WIN32
  int flag;
#endif
  char name[4];
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /* first check if a special handler is set */
  scig_handler(*winnumber);
  if (ListPFirst != NULL)
    {
      if ( name[0] =='R' )
	{
#ifdef WIN32
	  /** win32 : we dont't want to use dr because it will 
	  change the hdc **/
	  C2F(SetDriver)("Int",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
#else
	  C2F(dr)("xsetdr","X11",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif
	}
#ifdef WIN32
      /** if hdc is not set we use the current window hdc **/
      flag=MaybeSetWinhdc();
#endif
      Tape_Replay1(ListPFirst,*winnumber);
#ifdef WIN32
      if ( flag == 1) ReleaseWinHdc();
#endif
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
}

static void Tape_Replay1(struct listplot *list, integer winnumber)
{
  if (list->window == winnumber) GReplay(list->type,list->theplot);
  if (list->ptrplot != NULL) Tape_Replay1(list->ptrplot,winnumber);
}

static void Replay3D(char *theplot)
{
  struct plot3d_rec *pl3d;
  pl3d= (struct plot3d_rec *)theplot;
  C2F(plot3d)(pl3d->x,pl3d->y,pl3d->z,&pl3d->p,&pl3d->q,&pl3d->teta,
	  &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

static void ReplayFac3D(char *theplot)
{
  struct fac3d_rec *pl3d;
  pl3d= (struct fac3d_rec *)theplot;
  C2F(fac3d)(pl3d->x,pl3d->y,pl3d->z,pl3d->cvect,&pl3d->p,&pl3d->q,&pl3d->teta,
	     &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

static void ReplayFac3D1(char *theplot)
{
  struct fac3d_rec *pl3d;
  pl3d= (struct fac3d_rec *)theplot;
  C2F(fac3d1)(pl3d->x,pl3d->y,pl3d->z,pl3d->cvect,
	      &pl3d->p,&pl3d->q,&pl3d->teta,
	      &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

static void ReplayFac3D2(char *theplot)
{
  struct fac3d_rec *pl3d;
  pl3d= (struct fac3d_rec *)theplot;
  C2F(fac3d2)(pl3d->x,pl3d->y,pl3d->z,pl3d->cvect,
	      &pl3d->p,&pl3d->q,&pl3d->teta,
	      &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

/****** code added by polpoth 4/5/2000 ******/

static void ReplayFac3D3(char *theplot)
{
  struct fac3d_rec *pl3d;
  pl3d= (struct fac3d_rec *)theplot;
  C2F(fac3d3)(pl3d->x,pl3d->y,pl3d->z,pl3d->cvect,
	      &pl3d->p,&pl3d->q,&pl3d->teta,
	      &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

/****** end of code added ******************/

static void ReplayFec(char *theplot)
{
  struct fec_rec *plfec;
  plfec= (struct fec_rec *)theplot;
  C2F(fec)(plfec->x,plfec->y,plfec->triangles,plfec->func,
	   &plfec->Nnode,&plfec->Ntr,
	   plfec->strflag,plfec->legend,plfec->brect,plfec->aaint,
	   plfec->zminmax, plfec->colminmax,    /* added by bruno */
	   0L,0L);
}

static void ReplayContour(char *theplot)
{
  struct contour_rec *pl3d;
  pl3d= (struct contour_rec *)theplot;
  C2F(contour)(pl3d->x,pl3d->y,pl3d->z,&pl3d->n1,&pl3d->n2,&pl3d->flagnz,&pl3d->nz,
	   pl3d->zz,&pl3d->teta,
	  &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,&pl3d->zlev,0L);
}

static void ReplayContour2D(char *theplot)
{
  struct contour2d_rec *pl3d;
  pl3d= (struct contour2d_rec *)theplot;
  C2F(contour2)(pl3d->x,pl3d->y,pl3d->z,&pl3d->n1,&pl3d->n2,&pl3d->flagnz,&pl3d->nz,
	       pl3d->zz, pl3d->style,pl3d->strflag,pl3d->legend,
	       pl3d->brect,pl3d->aint,0L,0L);
}


static void ReplayGray(char *theplot)
{
  struct gray_rec *pl3d;
  pl3d= (struct gray_rec *)theplot;
  C2F(xgray)(pl3d->x,pl3d->y,pl3d->z,&pl3d->n1,&pl3d->n2,
	     pl3d->strflag,pl3d->brect,pl3d->aaint,0L);
}



static void ReplayGray1(char *theplot)
{
  struct gray_rec *pl3d;
  pl3d= (struct gray_rec *)theplot;
  C2F(xgray1)(pl3d->z,&pl3d->n1,&pl3d->n2,
	     pl3d->strflag,pl3d->brect,pl3d->aaint,0L);
}



static void ReplayGray2(char *theplot)
{
  struct gray_rec_2 *pl3d;
  pl3d= (struct gray_rec_2 *)theplot;
  C2F(xgray2)(pl3d->z,&pl3d->n1,&pl3d->n2,
	     pl3d->xrect);
}


static void ReplayParam3D(char *theplot)
{
  struct param3d_rec *pl3d;
  pl3d= (struct param3d_rec *)theplot;
  C2F(param3d)(pl3d->x,pl3d->y,pl3d->z,&pl3d->n,&pl3d->teta,
	  &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

static void ReplayParam3D1(char *theplot)
{
  struct param3d1_rec *pl3d;
  pl3d= (struct param3d1_rec *)theplot;
  C2F(param3d1)(pl3d->x,pl3d->y,pl3d->z,&pl3d->m,&pl3d->n,&pl3d->iflag,
	       pl3d->colors, &pl3d->teta,
	       &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}

static void Replay3D1(char *theplot)
{
  struct plot3d_rec *pl3d;
  pl3d= (struct plot3d_rec *)theplot;
  C2F(plot3d1)(pl3d->x,pl3d->y,pl3d->z,&pl3d->p,&pl3d->q,&pl3d->teta,
	  &pl3d->alpha,pl3d->legend,pl3d->flag,pl3d->bbox,0L);
}


typedef  struct  {
  char *name;
  int  (*fonc)();} OpTab ;

static int fnvide(void) {return(0);}

extern int C2F(plot2d1)(),C2F(plot2d2)(),C2F(plot2d3)(),C2F(plot2d4)();

OpTab plottab[] ={
  {"plot2d1",C2F(plot2d1)},
  {"plot2d2",C2F(plot2d2)},
  {"plot2d3",C2F(plot2d3)},
  {"plot2d4",C2F(plot2d4)},
  {(char *) NULL,fnvide}
};


static void Replay2D(char *theplot)
{
  int i=0;
  struct plot2d_rec *pl2d;
  pl2d= (struct plot2d_rec *)theplot;
      while ( plottab[i].name != (char *) NULL)
      {
       int j;
       j = strcmp(pl2d->name,plottab[i].name);
       if ( j == 0 ) 
	 { 
	   (*(plottab[i].fonc))(pl2d->xf,
		    pl2d->x,pl2d->y,&(pl2d->n1),&(pl2d->n2),
		    pl2d->style,pl2d->strflag,pl2d->legend,
		    pl2d->brect,pl2d->aint,0L,0L,0L);
	   return;}
       else 
	 { if ( j <= 0)
	     {
	       sciprint("\nUnknow operator <%s>\r\n",pl2d->name);
	       return;
	     }
	   else i++;
	 }
     }
      sciprint("\n Unknow  operator <%s>\r\n",pl2d->name);
}

static void ReplaySciAxis(char *theplot)
{
  struct sciaxis_rec *p = (struct sciaxis_rec *) theplot;
  sci_axis(p->pos,p->xy_type,p->x,&p->nx,p->y,&p->ny,p->str,p->subtics,p->format,p->fontsize,p->textcolor,
	   p->ticscolor,p->logflag,p->seg_flag);

}

static void ReplayGrid(char *theplot)
{
  struct xgrid_rec *plch;
  plch= (struct xgrid_rec *)theplot;
  C2F(xgrid)(&(plch->style));
}

static void ReplayEch(char *theplot)
{
  struct scale_rec *plch;
  plch= (struct scale_rec *)theplot;
  C2F(setscale2d)(plch->Wrect,plch->Frect,plch->logflag,0L);
}

static void ReplayNEch(char *theplot)
{
  struct nscale_rec *plch;
  plch= (struct nscale_rec *)theplot;
  set_scale(plch->flag,plch->Wrect,plch->Frect,NULL,plch->logflag,plch->Arect);
}

static void ReplayChamp(char *theplot)
{
  struct champ_rec *plch;
  plch= (struct champ_rec *)theplot;
  if ( strcmp(plch->name,"champ")==0)
    C2F(champ)(plch->x,plch->y,plch->fx,plch->fy,&(plch->n1),&(plch->n2),
	       plch->strflag,plch->vrect,&(plch->arfact),0L);
  else 
    C2F(champ1)(plch->x,plch->y,plch->fx,plch->fy,&(plch->n1),&(plch->n2),
	       plch->strflag,plch->vrect,&(plch->arfact),0L);
}

/** 
  This function set a flag for color behaviour : 
  when this flag is set, the "xset","use color" 
  events are not replayed by the graphic recorder 
  this is used in Actions.c to force color or b&w 
  when sending file to Postscript 
**/

static int special_color=0;

void UseColorFlag(int flag)
{
  special_color=flag;
}

static void ReplayX1(char *theplot)
{
  struct xcall1_rec *plch;
  plch= (struct xcall1_rec *)theplot;
  if ( special_color == 1 && strcmp(plch->fname,"xset") == 0 
       && strcmp(plch->string,"use color")==0 )
    {
      /* sciprint("!!!!! Not replaying use color\r\n");*/
      return;
    }
  else 
    {
      C2F(dr1)(plch->fname,plch->string,plch->x1,plch->x2,
	       plch->x3,plch->x4,plch->x5,plch->x6,
	       plch->dx1,plch->dx2,plch->dx3,plch->dx4,0L,0L);
    }
}


typedef  struct  {
  char *name;
  void  (*replay) __PARAMS((char *));
} ReplayTable;



static ReplayTable RTable[] ={
  {"axis", ReplaySciAxis},
  {"champ",ReplayChamp},
  {"contour",ReplayContour},
  {"contour2",ReplayContour2D},
  {"fac3d",ReplayFac3D},
  {"fac3d1",ReplayFac3D1},
  {"fac3d2",ReplayFac3D2},
  {"fac3d3",ReplayFac3D3},
  {"fec",ReplayFec},
  {"fec_n",ReplayFec},
  {"gray",ReplayGray},
  {"gray1",ReplayGray1},
  {"gray2",ReplayGray2},
  {"nscale" ,ReplayNEch},
  {"param3d",ReplayParam3D},
  {"param3d1",ReplayParam3D1},
  {"plot2d",Replay2D},
  {"plot3d",Replay3D},
  {"plot3d1",Replay3D1},
  {"scale" ,ReplayEch},
  {"xcall1",ReplayX1},
  {"xgrid",ReplayGrid},
  {(char *) NULL,fnvoid}
};

static void GReplay(char *type, char *plot)
{
  int i=0;
  while ( RTable[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(type,RTable[i].name);
       if ( j == 0 ) 
	 { 
	   (*(RTable[i].replay))(plot);
	   return;
	 }
       else 
	 { if ( j <= 0)
	     {
	       sciprint("\nUnknow Plot type <%s>\r\n",type);
	       return;
	     }
	   else i++;
	 }
     }
  sciprint("\n Unknow Plot type <%s>\r\n",type);
}


/*---------------------------------------------------------------------
 * Add a new graphics record in the graphic recorder lisy 
 *---------------------------------------------------------------------------*/

int Store(char *type, char *plot)
{
  if (ListPFirst == NULL)
      {
	ListPFirst = (struct listplot *)MALLOC(sizeof(struct listplot));
	if (ListPFirst != NULL)
	  {
	    if (CopyVectC(&(ListPFirst->type),type,((int)strlen(type))+1)==0)
	      { ListPFirst=NULL;
		Scistring("Store : No more Place \n");
		return(0);
	      }
	    ListPFirst->theplot=plot;
	    ListPFirst->window=curwin();
	    ListPFirst->ptrplot=NULL;
	    ListPFirst->previous=NULL;
	  }
	else
	  {
	    Scistring("Store (store-1): malloc No more Place");
	    return(0);
	  }
      }
  else 
    {
      struct listplot *list;
      list=ListPFirst;
      while (list->ptrplot != NULL) 
	list=list->ptrplot;
      list->ptrplot=(struct listplot *)
	MALLOC(sizeof(struct listplot));
      if (list->ptrplot != NULL)
	{
	  if (CopyVectC(&(list->ptrplot->type),type,((int)strlen(type))+1)==0)
	    { list=NULL;
	      Scistring("Store (store-2): No more Place \n");
	      return(0);}
	  list->ptrplot->theplot=plot;
	  list->ptrplot->previous=list;
	  list->ptrplot->window=curwin();
	  list->ptrplot->ptrplot=NULL;
	}
      else 
	{
	  Scistring("Store (store-3):No more Place\n");
	  return(0);
	}
    }
  return(1);
}











