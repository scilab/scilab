/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: axesScale.c                                                      */
/* desc : Contains functions to compute scale changes in a specific       */
/*        subwindow.                                                      */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "axesScale.h"
#include "math_graphics.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "SetJavaProperty.h"
#include "Interaction.h"
#include "JavaInteraction.h"
#include "HandleManagement.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------------*/
static void zoomSubwin(sciPointObj * pSubwin, int posX, int posY, int width, int height);
static void zoomFigure(sciPointObj * pFigure, int posX, int posY, int width, int height);
/*------------------------------------------------------------------------------*/
/**
 * Specify new zoom box for a subwin object.
 * @param subwinUID the subwin's identifier.
 * @param zoomRect vector [xMin, yMin, xMax, yMax].
 */
int sciZoom2D(char * subwinUID, const double zoomRect[4])
{
    double* zoomBox;

    // add Z scale to data bounds.
    getGraphicObjectProperty(subwinUID, __GO_DATA_BOUNDS__, jni_double_vector, &zoomBox);

    zoomBox[0] = zoomRect[0];
    zoomBox[1] = zoomRect[1];
    zoomBox[2] = zoomRect[2];
    zoomBox[3] = zoomRect[3];

    return sciZoom3D(subwinUID, zoomBox);
}
/*------------------------------------------------------------------------------*/
/**
 * Specify a new zoom box for a subwin object
 * @param subwinUID the subwin's identifier.
 * @param zoomBox vector [xMin, yMin, xMax, yMax, zMin, zMax].
 */
int sciZoom3D(char * subwinUID, const double zoomBox[6])
{
    BOOL status;
    int zoomEnabled = 1;

    // convert zoomBox to [xMin, xMax, yMin, yMax, zMin, zMax]
    double zoomBox3D[6];
    zoomBox3D[0] = zoomBox[0];
    zoomBox3D[1] = zoomBox[2];
    zoomBox3D[2] = zoomBox[1];
    zoomBox3D[3] = zoomBox[3];
    zoomBox3D[4] = zoomBox[4];
    zoomBox3D[5] = zoomBox[5];

    if (!checkDataBounds(subwinUID, zoomBox3D[0], zoomBox3D[1], zoomBox3D[2],
                         zoomBox3D[3], zoomBox3D[4], zoomBox3D[5]))
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(subwinUID, __GO_ZOOM_BOX__, zoomBox3D, jni_double_vector, 6);

    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"zoom_box");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(subwinUID, __GO_ZOOM_ENABLED__, &zoomEnabled, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------------*/
/**
 * get the zoom box to dispplay in Scilab for a sunwin object
 * @param[out] zoomBox [xMin, yMin, xMax, yMax, zMin, zMax];
 */
void sciGetZoom3D(sciPointObj * pObj, double zoomBox[6])
{
  double temp;

  // here we get [xMin, xMax, yMin, yMax, zMin, zMax]
  // we need to switch xMax and yMin
  sciGetZoomBox(pObj, zoomBox);
  temp = zoomBox[1];
  zoomBox[1] = zoomBox[2];
  zoomBox[2] = temp;
}
/*------------------------------------------------------------------------------*/
int sciZoomRect(sciPointObj * pObj, const double zoomRect[4])
{
  int status = SET_PROPERTY_ERROR;
  if (sciGetEntityType(pObj) == SCI_FIGURE)
  {
    status = sciFigureZoom2D(pObj, zoomRect);
  }
  else if (sciGetEntityType(pObj) == SCI_SUBWIN)
  {
    status = sciZoom2D(pObj, zoomRect);
  }

  /* redraw everything */
  if (status == SET_PROPERTY_SUCCEED)
  {
    //sciDrawObj(pObj);
  }

  return status;
}
/*------------------------------------------------------------------------------*/
int sciDefaultZoom2D(const double zoomRect[4])
{
  sciPointObj * curFigure = NULL;
  //startGraphicDataWriting();
  curFigure = sciGetCurrentFigure();
  //endGraphicDataWriting();

  return sciZoomRect(curFigure, zoomRect);
}
/*------------------------------------------------------------------------------*/
int sciFigureZoom2D(sciPointObj * figure, const double zoomRect[4])
{
  /* try to zoom on all the subwindows */
  sciSons * pSons = sciGetSons(figure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      int status = sciZoom2D(curObj, zoomRect);
      if (status == SET_PROPERTY_SUCCEED)
      {
        //forceRedraw(curObj);
      }
      else
      {
        return SET_PROPERTY_ERROR;
      }
    }
    pSons = pSons->pnext;
  }

  return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------------*/
/**
 * Try to zoom on a single subwindow using a selection area
 */
static void zoomSubwin(sciPointObj * pSubwin, int posX, int posY, int width, int height)
{
  if (sciJavaZoomRect(pSubwin, posX, posY, width, height))
  {
    /* subwindow has been zoomed */
    /* force zooming */
    sciSetZooming(pSubwin, TRUE);

    // window has changed
    //forceRedraw(pSubwin);
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Zoom a figure using an already computed selection area
 */
static void zoomFigure(sciPointObj * pFigure, int posX, int posY, int width, int height)
{
  /* try to zoom on all the subwindows */
  sciSons * pSons = sciGetSons(pFigure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      zoomSubwin(curObj, posX, posY, width, height);
    }
    pSons = pSons->pnext;
  }
}
/*------------------------------------------------------------------------------*/
void sciZoomObject(sciPointObj * pObj, int x1, int y1, int x2, int y2)
{
	/* convert found data to [x,y,w,h] */
  int x = Min(x1, x2);
  int y = Min(y1, y2);
  int w = Abs(x1 - x2);
  int h = Abs(y1 - y2);

	if (w == 0 || h == 0)
	{
		/* Zoom is not possible */
		return;
	}

	if (sciGetEntityType(pObj) == SCI_FIGURE)
	{
		zoomFigure(pObj, x, y, w, h);
	}
	else if (sciGetEntityType(pObj) == SCI_SUBWIN)
	{
		zoomSubwin(pObj, x, y, w, h);
	}
}
/*------------------------------------------------------------------------------*/
/**
 * Perform an interactive zoom (rectangular selection +  zoom)
 * @param pObj object on which the zoom will be applied.
 *             Might be a Figure or a Subwindow. If it is a figure the zoom
 *             is applied to the axes children of the figure
 */
void sciInteractiveZoom(sciPointObj * pObj)
{
	interactiveZoom(pObj);
}
/*------------------------------------------------------------------------------*/
/**
 * Perform a zoom rect (rectangular selection + zoom) on the current figure
 */
void sciDefaultInteractiveZoom(void)
{
  sciPointObj * curFigure;

 // startGraphicDataWriting();
  curFigure = sciGetCurrentFigure();
  //endGraphicDataWriting();


  /* zoom current figure */
  interactiveZoom(curFigure);
}
/*------------------------------------------------------------------------------*/
/**
 * Check if the following data bounds can be used as new data bounds for the subwin object
 * @param subwinUID the subwin's identifier.
 * @param the lower x bound.
 * @param the upper x bound.
 * @param the lower y bound.
 * @param the upper y bound.
 * @param the lower z bound.
 * @param the upper z bound.
 * @return TRUE if values can be used, false otherwise
 */
BOOL checkDataBounds(char * subwinUID, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax)
{
    char logFlags[3];

    sciGetLogFlags(subwinUID, logFlags);

    /* check if there is not an inf within the values */
    /* since this has not any meaning */
    if (    !finite(xMin) || !finite(xMax)
         || !finite(yMin) || !finite(yMax)
         || !finite(zMin) || !finite(zMax) )
    {
        Scierror(999, "Error : data bounds values must be finite.");
        return FALSE;
    }

    /* check if the bounds are correct */
    /* allows equality with bounds since it is working */
    if ( xMin > xMax || yMin > yMax || zMin > zMax )
    {
        Scierror(999, "Error : Min and Max values for one axis do not verify Min <= Max.\n");
        return FALSE;
    }

    /* check for logflags that values are greater than 0 */
    if (   ( logFlags[0] == 'l' && xMin <= 0.0 )
        || ( logFlags[1] == 'l' && yMin <= 0.0 )
        || ( logFlags[2] == 'l' && zMin <= 0.0 ) )
    {
        Scierror(999, "Error: Bounds on axis must be strictly positive to use logarithmic mode.\n" );
        return FALSE;
    }

    return TRUE;
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a single subwindow
 */
void sciUnzoomSubwin(sciPointObj * pSubwin)
{
  // We directly do a call to unzoomSubwin instead off path through the java code
  // See bug 4979
  // javaUnzoomSubwin(pSubwin);

  /* To be implemented using the MVC framework */
  sciPointObj * parentFigure = sciGetParentFigure(pSubwin);

  if( pSubwin == NULL || parentFigure == NULL )
  {
    return;
  }

/*
 * Deactivated since it performs drawing operations.
 * To be implemented.
 */
#if 0
  startFigureDataWriting(parentFigure);
    unzoomSubwin(pSubwin);
  endFigureDataWriting(parentFigure);
#endif

}
/*------------------------------------------------------------------------------*/
void unzoomSubwin(sciPointObj * pSubwin)
{
	int currentStatus;
  currentStatus = sciSetZooming(pSubwin, FALSE);
  if (currentStatus == 0)
  {
    /* redraw only if needed */
    //forceRedraw(pSubwin);
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom all the subwindows contained in a figure
 */
void sciUnzoomFigure(sciPointObj * figure)
{
  /* Copy subwins into the array */
  sciSons * pSons = sciGetSons(figure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      sciUnzoomSubwin(curObj);
    }
    pSons = pSons->pnext;
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Un zoom all the subwindow of a figure
 */
void sciUnzoomAll(void)
{
  sciPointObj * pFigure = NULL;

  //startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  //endGraphicDataWriting();

  /* unzoom current figure */
  sciUnzoomFigure(pFigure);

  //sciDrawObj(pFigure);
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a set of subwindows and figures
 * @param zoomedObjects array of figure and subwindow objects
 */
void sciUnzoomArray(sciPointObj * zoomedObjects[], int nbObjects)
{
  int i;
  /* list of parent figure to redraw */
  DoublyLinkedList * redrawnFigures = DoublyLinkedList_new();
  for (i = 0; i < nbObjects; i++)
  {
    sciPointObj * parentFigure = sciGetParentFigure(zoomedObjects[i]);

    if (sciGetEntityType(zoomedObjects[i]) == SCI_FIGURE)
    {
      /* Unzoom all subwindows of the figure */
      sciUnzoomFigure(zoomedObjects[i]);
    }
    else if (sciGetEntityType(zoomedObjects[i]) == SCI_SUBWIN)
    {
      /* Unzoom only figure */
      sciUnzoomSubwin(zoomedObjects[i]);
    }

    if (List_find(redrawnFigures, parentFigure) == NULL)
    {
      /* figure not already added for redraw */
      redrawnFigures = List_push(redrawnFigures, parentFigure);
    }
  }

  /* redraw only needed figures */
  while (!List_is_empty(redrawnFigures))
  {
    sciPointObj * curFigure = NULL;
    redrawnFigures = List_pop(redrawnFigures, (void**)&curFigure);
    //sciDrawObj(curFigure);
  }

  List_free(redrawnFigures);
}
/*--------------------------------------------------------------------------------*/
void updateSubwinScale(char * pSubwinUID)
{
    sciJavaUpdateSubwinScale(pSubwinUID);
}
/*------------------------------------------------------------------------------*/
void updateTextBounds(char * pTextUID)
{
    char* parentAxes = NULL;

    /* Update coordinate transformation if needed */
    getGraphicObjectProperty(pTextUID, __GO_PARENT_AXES__, jni_string, &parentAxes);
    updateSubwinScale(parentAxes);

    /* Compute the bounding box of the text */
    sciJavaUpdateTextBoundingBox(pTextUID);
}
/*------------------------------------------------------------------------------*/
