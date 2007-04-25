/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE,   INRIA 2001-2002
 *    Djalel ABDEMOUCHE,   INRIA 2002-2004
 *    Fabrice Leray,       INRIA 2004-2006
 *    Jean-Baptiste Silvy, INRIA 2006-xxxx
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 ------------------------------------------------------------------------/-*/
#include <math.h>

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "DrawObjects.h"
#include "GetProperty.h"
#include "bcg.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "ObjectStructure.h"
#include "Axes.h"
#include "Xcall1.h"
#include "Champ.h"
#include "StringBox.h"
#include "handleDrawing/drawTextEntity.h"
#include "handleDrawing/drawFigureEntity.h"
#include "handleDrawing/drawSubWinEntity.h"
#include "handleDrawing/drawCompoundEntity.h"
#include "handleDrawing/drawLegendEntity.h"
#include "handleDrawing/drawFecEntity.h"
#include "handleDrawing/drawSegsEntity.h"
#include "handleDrawing/drawGrayplotEntity.h"
#include "handleDrawing/drawPolylineEntity.h"
#include "handleDrawing/drawArcEntity.h"
#include "handleDrawing/drawRectangleEntity.h"
#include "handleDrawing/drawAxesEntity.h"
#include "handleDrawing/drawSurfaceEntity.h"
#include "periScreen.h" /* to be removed */
#include "PloEch.h"
#include "axesScale.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"

#include "math_graphics.h"
#include "graphicSession.h"
#include "Format.h"
#include "axesScale.h"
#include "drawMarks.h"
#include "BasicAlgos.h"

#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */

#ifdef WITH_TK
#include "../../../tclsci/includes/GedManagement.h"
#endif

#ifdef _MSC_VER
extern HDC TryToGetDC(HWND hWnd);
#endif

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)

extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);

void GradFixedlog( double minVal, double maxVal, double * ticks, int nbGrads );
int sciGetLogExponent( double minBound, double maxBound, double * expMin, double * expMax );

unsigned short defcolors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   255,   0, /* Yellow */
  255, 255, 255, /* White: DEFAULTWHITE */
  0,   0, 144, /* Blue4 */
  0,   0, 176, /* Blue3 */
  0,   0, 208, /* Blue2 */
  135, 206, 255, /* LtBlue */
  0, 144,   0, /* Green4 */
  0, 176,   0, /* Green3 */
  0, 208,   0, /* Green2 */
  0, 144, 144, /* Cyan4 */
  0, 176, 176, /* Cyan3 */
  0, 208, 208, /* Cyan2 */
  144,   0,   0, /* Red4 */
  176,   0,   0, /* Red3 */
  208,   0,   0, /* Red2 */
  144,   0, 144, /* Magenta4 */
  176,   0, 176, /* Magenta3 */
  208,   0, 208, /* Magenta2 */
  128,  48,   0, /* Brown4 */
  160,  64,   0, /* Brown3 */
  192,  96,   0, /* Brown2 */
  255, 128, 128, /* Pink4 */
  255, 160, 160, /* Pink3 */
  255, 192, 192, /* Pink2 */
  255, 224, 224, /* Pink */
  255, 215,   0  /* Gold */
};




void sciRedrawFigure( void )
{
  sciDrawObj(sciGetCurrentFigure ());
}

void sciRedrawF(value)
     int *value;   
{ 
  sciPointObj *figure; 
  integer cur,na,verb=0;
  figure= (sciPointObj *) sciIsExistingFigure(value); 
  /* F.Leray 13.04.04 : Test if returned sciPointObj* is NULL (means Figure has been destroyed) */

  if(figure == (sciPointObj *)  NULL)
    {
      /* Do nothing */
    }
  else
    {
      C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
      C2F (dr) ("xset", "window",value,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
      sciDrawObj(figure);
      C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
    }
}

void sciXbasc()
{  
  static sciPointObj *mafigure;
  static sciPointObj *masousfen;  
 
  mafigure= (sciPointObj *) sciGetCurrentFigure(); 
  DestroyAllGraphicsSons((sciPointObj *)mafigure);
  
   
  if ((masousfen = ConstructSubWin (mafigure, 'o')) != NULL){
    sciSetCurrentObj(masousfen);
    sciSetOriginalSubWin (mafigure, masousfen);}
  notifyObservers(sciGetCurrentFigure ());      
} 	

void sciXclear()
{
  static sciPointObj *masousfen, *tmpsousfen;
  sciSons *psonstmp; 
     
  tmpsousfen= sciGetCurrentSubWin();
  psonstmp = sciGetSons (sciGetCurrentFigure());
  
  while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
	{
	  masousfen= (sciPointObj *)psonstmp->pointobj;
	  sciSetSelectedSubWin (masousfen);
	  sciSetdrawmode (FALSE); 
	}
      psonstmp = psonstmp->pnext;
    }
  sciSetSelectedSubWin (tmpsousfen);
  notifyObservers(sciGetCurrentFigure ());      
}

/* update the scale and retrieve the bounding box of the axis */
void updateScale3d( sciPointObj * pobj    ,
                    double        dbox[6] ,
                    double        xbox[8] ,
                    double        ybox[8] ,
                    double        zbox[8]  )
{
  int    flag     ;
  int    i        ;
  double xmmin    ;
  double xmmax    ;
  double ymmin    ;
  double ymmax    ;
  double wmax     ;
  double hmax     ;
  integer aaint[] = {2,10,2,10} ;
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);

  sci_update_frame_bounds_3d( pobj ) ;
       
  dbox[0] =  ppsubwin->FRect[0] ; /*xmin*/
  dbox[1] =  ppsubwin->FRect[2] ; /*xmax*/
  dbox[2] =  ppsubwin->FRect[1] ; /*ymin*/
  dbox[3] =  ppsubwin->FRect[3] ; /*ymax*/
  dbox[4] =  ppsubwin->FRect[4] ; /*zmin*/
  dbox[5] =  ppsubwin->FRect[5] ; /*zmax*/

  if( ppsubwin->cube_scaling )
  {
    dbox[0] =  0.; 
    dbox[1] =  1.;
    dbox[2] =  0.;
    dbox[3] =  1.;
    dbox[4] =  0.;
    dbox[5] =  1.;
  }


  /** changement de coordonnees 3d */
  flag = ( ppsubwin->axes.flag[1] + 1 ) / 2 ; /* F.Leray Adding HERE 19.04.04 */

  Cscale.alpha = ppsubwin->alpha ;
  Cscale.theta = ppsubwin->theta ;
      
  /* update Cscale.m from the new viewing angles */
  sciUpdateScaleAngles( ppsubwin->theta, ppsubwin->alpha ) ;
      
  for ( i = 0 ; i < 6 ; i++ ) 
  { 
    if ( flag == 0 )
    { 
      dbox[i] = Cscale.bbox1[i];
    }
    else
    { 
      Cscale.bbox1[i] = dbox[i];
    }
  }

  /* get the vertices of the axes box */
  sciGetAxisBox( dbox, xbox, ybox, zbox ) ;
      
  /** Calcul des echelles en fonction de la taille du dessin **/
  if ( flag == 1 || flag == 3 ) /* ALL the expanded cases : flag[1] = 1 or 2 or 5 or 6 */
  {
    xmmin =  Mini(xbox,8L) ;
    xmmax =  Maxi(xbox,8L) ; /* search for x Min/Max on all the edges (there are 8 edges that compose the box) F.Leray 13.10.04 */
    ymmax = -Mini(ybox,8L) ; /* same thing on ybox vector ( 1) why - (minus) ? see 2) )*/
    ymmin = -Maxi(ybox,8L);
  }
  
  if ( flag == 2 || flag == 3 ) /* ALL the isometric cases : flag[1] = 3 or 4 or 5 or 6 */
  {
    int verbose = 0 ;
    int narg        ;
    int wdim[2]     ;
    double FRect[4] ;
    double WRect[4] ;
    double ARect[4] ;
    char   logf_[2]  ;
    /* get current window size */
    C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    getscale2d(WRect,FRect,logf_,ARect);
    wmax=linint((double)wdim[0] * WRect[2]);
    hmax=linint((double)wdim[1] * WRect[3]); 
  }
  
  if ( flag == 2 ) /* the "NON expanded isometric" cases : flag[1] = 3 or 4 */
  {
    double R  ;
    double xo ;
    double yo ;
    double zo ;
    double dx ;
    double dy ;
    double dz ;
    
    /* radius and center of the sphere circumscribing the box */
    dx = dbox[1] - dbox[0] ;
    dy = dbox[3] - dbox[2] ;
    dz = dbox[5] - dbox[4] ;
    
    R  = sqrt( dx*dx + dy*dy + dz*dz)/2;
    xo = ( xbox[0] + xbox[6] ) / 2.0 ;
    yo = ( ybox[0] + ybox[6] ) / 2.0 ;
    zo = ( zbox[0] + zbox[6] ) / 2.0 ;
    
    xmmin=   xo - R ;
    xmmax=   xo + R ;
    ymmax=  -yo + R ;
    ymmin=  -yo - R ;
  }

  if (flag==2 || flag==3)
  {
    double hx ;
    double hy  ;
    hx=xmmax-xmmin;
    hy=ymmax-ymmin;
    if ( hx/(double)wmax  < hy/(double)hmax ) 
    {
      double hx1 ;
      hx1   = wmax * hy / hmax ;
      xmmin = xmmin - ( hx1 - hx ) / 2.0 ;
      xmmax = xmmax + ( hx1 - hx ) / 2.0 ;
    }
    else 
    {
      double hy1 ;
      hy1 = hmax * hx / wmax ;
      ymmin = ymmin - ( hy1 - hy ) / 2.0 ;
      ymmax = ymmax + ( hy1 - hy ) / 2.0 ;
    }
  }

  if (flag != 0 ) /* != using current 3D scale */
  {
    double FRect[4] ;
    /* FRect = [Xmin,Ymin,Xmax,Ymax] */
    FRect[0] =  xmmin ;
    FRect[1] = -ymmax ;
    FRect[2] =  xmmax ;
    FRect[3] = -ymmin ; /* 2) ... (why - (minus) ? )*/
    set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
    Cscale.metric3d = flag; 
  }
}


/**Nextind
 * @author Djalel Abdemouche 10/2003
 * Should be in Action.c file
 */
void Nextind(integer ind1, integer *ind2, integer *ind3)
{
  *ind2 = ind1+1;
  *ind3 = ind1-1;
  if (ind1 > 3)
    {
      if (*ind2 == 8) *ind2 = 4;
      if (*ind3 == 3) *ind3 = 7;
    }
  else
    {
      if (*ind2 == 4) *ind2 = 0;
      if (*ind3 == -1) *ind3 = 3;
    }
}




BOOL Ishidden(sciPointObj *pobj)
{
  double alpha;
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;
    if ((alpha <0.0 ) && (alpha > -90.0))
      return TRUE;
    if ((alpha <360.0 ) && (alpha > 270.0)) /* missing case added to fix bug 839 F.Leray */
      return TRUE;
    if ((alpha <-180.0 ) && (alpha > -270.0))
      return TRUE;
    if ((alpha <180.0 ) && (alpha > 90.0))
      return TRUE;
  }
  return FALSE;
}

/* When alpha is close to a singularity (90,-90,270...), use to determine how to draw the x and y graduations */
/* depending on cof */
BOOL IsDownAxes(sciPointObj *pobj)
{
  double alpha,cof;
  
  if (sciGetEntityType(pobj) == SCI_SUBWIN){
    alpha = pSUBWIN_FEATURE (pobj)->alpha;   
    if (!(pSUBWIN_FEATURE (pobj)->isoview))
      cof=10.0;
    else
      cof = 5.0; /* F.Leray : hard fixed here */
    if (cof == 0 ) cof =5;
    if ((alpha <=(-90.0+cof) ) && (alpha >= (-90.0-cof))) 
      return TRUE;
    if ((alpha <=(-270.0+cof) ) && (alpha >= (-270.0-cof)))
      return TRUE;
    if ((alpha <=(90.0+cof) ) && (alpha >= (90.0-cof)))
      return TRUE;
    if ((alpha <=(270.0+cof) ) && (alpha >= (270.0-cof)))
      return TRUE;
  }
  return FALSE;
}

/* compute the graduation of the segment [minVal,maxVal] knowing the number of ticks */
void GradFixedlog( double minVal, double maxVal, double * ticks, int nbGrads )
{
  int initSize ;
  int i ;

  /* intialize the array as usual */
  GradLog( minVal, maxVal, ticks, &initSize, FALSE ) ;

  if ( initSize > nbGrads )
  {
    /* we create a smaller vector from a bigger one */
    int nbRemove = initSize - nbGrads ;
    
    BOOL * removedTicks ;
    if( ( removedTicks = MALLOC( initSize * sizeof(BOOL) ) ) == NULL )
    {
      return ;
    }

    for ( i = 0 ; i < initSize ; i++ )
    {
      removedTicks[i] = FALSE ;
    }

    /* we now remove the nbremove indexes : round( ( 0.5 + i ) * size / nbRemove ) */
    /* i=0..nbReg-1 should do the thing */
    for ( i = 0 ; i < nbRemove ; i++ )
    {
      int remIndex = 1 + (int) round(  i  * ((double) initSize - 2 ) / ( (double) nbRemove ) ) ;
      removedTicks[remIndex] = TRUE ;
    }

    removeBadTicks( ticks, removedTicks, &initSize ) ;

    FREE( removedTicks ) ;

  }

}


/* compute the automatic graduation of the segment [_min,_max] and store it in _grads */
/* the number of graduation may be fixed if compNgrads is TRUE or automaticaly computed */
/* otherwise. */
int GradLog( double   _min   ,
             double   _max   ,
             double * _grads ,
             int    * n_grads,
             int      compNgrads )
{
  int i;
  int log_min, log_max;
  int size;
  
  if ( compNgrads )
  {
    GradFixedlog( _min, _max, _grads, *n_grads ) ;
    return 0 ;
  }

  log_max =  (int) ceil(_max);
  log_min =  (int) floor(_min);
  

  size = log_max - log_min +1;
  /*  tab=(int *)MALLOC(size*sizeof(int)); */
  
  /*   for(i=0;i<size;i++) tab[i]=log_min+i; */

  *n_grads = 0 ;

  if(size<=7)    {
      for(i=0;i<size;i++)
	{
	  /*    _grads[i] = exp10(tab[i]); */
	  _grads[i] = log_min+i;
	  *n_grads = (*n_grads) + 1;
	  /* 	  sciprint("Juste en sortie, _grads[%d] = %lf\n",i, _grads[i]); */
	}
    }
  else
    {
      int pas = 0, old_pas= 0,j;
      int val = size, passed = 0;
      
      for(j=val-1;j>1;j--)
	if(val%j == 0){
	  old_pas = pas;
	  pas=j; 
	  passed = 1;

	  if((7*pas)<=val){ 
	    if(old_pas != 0) {pas = old_pas; }
	    break;
	  }
	}
      
      if(passed != 1 || (size/pas)>15 ) pas = size;
            
      if(pas==size)
	{
	  _grads[0] = log_min;
	  _grads[1] = log_max;
	  *n_grads =2;
	}
      else
      {
	for(i=0;i<=(int )(size/pas);i++)
	  {
	    _grads[i] = log_min+(i*pas);
	    
	    *n_grads = (*n_grads) + 1;
	    /* 	    sciprint("Juste en sortie, _grads[%d] = %lf\n",i, _grads[i]); */
	  }
      }
    }
  
  return 0;
}

/**
 * get the exponent used for log axis from given data bounds
 * @return 0 if OK, -1 if negative bounds.
 */
int sciGetLogExponent( double minBound, double maxBound, double * expMin, double * expMax )
{
  if ( minBound > 0 )
  {
    *expMin = floor( log10( minBound ) ) ;
    *expMax = ceil(  log10( maxBound ) ) ;
    return 0 ;
  }
  *expMax = 1.0 ;
  *expMin = 0.0 ;
  return -1 ;
}

/* get the displayed bounds of an axis */
void sciGetDisplayedBounds( sciPointObj * pSubWin,
                            double      * xmin   ,
                            double      * xmax   ,
                            double      * ymin   ,
                            double      * ymax   ,
                            double      * zmin   ,
                            double      * zmax    )
{
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE ( pSubWin ) ;
    /*****************************************************************
     * get initial bounds
   *****************************************************************/
  if( sciGetZooming( pSubWin ) )
  {
    *xmin = ppsubwin->ZRect[0] ; 
    *ymin = ppsubwin->ZRect[1] ; 
    *xmax = ppsubwin->ZRect[2] ;
    *ymax = ppsubwin->ZRect[3] ;
    *zmin = ppsubwin->ZRect[4] ;
    *zmax = ppsubwin->ZRect[5] ;
  }
  else
  {
    *xmin = ppsubwin->SRect[0] ;
    *ymin = ppsubwin->SRect[2] ;
    *xmax = ppsubwin->SRect[1] ;
    *ymax = ppsubwin->SRect[3] ;
    *zmin = ppsubwin->SRect[4] ;
    *zmax = ppsubwin->SRect[5] ;
  }
  
  
   
  /*****************************************************************
   * modify  bounds and aaint  if using log scaling X axis
   *****************************************************************/
  if ( ppsubwin->logflags[0] == 'l' )
  {
    if ( sciGetLogExponent( *xmin, *xmax, xmin, xmax ) != 0 )
    {
      sciprint("Warning: Can't use Log on X-axis xmin is negative. \n");
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[1] == 'l' )
  {
    if ( sciGetLogExponent( *ymin, *ymax, ymin, ymax ) != 0 )
    {
      sciprint("Warning: Can't use Log on Y-axis ymin is negative. \n");
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Z axis
   *****************************************************************/
  if ( ppsubwin->logflags[2] == 'l' )
  {
    if ( sciGetLogExponent( *zmin, *zmax, zmin, zmax ) != 0 )
    {
      sciprint("Warning: Can't use Log on Z-axis zmin is negative. \n");
    }
  }

}

/* F.Leray au 13.10.04 completly review for new axes graduations */
/*** F.Leray 02.04.04 */
/* FUNCTION FOR 2D UPDATE ONLY !!!!! <=> beginning of axis_3ddraw (in 2d HERE of course! ) */
/* Copy on update_frame_bounds */
BOOL sci_update_frame_bounds_2d(sciPointObj *pobj)
{
  double xmax, xmin, ymin, ymax, zmin, zmax ;
  double hx,hy,hx1,hy1;
  int i;
  
  sciSubWindow * ppsubwin =  pSUBWIN_FEATURE (pobj);
  double FRect[4],WRect[4],ARect[4]; 
  char logscale[2];
  
  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */
  double ExistingFRect[4]; /* the Existing FRect at start to be compared at the end of this routine */
  /* in order to determine wheter or not the bounds have changed... */
  int nbsubtics[2];
  int nbgrads[2];
  /* End of Temp variables */
  
  for(i=0;i<4;i++) ExistingFRect[i] =  ppsubwin->FRect[i]; /* store old initial bounds*/

  for(i=0;i<2;i++) nbsubtics[i] = ppsubwin->axes.nbsubtics[i];
  nbgrads[0] = ppsubwin->axes.nxgrads;
  nbgrads[1] = ppsubwin->axes.nygrads;
  
  /* nbtics on z put to 0 */
  /*   ppsubwin->axes.nzgrads = 0; */
  

  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;


  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
    }
   
 
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE); 
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE );
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
   
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
   
  if(ppsubwin->tight_limits == FALSE )
    {
      xmin = ppsubwin->axes.xgrads[0];
      xmax = ppsubwin->axes.xgrads[ ppsubwin->axes.nxgrads - 1];
      ymin = ppsubwin->axes.ygrads[0];
      ymax = ppsubwin->axes.ygrads[ ppsubwin->axes.nygrads - 1];
    }

  /*****************************************************************
   * modify  bounds if  isoview requested 
   *****************************************************************/
  if ( ppsubwin->isoview == TRUE) {      
    int verbose=0,wdim[2],narg; 
        
    C2F(dr)("xget","wdim",&verbose,wdim,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    hx=xmax-xmin;
    hy=ymax-ymin;
    getscale2d(WRect,FRect,logscale,ARect);
    
    wdim[0]=linint((double)wdim[0] *WRect[2]);
    wdim[1]=linint((double)wdim[1] *WRect[3]);
    
    if ( hx/(double)wdim[0]  <hy/(double) wdim[1] ) {
      hx1=wdim[0]*hy/wdim[1];
      xmin=xmin-(hx1-hx)/2.0;
      xmax=xmax+(hx1-hx)/2.0;
    }
    else {
      hy1=wdim[1]*hx/wdim[0];
      ymin=ymin-(hy1-hy)/2.0;
      ymax=ymax+(hy1-hy)/2.0;
    }
    
    /* F.Leray 28.09.04 */
    /* I need to recompute the correct xgrads and ygrads vector to have a good display */
     
    if ( ppsubwin->logflags[0]=='n') { /* x-axis */
      TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL, ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE);
      ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
    }

 

    if ( ppsubwin->logflags[1]=='n') { /* y-axis */
      TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
      ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    }
    else{ /* log. case */
      GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
      ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
    }
   
   
    /* END ISO if */
  }
  
  /* Changement ci-dessous F.Leray 21.09.04 */
  ppsubwin->axes.xlim[0]=xmin;
  ppsubwin->axes.xlim[1]=xmax;
  ppsubwin->axes.xlim[2]=0;
  ppsubwin->axes.ylim[0]=ymin;
  ppsubwin->axes.ylim[1]=ymax;
  ppsubwin->axes.ylim[2]=0;
   
   
   
  /*****************************************************************
   * set the actual bounds in subwindow data structure
   *****************************************************************/
   
   
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
   
  ppsubwin->axes.xlim[3] = ppsubwin->axes.nxgrads;
  ppsubwin->axes.ylim[3] = ppsubwin->axes.nygrads;
   
  /*   ppsubwin->axes.reverse[0] = FALSE; /\*TRUE;*\/  /\* TEST en DUR F.Leray ICIIIIIIIIIIIII 12.10.04 *\/ */
  /*    ppsubwin->axes.reverse[1] = FALSE; /\*TRUE;*\/  */
   
   
  set_scale("tftftf",NULL,ppsubwin->FRect,NULL,ppsubwin->logflags,NULL); 
   
   
  for(i=0;i<4;i++)
    if(ppsubwin->FRect[i] != ExistingFRect[i]){
      return TRUE;
      break;
    } 
   
  for(i=0;i<2;i++) 
    if(nbsubtics[i] != ppsubwin->axes.nbsubtics[i]){
      return TRUE;
      break;
    }
   
  if(nbgrads[0] != ppsubwin->axes.nxgrads) return TRUE;
  if(nbgrads[1] != ppsubwin->axes.nygrads) return TRUE;
   
  return FALSE;
}

/**update_3dbounds -> renammed sci_update_frame_bounds_3d
 * @author Djalel Abdemouche 10/2003
 * Should be in Plo2dEch.c file
 */
BOOL sci_update_frame_bounds_3d(sciPointObj *pobj)
{
  double xmin,xmax,ymin,ymax,zmin,zmax; 
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj); 

  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */
  double ExistingFRect[6]; /* the Existing FRect at start to be compared at the end of this routine */
  /* in order to determine wheter or not the bounds have changed... */
  int nbsubtics[3];
  int nbgrads[3];
  /* End of Temp variables */

  for(i=0;i<6;i++) ExistingFRect[i] =  ppsubwin->FRect[i]; /* store old initial bounds*/
  
  for(i=0;i<3;i++) nbsubtics[i] = ppsubwin->axes.nbsubtics[i];
  nbgrads[0] = ppsubwin->axes.nxgrads;
  nbgrads[1] = ppsubwin->axes.nygrads;
  nbgrads[2] = ppsubwin->axes.nzgrads;

  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;
  

  /* _grad Init. to 0. */
  for(i=0;i<20;i++) 
    {
      ppsubwin->axes.xgrads[i] = 0.;
      ppsubwin->axes.ygrads[i] = 0.;
      ppsubwin->axes.zgrads[i] = 0.;
    }
   
  
  if ( ppsubwin->logflags[0]=='n') { /* x-axis */
    TheTicks(&xmin, &xmax, &(ppsubwin->axes.xgrads[0]), &ppsubwin->axes.nxgrads, FALSE);
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(xmin,xmax,ppsubwin->axes.xgrads,&ppsubwin->axes.nxgrads, FALSE );
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(pobj,ppsubwin->axes.nxgrads,'l',ppsubwin->axes.xgrads,0);
  }
  
  if ( ppsubwin->logflags[1]=='n') { /* y-axis */
    TheTicks(&ymin, &ymax, &(ppsubwin->axes.ygrads[0]), &ppsubwin->axes.nygrads, FALSE);
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'n',NULL, ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(ymin,ymax,ppsubwin->axes.ygrads,&ppsubwin->axes.nygrads, FALSE );
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(pobj,ppsubwin->axes.nygrads,'l',ppsubwin->axes.ygrads,0);
  }
  
  if ( ppsubwin->logflags[2]=='n') { /* z-axis */
    TheTicks(&zmin, &zmax, &(ppsubwin->axes.zgrads[0]), &ppsubwin->axes.nzgrads, FALSE);
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'n',NULL, ppsubwin->axes.nbsubtics[2]); /* Nb of subtics computation and storage */
  }
  else{ /* log. case */
    GradLog(zmin,zmax,ppsubwin->axes.zgrads,&ppsubwin->axes.nzgrads, FALSE );
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(pobj,ppsubwin->axes.nzgrads,'l',ppsubwin->axes.zgrads,0);
  }
  
  if(ppsubwin->tight_limits == FALSE )
    {
      xmin = ppsubwin->axes.xgrads[0];
      xmax = ppsubwin->axes.xgrads[ ppsubwin->axes.nxgrads - 1];
      ymin = ppsubwin->axes.ygrads[0];
      ymax = ppsubwin->axes.ygrads[ ppsubwin->axes.nygrads - 1];
      zmin = ppsubwin->axes.zgrads[0];
      zmax = ppsubwin->axes.zgrads[ ppsubwin->axes.nzgrads - 1];
    }

  ppsubwin->axes.xlim[0]=xmin;
  ppsubwin->axes.xlim[1]=xmax;
  ppsubwin->axes.xlim[2]=0;
  ppsubwin->axes.ylim[0]=ymin;
  ppsubwin->axes.ylim[1]=ymax;
  ppsubwin->axes.ylim[2]=0;
  ppsubwin->axes.zlim[0]=zmin; /* rajout pour z */
  ppsubwin->axes.zlim[1]=zmax;
  ppsubwin->axes.zlim[2]=0;
     
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
  ppsubwin->FRect[4]=zmin;
  ppsubwin->FRect[5]=zmax;
    
  ppsubwin->axes.xlim[3] = ppsubwin->axes.nxgrads;
  ppsubwin->axes.ylim[3] = ppsubwin->axes.nygrads;
  ppsubwin->axes.zlim[3] = ppsubwin->axes.nzgrads;
   
  set_scale("tftftf",NULL,ppsubwin->FRect,NULL,ppsubwin->logflags,NULL); 
    
  for(i=0;i<6;i++)
    if(ppsubwin->FRect[i] != ExistingFRect[i]){
      return TRUE;
      break;
    } 
    
  for(i=0;i<3;i++) 
    if(nbsubtics[i] != ppsubwin->axes.nbsubtics[i]){
      return TRUE;
      break;
    }
  
  if(nbgrads[0] != ppsubwin->axes.nxgrads) return TRUE;
  if(nbgrads[1] != ppsubwin->axes.nygrads) return TRUE;
  if(nbgrads[2] != ppsubwin->axes.nzgrads) return TRUE;
  
  return FALSE;  
}







int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2 ,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {10,7,5,5,4,4,3,2,2 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);


  if(logflag =='l'){
    if((grads[1]-grads[0])==1) /* intervalle de type ...10^n 10^(n+1)...*/
      {
	return 9; /* 9 subtics to have a pretty tics/grid in log.*/
      }
    else
      {
	return 1; /* no subtics at all (1 but draw on a tics place) */
      }
  }
  else{
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
      { 
	for(i=0;i<19;i++)
	  if(nbtics == ticsval[i])
	    {
	      return subticsval[i];
	    }
      }
    else /* if auto subtics mode == OFF already computed in Plo2dn.c, Champ.c routines... */
      {  /* or given via a.subtics=[nbsubtics_on_x, nbsubtics_on_y, nbsubtics_on_z] command */
	return nbsubtics_input;
      }
  }
  
  return -1;
}

/* test on x and y axes only : used in 2D routines only */
BOOL GetIsAxes2D(sciPointObj *psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  
  if((ppsubwin->axes.axes_visible[0] == FALSE)
     && (ppsubwin->axes.axes_visible[1] == FALSE))
    return FALSE;
  else
    return TRUE;
}

int ComputeCorrectXindAndInsideUD(double Teta,double Alpha, double *dbox, integer *xind, integer *InsideU, integer *InsideD)
{
  double xbox[8], ybox[8], zbox[8];

  Teta = 0.1; /* Little offset to compute correct values for xind,  InsideU and InsideD */

  /* update Cscale.m from the new viewing angles */
  sciUpdateScaleAngles( Teta, Alpha ) ;
 
  sciGetAxisBox( dbox, xbox, ybox, zbox ) ;
 
  sciAxesVerticesIndices( InsideU, InsideD, xbox, ybox, xind ) ;

  return 0;
}



int ComputeGoodTrans3d( sciPointObj * psubwin, int n, int *xm, int *ym, double * fx, double *fy, double *fz)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  
  double tmp_fx = *fx;
  double tmp_fy = *fy;
  double tmp_fz = *fz;
  
  if(ppsubwin->logflags[0] == 'l')
    tmp_fx = exp10(tmp_fx);
  
  if(ppsubwin->logflags[1] == 'l')
    tmp_fy = exp10(tmp_fy);
  
  if(ppsubwin->logflags[2] == 'l')
    tmp_fz = exp10(tmp_fz);
  
  trans3d(psubwin, n, xm, ym, &tmp_fx, &tmp_fy, &tmp_fz);
  
  return 0;
}


/**DrawAxesIfRequired
 * Draws Axes (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
void DrawAxesIfRequired(sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);
  
  if( pFIGURE_FEATURE(pfigure)->auto_redraw && pFIGURE_FEATURE(pfigure)->visible )
  {
    DrawAxes(pobj);
  }
  
}

/* Routine used inside Plo2dn.c, Champ.c, Gray.c... */
/* to force the drawing of the axes after a new object is created */
void DrawAxes(sciPointObj * pobj)
{
  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  /*   sciPointObj * pfigure = sciGetParentFigure(pobj); */
  char STRFLAG[4];
  integer x[6], v, markidsizenew[2];
  double dv;

  if (!sciGetVisibility(pobj)) return;
  
  x[0] = sciGetForeground (psubwin);
  x[2] = sciGetLineWidth (psubwin);
  x[3] = sciGetLineStyle (psubwin);
  markidsizenew[0] = sciGetMarkStyle(psubwin);
  markidsizenew[1] = sciGetLineWidth (psubwin);
  x[4] = 0;
  v = 0;
  dv = 0;

  
  sciSetSelectedSubWin(psubwin); 
  
  set_scale ("tttftt", pSUBWIN_FEATURE (psubwin)->WRect, pSUBWIN_FEATURE (psubwin)->FRect,
	     NULL, pSUBWIN_FEATURE (psubwin)->logflags, 
	     pSUBWIN_FEATURE (psubwin)->ARect); 
     
  if (!pSUBWIN_FEATURE (psubwin)->is3d)   /* we are in 2D mode...*/
    {
      /* F.Leray 07.12.04 */
      /* TO CORRECT the bug 1115 : Big object (grayplots) could cover axes*/
      C2F (dr) ("xset","dashes",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
      C2F (dr) ("xset","foreground",x,x,x+4,x+4,x+4,&v,&dv,&dv,&dv,&dv,5L,4096);
      C2F (dr) ("xset","thickness",x+2,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F (dr) ("xset","mark",&markidsizenew[0],&markidsizenew[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      rebuild_strflag(psubwin,STRFLAG);
      axis_draw2 (STRFLAG); /* Axes is rebuilt here to avoid being covered by the new created object */
    }
}


/* Used only when switching from a plot3d to a plot2d or 
 * when typing: a=gca();
 *              a.view='2d' and we were in 3d 
 * to have a good update of 3d graphics scale in 2D
 * whenever we are in auto_redraw OFF  */
void UpdateSubwinScale(sciPointObj * pobj)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);
  sciPointObj * currentsubwin = NULL;
  BOOL vis_save = FALSE;
  double xbox[8],ybox[8],zbox[8];
  static integer InsideU[4],InsideD[4];
  	
  currentsubwin = sciGetCurrentSubWin();

  sciSetSelectedSubWin(pobj); 
  
  /*   set_scale ("tttftt", pSUBWIN_FEATURE (pobj)->WRect, pSUBWIN_FEATURE (pobj)->FRect, */
  /* 	     NULL, pSUBWIN_FEATURE (pobj)->logflags,  */
  /* 	     pSUBWIN_FEATURE (pobj)->ARect);  */
  
  if (ppsubwin->is3d) 
    {  /* 3D Coordinates */ /* verifier si c'est encore utile SS */
      /*To have directly all the possible ISOVIEW Modes*/
      long isoflag = (long)(ppsubwin->axes.flag[1]+1)/2; 
      
      if(ppsubwin->isoview == TRUE) {
	if(isoflag ==2 || isoflag == 3){		}
	else {
	  if((ppsubwin->axes.flag[1] == 0)
	     || (ppsubwin->axes.flag[1] == 2))
	    /* The default isoview mode is type=4 3d isometric bounds 
	       derived from the data, to similarily type=2  */
	    ppsubwin->axes.flag[1] = 4; 
	  else if(ppsubwin->axes.flag[1] == 1)
	    ppsubwin->axes.flag[1] = 3;
	}
      }
      else {
	if((ppsubwin->axes.flag[1] == 3) 
	   || (ppsubwin->axes.flag[1] == 5))
	  ppsubwin->axes.flag[1] = 1; /* computed from ebox*/
	else if((ppsubwin->axes.flag[1] == 4) 
		|| (ppsubwin->axes.flag[1] == 6))
	  /* The default NON-isoview mode is 2 computed from data*/
	  ppsubwin->axes.flag[1] = 2; 
      }
      

      vis_save = sciGetVisibility(pobj);

      sciSetVisibility(pobj,FALSE);
      axis_3ddraw(pobj,xbox,ybox,zbox,InsideU,InsideD); /* TEST on sciGetVisibility inside */
      /* because axis_3ddraw displays 3d axes BUT ALSO compute + reset the 3d scale BEFORE !! */
      sciSetVisibility(pobj,vis_save);
    }
  else /* we are in 2D mode...*/
    {
      sci_update_frame_bounds_2d(pobj);
    }

  sciSetSelectedSubWin(currentsubwin); 
  
}

int Gen3DPoints(integer type,integer *polyx, integer *polyy, integer *fill, integer whiteid, double zmin, double zmax, double *x, double *y, double *z, integer i, integer j, integer jj1, integer *p, integer dc, integer fg, sciPointObj * psurface)
{
  sciPointObj *pobj;
  int facteur = 1;

  sciPointObj *psubwin = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  sciSurface * ppsurface = pSURFACE_FEATURE (psurface);
  
  pobj = sciGetCurrentSubWin(); 
  if (trans3d(pobj ,1, &(polyx[  5*jj1]),&(polyy[  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[1+  5*jj1]),&(polyy[1+  5*jj1]),&(x[i]),&(y[j+1]),&(z[i+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[2+  5*jj1]),&(polyy[2+  5*jj1]),&(x[i+1]),&(y[j+1]),&(z[(i+1)+(*p)*(j+1)]))==0) return 0; 
  if (trans3d(pobj ,1, &(polyx[3+  5*jj1]),&(polyy[3+  5*jj1]),&(x[i+1]),&(y[j]),&(z[(i+1)+(*p)*j]))==0) return 0;   
  if (trans3d(pobj ,1, &(polyx[4+  5*jj1]),&(polyy[4+  5*jj1]),&(x[i]),&(y[j]),&(z[i+(*p)*j]))==0) return 0; 
  
  
  if(ppsubwin->axes.reverse[0] == TRUE) facteur = -facteur;
  if(ppsubwin->axes.reverse[1] == TRUE) facteur = -facteur;
  if(ppsubwin->axes.reverse[2] == TRUE) facteur = -facteur;
  
  /* try */
  facteur = ppsurface->flag_x * ppsurface->flag_y * facteur;
  
  /* type == flagcolor and dc == color_mode */
  /* fg = hidden color */
  
  if ((((polyx[1+5*jj1]-polyx[0+5*jj1])*(polyy[2+5*jj1]-polyy[0+5*jj1])-
	(polyy[1+5*jj1]-polyy[0+5*jj1])*(polyx[2+5*jj1]-polyx[0+5*jj1]))*facteur <  0) && (fg >=0 )) 
    {
      
      /* ------------------- */
      /* Beneath the surface */
      /* ------------------- */

      if (type != 0) /* flagcolor = 1 case : special treatment compared to flagcolor = 0 */
	             /* don't know why... F.Leray */
	fill[jj1]= (dc < 0 ) ? -fg : fg ;
      else           /* flagcolor = 0 :  No shading at all, fixed facecolor. */
	fill[jj1]=  (dc != 0 ) ? fg : dc ;
    }
  else
    {

      /* ------------------- */
      /* Above the surface */
      /* ------------------- */

     if (type != 0)
	{  /* flagcolor = 1 :  Z-level flat shading. */
	  fill[jj1]=inint((whiteid-1)*((1/4.0*( z[i+(*p)*j]+ z[i+1+(*p)*j]+
						z[i+(*p)*(j+1)]+ z[i+1+(*p)*(j+1)])-zmin)
				       /(zmax-zmin)))+1;
	  if ( dc < 0 ) fill[jj1]= -fill[jj1];  
	}
      else
	fill[jj1]= dc;     
    }
  
  return(1);
  
}


/*------------------------------------------------------------------------------------------*/
/**
 * draw the figure number numFigure.
 */
void sciDrawFigure( int numFigure )
{
  int curFigure = sciGetNumFigure( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( numFigure ) ;
  sciDrawObj( sciGetCurrentFigure() ) ;
  sciSetUsedWindow( curFigure ) ;
}
/*------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*/
/* sciRefreshObj                                                                             */
/* draw an object but before select the rigth figure for display                             */
/*-------------------------------------------------------------------------------------------*/
int sciRefreshObj( sciPointObj * pobj )
{
  
  int parentId = sciGetNum( sciGetParentFigure( pobj ) ) ;
  int currentId             ;
  int status                ;
  int verboseGet        = 0 ;
  int iDontKnowWhatItIs     ;
  /* get current Id in ScilabXgc */
  C2F (dr) ("xget", "window",&verboseGet,&currentId,&iDontKnowWhatItIs,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* set the parent figure of the object as the current figure */
  if ( parentId != currentId )
  {
    C2F (dr) ("xset", "window",&parentId,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  
  /* draw the object */
  status = sciDrawObjIfRequired( pobj ) ;

  /* set back the values */
  if ( parentId != currentId )
  {
    C2F (dr) ("xset", "window",&currentId,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }

  return status ;
  
  
}



/**sciDrawObjIfRequired
 * Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * if and only if pFIGURE_FEATURE(pobj)->auto_redraw == TRUE !!
 * Only used inside High Level functions calls (sucha as plot2d, plot3d...)
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
int
sciDrawObjIfRequired (sciPointObj * pobj)
{
  sciPointObj * pfigure = sciGetParentFigure(pobj);

  if( pFIGURE_FEATURE(pfigure)->auto_redraw && pFIGURE_FEATURE(pfigure)->visible )
  {
    sciDrawObj( pobj ) ;
  }

  return 0;
}

/**sciDrawObj
 * Draws Object (only the basic  graphicobject under subwindows) in its SubWindow or figure
 * @param sciPointObj * pobj: the pointer to the entity
 * @return  int 0 if OK, -1 if not
 */
/* int */
/* sciDrawObj (sciPointObj * pobj) */
/* { */
/*   /\* driver test *\/   */
/*   if((GetDriverId() != 0) && ( isGraphicSessionOpened() )){ */
/*     return -1; */
/*   } */

/*   switch (sciGetEntityType (pobj)) */
/*     { */
/*     case SCI_FIGURE: */
/*       return drawFigureEntity( pobj ) ; */
/*       break; */
/*     case SCI_SUBWIN: */
/*       return drawSubWinEntity( pobj ) ; */
/*       break ; */
/*     case SCI_AGREG: */
/*       return drawCompoundEntity( pobj ) ; */
/*       break; */
/*     case SCI_LEGEND: */
/*       return drawLegendEntity( pobj ) ; */
/*       break;    */
/*     case SCI_FEC: */
/*       return drawFecEntity( pobj ) ; */
/*       break;       */
/*     case SCI_SEGS: */
/*       return drawSegsEntity( pobj ) ; */
/*       break; */
/*     case SCI_GRAYPLOT: */
/*       return drawGrayplotEntity( pobj ) ; */
/*       break; */
/*     case SCI_POLYLINE:    */
/*       return drawPolylineEntity( pobj ) ; */
/*       break; */
/*     case SCI_ARC: */
/*       return drawArcEntity( pobj ) ; */
/*       break; */
/*     case SCI_RECTANGLE: */
/*       return drawRectangleEntity( pobj ) ; */
/*       break; */
/*     case SCI_TEXT: */
/*       return drawTextEntity( pobj ) ; */
/*       break; */
/*     case SCI_AXES: */
/*       return drawAxesEntity(pobj) ; */
/*       break; */
/*     case SCI_SURFACE: */
/*       return drawSurfaceEntity(pobj) ; */
/*       break; */
/*     case SCI_LIGHT: */
/*     case SCI_PANNER: */
/*     case SCI_SBH: */
/*     case SCI_SBV: */
/*     case SCI_MENU: */
/*     case SCI_MENUCONTEXT: */
/*     case SCI_STATUSB: */
/*     default: */
/*       return -1; */
/*       break; */
/*     } */
/*   /\*sciSetSele */
/*     ctedSubWin (curSubWin);*\/ */
/*   return -1; */
/* } */




/* F.Leray 25.02.05 */
/* This function is used to determine how the pixel data*/
/* is given to Xt method (like XDrawLines) */
/* 0 : pixel itself */
/* 1 : pixel's interval */
/* Explanation :*/
/* i.e.in X11 (same thing using Win32 driver), with x2=x1+1 and y1=y2, we have : */
/*   XDrawLine(x1,y1,x1,y1)  does nothing (no pixel lit) */
/*   XDrawLine(x1,y1,x2,y2)  ONE pixel lit */
/* BUT the corresponding function with driver Gif (and others?) give us: */
/* for now (and only for marks), I make a trick using a pixel_offset set to 0 or 1 */
/* depending on the driver. */

/* slight modif from Bruno : if fact GetDriverId return also 0 */
/* if gtk is enable so I have added a call to withgtk to get the */
/* the good pixel_offset */



int AdaptGraduationsOnYBottomLeft(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double yminval, double ymaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastyindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2];
  integer textcolor = -1;
  int logrect[4] ;
  int XX = 0 ;
  int YY = 0 ;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) { old_rect[i] = 0 ; } /* Init. old_rect to force first grad. to be displayed */
  
  lastyindex = ppsubwin->axes.nygrads - 1;
  
  if(lastyindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.ygrads[0],
		 ppsubwin->axes.ygrads[lastyindex],
		 ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
  
  nbtics = ppsubwin->axes.nygrads;
  nbsubtics = ppsubwin->axes.nbsubtics[1];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.ygrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256];
      double ytmp = ppsubwin->axes.ygrads[i];
		  
		  
      sprintf(foo,c_format,ytmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[1] == TRUE)
	ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  
      vx[0]=xm;vy[0]=ym;
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
		      
      
      if (IsDownAxes(psubwin)){
	vx[1]=vx[0];
	vy[1]=vy[0]+iof/2;
	posi[0] = inint(xm-rect[2]/2);
	posi[1]=inint( vy[0] + iof + rect[3]);}
      else{
	vx[1]=vx[0]+barlengthx;
	vy[1]=vy[0]+barlengthy;
	posi[0] = inint( xm+2*barlengthx-rect[2]);
	posi[1]=inint( ym + 2*barlengthy + rect[3]);}
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[1],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[1] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + (double)old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[1] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.ygrads[0] = (grads_tmp[0]+grads_tmp[lastyindex])/2.;
	ppsubwin->axes.nygrads = 1;
        
        if ( !ppsubwin->flagNax )
        {
          /* don't change the number if it aldready has been specified */
          ppsubwin->axes.nbsubtics[1] = 1 ;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.ygrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nygrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
  }
  else{
    ppsubwin->axes.ygrads[0] = grads_tmp[0];
    ppsubwin->axes.ygrads[1] = grads_tmp[lastyindex];
    ppsubwin->axes.nygrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
    
    if(ppsubwin->logflags[1] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas_ = (grads_tmp[lastyindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.ygrads[i] = grads_tmp[0] + i*pas_;
	}
	
	ppsubwin->axes.ygrads[3] = grads_tmp[lastyindex]; /* exact max */
	ppsubwin->axes.nygrads = 4;
        /* Nb of subtics computation and storage */
	ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nygrads,
                                                               'n',
							       ppsubwin->axes.ygrads,
                                                               ppsubwin->axes.nbsubtics[1]);
      }
    }
  }
  
  return 0;
}


int AdaptGraduationsOnXBottomLeft(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double xminval, double xmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastxindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4],XX,YY;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastxindex = ppsubwin->axes.nxgrads - 1;
  
  if(lastxindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.xgrads[0],
		 ppsubwin->axes.xgrads[lastxindex],
		 ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
  
  nbtics = ppsubwin->axes.nxgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[0];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.xgrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double xtmp = ppsubwin->axes.xgrads[i];
		  
      sprintf(foo,c_format,xtmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[0] == TRUE)
	xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      

      if (IsDownAxes(psubwin)){
	vx[1]=vx[0];
	vy[1]=vy[0]+iof/2;
	posi[0] = inint(xm-rect[2]/2); 
	posi[1]=inint( vy[0] + iof + rect[3]);}
      else{
	vx[1]=vx[0]+barlengthx;
	vy[1]=vy[0]+barlengthy;
	posi[0] = inint( xm+2*barlengthx-rect[2]); 
	posi[1]=inint( ym + 2*barlengthy + rect[3]);}
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[0],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[0] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[0] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.xgrads[0] = (grads_tmp[0]+grads_tmp[lastxindex])/2.;
	ppsubwin->axes.nxgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[0] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.xgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nxgrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]); 
  }
  else{
    ppsubwin->axes.xgrads[0] = grads_tmp[0];
    ppsubwin->axes.xgrads[1] = grads_tmp[lastxindex];
    ppsubwin->axes.nxgrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]); 
    
    if(ppsubwin->logflags[0] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas_ = (grads_tmp[lastxindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.xgrads[i] = grads_tmp[0] + i*pas_;
	}
	
	ppsubwin->axes.xgrads[3] = grads_tmp[lastxindex]; /* exact max */
	ppsubwin->axes.nxgrads = 4;
        
        /* Nb of subtics computation and storage */
        /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
	ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nxgrads,
                                                               'n',
							       ppsubwin->axes.xgrads,
                                                               ppsubwin->axes.nbsubtics[0]); 
      }
    }
  }
  
  return 0;
}



int AdaptGraduationsOnYBottomRight(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double yminval, double ymaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastyindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4] ;
  int XX = 0 ;
  int YY = 0 ;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastyindex = ppsubwin->axes.nygrads - 1;
  
  if(lastyindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.ygrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.ygrads[0],
		 ppsubwin->axes.ygrads[lastyindex],
		 ((ppsubwin->axes.ygrads[lastyindex])-(ppsubwin->axes.ygrads[0]))/(lastyindex));
  
  nbtics = ppsubwin->axes.nygrads;
  nbsubtics = ppsubwin->axes.nbsubtics[1];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.ygrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double ytmp = ppsubwin->axes.ygrads[i];
		  
      sprintf(foo,c_format,ytmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[1] == TRUE)
	ytmp = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&ytmp,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      

      if (IsDownAxes(psubwin)){
	vx[1]=vx[0];
	vy[1]=vy[0]+iof/2;
	posi[0] = inint(xm-rect[2]/2); 
	posi[1]=inint( vy[0] + iof + rect[3]);}
      else{
	vx[1]=vx[0]+barlengthx;
	vy[1]=vy[0]+barlengthy;
	posi[0] = inint( xm+2*barlengthx);
	posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		      
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[1],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[1] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[1] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.ygrads[0] = (grads_tmp[0]+grads_tmp[lastyindex])/2.;
	ppsubwin->axes.nygrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[1] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.ygrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nygrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
  }
  else{
    ppsubwin->axes.ygrads[0] = grads_tmp[0];
    ppsubwin->axes.ygrads[1] = grads_tmp[lastyindex];
    ppsubwin->axes.nygrads = 2;

    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nygrads,
                                                           ppsubwin->logflags[1],
							   ppsubwin->axes.ygrads,
                                                           ppsubwin->axes.nbsubtics[1]);
    
    if(ppsubwin->logflags[1] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas_ = (grads_tmp[lastyindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.ygrads[i] = grads_tmp[0] + i*pas_;
	}
	
	ppsubwin->axes.ygrads[3] = grads_tmp[lastyindex]; /* exact max */
	ppsubwin->axes.nygrads = 4;
        /* Nb of subtics computation and storage */
        /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
	ppsubwin->axes.nbsubtics[1] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nygrads,
                                                               'n',
							       ppsubwin->axes.ygrads,
                                                               ppsubwin->axes.nbsubtics[1]);
      }
    }
  }
  
  return 0;
}


int AdaptGraduationsOnXBottomRight(int iof, int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double xminval, double xmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastxindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4] ;
  int XX = 0;
  int YY = 0 ;
  int pas;
  double fact_h = 1.5, fact_w = 1.5;
  int compteur = 0;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  
  lastxindex = ppsubwin->axes.nxgrads - 1;
  
  if(lastxindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.xgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.xgrads[0],
		 ppsubwin->axes.xgrads[lastxindex],
		 ((ppsubwin->axes.xgrads[lastxindex])-(ppsubwin->axes.xgrads[0]))/(lastxindex));
  
  nbtics = ppsubwin->axes.nxgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[0];
  
  
  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.xgrads[i];

  for(i=0;i<nbtics;i++)
    {
      char foo[256]; 
      double xtmp = ppsubwin->axes.xgrads[i];
		  
      sprintf(foo,c_format,xtmp);

      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[0] == TRUE)
	xtmp = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp);
		  
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&xtmp,&fy,&fz);
		  
      vx[0]=xm;vy[0]=ym; 
		  
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);

      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      

      if (IsDownAxes(psubwin)){
	vx[1]=vx[0];
	vy[1]=vy[0]+iof/2;
	posi[0] = inint(xm-rect[2]/2); 
	posi[1]=inint( vy[0] + iof + rect[3]);}
      else{
	vx[1]=vx[0]+barlengthx;
	vy[1]=vy[0]+barlengthy;
	posi[0] = inint( xm+2*barlengthx);
	posi[1]=inint( ym + 2*barlengthy + rect[3]);}
		      
      
      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[0],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
      
      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[0] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];

	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  if(ppsubwin->logflags[0] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.xgrads[0] = (grads_tmp[0]+grads_tmp[lastxindex])/2.;
	ppsubwin->axes.nxgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[0] = 1;
        }
	return 0;
      }
  
  
  pas = nbtics - 2; /* pas == grads number - 2 */
  
  possible_pas = -99;
  possible_compteur = -99;
  
  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.xgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nxgrads = compteur;
    
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
 							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]);
  }
  else{
    ppsubwin->axes.xgrads[0] = grads_tmp[0];
    ppsubwin->axes.xgrads[1] = grads_tmp[lastxindex];
    ppsubwin->axes.nxgrads = 2;
    ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nxgrads,
                                                           ppsubwin->logflags[0],
 							   ppsubwin->axes.xgrads,
                                                           ppsubwin->axes.nbsubtics[0]);
    if(ppsubwin->logflags[0] == 'n'){
      if(nb_grads_max > 4) {
	/* we could display at least 4 graduations but we did not find a proper interval... */
	/* To avoid to display only the min and max (see above), we add 2 newly created grads by interpolating between min and max */
	double pas_ = (grads_tmp[lastxindex]-grads_tmp[0])/3;
	for(i=0;i<3;i++){
	  ppsubwin->axes.xgrads[i] = grads_tmp[0] + i*pas_;
	}
	
	ppsubwin->axes.xgrads[3] = grads_tmp[lastxindex]; /* exact max */
	ppsubwin->axes.nxgrads = 4;
        ppsubwin->axes.nbsubtics[0] = ComputeNbSubTicsFor3dUse(psubwin,
                                                               ppsubwin->axes.nxgrads,
                                                               'n',
							       ppsubwin->axes.xgrads,
                                                               ppsubwin->axes.nbsubtics[0] );
      }
    }
  }
  
  return 0;
}

int AdaptGraduationsOnZ(int x, int y, int size, integer *Ticsdir, int *fontid, sciPointObj * psubwin, double zminval, double zmaxval, double fx, double fy, double fz)
{
  int i;
  int nbtics, nbsubtics;
  int lastzindex;
  int old_rect[4];
  int nb_grads_max = 0;
  int vx[2], vy[2];
  int xm, ym;
  double grads_tmp[20];
  char c_format[5];
  integer barlengthx = 0,barlengthy = 0;
  integer rect[4],posi[2]; 
  integer textcolor = -1;
  int logrect[4] ;
  int XX = 0 ;
  int YY = 0 ;
  int pas;
  double fact_h = 1.5, fact_w = 1.;
  int compteur;

  int possible_pas, possible_compteur;
  
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  
  for(i=0;i<4;i++) old_rect[i] = 0; /* Init. old_rect to force first grad. to be displayed */
  


  lastzindex = ppsubwin->axes.nzgrads - 1;
  
  if(lastzindex == 0)
    ChooseFormatForOneGrad(c_format,&(ppsubwin->axes.zgrads[0]));
  else
    ChoixFormatE(c_format,
		 ppsubwin->axes.zgrads[0],
		 ppsubwin->axes.zgrads[lastzindex],
		 ((ppsubwin->axes.zgrads[lastzindex])-(ppsubwin->axes.zgrads[0]))/(lastzindex));
  
  nbtics = ppsubwin->axes.nzgrads;
  nbsubtics = ppsubwin->axes.nbsubtics[2];


  for(i=0;i<nbtics;i++) grads_tmp[i] = ppsubwin->axes.zgrads[i];
  
  for(i=0;i<nbtics;i++)
    {
      char foo[256];
      double ztmp = ppsubwin->axes.zgrads[i];

      sprintf(foo,c_format,ztmp);
      
      /***************************************************************/
      /************************* COMMON PART *************************/
      /***************************************************************/
      if(ppsubwin->axes.reverse[2] == TRUE)
	ztmp = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp);
      
      ComputeGoodTrans3d(psubwin,1,&xm,&ym,&fx,&fy,&ztmp);
            
      
      vx[0]=xm;vy[0]=ym;
      
      barlengthx= (integer) (( Ticsdir[0])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      barlengthy= (integer) (( Ticsdir[1])/sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1])*size);
      vx[1]=vx[0]+barlengthx;
      vy[1]=vy[0]+barlengthy;
      
      /* foo is set above with sprintf(foo,c_format,xtmp); */
  	      
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); /* fix bug noticed by R.N. */
      C2F(dr)("xstringl",foo,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      posi[0] = inint( xm+2*barlengthx - rect[2]); 
      posi[1]=inint( ym + 2*barlengthy + rect[3]/2);


      /* compute bounding of "10"  string used for log scale ON and auto_ticks ON */
      C2F(dr)("xstringl","10",&XX,&YY,logrect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	
      
      
      if(CheckDisplay(fact_h, fact_w, ppsubwin->logflags[2],foo,posi,fontid,old_rect) == 0)
	continue; /*  graduation too close, DO NOT display the graduation ! */
	      
      C2F(dr)("xset","pattern",&textcolor,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if ( ppsubwin->logflags[2] == 'l' )
	{
	  int smallersize = fontid[1]-2;
	  int old_rect10[4];
	  int posi10[2];
	  
	  posi10[0] = posi[0] - logrect[2];
	  posi10[1] = posi[1] + logrect[3];
	  
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  C2F(dr)("xstringl","10",(&posi10[0]),(&posi10[1]),old_rect10,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  posi[0] = old_rect10[0] + old_rect10[2];
	  posi[1] = (int) (old_rect10[1] - old_rect10[3]*.1);
	  
	  C2F(dr)("xset","font",fontid,&smallersize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  
	  /* update old_rect */
	  old_rect[2] = (int)(fact_w*(old_rect[2] + old_rect10[2]));
	  old_rect[3] = (int)(fact_h*(old_rect[3] + old_rect10[3] + (int) (old_rect10[3]*.1)));
	  old_rect[0] = old_rect10[0];
	  old_rect[1] = old_rect[1];
	  
	  nb_grads_max++;
	  
	  
	  /* put back the current fontid */
	  C2F(dr)("xset","font",fontid,fontid+1,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else{
	/* update old_rect */
	C2F(dr)("xstringl",foo,(&posi[0]),(&posi[1]),old_rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	old_rect[3] = (int)(fact_h* old_rect[3]);
	old_rect[2] = (int)(fact_w* old_rect[2]);
	nb_grads_max++;
      }
    }
  
  
  if(ppsubwin->logflags[2] != 'l')
    if(nb_grads_max == 1) /* only one grad. can be displayed : we choose to display the average value (max+min)/2 */
      {
	ppsubwin->axes.zgrads[0] = (grads_tmp[0]+grads_tmp[lastzindex])/2.;
	ppsubwin->axes.nzgrads = 1;
        if ( !ppsubwin->flagNax )
        {
          ppsubwin->axes.nbsubtics[2] = 1;
        }
	return 0;
      }
  
  pas = nbtics - 2; /* pas == grads number - 2 */

  possible_pas = -99;
  possible_compteur = -99;

  while(pas > 0)
    {
      int tmp = 0;
      compteur = 0;
      for(;;)
	{
	  tmp = tmp + pas;
	  compteur++;
	  
	  if((tmp == (nbtics - 1)) && (compteur < nb_grads_max)){
	    possible_pas = pas;
	    possible_compteur = ++compteur;
	    break;
	  }
	  
	  if(tmp > (nbtics - 1))
	    break;
	}
      
      pas--;
    }
  
  if(possible_compteur != -99){
    compteur = possible_compteur;
    pas = possible_pas;
    
    for(i=0;i<compteur;i++)
      ppsubwin->axes.zgrads[i] = grads_tmp[i*pas];
    
    ppsubwin->axes.nzgrads = compteur;
    
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nzgrads,
                                                           ppsubwin->logflags[2],
							   ppsubwin->axes.zgrads,
                                                           ppsubwin->axes.nbsubtics[2]); 
  }
  else{
    ppsubwin->axes.zgrads[0] = grads_tmp[0];
    ppsubwin->axes.zgrads[1] = grads_tmp[lastzindex];
    ppsubwin->axes.nzgrads = 2;
    /* Nb of subtics computation and storage */
    /* jb Silvy 01/2006 : default value was Max((int) abs((13-compteur)/2),2) */
    ppsubwin->axes.nbsubtics[2] = ComputeNbSubTicsFor3dUse(psubwin,
                                                           ppsubwin->axes.nzgrads,
                                                           ppsubwin->logflags[2],
							   ppsubwin->axes.zgrads,
                                                           ppsubwin->axes.nbsubtics[2]); 
  }
  
  return 0;
}



int ComputeNbSubTicsFor3dUse(sciPointObj * pobj, int nbtics, char logflag, double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {6,4,4,3,3,3,2,2,2 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1};
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);


  if(logflag =='l'){
    if( (grads[1]-grads[0]) == 1 ) /* intervalle de type ...10^n 10^(n+1)...*/
      {
	return 9; /* 9 subtics to have a pretty tics/grid in log.*/
      }
    else
      {
	return 1; /* no subtics at all (1 but draw on a tics place) */
      }
  }
  else
  {
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
    { 
      for(i=0;i<19;i++)
      {
        if(nbtics == ticsval[i])
        {
          return subticsval[i];
        }
      }
    }
    else /* if auto subtics mode == OFF already computed in Plo2dn.c, Champ.c routines... */
    {  /* or given via a.subtics=[nbsubtics_on_x, nbsubtics_on_y, nbsubtics_on_z] command */
      return nbsubtics_input;
    }
  }
  
  return -1;
}


int ChooseFormatForOneGrad(char *c_format, double *grad)
{
  int compteur = 0;
  double epsilon = 1e-16;
  
  if((double)(abs((int)*grad)) < epsilon){ /* case where grad is strictly equal to 0. */
    *grad = 0.;
    strcpy(c_format,"%d");
    return 0;
  }
  
  while((int) (*grad) == 0){
    compteur++;
    *grad = 10*(*grad);
    
    if(compteur > 100){
      Scierror(999,"Error in function ChooseFormatForOneGrad\n");
      return -1;
    }
  }
  
  sprintf(c_format,"%%.%df",compteur);
  
  return 0;
}



/*-------------------------------------------------------------------------------------*/
/* compute the 8 vertices (xbox[i],ybox[i],zbox[i]) of an axis aligned box knowing */
/* xmin, xmax, ymin, ymax, zmin and zmax in limits */
void sciGetAxisBox( double limits[6], double xbox[8], double ybox[8], double zbox[8] )
{
  xbox[0]=TRX(limits[0],limits[2],limits[4]); /* transfo. 3D of [xmin,ymin,zmin] */
  ybox[0]=TRY(limits[0],limits[2],limits[4]); /* into [xbox[0],ybox[0],zbox[0] ] */
  zbox[0]=TRZ(limits[0],limits[2],limits[4]); /*                                 */ 
  
  xbox[1]=TRX(limits[0],limits[3],limits[4]); /* transfo. 3D of [xmin,ymax,zmin] */
  ybox[1]=TRY(limits[0],limits[3],limits[4]); /* into [xbox[1],ybox[1],zbox[1] ] */
  zbox[1]=TRZ(limits[0],limits[3],limits[4]); /*                                 */
  
  xbox[2]=TRX(limits[1],limits[3],limits[4]); /* transfo. 3D of [xmax,ymax,zmin] */
  ybox[2]=TRY(limits[1],limits[3],limits[4]); /* into [xbox[2],ybox[2],zbox[2] ] */
  zbox[2]=TRZ(limits[1],limits[3],limits[4]); /*                                 */
  
  xbox[3]=TRX(limits[1],limits[2],limits[4]); /* transfo. 3D of [xmax,ymin,zmin] */
  ybox[3]=TRY(limits[1],limits[2],limits[4]); /* into [xbox[3],ybox[3],zbox[3] ] */
  zbox[3]=TRZ(limits[1],limits[2],limits[4]); /*                                 */
  
  xbox[4]=TRX(limits[0],limits[2],limits[5]); /* transfo. 3D of [xmin,ymin,zmax] */
  ybox[4]=TRY(limits[0],limits[2],limits[5]); /* into [xbox[4],ybox[4],zbox[4] ] */
  zbox[4]=TRZ(limits[0],limits[2],limits[5]); /*                                 */
  
  xbox[5]=TRX(limits[0],limits[3],limits[5]); /* transfo. 3D of [xmin,ymax,zmax] */
  ybox[5]=TRY(limits[0],limits[3],limits[5]); /* into [xbox[5],ybox[5],zbox[5] ] */
  zbox[5]=TRZ(limits[0],limits[3],limits[5]); /*                                 */
  
  xbox[6]=TRX(limits[1],limits[3],limits[5]); /* transfo. 3D of [xmax,ymax,zmax] */
  ybox[6]=TRY(limits[1],limits[3],limits[5]); /* into [xbox[6],ybox[6],zbox[6] ] */
  zbox[6]=TRZ(limits[1],limits[3],limits[5]); /*                                 */
  
  xbox[7]=TRX(limits[1],limits[2],limits[5]); /* transfo. 3D of [xmax,ymin,zmax] */
  ybox[7]=TRY(limits[1],limits[2],limits[5]); /* into [xbox[7],ybox[7],zbox[7] ] */
  zbox[7]=TRZ(limits[1],limits[2],limits[5]); /*                                 */
}
/*-------------------------------------------------------------------------------------*/
/* update the Csacle value from new viewing angles */
void sciUpdateScaleAngles( double theta, double alpha )
{
  double cost = 0.5 ;
  double sint = 0.5 ;
  double cosa = 0.5 ;
  double sina = 0.5 ;

  cost = cos( DEG2RAD(theta) ) ;
  cosa = cos( DEG2RAD(alpha) ) ;
  sint = sin( DEG2RAD(theta) ) ;
  sina = sin( DEG2RAD(alpha) ) ;
  
  Cscale.m[0][0]= -sint        ;
  Cscale.m[0][1]=  cost        ;
  Cscale.m[0][2]=  0           ;
  Cscale.m[1][0]= -cost * cosa ;
  Cscale.m[1][1]= -sint * cosa ;
  Cscale.m[1][2]=  sina        ;
  Cscale.m[2][0]=  cost * sina ;
  Cscale.m[2][1]=  sint * sina ;
  Cscale.m[2][2]=  cosa        ;

}

/*-------------------------------------------------------------------------------------*/
/* compute the drawing order of the axes box vertices int xind */
void sciAxesVerticesIndices( integer insideU[4],
                             integer insideD[4],
                             double  xbox[8]   ,
                             double  ybox[8]   ,
                             integer xind[8]    )
{
  int    i      ;
  int    ind    ;
  int    ind2   ;
  int    ind3   ;
  double xmaxi  ;
  int    tmpind ;

  /* F.Leray 23.02.04 Mise a 0 du tableau xind pour corriger bug*/
  /* dans le cas ind < 3 ET ybox[tmpind] < ybox[tmpind]*/
  for( i = 0 ; i < 8 ; i++ )
  { 
    xind[i] = 0 ;
  }

  /* indices */
  /* determine the indices for the 3d represention */
  xmaxi = ( (double) Maxi(xbox,8L) ) ;
  ind = -1 ;
  MaxiInd( xbox, 8L, &ind, xmaxi ) ;
  if ( ind > 3 )
  {
    xind[0]=ind;
  }
  tmpind = ind ;  
  MaxiInd(xbox,8L,&ind,xmaxi);
  if ( ind > 3 )
  {
    xind[0] = ind ;
  }
  if (ybox[tmpind] > ybox[ind] )
  {
    xind[0] = tmpind ;
  }
  
  if (ind < 0 || ind > 8) 
  {
    sciprint("xind out of bounds");
    xind[0]=0;
  }
  Nextind(xind[0],&ind2,&ind3);
  if (ybox[ind2] > ybox[ind3]) 
  {
    xind[1]=ind2;insideU[0]=ind3;
  }
  else 
  {
    xind[1]=ind3;insideU[0]=ind2;
  }
  Nextind(ind2,&ind2,&ind3); insideU[1]=xind[0];
  insideU[2]=ind2; 
  if ( insideU[0] > 3 )
  {
    insideU[3] = insideU[0] - 4 ;
  }
  else
  {
    insideU[3] = insideU[0] + 4 ;
  }
  xind[2]=ind2;
  /* le pointeger en bas qui correspond */	  
  if ( ind2 > 3 )
  {
    xind[3] = ind2 - 4 ;
  }
  else
  {
    xind[3] = ind2 + 4 ;
  }
  Nextind(xind[3],&ind2,&ind3);
  if (ybox[ind2] < ybox[ind3]) 
  {
    xind[4]=ind2;insideD[0]=ind3;
  }
  else  
  {
    xind[4]=ind3;insideD[0]=ind2;
  }
  Nextind(ind2,&ind2,&ind3);
  insideD[1]=xind[3];
  insideD[2]=ind2;
  if ( insideD[0] > 3 )
  {
    insideD[3]=insideD[0] - 4 ;
  }
  else
  {
    insideD[3]=insideD[0] + 4 ;
  }
  xind[5]=ind2;
         
}
/*------------------------------------------------------------------------------------------*/
/**
 * Update the coordinates of a label to put it in the middle of a segment (normally an axis)
 * but with a little orthogonal offset.
 * Let Ri i=0:3 be the four corners of the label boudinbg box. The algorithm will put the
 * center of the bounding box C on the middle of the segment I. Then we compute for each corner
 * the needed translation to put them at a distance given by offset. The translation is done
 * along an axis orthogonal to the segment. Finally we transalte the box from I with the longest
 * translation, so each corner is at least at the wanted distance.
 * @param pLabel pointer to the label to place.
 * @param axisStart Starting point of the segment.
 * @param axisEnd   The other end of the segment. The order of the two points determine the
 *                  side in which the label will be placed.
 * @param offsets   The label is not put exactly on the middle of the segment. It is moved
 *                  from a little offset orthogonally from the segment.
 */
void computeLabelAutoPos( sciPointObj * pLabel, int axisStart[2], int axisEnd[2], int offsets[2] )
{
  /* computation are done in double to avoid loss of data */
  double centerDir[2]  ; /* give the direction of the line on which is the center. */
                         /* it is orthogonal with the axis direction */
  double axisMiddle[2] ; /* middle of the axis = ( axisStart + axisEnd ) / 2 */
  int    bbox[4][2]    ; /* real bounding box of the label */
  double corners[4][2] ; /* the four corners of the boundign box */
  double centerDist    ; /* final distance between the center and the axis */
  int wantedBlPoint[2] ; /* the position we want for the bottom left point of the axis */
  double distance      ;
  double rectCenter[2] ;

  if ( axisStart[0] == axisEnd[0] && axisStart[1] == axisEnd[1] )
  {
    /* the axis is invisible. Its label is not displayed also. */
    return ;
  }

  /* normalized vector orthogonal to the axe */
  /* its direction (+ or -) determines on which side will be displayed the axis */
  centerDir[0] = axisStart[1] - axisEnd[1]   ;
  centerDir[1] = axisEnd[0]   - axisStart[0] ;
  normalize2d( centerDir ) ;

  /* the distance between the aabb and the axis is offsets[0] if the axis is vertical, */
  /* offest[1] if horizontal and something in between otherwise */
  distance = Abs( (double)offsets[0] * centerDir[0] ) + Abs( (double)offsets[1] * centerDir[1] ) ;

  axisMiddle[0] = ( axisStart[0] + axisEnd[0] ) / 2.0 ;
  axisMiddle[1] = ( axisStart[1] + axisEnd[1] ) / 2.0 ;
  
  /* get the size of the label axis aligned bouding box. */
  getTextBoundingBox( pLABEL_FEATURE(pLabel)->text, bbox, NULL ) ;

  /* get the center of the rectangle */
  rectCenter[0] = ( bbox[0][0] + bbox[1][0] + bbox[2][0] + bbox[3][0] ) / 4.0 ;
  rectCenter[1] = ( bbox[0][1] + bbox[1][1] + bbox[2][1] + bbox[3][1] ) / 4.0 ;

  /* We suppose the box is centered on I and we compute IRi, i=0:3 */
  corners[0][0] = bbox[0][0] - rectCenter[0] ;
  corners[0][1] = bbox[0][1] - rectCenter[1] ;
  corners[1][0] = bbox[1][0] - rectCenter[0] ;
  corners[1][1] = bbox[1][1] - rectCenter[1] ;
  corners[2][0] = bbox[2][0] - rectCenter[0] ;
  corners[2][1] = bbox[2][1] - rectCenter[1] ;
  corners[3][0] = bbox[3][0] - rectCenter[0] ;
  corners[3][1] = bbox[3][1] - rectCenter[1] ;

  /* compute IRi.v, i=0:3 to get the current distance between Ri and the axis. */
  /* Then for each Ri we can compute the need displacment along the centerDir to push it */
  /* at distance dist of the axis. Finally, the needed displacement is the maximum of the 4. */
  centerDist = distance - DOT_PROD_2D( corners[0], centerDir ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[1], centerDir ) ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[2], centerDir ) ) ;
  centerDist = Max( centerDist, distance - DOT_PROD_2D( corners[3], centerDir ) ) ;

  /* the wanted center is C = I + a.v */
  wantedBlPoint[0] = round( axisMiddle[0] + centerDist * centerDir[0] + corners[0][0] ) ;
  wantedBlPoint[1] = round( axisMiddle[1] + centerDist * centerDir[1] + corners[0][1] ) ;

  sciSetPosition( pLabel,
                  XPixel2Double( wantedBlPoint[0] ),
                  YPixel2Double( wantedBlPoint[1] ) ) ;

}
/*------------------------------------------------------------------------------------------*/

#undef round
