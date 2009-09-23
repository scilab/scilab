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

#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "PloEch.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "math_graphics.h"
#include "Format.h"
#include "HandleManagement.h"


#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

//#include "../../../tclsci/includes/GedManagement.h"


#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)



static BOOL subwinNeedsDisplay(sciPointObj * pSubwin);



void sciRedrawFigure( void )
{
  sciDrawObj(sciGetCurrentFigure ());
}


void sciClearFigure(sciPointObj * pFigure)
{
  destroyGraphicsSons(pFigure);

	/* recreate the subwindow */
	createFirstSubwin(pFigure);

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

   
   
   
  /*****************************************************************
   * set the actual bounds in subwindow data structure
   *****************************************************************/
   
   
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
   
   
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

     
  ppsubwin->FRect[0]=xmin;
  ppsubwin->FRect[2]=xmax;
  ppsubwin->FRect[1]=ymin;
  ppsubwin->FRect[3]=ymax;
  ppsubwin->FRect[4]=zmin;
  ppsubwin->FRect[5]=zmax;
   
 
    
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
BOOL needsDisplay(sciPointObj * pFigure)
{
	/* return false if the figure contains no or one subwindow and the subwindow is not displayed. */
	
	if (!sciGetVisibility(pFigure))
	{
		/* Figure not visible */
		return FALSE;
	}
	else if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) == 0)
	{
		/* No subwindows, return false */
		return FALSE;
	}
	else if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) == 1)
	{
		/* One subwindow check if it is visible */
		sciPointObj * onlySubwin = sciGetFirstTypedSelectedSon(pFigure, SCI_SUBWIN);
		return subwinNeedsDisplay(onlySubwin);
	}
	else
	{
		return TRUE;
	}
}
/*---------------------------------------------------------------------------------*/
static BOOL subwinNeedsDisplay(sciPointObj * pSubwin)
{
	/* the subwindow is not displayed if it does not have any children, its box is of and is transparent or */
	/* has the same background as the figure */
	if (!sciGetVisibility(pSubwin))
	{
		/* subwin invisible */
		return FALSE;
	}
	else if (sciGetNbChildren(pSubwin) > 4)
	{
		/* Other children than the labels */
		return TRUE;
	}
	else
	{
		BOOL axesVisible[3];

		if (sciGetBoxType(pSubwin) != BT_OFF)
		{
			/* Box is displayed */
			return TRUE;
		}

		sciGetAxesVisible(pSubwin, axesVisible);
		if (axesVisible[0] || axesVisible[1] || axesVisible[2])
		{
			/* One axis is visible */
			return TRUE;
		}

		if (   sciGetIsFilled(pSubwin)
			  && sciGetBackground(sciGetParentFigure(pSubwin)) != sciGetBackground(pSubwin))
		{
			/* Compare subwin background and figure one */
			return TRUE;
		}

		/* Check that labels texts are empty */
		if (   !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_title)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_x_label)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_y_label)
				|| !sciisTextEmpty(pSUBWIN_FEATURE(pSubwin)->mon_z_label))
		{
			return TRUE;
		}

		/* apparently no need to display the axes */
		return FALSE;
	}
}
/*---------------------------------------------------------------------------------*/
#undef round
