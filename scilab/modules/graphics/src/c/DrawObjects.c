/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
 *    newGraph Library header
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
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "ObjectStructure.h"
#include "Axes.h"
#include "Champ.h"
#include "StringBox.h"
#include "PloEch.h"
#include "axesScale.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "WindowList.h"
#include "machine.h"

#include "math_graphics.h"
#include "graphicSession.h"
#include "Format.h"
#include "axesScale.h"
#include "BasicAlgos.h"

#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "../../../tclsci/includes/GedManagement.h"

#ifdef _MSC_VER
extern HDC TryToGetDC(HWND hWnd);
#endif

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)

extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);

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

void sciRedrawF(int *value)
{ 
  sciDrawObj(getFigureFromIndex(*value));
}

void sciClearFigure(sciPointObj * pFigure)
{
  sciPointObj * newSubWin = NULL;
  destroyGraphicsSons(pFigure);

  if ((newSubWin = ConstructSubWin(pFigure)) != NULL)
  {
    sciSetCurrentObj(newSubWin);
    sciSetOriginalSubWin(pFigure, newSubWin);
  }
  forceHierarchyRedraw(pFigure);    
}

void sciXbasc()
{  
  sciClearFigure(sciGetCurrentFigure());  
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
      sciprint(_("Warning: Can't use Log on X-axis xmin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/
  if ( ppsubwin->logflags[1] == 'l' )
  {
    if ( sciGetLogExponent( *ymin, *ymax, ymin, ymax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Y-axis ymin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Z axis
   *****************************************************************/
  if ( ppsubwin->logflags[2] == 'l' )
  {
    if ( sciGetLogExponent( *zmin, *zmax, zmin, zmax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Z-axis zmin is negative.\n"));
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
    int wdim[2];
    
    wdim[0] = sciGetWindowWidth(sciGetParentFigure(pobj));
    wdim[1] = sciGetWindowHeight(sciGetParentFigure(pobj));

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







int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, const double * grads, int nbsubtics_input)
{
  int ticsval[] =    {2 ,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  int subticsval[] = {9,6,4,4,3,3,2,1,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
  int i;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);


  if(logflag =='l'){
    return 8; /* 9 subtics to have a pretty tics/grid in log.*/
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
  
  if( sciGetIsAutoDrawable(pfigure) && sciGetVisibility(pfigure) )
  {
    DrawAxes(pobj);
  }
  
}

/* Routine used inside Plo2dn.c, Champ.c, Gray.c... */
/* to force the drawing of the axes after a new object is created */
void DrawAxes(sciPointObj * pobj)
{
  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  sciDrawObj(psubwin);
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


/*---------------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/* sciRefreshObj                                                                             */
/* draw an object but before select the rigth figure for display                             */
/*----------------------------------------------------------------------------------*/
int sciRefreshObj( sciPointObj * pobj )
{
  
  /* draw the object */
  int status = sciDrawObjIfRequired( pobj ) ;
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
  /*sciPointObj * pfigure = sciGetParentFigure(pobj);

  if( sciGetIsAutoDrawable(pfigure) && sciGetVisibility(pfigure) )
  {
    sciDrawObj( pobj ) ;
  }*/

  sciDrawObj( pobj ) ;

  return 0;
}


/*----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
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
    sciprint(_("xind out of bounds"));
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
/*---------------------------------------------------------------------------------*/
void showPixmap(sciPointObj * pFigure)
{
  /* Hack here. Pixmap has same action as drawlater() */
  /* So we just draw the figure here to make it appear */
  if (sciGetPixmapMode(pFigure))
  {
    sciSetPixmapMode(pFigure, FALSE);
    sciDrawObj(pFigure);
    sciSetPixmapMode(pFigure, TRUE);
    sciDrawObj(pFigure);
  }
}
/*---------------------------------------------------------------------------------*/
void clearPixmap(sciPointObj * pFigure)
{
  // nothing to do with the hack
}
/*---------------------------------------------------------------------------------*/

#undef round
