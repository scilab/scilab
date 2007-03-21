/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Math.h"
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "ObjectStructure.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

static double MiniD __PARAMS((double *x,integer n));
extern void Champ2DRealToPixel __PARAMS((integer *xm,integer *ym,integer *zm,integer *na,
					 integer *arsize,integer *colored,double *x,double *y,
                                         double  *fx,double *fy,integer *n1,integer *n2,double *arfact));
extern void initsubwin();
/* extern void compute_data_bounds(int cflag,char dataflag,double *x,double *y,int n1,int n2,double *drect); */
extern void compute_data_bounds2(int cflag,char dataflag,char *logflags,double *x,double *y,int n1,int n2,double *drect);
extern BOOL update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);
extern int re_index_brect(double * brect, double * drect);
extern BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag);

void getChampDataBounds( double   xCoords[]  ,
                         double   yCoords[]  ,
                         double   xLength[]  ,
                         double   yLength[]  ,
                         int      nbRow      ,
                         int      nbCol      ,
                         int      typeOfChamp,
                         double * xMin       ,
                         double * xMax       ,
                         double * yMin       ,
                         double * yMax        ) ;


/*-----------------------------------------------------------------
 *  int C2F(champ)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *  int C2F(champ1)(x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr)
 *
 * Used for Drawing 2 dimensional vector fields 
 * (fx[i+(*n1)*j], fy[i+(*n1)*j]) is the value of the vector field 
 * at point X=(x[i],y[j]);
 * 
 * - fx and fy are (*n1)*(*n2) matrix of double
 * - arfact : a factor by which to multiply the default arrow size 
 *          usr 1.0 by defaut 
 * - strflag : a string of length 3 (see plot2d) 
 * - brect=[xmin,ymin,xmax,ymax]    (see plot2d) 
 *
 * - lstr : (used when called from Fortran code)
 -------------------------------------------------------------------*/

void champg(char *name, integer colored, double *x, double *y, double *fx, double *fy, integer *n1, 
	    integer *n2, char *strflag, double *brect, double *arfact, integer lstr )
{
  static integer aaint[]={2,10,2,10};
  integer *xm = NULL,*ym = NULL,*zm = NULL,na,n;
  double  xx[2],yy[2];
  integer arsize,nn1=1,nn2=2,iflag=0;  
  /* NG */
  sciPointObj  *psubwin;
  integer flag,type =1;
  double arsize1;
  integer *style;
  integer i;
  double drect[6];
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;
  
  /* champ with color inheritated from subwin */
  /* or champ1 (normed vector + color) is enabled */
  int typeofchamp = ( colored == 0 ? 0 : 1 ) ;

  /* get default dash for arrows **/
  integer verbose=0,narg,xz[10],uc;

    
  C2F(dr)("xget","use color",&verbose, &uc, &narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (uc)
    C2F(dr)("xget","color",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  else
    C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /** The arrowsize acording to the windowsize **/
  n=2*(*n1)*((*n2)+1); /*F.Leray 17.02.04*/
  
  /* get the bounding rect of the displayed champ */
  getChampDataBounds( x, y, fx, fy, *n1, *n2,typeofchamp,  &(xx[0]), &(xx[1]), &(yy[0]), &(yy[1]) ) ;
  
  if (version_flag() == 0)
  {
    sciPointObj * newChamp = NULL ;
    if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
      sciXbasc();  
      initsubwin();
      sciRedrawFigure();
    }  
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    
    /* Force psubwin->is3d to FALSE: we are in 2D mode */
    if (sciGetSurface(psubwin) == (sciPointObj *) NULL)
    {
      pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
      pSUBWIN_FEATURE (psubwin)->project[2]= 0;
    } 
    else
    {
      pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
      pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;  
    }
    
    pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
    pSUBWIN_FEATURE (psubwin)->theta  = 270.0;
    
    
    /* Force "cligrf" clipping */
    sciSetIsClipping (psubwin,0); 
    
    /* Force  axes_visible property */
    
    if (sciGetGraphicMode (psubwin)->autoscaling) {
      /* compute and merge new specified bounds with psubwin->Srect */
      switch (strflag[1])  {
      case '0': 
        /* do not change psubwin->Srect */
        break;
      case '1' : case '3' : case '5' : case '7':
        re_index_brect(brect,drect);
        break;
      case '2' : case '4' : case '6' : case '8': case '9':
        /* Force psubwin->Srect to the x and y bounds */
        compute_data_bounds2(0,'g',pSUBWIN_FEATURE (psubwin)->logflags,xx,yy,nn1,nn2,drect);
        break;
      }
      if (!pSUBWIN_FEATURE(psubwin)->FirstPlot &&
          (strflag[1] == '7' || strflag[1] == '8'|| strflag[1] == '9')) { /* merge psubwin->Srect and drect */
        drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
        drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
        drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
        drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
      }
      
      
      if (strflag[1] != '0')
        bounds_changed = update_specification_bounds(psubwin, drect,2);
    }
    
    if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;
    
    axes_properties_changed = strflag2axes_properties(psubwin, strflag);
    
    pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */
    
    if( bounds_changed || axes_properties_changed )
    {
      sciDrawObj(sciGetCurrentFigure());
    }
    
    flag = 1; /* je le mets à 1 pour voir F.Leray 19.02.04*/
    arsize1 = *arfact;
    
    /* F.Leray Allocation de style[dim = Nbr1] */
    if ((style = MALLOC ((*n1) * sizeof (integer))) == NULL) {
      sciprint("No more memory available\n");
      return;
    }
    
    for( i = 0 ; i < (*n1) ; i++ ) { style[i] = i ; }

    newChamp = ConstructSegs(psubwin,type,x,y,*n1,*n2,fx,fy,flag,
                             style,arsize1,colored,*arfact,typeofchamp) ;

    sciSetCurrentObj( newChamp ) ; 
      
    sciDrawObjIfRequired(sciGetCurrentObj ()); 
    DrawAxesIfRequired(sciGetCurrentObj ()); /* force axes redrawing */
    /* F.Leray Liberation de style[dim = Nbr1]*/
    if( style != (integer *) NULL) FREE(style); style = (integer *) NULL;
  }
  else {
    update_frame_bounds(0,"gnn",xx,yy,&nn1,&nn2,aaint,strflag,brect);
    
    /* Storing values if using the Record driver */
    if (GetDriver()=='R')
      StoreChamp(name,x,y,fx,fy,n1,n2,strflag,brect,arfact); 
    
    
    axis_draw(strflag);
    /** Allocation **/  
    xm = graphic_alloc(0,n,sizeof(int));
    ym = graphic_alloc(1,n,sizeof(int));
    if ( xm == 0 || ym == 0) {
      sciprint("Running out of memory \n");
      return ;
    }      
    if ( colored != 0) {
      zm = graphic_alloc(2,n/2,sizeof(int)); /* F.Leray 19.02.04 OK one norm by vector => dim(zm) = n1*n2*/
      if (  zm == 0 ) {
	sciprint("Running out of memory \n");
	return ;
      }      
    }
    Champ2DRealToPixel(xm,ym,zm,&na,&arsize,&colored,x,y,fx,fy,n1,n2,arfact); 
    /** Drawing the curves **/
    frame_clip_on();
    if ( colored ==0) 
      C2F(dr)("xarrow","v",xm,ym,&na,&arsize,xz,&iflag,PD0,PD0,PD0,PD0,0L,0L);
    else
      C2F(dr)("xarrow","v",xm,ym,&na,&arsize,zm,(iflag=1,&iflag),PD0,PD0,PD0,PD0,0L,0L);
    frame_clip_off();
  }
}

int C2F(champ)(double *x, double *y, double *fx, double *fy, integer *n1, integer *n2, char *strflag, double *brect, double *arfact, integer lstr)
{
  champg("champ",0,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0); 
}

int C2F(champ1)(double *x, double *y, double *fx, double *fy, integer *n1, integer *n2, char *strflag, double *brect, double *arfact, integer lstr)
{
  champg("champ1",1,x,y,fx,fy,n1,n2,strflag,brect,arfact,lstr);
  return(0);
}


/*----------------------------------
 * Returns min( abs(x)) excluding null x(i)  values 
 * if x==0 then 1 is returned 
 *----------------------------------*/

static double MiniD(double *x, integer n)
{
  int i;
  double dx=1,mindx=1;
  if ( n < 2 ) return(mindx);
  mindx= Abs(x[1]-x[0]);
  mindx = ( mindx != 0 ) ? mindx : 1;
  for ( i = 2 ; i < n ; i++) 
    {
      dx = Abs(x[i]-x[i-1]);
      if ( dx < mindx && dx != 0 ) mindx=dx;
    }
  return(mindx);
}


extern void Champ2DRealToPixel(xm,ym,zm,na,arsize,colored,x,y,fx,fy,n1,n2,arfact)

     integer *xm,*ym,*zm;
     integer *na,*arsize,*colored;
     integer *n1,*n2;
     double *x, *y, *fx, *fy;
     double *arfact;
{  
 
  integer i,j;
  double  maxx;
  double  nx,ny,sc,sfx,sfy,sfx2,sfy2;
  double  arsize1=0.5,arsize2=0.5;
  /* get default dash for arrows **/
  integer verbose=0,narg;
  int xfacteur = 1;
  int yfacteur = 1;
  

  /* From double to pixels */
  for ( i = 0 ; i < *n1 ; i++)
    for ( j =0 ; j < *n2 ; j++)
      {
	xm[2*(i +(*n1)*j)]= XScale(x[i]);
	ym[2*(i +(*n1)*j)]= YScale(y[j]);
      }

  /** Scaling **/
  nx=MiniD(x,*n1)*Cscale.Wscx1;
  ny=MiniD(y,*n2)*Cscale.Wscy1;
  sfx= Cscale.Wscx1;
  sfy= Cscale.Wscy1;
  sfx2= sfx*sfx;
  sfy2= sfy*sfy;
  maxx = getLongestVector( fx, fy, *n1, *n2, sfx, sfy ) ;
  sc=maxx;
  sc= sqrt(nx*nx+ny*ny)/sc;
  sfx *= sc;
  sfy *= sc;
  /** size of arrow **/
  arsize1= ((double) Cscale.WIRect1[2])/(5*(*n1));
  arsize2= ((double) Cscale.WIRect1[3])/(5*(*n2));
  *arsize=  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
  *arsize = (int)((*arsize)*(*arfact));

  set_clip_box(Cscale.WIRect1[0],Cscale.WIRect1[0]+Cscale.WIRect1[2],Cscale.WIRect1[1],
	       Cscale.WIRect1[1]+Cscale.WIRect1[3]);

  if ( *colored == 0 ) 
    {
      int j=0;
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  integer x1n,y1n,x2n,y2n,flag1=0;
	  
	  xm[1+2*j]= (int)(xfacteur*sfx*fx[i]+xm[2*i]);
	  xm[2*j]  = (int)(xm[2*i]);
 	  ym[1+2*j]= (int)(-yfacteur*sfy*fy[i]+ym[2*i]);
	  ym[2*j]  = (int)(ym[2*i]);
	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if (flag1 !=0)
	    {
	      if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
	      if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
              j++;
	    }
	}
      *na=2*j;
    }
  else 
    {
      integer x1n,y1n,x2n,y2n,flag1=0;
      integer whiteid;
      int j=0;
      C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  double nor= sqrt(sfx2*fx[i]*fx[i]+sfy2*fy[i]*fy[i]);
	  zm[j] = inint( ((double) whiteid - 1)*(1.0 - nor/maxx)) +1;
	  nor= sqrt(fx[i]*(fx[i])+fy[i]*(fy[i]));

	  /*        modif bruno (juin 2003) to have the "queue" of the arrow positionned
	   *        at the point (before the arrow was placed such as the corresponding
	   *        point was at the middle of the arrow)       
	   *
	   *        this is the old code :
	   *
	   * 	  xm[1+2*j]= (int)(sfx*fx[i]/(2*nor)+xm[2*i]); 
	   * 	  xm[2*j]  = (int)(-sfx*fx[i]/(2*nor)+xm[2*i]); 
	   * 	  ym[1+2*j]= (int)(-sfy*fy[i]/(2*nor)+ym[2*i]); 
	   * 	  ym[2*j]  = (int)(sfy*fy[i]/(2*nor)+ym[2*i]); 
	   *
	   *        the new code :
	   */
	  xm[1+2*j]= (int)(xfacteur*sfx*(fx[i])/(nor)+xm[2*i]);
	  xm[2*j]  = (int)(xm[2*i]);
	  ym[1+2*j]= (int)(-yfacteur*sfy*(fy[i])/(nor)+ym[2*i]);
	  ym[2*j]  = (int)(ym[2*i]);
	  /* end of the modif */

	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if (flag1 !=0)
	    {
	      if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
	      if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
	      j++;
	    }
	}
      *na=2*j;
    }
}



/* F.Leray 11.03.05 */
/* For new graphic style only */
/* same thing has above (Champ2DRealToPixel) */
/* only difference is in the typeofchamp treatment (taht replaces the colored flag) */
extern void sciChamp2DRealToPixel( integer * xm         ,
                                   integer * ym         ,
                                   integer * zm         ,
                                   integer * na         ,
                                   integer * arsize     ,
                                   double  * x          ,
                                   double  * y          ,
                                   double  * fx         ,
                                   double  * fy         ,
                                   integer * n1         ,
                                   integer * n2         ,
                                   double  * arfact     ,
                                   int     * typeofchamp,
                                   BOOL      clipping    )
{  
 
  integer i,j;
  double  maxx;
  double  nx,ny,sc,sfx,sfy,sfx2,sfy2;
  double  arsize1=0.5,arsize2=0.5;
  /* get default dash for arrows **/
  integer verbose=0,narg;
  int xfacteur = 1;
  int yfacteur = 1;
  
  sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  
  if(ppsubwin->axes.reverse[0] == TRUE) xfacteur = -1;
  if(ppsubwin->axes.reverse[1] == TRUE) yfacteur = -1;


  /* From double to pixels */
  for ( i = 0 ; i < *n1 ; i++)
    for ( j =0 ; j < *n2 ; j++)
      {
	xm[2*(i +(*n1)*j)]= XScale(x[i]);
	ym[2*(i +(*n1)*j)]= YScale(y[j]);
      }

  /** Scaling **/
  nx=MiniD(x,*n1)*Cscale.Wscx1;
  ny=MiniD(y,*n2)*Cscale.Wscy1;
  sfx= Cscale.Wscx1;
  sfy= Cscale.Wscy1;
  sfx2= sfx*sfx;
  sfy2= sfy*sfy;
  maxx = getLongestVector( fx, fy, *n1, *n2, sfx, sfy ) ;
  sc=maxx;
  sc= sqrt(nx*nx+ny*ny)/sc;
  sfx *= sc;
  sfy *= sc;
  /** size of arrow **/
  arsize1= ((double) Cscale.WIRect1[2])/(5*(*n1));
  arsize2= ((double) Cscale.WIRect1[3])/(5*(*n2));
  *arsize=  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
  *arsize = (int)((*arsize)*(*arfact));

  set_clip_box(Cscale.WIRect1[0],Cscale.WIRect1[0]+Cscale.WIRect1[2],Cscale.WIRect1[1],
	       Cscale.WIRect1[1]+Cscale.WIRect1[3]);

  if ( *typeofchamp == 0 ) 
    {
      int j=0;
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  integer x1n,y1n,x2n,y2n,flag1=0;
          xm[1+2*j]= (int)(xfacteur*sfx*fx[i]+xm[2*i]);
	  xm[2*j]  = (int)(xm[2*i]);
 	  ym[1+2*j]= (int)(-yfacteur*sfy*fy[i]+ym[2*i]);
	  ym[2*j]  = (int)(ym[2*i]);
	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if ( flag1 !=0 || !clipping )
          {
            if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
            if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
            j++;
          }
          else if ( !clipping )
          {
            j++ ;
          }
	}
      *na=2*j;
    }
  else 
    {
      integer x1n,y1n,x2n,y2n,flag1=0;
      integer whiteid;
      int j=0;
      C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      for ( i = 0 ; i < (*n1)*(*n2) ; i++)
	{
	  double nor= sqrt(sfx2*fx[i]*fx[i]+sfy2*fy[i]*fy[i]);
	  zm[j] = inint( ((double) whiteid -1 )*(1.0 - nor/maxx)) +1;
	  nor= sqrt(fx[i]*(fx[i])+fy[i]*(fy[i]));

	  /*        modif bruno (juin 2003) to have the "queue" of the arrow positionned
	   *        at the point (before the arrow was placed such as the corresponding
	   *        point was at the middle of the arrow)       
	   *
	   *        this is the old code :
	   *
	   * 	  xm[1+2*j]= (int)(sfx*fx[i]/(2*nor)+xm[2*i]); 
	   * 	  xm[2*j]  = (int)(-sfx*fx[i]/(2*nor)+xm[2*i]); 
	   * 	  ym[1+2*j]= (int)(-sfy*fy[i]/(2*nor)+ym[2*i]); 
	   * 	  ym[2*j]  = (int)(sfy*fy[i]/(2*nor)+ym[2*i]); 
	   *
	   *        the new code :
	   */
	  xm[1+2*j]= (int)(xfacteur*sfx*(fx[i])/(nor)+xm[2*i]);
	  xm[2*j]  = (int)(xm[2*i]);
	  ym[1+2*j]= (int)(-yfacteur*sfy*(fy[i])/(nor)+ym[2*i]);
	  ym[2*j]  = (int)(ym[2*i]);
	  /* end of the modif */

	  clip_line(xm[2*j],ym[2*j],xm[2*j+1],ym[2*j+1],&x1n,&y1n,&x2n,&y2n,&flag1);
	  if ( flag1 != 0 )
          {
            if (flag1==1||flag1==3) { xm[2*j]=x1n;ym[2*j]=y1n;};
            if (flag1==2||flag1==3) { xm[2*j+1]=x2n;ym[2*j+1]=y2n;};
            j++;
          }
          else if ( !clipping )
          {
            j++ ;
          }
	}
      *na=2*j;
    }
}

/*-------------------------------------------------------------------------------------------*/
/**
 * Compute the size of the area used by a champ object( ie scisegs with ptype = 1).
 * @param[in]  xCoords     position of the champ grid
 * @param[in]  yCoords     position of the champ grid
 * @param[in]  xLength     size of the arrow
 * @param[in]  yLength     size of the arrow
 * @param      nbRow       number of row in the champ grid
 * @param      nbCol       number of column in the champ grid.
 * @param      typeOfChamp 0 for champ and 1 for champ1
 * @param[out] xMin        bounding rect of the champ object
 * @param[out] xMax        bounding rect of the champ object
 * @param[out] yMin        bounding rect of the champ object
 * @param[out] yMax        bounding rect of the champ object
 */
void getChampDataBounds( double   xCoords[]  ,
                         double   yCoords[]  ,
                         double   xLength[]  ,
                         double   yLength[]  ,
                         int      nbRow      ,
                         int      nbCol      ,
                         int      typeOfChamp,
                         double * xMin       ,
                         double * xMax       ,
                         double * yMin       ,
                         double * yMax        )
{
  int      i                                ;
  int    * xPixCoords   = NULL              ;
  int    * yPixCoords   = NULL              ;
  int    * zPixCoords   = NULL              ;
  int      nbArrowEnds  = 2 * nbRow * nbCol ; /* 2 time the number of arrows */
  int      nbArrows                         ;
  int      arrowSize     = 0                ; /* arrow size does not modify bounds for now */
  double   arrowSizeFact = 0.0              ;
    

  if ( nbArrowEnds == 0 )
  {
    *xMin = 0.0 ;
    *xMax = 0.0 ;
    *yMin = 0.0 ;
    *yMax = 0.0 ;
    return ;
  }

  xPixCoords = MALLOC( nbArrowEnds   * sizeof(int) ) ;
  yPixCoords = MALLOC( nbArrowEnds   * sizeof(int) ) ;
  zPixCoords = MALLOC( nbRow * nbCol * sizeof(int) ) ;

  if ( xPixCoords == NULL || yPixCoords == NULL || zPixCoords == NULL )
  {
    FREE( xPixCoords ) ;
    FREE( yPixCoords ) ;
    FREE( zPixCoords ) ;
    sciprint( "Cannot allocate temporary vector, memory full.\n" ) ;
    *xMin = 0.0 ;
    *xMax = 0.0 ;
    *yMin = 0.0 ;
    *yMax = 0.0 ;
    return ;
  }
  
  /* get the bounds in pixels */
  sciChamp2DRealToPixel( xPixCoords    ,
                         yPixCoords    ,
                         zPixCoords    ,
                         &nbArrows     ,
                         &arrowSize    ,
                         xCoords       ,
                         yCoords       ,
                         xLength       ,
                         yLength       ,
                         &nbRow        ,
                         &nbCol        ,
                         &arrowSizeFact,
                         &typeOfChamp  ,
                         FALSE          ) ;

  

  /* get extrema on X and Y */
  *xMin = XPixel2Double( xPixCoords[0] ) ;
  *xMax = *xMin ;

  *yMin = YPixel2Double( yPixCoords[0] ) ;
  *yMax = *yMin ;

  for ( i = 1 ; i < nbArrows ; i++ )
  {
    double currentCoordX = XPixel2Double( xPixCoords[i] ) ;
    double currentCoordY = YPixel2Double( yPixCoords[i] ) ;
    
    *xMin = Min( *xMin, currentCoordX ) ;
    *xMax = Max( *xMax, currentCoordX ) ;

    *yMin = Min( *yMin, currentCoordY ) ;
    *yMax = Max( *yMax, currentCoordY ) ;
  }

  FREE( xPixCoords ) ;
  FREE( yPixCoords ) ;
  FREE( zPixCoords ) ;
    
}
/*-------------------------------------------------------------------------------------------*/
double computeGridMinGap( double gridX[], double gridY[], int nbRow, int nbCol )
{
  int i ;
  double min = 0 ;
  double minX = Abs( gridX[1] - gridX[0] ) ;
  double minY = Abs( gridY[1] - gridY[0] ) ;

  for ( i = 1 ; i < nbRow-1 ; i++ )
  {
    double tmp = Abs( gridX[i+1] - gridX[i] ) ;
    if ( tmp < minX )
    {
      minX = tmp ;
    }
  }

  for ( i = 1 ; i < nbCol-1 ; i++ )
  {
    double tmp = Abs( gridY[i+1] - gridY[i] ) ;
    if ( tmp < minY )
    {
      minY = tmp ;
    }
  }


  min = minX * minX + minY * minY ;
  min = ( min < SMDOUBLE) ? SMDOUBLE : sqrt(min) ;

  return min ;
}
/*-------------------------------------------------------------------------------------------*/
int computeArrowColor( double gridX[], double gridY[], int nbRow, int nbCol, int index )
{
  integer whiteid;
  int verbose = 0 ;
  int narg = 0 ;
  int color = 0 ;
  double scx = Cscale.Wscx1 ;
  double scy = Cscale.Wscy1 ;

  double nor = sqrt( scx*scx*gridX[index]*gridX[index] + scy*scy*gridY[index]*gridY[index] ) ;
  double max = getLongestVector( gridX, gridY, nbRow, nbCol, scx, scy ) ;
  
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  
  color = inint( (whiteid-1) * (1.0-nor/max) ) + 1 ;

  return color ;
}
/*-------------------------------------------------------------------------------------------*/
double getLongestVector( double vectX[], double vectY[], int nbRow, int nbCol, double scx, double scy )
{
  int i ;

  double scx2 = scx * scx ;
  double scy2 = scy * scy ;
  double max  = ( scx2 * vectX[0] * vectX[0] ) + ( scy2 * vectY[0] * vectY[0] ) ;

  for ( i = 1 ; i < nbRow * nbCol ; i++ )
  {
    double tmp = ( scx2 * vectX[i] * vectX[i] ) + ( scy2 * vectY[i] * vectY[i] ) ;
    if ( tmp > max ) { max = tmp ; }
  }
  max = ( max < SMDOUBLE) ? SMDOUBLE : sqrt(max) ;

  return max ;
}
/*-------------------------------------------------------------------------------------------*/
