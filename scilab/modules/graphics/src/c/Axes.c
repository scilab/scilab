/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots
 *--------------------------------------------------------------------------*/

#include <string.h>
#include "math_graphics.h"
#include "Axes.h"
#include "DestroyObjects.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "SetProperty.h"
#include "SetJavaProperty.h"
#include "axesScale.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "pixel_mode.h"
#include "DoublyLinkedList.h"


#include "MALLOC.h" /* MALLOC */

#include "GetProperty.h"

/*--------------------------------------------------------------------------------*/
static int getSqDistanceToCenter(sciPointObj * pSubwin, int xCoord, int yCoord);
static BOOL isSubwinUnderPixel(sciPointObj * pSubwin, int xCoord, int yCoord);

/*--------------------------------------------------------------------------------*/
/* clear a subwindow from all of its children */
void clearSubWin( sciPointObj * pSubWin )
{
  sciSons * curSon = sciGetSons (pSubWin);

  while ( curSon != NULL && curSon->pointobj != NULL )
  {
    if ( curSon->pointobj->entitytype != SCI_LABEL )
    {
      destroyGraphicHierarchy (curSon->pointobj) ;
      curSon = sciGetSons ( pSubWin ) ;
    }
    else
    {
      curSon = curSon->pnext ;
    }

  }
}
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position ) */
void reinitSubWin( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin) ;

  clearSubWin( pSubWin ) ;

  initSubWinBounds( pSubWin ) ;
  ppSubWin->axes.xdir = 'd' ;
  ppSubWin->axes.ydir = 'l' ;

  ppSubWin->visible = TRUE;

  initSubWinAngles( pSubWin ) ;

  ppSubWin->FirstPlot = TRUE;


}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
void initSubWinAngles( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->is3d     = ppAxesMdl->is3d     ;
  ppSubWin->alpha_kp = ppAxesMdl->alpha_kp ;
  ppSubWin->theta_kp = ppAxesMdl->theta_kp ;
  ppSubWin->alpha    = ppAxesMdl->alpha    ;
  ppSubWin->theta    = ppAxesMdl->theta    ;
}

/*--------------------------------------------------------------------------------*/
/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->WRect[0] = ppAxesMdl->WRect[0] ;
  ppSubWin->WRect[1] = ppAxesMdl->WRect[1] ;
  ppSubWin->WRect[2] = ppAxesMdl->WRect[2] ;
  ppSubWin->WRect[3] = ppAxesMdl->WRect[3] ;

}
/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
void initSubWinBounds( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->FRect[0] = ppAxesMdl->FRect[0] ;
  ppSubWin->FRect[1] = ppAxesMdl->FRect[1] ;
  ppSubWin->FRect[2] = ppAxesMdl->FRect[2] ;
  ppSubWin->FRect[3] = ppAxesMdl->FRect[3] ;
  ppSubWin->FRect[4] = ppAxesMdl->FRect[4] ;
  ppSubWin->FRect[5] = ppAxesMdl->FRect[5] ;

  ppSubWin->SRect[0] = ppAxesMdl->SRect[0] ;
  ppSubWin->SRect[1] = ppAxesMdl->SRect[1] ;
  ppSubWin->SRect[2] = ppAxesMdl->SRect[2] ;
  ppSubWin->SRect[3] = ppAxesMdl->SRect[3] ;
  ppSubWin->SRect[4] = ppAxesMdl->SRect[4] ;
  ppSubWin->SRect[5] = ppAxesMdl->SRect[5] ;
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void )
{
  //  nbCheckRedraw++;
  //  fprintf(stderr, "[DEBUG] checkRedrawing : %d\n", nbCheckRedraw);
  sciPointObj * pSubWin = sciGetCurrentSubWin() ;
  if ( !sciGetAddPlot( pSubWin ) )
  {
      /* redraw the axis */
      reinitSubWin( pSubWin ) ;
      forceRedraw(pSubWin);
      return TRUE ;
  }
  return FALSE ;
}
/*--------------------------------------------------------------------------------*/
/**
 * Return the square distance between the center of axes box anf point (xCoord, yCoord);
 */
static int getSqDistanceToCenter(sciPointObj * pSubwin, int xCoord, int yCoord)
{
  int xPos;
  int yPos;
  int width;
  int height;
  /* get area used by the subwindow */
  sciGetViewingArea(pSubwin, &xPos, &yPos, &width, &height);

  /* get coordinate sof middle */
  xPos = xPos + width / 2;
  yPos = yPos + height / 2;

  return (xCoord - xPos) * (xCoord - xPos) + (yCoord - yPos) * (yCoord - yPos);

}
/*--------------------------------------------------------------------------------*/
/**
 * @return TRUE if the point in pixel (xCoord, yCoord) is above the subwin
 */
static BOOL isSubwinUnderPixel(sciPointObj * pSubwin, int xCoord, int yCoord)
{
  int xPos;
  int yPos;
  int width;
  int height;
  /* get area used by the subwindow */
  sciGetViewingArea(pSubwin, &xPos, &yPos, &width, &height);

  return (   xCoord > xPos && xCoord < xPos + width
          && yCoord > yPos && yCoord < yPos + height);
}
/*--------------------------------------------------------------------------------*/
sciPointObj * getClickedSubwin(sciPointObj * pFigure, int xCoord, int yCoord)
{
  int nbItem;
  sciPointObj * res = NULL;

  /* First get the list of subwindow that are under the click */
  /* Might be several if some are hidding others */
  DoublyLinkedList * foundSubwins = DoublyLinkedList_new();
  sciSons * pSons = sciGetSons(pFigure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      updateSubwinScale(curObj);
      if (isSubwinUnderPixel(curObj, xCoord, yCoord))
      {
        foundSubwins = List_push(foundSubwins, curObj);
      }
    }
    pSons = pSons->pnext;
  }

  /* all the subwindows that are under the clicked pixel has been found */

  nbItem = List_nb_item(foundSubwins);
  if (nbItem == 0)
  {
    res = NULL;
  }
  else if (nbItem == 1)
  {
    /* index starts to 1 */
    res = List_item(foundSubwins, 1);
  }
  else
  {
    /* select the one whose middle is closer the point */
    int minDist = 0;
    int i;

    res = (sciPointObj *) List_item(foundSubwins, 1);
    minDist = getSqDistanceToCenter(res, xCoord, yCoord);

    for (i = 2; i <= nbItem; i++)
    {
      sciPointObj * curSubwin = (sciPointObj *) List_item(foundSubwins, i);
      int curDist = getSqDistanceToCenter(curSubwin, xCoord, yCoord);
      if (curDist < minDist)
      {
        res = curSubwin;
        minDist = curDist;
      }
    }
  }

  List_free(foundSubwins);

  return res;

}
/*--------------------------------------------------------------------------------*/
sciLegendPlace propertyNameToLegendPlace(const char * string)
{
	if ( strcmp(string, "in_upper_right" ) == 0 )
	{
		return SCI_LEGEND_IN_UPPER_RIGHT;
	}
	else if ( strcmp(string, "in_upper_left" ) == 0 )
	{
		return SCI_LEGEND_IN_UPPER_LEFT;
	}
	else if ( strcmp(string, "in_lower_right" ) == 0 )
	{
		return SCI_LEGEND_IN_LOWER_RIGHT;
	}
	else if ( strcmp(string, "in_lower_left" ) == 0 )
	{
		return SCI_LEGEND_IN_LOWER_LEFT;
	}
	else if ( strcmp(string, "out_upper_right" ) == 0 )
	{
		return SCI_LEGEND_OUT_UPPER_RIGHT;
	}
	else if ( strcmp(string, "out_upper_left" ) == 0 )
	{
		return SCI_LEGEND_OUT_UPPER_LEFT;
	}
	else if ( strcmp(string, "out_lower_right" ) == 0 )
	{
		return SCI_LEGEND_OUT_LOWER_RIGHT;
	}
	else if ( strcmp(string, "out_lower_left" ) == 0 )
	{
		return SCI_LEGEND_OUT_LOWER_LEFT;
	}
	else if ( strcmp(string, "upper_caption" ) == 0 )
	{
		return SCI_LEGEND_UPPER_CAPTION;
	}
	else if ( strcmp(string, "lower_caption" ) == 0 )
	{
		return SCI_LEGEND_LOWER_CAPTION;
	}
	else if ( strcmp(string, "by_coordinates" ) == 0 )
	{
		return SCI_LEGEND_BY_COORDINATES;
	}
	else {
		return (sciLegendPlace) 0;
	}
}
/*--------------------------------------------------------------------------------*/
