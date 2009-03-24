/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - INRIA - Serge Steer
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
 --------------------------------------------------------------------------*/

#include "math_graphics.h" 
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "axesScale.h"
#include "Format.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "GraphicSynchronizerInterface.h"
#include "HandleManagement.h"

#include "MALLOC.h" /* MALLOC */

/*-------------------------------------------
 * setscale2d 
 * uses WRect,ARect,FRect,logscale to update 
 * current subwindow
 *  WRect gives the subwindow to use 
 *  ARect gives the axis rectangle 
 *  FRect gives the bounds 
 *  WRect=[<x-upperleft>,<y-upperleft>,largeur,hauteur]
 *    example WRect=[0,0,1.0,1.0] we use all the window 
 *            WRect=[0.5,0.5,0.5,0.5] we use the down right 
 *            quarter of the window
 *  logscale : gives xy log axis flags 
 *  each argument can be a null pointer if they are 
 *  not to be changed from their current value 
 *
 *  
 *-------------------------------------------*/

int setscale2d(double WRect[4],
                double ARect[4],
                double FRect[4],
                char   logscale[2])
{
  sciPointObj * masousfen = NULL;
  sciPointObj * currentFigure = sciGetCurrentFigure();
 
	startFigureDataWriting(currentFigure);
  if (WRect != NULL) 
  {
      /* Ajout djalel */
      if (( masousfen = sciIsExistingSubWin(WRect)) != NULL)
			{
				/* Select the subwin */
        sciSetSelectedSubWin(masousfen);
      }
      else if ((masousfen = ConstructSubWin (sciGetCurrentFigure())) != NULL)
      {
        /* F.Leray Adding here 26.03.04*/
        sciSetCurrentObj(masousfen);
        sciSetSelectedSubWin(masousfen);
        pSUBWIN_FEATURE (masousfen)->WRect[0]   = WRect[0];
        pSUBWIN_FEATURE (masousfen)->WRect[1]   = WRect[1];
        pSUBWIN_FEATURE (masousfen)->WRect[2]   = WRect[2];
        pSUBWIN_FEATURE (masousfen)->WRect[3]   = WRect[3];
      }
			else
			{
				/* An error occured */
				endFigureDataWriting(currentFigure);
				return -1;
			}
	}
   
	masousfen = sciGetCurrentSubWin();

  if (FRect != NULL)
  {
		/* Frect is [xMin, yMin, xMax, yMax] */
    pSUBWIN_FEATURE (masousfen)->SRect[0] = FRect[0];
    pSUBWIN_FEATURE (masousfen)->SRect[2] = FRect[1];
    pSUBWIN_FEATURE (masousfen)->SRect[1] = FRect[2];
    pSUBWIN_FEATURE (masousfen)->SRect[3] = FRect[3];
  }

  if (ARect != NULL)
  {
    pSUBWIN_FEATURE (masousfen)->ARect[0] = ARect[0];
    pSUBWIN_FEATURE (masousfen)->ARect[1] = ARect[1];
    pSUBWIN_FEATURE (masousfen)->ARect[2] = ARect[2];
    pSUBWIN_FEATURE (masousfen)->ARect[3] = ARect[3];
  }

	if (logscale != NULL)
	{
		/* only log flsgs on X and Y are modified */
		char logFlags[3];
		sciGetLogFlags(masousfen, logFlags);
		logFlags[0] = logscale[0];
		logFlags[1] = logscale[1];
		sciSetLogFlags(masousfen, logFlags);
	}
	endFigureDataWriting(currentFigure);

  if (masousfen != NULL)
  {
    // subwindow has changed, redraw it
    sciDrawObj(masousfen);
  }

  return(0);
}

/* used to send values to Scilab */
int getscale2d( double WRect[4], double FRect[4], char logscale[2], double ARect[4] )
{
  int i;
	sciPointObj * curSubwin = sciGetCurrentSubWin();
	
	char logFlags[3];
	sciGetLogFlags(curSubwin, logFlags);
	logscale[0] = logFlags[0];
	logscale[1] = logFlags[1];
  
	for ( i=0; i < 4 ; i++) 
  {
		WRect[i] = pSUBWIN_FEATURE(curSubwin)->WRect[i];
    ARect[i] = pSUBWIN_FEATURE(curSubwin)->ARect[i];
  }

	/* Frect is [xMin, yMin, xMax, yMax] */
	FRect[0] = pSUBWIN_FEATURE(curSubwin)->FRect[0];
	FRect[1] = pSUBWIN_FEATURE(curSubwin)->FRect[2];
	FRect[2] = pSUBWIN_FEATURE(curSubwin)->FRect[1];
	FRect[3] = pSUBWIN_FEATURE(curSubwin)->FRect[3];

  return(0);
}


extern void unzoom()
{
  /** 17/09/2002 ***/
  double fmin,fmax,lmin,lmax;
  int min,max,puiss,deux=2,dix=10;
  sciPointObj *psousfen;
  sciSons *psonstmp;

  /***** 02/10/2002 ****/
  psonstmp = sciGetSons(sciGetCurrentFigure());
  while (psonstmp != (sciSons *) NULL)
  {
    if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN)
    {
      psousfen= (sciPointObj *)psonstmp->pointobj;
      if (sciGetZooming(psousfen))
      {
        sciSetZooming(psousfen, 0);

        pSUBWIN_FEATURE (psousfen)->ZRect[0]   = pSUBWIN_FEATURE (psousfen)->SRect[0];
        pSUBWIN_FEATURE (psousfen)->ZRect[1]   = pSUBWIN_FEATURE (psousfen)->SRect[1];
        pSUBWIN_FEATURE (psousfen)->ZRect[2]   = pSUBWIN_FEATURE (psousfen)->SRect[2];
        pSUBWIN_FEATURE (psousfen)->ZRect[3]   = pSUBWIN_FEATURE (psousfen)->SRect[3];

        /*}  SS: moved below because if sciGetZooming(psousfen)==0 
        ZRect is undefined -> code may enter in infinite recursion loop to compute graduation
        and there is no use to regraduate */

        /** regraduation de l'axe des axes ***/
        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[0];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[2];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;

        fmin= pSUBWIN_FEATURE (psousfen)->ZRect[1];
        fmax= pSUBWIN_FEATURE (psousfen)->ZRect[3];
        C2F(graduate)(&fmin, &fmax,&lmin,&lmax,&deux,&dix,&min,&max,&puiss) ;
        /*****/
      }
    }
    psonstmp = psonstmp->pnext;
  }

  sciDrawObj(sciGetCurrentFigure());
}

/*--------------------------------------------------------------------------*/
/**
 * Interface to function xchange "f2i". Convert user 2d coordinates
 * to pixel ones.
 * @param rect [x,y,w,h] of current subwin
 */
void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4])
{ 
  /* coordinates transformation */
  int i;
  sciPointObj * selectedSubwin = sciGetCurrentSubWin();
  updateSubwinScale(selectedSubwin);
  for (i = 0; i < nbCoords; i++)
  {
    // specify a default value for Z
    double curCoords[3] = {xCoords[i], yCoords[i], 0.0};
    int curPixCoords[2];
    sciGet2dViewPixelCoordinates(selectedSubwin, curCoords, curPixCoords);
    xPixCoords[i] = curPixCoords[0];
    yPixCoords[i] = curPixCoords[1];
  }

  /* get viewing area */
  sciGetViewingArea(selectedSubwin, &rect[0], &rect[1], &rect[2], &rect[3]);
 
}
/*--------------------------------------------------------------------------*/
/**
* Interface to function xchange "i2f". Convert pixel coordinates
* to user 2d coordinates.
* @param rect [x,y,w,h] of current subwin
*/
void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4])
{ 
  /* coordinates transformation */
  int i;
  sciPointObj * selectedSubwin = sciGetCurrentSubWin();
  updateSubwinScale(selectedSubwin);
  for (i = 0; i < nbCoords; i++)
  {
    // specify a default value for Z
    int curPixCoords[2] = {xPixCoords[i], yPixCoords[i]};
    double curCoords[2];
    sciGet2dViewCoordFromPixel(selectedSubwin, curPixCoords, curCoords);
    xUserCoords[i] = curCoords[0];
    yUserCoords[i] = curCoords[1];
  }

  /* get viewing area */
  sciGetViewingArea(selectedSubwin, &rect[0], &rect[1], &rect[2], &rect[3]);

}
/*--------------------------------------------------------------------------*/
