/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
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

#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "PloEch.h"
#include "sciprint.h"
#include "ObjectSelection.h"
#include "math_graphics.h"
#include "Format.h"
#include "HandleManagement.h"


#include "BuildDrawingObserver.h"
#include "DrawingBridge.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "math.h" /* fabs, floor, log10, pow */
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

//#include "../../../tclsci/includes/GedManagement.h"


#define round(a)  (int)(((a)<0.0)?(a)-.5:(a)+.5)

/* Specifies the ticks labels buffer size */
#define LABEL_BUFFER_LENGTH    128


static void computeCoordinatesFromLogExponents(double* coordinates, int numCoordinates);
static void printLabels(char** stringVector, double* ticksLocations, int numTicks, BOOL logFlag);

static BOOL subwinNeedsDisplay(sciPointObj * pSubwin);



void sciRedrawFigure( void )
{
// ???
//  sciDrawObj(sciGetCurrentFigure ());
}


void sciClearFigure(sciPointObj * pFigure)
{
  destroyGraphicsSons(pFigure);

  /* recreate the subwindow */
  //createFirstSubwin(pFigure);

  forceHierarchyRedraw(pFigure);
}

void sciXclear()
{
// ???
#if 0
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
#endif
}


/* get the displayed bounds of an axis */
void sciGetDisplayedBounds( char * pSubWinUID,
                            double      * xmin   ,
                            double      * xmax   ,
                            double      * ymin   ,
                            double      * ymax   ,
                            double      * zmin   ,
                            double      * zmax    )
{
    /*****************************************************************
     * get initial bounds
   *****************************************************************/

  int iZoomEnabled = 0;
  int* piZoomEnabled = &iZoomEnabled;
  double* bounds;
  int iLogFlag = 0;
  int *piLogFlag = &iLogFlag;

  getGraphicObjectProperty(pSubWinUID, __GO_ZOOM_ENABLED__, jni_bool, &piZoomEnabled);

  if (iZoomEnabled)
  {
      getGraphicObjectProperty(pSubWinUID, __GO_ZOOM_BOX__, jni_double_vector, &bounds);
  }
  else
  {
      getGraphicObjectProperty(pSubWinUID, __GO_DATA_BOUNDS__, jni_double_vector, &bounds);
  }

  *xmin = bounds[0];
  *xmax = bounds[1];
  *ymin = bounds[2];
  *ymax = bounds[3];
  *zmin = bounds[4];
  *zmax = bounds[5];

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling X axis
   *****************************************************************/

  getGraphicObjectProperty(pSubWinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);

  if (iLogFlag == 1)
  {
    if ( sciGetLogExponent( *xmin, *xmax, xmin, xmax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on X-axis xmin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Y axis
   *****************************************************************/

  getGraphicObjectProperty(pSubWinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);

  if (iLogFlag == 1)
  {
    if ( sciGetLogExponent( *ymin, *ymax, ymin, ymax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Y-axis ymin is negative.\n"));
    }
  }

  /*****************************************************************
   * modify  bounds and aaint  if using log scaling Z axis
   *****************************************************************/

  getGraphicObjectProperty(pSubWinUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);

  if (iLogFlag == 1)
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
/*
 * This function has been adapted to the MVC framework.
 */
BOOL sci_update_frame_bounds_2d(char *pobjUID)
{
  double xmax, xmin, ymin, ymax, zmin, zmax ;
  double hx,hy,hx1,hy1;
  int i;

  double FRect[4],WRect[4],ARect[4];
  char logscale[2];

  /* Temp variables only used when called from update_specification_bounds */
  /* to know if we have to redraw all the figure */

  double* previousDataBounds;
  double updatedDataBounds[6];
  int logFlags[2];

  int tightLimits = 0;
  int *piTightLimits = &tightLimits;
  int isoview = 0;
  int * piIsoView = &isoview;

  /* in order to determine whether or not the bounds have changed... */
  int nbsubtics[2];
  int nbgrads[2];
  int updatedNbsubtics[2];

  /* Temporary variables for ticks computation */
  double xGrads[20];
  double yGrads[20];

  int updatedNxgrads;
  int updatedNygrads;

  int nbxsubticks = 0;
  int *piNbXSubticks = &nbxsubticks;
  int nbysubticks = 0;
  int *piNbYSubticks = &nbysubticks;

  int iLogFlag = 0;
  int *piLogFlag = &iLogFlag;

  /* Used to print labels */
  char** stringVector;

  int* tmp;
  /* End of Temp variables */

  /* Get the initial data bounds, number of ticks and number of subticks */

  /* Formerly FRect */
  getGraphicObjectProperty(pobjUID, __GO_REAL_DATA_BOUNDS__, jni_double_vector, &previousDataBounds);

  getGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);
  nbsubtics[0] = nbxsubticks;
  getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);
  nbsubtics[1] = nbysubticks;

  getGraphicObjectProperty(pobjUID, __GO_X_AXIS_NUMBER_TICKS__, jni_int, &piNbXSubticks);
  nbgrads[0] = nbxsubticks;
  getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int, &piNbYSubticks);
  nbgrads[1] = nbysubticks;

  /* Sets the z-axis number of ticks to 0 */
  setGraphicObjectProperty(pobjUID, __GO_Z_AXIS_TICKS_LOCATIONS__, NULL, jni_double_vector, 0);

  sciGetDisplayedBounds( pobjUID, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax ) ;

  /* _grad Init. to 0. */
  for(i = 0; i < 20; i++)
  {
    xGrads[i] = 0.0;
    yGrads[i] = 0.0;
  }

  /*
   * The code generating labels should be moved inside the Java Model.
   * Ticks labels were previously generated in the C++ Renderer module
   * (see AutomaticTicksComputer and AutoLogTicksComputer)
   * using the C-computed locations.
   * This is temporarily done here in order to get updated labels according
   * to the data bounds.
   */

  getGraphicObjectProperty(pobjUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);
  logFlags[0] = iLogFlag;

  /* x-axis */
  if (logFlags[0] == 0)
  {
      getGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);

    TheTicks(&xmin, &xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobjUID,updatedNxgrads,'n',NULL, nbxsubticks);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, 0);
  }
  else
  {
      getGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);

    GradLog(xmin, xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobjUID, updatedNxgrads, 'l', xGrads, 0);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(xGrads, updatedNxgrads);
  }

  setGraphicObjectProperty(pobjUID, __GO_X_AXIS_TICKS_LOCATIONS__, xGrads, jni_double_vector, updatedNxgrads);
  setGraphicObjectProperty(pobjUID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNxgrads);

  setGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, &nbxsubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNxgrads);


  getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);
  logFlags[1] = iLogFlag;

  /* y-axis */
  if (logFlags[1] == 0)
  {
      getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);

    TheTicks(&ymin, &ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobjUID,updatedNygrads,'n',NULL, nbysubticks);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, 0);
  }
  else
  {
      getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);

    GradLog(ymin, ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobjUID, updatedNygrads, 'l', yGrads, 0);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(yGrads, updatedNygrads);
  }

  setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_TICKS_LOCATIONS__, yGrads, jni_double_vector, updatedNygrads);
  setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNygrads);

  setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, &nbysubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNygrads);


  getGraphicObjectProperty(pobjUID, __GO_TIGHT_LIMITS__, jni_bool, &piTightLimits);

  if (tightLimits == FALSE)
  {
    xmin = xGrads[0];
    xmax = xGrads[updatedNxgrads - 1];
    ymin = yGrads[0];
    ymax = yGrads[updatedNygrads - 1];
  }

  /*****************************************************************
   * modify  bounds if  isoview requested
   *****************************************************************/
  getGraphicObjectProperty(pobjUID, __GO_ISOVIEW__, jni_bool, &piIsoView);

  /*
   * Depending on the high-level function calling update_specification_bounds, this section
   * may not be useful. Considering for example Plo2dn, the isoview property is initialized within
   * the strflag2properties function, called from Plo2dn after update_specification_bounds has returned.
   * Furthermore, in the previous version of the renderer, the C++ part recomputes the "real" data bounds
   * and sets them again.
   */
  if (isoview)
  {
    int wdim[2];
    char* parentId;
    int* figureDimensions;

    getGraphicObjectProperty(pobjUID, __GO_PARENT__, jni_string, &parentId);

    /*
     * Window width and height are currently 0 (default Figure values)
     * To be implemented within the MVC (by copying the Figure model's values)
     */
    getGraphicObjectProperty(parentId, __GO_SIZE__, jni_int_vector, &figureDimensions);
    wdim[0] = figureDimensions[0];
    wdim[1] = figureDimensions[1];

#if 0
    wdim[0] = sciGetWindowWidth(sciGetParentFigure(pobj));
    wdim[1] = sciGetWindowHeight(sciGetParentFigure(pobj));
#endif

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

    /* x-axis */
    if (logFlags[0] == 0)
    {
      TheTicks(&xmin, &xmax, xGrads, &updatedNxgrads, FALSE);
      /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
      nbxsubticks = ComputeNbSubTics(pobjUID, updatedNxgrads, 'n', NULL, nbxsubticks);

      stringVector = createStringArray(updatedNxgrads);
      printLabels(stringVector, xGrads, updatedNxgrads, FALSE);
    }
    else{ /* log. case */
      GradLog(xmin, xmax, xGrads, &updatedNxgrads, FALSE);
      nbxsubticks = ComputeNbSubTics(pobjUID, updatedNxgrads, 'l',xGrads,0);

      stringVector = createStringArray(updatedNxgrads);
      printLabels(stringVector, xGrads, updatedNxgrads, TRUE);

      /* Transform back exponents to non-log coordinates to set positions */
      computeCoordinatesFromLogExponents(xGrads, updatedNxgrads);
    }

    setGraphicObjectProperty(pobjUID, __GO_X_AXIS_TICKS_LOCATIONS__, xGrads, jni_double_vector, updatedNxgrads);
    setGraphicObjectProperty(pobjUID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNxgrads);

    setGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, &nbxsubticks, jni_int, 1);

    destroyStringArray(stringVector, updatedNxgrads);

    /* y-axis */
    if (logFlags[1] == 0)
    {
      TheTicks(&ymin, &ymax, yGrads, &updatedNygrads, FALSE);
      /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
      nbysubticks = ComputeNbSubTics(pobjUID, updatedNygrads, 'n', NULL, nbysubticks);

      stringVector = createStringArray(updatedNygrads);
      printLabels(stringVector, yGrads, updatedNygrads, FALSE);
    }
    else{ /* log. case */
      GradLog(ymin, ymax, yGrads, &updatedNygrads, FALSE);
      nbysubticks = ComputeNbSubTics(pobjUID, updatedNygrads, 'l',yGrads,0);

      stringVector = createStringArray(updatedNygrads);
      printLabels(stringVector, yGrads, updatedNygrads, TRUE);

      /* Transform back exponents to non-log coordinates to set positions */
      computeCoordinatesFromLogExponents(yGrads, updatedNygrads);
    }

    setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_TICKS_LOCATIONS__, yGrads, jni_double_vector, updatedNygrads);
    setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNygrads);

    setGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, &nbysubticks, jni_int, 1);

    destroyStringArray(stringVector, updatedNygrads);

    /* END ISO if */
  }

  /*****************************************************************
   * set the actual bounds in the Axes object
   *****************************************************************/

  updatedDataBounds[0] = xmin;
  updatedDataBounds[1] = xmax;
  updatedDataBounds[2] = ymin;
  updatedDataBounds[3] = ymax;
  updatedDataBounds[4] = zmin;
  updatedDataBounds[5] = zmax;

  setGraphicObjectProperty(pobjUID, __GO_REAL_DATA_BOUNDS__, updatedDataBounds, jni_double_vector, 6);

  for(i=0;i<4;i++)
  {
    if (updatedDataBounds[i] != previousDataBounds[i])
    {
      return TRUE;
      break;
    }
  }

  getGraphicObjectProperty(pobjUID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);
  updatedNbsubtics[0] = nbxsubticks;
  getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);
  updatedNbsubtics[1] = nbysubticks;

  for(i=0;i<2;i++)
  {
    if(nbsubtics[i] != updatedNbsubtics[i])
    {
      return TRUE;
      break;
    }
  }

  getGraphicObjectProperty(pobjUID, __GO_X_AXIS_NUMBER_TICKS__, jni_int, &piNbXSubticks);
  updatedNxgrads = nbxsubticks;
  getGraphicObjectProperty(pobjUID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int, &piNbYSubticks);
  updatedNygrads = nbysubticks;

  if(nbgrads[0] != updatedNxgrads) return TRUE;
  if(nbgrads[1] != updatedNygrads) return TRUE;

  return FALSE;
}

/**update_3dbounds -> renamed sci_update_frame_bounds_3d
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

  /* The initial data bounds to be compared with the updated ones at the end of this function */
  double* previousDataBounds;
  double updatedDataBounds[6];

  int logFlags[3];
  int tightLimits = 0;
  int *piTightLimits = &tightLimits;

  /* in order to determine whether or not the bounds have changed... */
  int nbsubtics[3];
  int nbgrads[3];
  int updatedNbsubtics[3];

  int iLogFlag = 0;
  int *piLogFlag = &iLogFlag;

  /* Temporary variables for ticks computation */
  double xGrads[20];
  double yGrads[20];
  double zGrads[20];

  int updatedNxgrads;
  int updatedNygrads;
  int updatedNzgrads;

  int nbxsubticks = 0;
  int *piNbXSubticks = &nbxsubticks;
  int nbysubticks = 0;
  int *piNbYSubticks = &nbysubticks;
  int nbzsubticks = 0;
  int *piNbZSubticks = &nbzsubticks;

  /* Used to print labels */
  char** stringVector;

  /* End of Temp variables */

  /* Formerly FRect */
  getGraphicObjectProperty(pobj->UID, __GO_REAL_DATA_BOUNDS__, jni_double_vector, &previousDataBounds);

  getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);
  nbsubtics[0] = nbxsubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);
  nbsubtics[1] = nbysubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, jni_int, &piNbZSubticks);
  nbsubtics[2] = nbzsubticks;

  getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_NUMBER_TICKS__, jni_int, &piNbXSubticks);
  nbgrads[0] = nbxsubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int, &piNbYSubticks);
  nbgrads[1] = nbysubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_NUMBER_TICKS__, jni_int, &piNbZSubticks);
  nbgrads[2] = nbzsubticks;


  sciGetDisplayedBounds( pobj, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax );

  /* _grad Init. to 0. */
  for(i=0;i<20;i++)
  {
      xGrads[i] = 0.0;
      yGrads[i] = 0.0;
      zGrads[i] = 0.0;
  }

  getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);
  logFlags[0] = iLogFlag;

  /*
   * The code generating labels should be moved inside the Java Model, which
   * is currently done here (see sci_update_frame_bounds_2d for the reasons why)
   */

  /* x-axis */
  if (logFlags[0] == 0)
  {
      getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);

    TheTicks(&xmin, &xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobj,updatedNxgrads,'n',NULL, nbxsubticks);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, 0);
  }
  else
  {
      getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);

    GradLog(xmin, xmax, &xGrads[0], &updatedNxgrads, FALSE);

    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    nbxsubticks = ComputeNbSubTics(pobj, updatedNxgrads, 'l', xGrads, 0);

    stringVector = createStringArray(updatedNxgrads);
    printLabels(stringVector, xGrads, updatedNxgrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(xGrads, updatedNxgrads);
  }

  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LOCATIONS__, xGrads, jni_double_vector, updatedNxgrads);
  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNxgrads);
  setGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, &nbxsubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNxgrads);

  /* y-axis */
  if (logFlags[1] == 0)
  {
      getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);

    TheTicks(&ymin, &ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobj,updatedNygrads,'n',NULL, nbysubticks);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, 0);
  }
  else
  {
    getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);

    GradLog(ymin, ymax, &yGrads[0], &updatedNygrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbysubticks = ComputeNbSubTics(pobj, updatedNygrads, 'l', yGrads, 0);

    stringVector = createStringArray(updatedNygrads);
    printLabels(stringVector, yGrads, updatedNygrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(yGrads, updatedNygrads);
  }

  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LOCATIONS__, yGrads, jni_double_vector, updatedNygrads);
  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNygrads);
  setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, &nbysubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNygrads);

  /* z-axis */
  if (logFlags[2] == 0)
  {
      getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, jni_int, &piNbZSubticks);

    TheTicks(&zmin, &zmax, &zGrads[0], &updatedNzgrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbzsubticks = ComputeNbSubTics(pobj,updatedNzgrads,'n',NULL, nbzsubticks);

    stringVector = createStringArray(updatedNzgrads);
    printLabels(stringVector, zGrads, updatedNzgrads, 0);
  }
  else
  {
      getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, jni_int, &piNbZSubticks);

    GradLog(zmin, zmax, &zGrads[0], &updatedNzgrads, FALSE);

    /* Nb of subtics computation and storage */
    /* F.Leray 07.10.04 */
    nbzsubticks = ComputeNbSubTics(pobj, updatedNzgrads, 'l', zGrads, 0);

    stringVector = createStringArray(updatedNzgrads);
    printLabels(stringVector, zGrads, updatedNzgrads, TRUE);

    /* Transform back exponents to non-log coordinates to set positions */
    computeCoordinatesFromLogExponents(zGrads, updatedNzgrads);
  }

  setGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_TICKS_LOCATIONS__, zGrads, jni_double_vector, updatedNzgrads);
  setGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, updatedNzgrads);
  setGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, &nbzsubticks, jni_int, 1);

  destroyStringArray(stringVector, updatedNzgrads);

  getGraphicObjectProperty(pobj->UID, __GO_TIGHT_LIMITS__, jni_bool, &piTightLimits);

  if (tightLimits == FALSE)
  {
      xmin = xGrads[0];
      xmax = xGrads[updatedNxgrads - 1];
      ymin = yGrads[0];
      ymax = yGrads[updatedNygrads - 1];
      zmin = zGrads[0];
      zmax = zGrads[updatedNzgrads - 1];
  }

  updatedDataBounds[0] = xmin;
  updatedDataBounds[1] = xmax;
  updatedDataBounds[2] = ymin;
  updatedDataBounds[3] = ymax;
  updatedDataBounds[4] = zmin;
  updatedDataBounds[5] = zmax;

  setGraphicObjectProperty(pobj->UID, __GO_REAL_DATA_BOUNDS__, updatedDataBounds, jni_double_vector, 6);

  for(i=0;i<6;i++)
  {
    if (updatedDataBounds[i] != previousDataBounds[i])
    {
      return TRUE;
      break;
    }
  }

  getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_SUBTICKS__, jni_int, &piNbXSubticks);
  updatedNbsubtics[0] = nbxsubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbYSubticks);
  updatedNbsubtics[1] = nbysubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_SUBTICKS__, jni_int, &piNbZSubticks);
  updatedNbsubtics[2] = nbzsubticks;

  for(i=0;i<3;i++)
  {
    if(nbsubtics[i] != updatedNbsubtics[i])
    {
      return TRUE;
      break;
    }
  }

  getGraphicObjectProperty(pobj->UID, __GO_X_AXIS_NUMBER_TICKS__, jni_int, &piNbXSubticks);
  updatedNxgrads = nbxsubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int, &piNbYSubticks);
  updatedNygrads = nbysubticks;
  getGraphicObjectProperty(pobj->UID, __GO_Z_AXIS_NUMBER_TICKS__, jni_int, &piNbZSubticks);
  updatedNzgrads = nbzsubticks;

  if(nbgrads[0] != updatedNxgrads) return TRUE;
  if(nbgrads[1] != updatedNygrads) return TRUE;
  if(nbgrads[2] != updatedNzgrads) return TRUE;

  return FALSE;
}






/** ComputeNbSubTics. Compute an appropriate number of subtics based on the number of major ticks,
 * the tick graduations, and the axes scaling (log or normal).  If auto subtics is off, then nbsubtics_input is returned.
 * If the axis scaling is linear and the major tick graduations lie along multiples of 10^n, where n is the greatest
 * integer such that 10^n is less than the graduation step size, then the number of subtics is selected to
 * place subtics also at multiples of 10^n. If major ticks are at every multiple of 10^n, a heuristic is used to
 * compute the number of subtics within a graduation. This heuristic assigns more subtics as the number of major
 * ticks decreases. If the graduations are not provided (grads = NULL), the heuristic is used.
 *
 * For logarithmic scaling, the number of subticks returned is 8 if the major tick graduations are at every factor of 10.
 * Subtics are suppressed (0 is returned) if the number of major ticks exceeds a fixed threshold. If major tick
 * graduations are separated by factors of 10^n (n>1) then the number of subtics is selected to place subtics
 * at factors of 10.
 *
 * (Update 8/24/2010 by Paul Griffiths to resolve poor choice of subtics. See bug#6686.)
 *
 * @param pobjUID UID of an Axes object
 * @param nbtics Number of major ticks.
 * @param logflag Character value of 'l' or 'n' that indicates log or normal axis scaling, respectively.
 * @param grads Value of major tick graduations. May be NULL.
 * @param nbsubtics_input Value returned if auto subtics feature is disabled.
 * @return Number of subticks to display.
 */
int ComputeNbSubTics(char * pobjUID, int nbtics, char logflag, const double * grads, int nbsubtics_input)
{
  /* Parameters for calculating the number of subtics */
  const int subticsval_len = 13; /* <--- MUST MATCH LENGTH OF subticsval */
  /** Lookup-table Heuristic for number of subtics based on the number of tics */
  const int subticsval[] =      {0,0,19,9,7,4,4,3,2,2,1 ,1, 0};
  const int dec_subticsval[] =  {0,0, 9,9,4,4,4,4,1,1,1 ,1, 0};
  /** Maximum subtics beyond the heuristic when fitting subtics along integer intervals of base 10. */
  //const int max_extra_tics = 2;
  /** Threshold remainder used to decide if one number is a mulitple of the other. */
  const double mult_thrsh = 1e-6;
  /** Threshold number of major ticks above which subtics are supressed in logarithmic-scaling. */
  const int lognbtics_thrsh  = 7;

  double grads_diff;        /* Used for computing spacing between major ticks. */
  int nbtics_safe = nbtics; /* nbtics clamped to the range 0 to subticsval_len-1. Safe as an index into subticsval. */

#if 0
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (pobj);
#endif

  int iAutoSubticks = 0;
  int* piAutoSubticks = &iAutoSubticks;

  if (nbtics_safe < 0 )
  {
    nbtics_safe = 0;
  }
  else if (nbtics_safe >= subticsval_len)
  {
    nbtics_safe = subticsval_len - 1;
  }

  if(logflag =='l')
  {
    /* If tics are at every power of 10, then return 8 subtics
     * If tics are at every 10^n (n>1), then place subtics at powers of 10. */

    /* Without provided graduations, suggest 8. */
    if( grads == NULL )
    {
      return 8; /* This may or may not be a good value but it maintains this function's old behavior. */
    }

    /* We need at least two tics to be able to place subtics. */
    if( nbtics < 2 )
    {
      return 0;
    }

    /* Compute average. (Note that nbtics >= 2.) */
    grads_diff = (grads[nbtics-1] - grads[0] ) / (nbtics-1);
    grads_diff = fabs( grads_diff );  // Make into a positive difference.

    /* Check that graduations are actually on powers of 10 (not just spaced by factors of 10) */
    if( fabs( round( grads[0] ) - grads[0] ) >=  mult_thrsh )
    {
      return 0; /* Not on powers of 10. Suppress subtics. */
    }

    /* If grads_diff is nearly 1.0 (i.e. major graduations are at every factor of 10)
     * then return 8 unless there are too many major ticks. */
    if( fabs(grads_diff - 1.0) < mult_thrsh )
    {
      if( nbtics < lognbtics_thrsh )
      {
         return 8; /* show logarithmic subdivision */
      }
      else
      {
        return 0; /* Too many major ticks -- suppress subtics. */
      }
    }
    else
    {
      /* Try to place subtics at each factor 10 */
      int nbsubtics = round( grads_diff ) - 1;

      /* Use the heuristic to guide the maximum allowable subtics based on the
       * the number of major tics. */
      if( nbsubtics <= subticsval[ nbtics_safe ] )
      {
        return /* nbsubtics */ 0;  /* !! We have to suppress subtics because the subtic drawer assumes linear spacing between subtics. */
      }
      else
      {
        return 0;
      }
    }
  }
  else /* linear scaling case */
  {
      getGraphicObjectProperty(pobjUID, __GO_AUTO_SUBTICKS__, jni_bool, &piAutoSubticks);

#if 0
    if(ppsubwin->flagNax == FALSE) /* if auto subtics mode == ON */
#endif

    /* if auto subtics mode == ON */
    if (iAutoSubticks)
    {
      double intbase10 = 0.;
      /* Without graduations, use the heuristic */
      if( grads == NULL )
      {
        return subticsval[nbtics_safe];
      }

      /* We need at least two tics to be able to place subtics. */
      if( nbtics < 2 )
      {
        return 0;
      }

      /* Compute average graduation. (Note that nbtics >= 2.) */
      grads_diff = (grads[nbtics-1] - grads[0]) / (double)(nbtics-1);
      grads_diff = fabs( grads_diff );  /* Make into a positive difference. */

      /* Compute the largest integral power of 10 smaller than grads_diff. */
      intbase10 = pow(10, floor( log10(grads_diff) ) );

      /* Check if grads_diff is very close to a multiple of intbase10 -- if not, try one power of 10 lower */
      if( fabs( round( grads_diff/intbase10 ) - grads_diff/intbase10 ) >= mult_thrsh )
      {
        intbase10 /= 10;
      }

      if( fabs( round( grads_diff/intbase10 ) - grads_diff/intbase10 ) < mult_thrsh )
      {
        /* Place subtics at integer multiples of intbase10  */
        int intvls = round( grads_diff / intbase10 );

        /* If ticks are already placed at every multiple of intbase10, use the heuristic to subdivide. */
        if( intvls == 1 )
        {
          return dec_subticsval[ nbtics_safe ]; /* Use heuristic */
        }
        else
        {
          /* It may be necessary to use multiples of intbase10 to have an acceptable number of subtics.
           * Find a divisor from 1-9 such that nbsubtics+1 is evenly divisible and use the heuristic
           * plus max_extra_tics as a maximum number of subtics. */
          int d;

          for( d=1; d < 10; d++ )
          {
            int max_subtics = subticsval[ nbtics_safe ]; /* compute just once */

            /* Check divisibility by d. */
            if( intvls % d != 0 )
            {
              continue;
            }

            /* Compare nbsubtics with the heuristic subticsval[i].  */
            if( (intvls/d - 1) <= max_subtics )
            {
              return intvls/d - 1; /* subtics on intervals of intbase10 */
            }
          }

          return 0;  /* Cannot fit enough subtics to cover intervals using m*intbase10 (0<m<10).  */
        }
      }
      else /* Did not find a reasonable integer multiple of 10^n along which to place subtics. */
      {
        return 0;
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
void showPixmap(char *pFigureUID)
{
#if 0
  /* Hack here. Pixmap has same action as drawlater() */
  /* So we just draw the figure here to make it appear */
  if (sciGetPixmapMode(pFigure))
  {
    sciSetPixmapMode(pFigure, FALSE);
    sciDrawObj(pFigure);
    sciSetPixmapMode(pFigure, TRUE);
    sciDrawObj(pFigure);
  }
#endif
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
/*
 * This function has been only partially adapted to the MVC, due to sciisTextEmpty's prototype
 * having changed.
 * To be completed.
 */
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
    char* titleId;
    char* xLabelId;
    char* yLabelId;
    char* zLabelId;

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

    getGraphicObjectProperty(pSubwin->UID, __GO_TITLE__, jni_string, &titleId);
    getGraphicObjectProperty(pSubwin->UID, __GO_X_AXIS_LABEL__, jni_string, &xLabelId);
    getGraphicObjectProperty(pSubwin->UID, __GO_Y_AXIS_LABEL__, jni_string, &yLabelId);
    getGraphicObjectProperty(pSubwin->UID, __GO_Z_AXIS_LABEL__, jni_string, &zLabelId);

    /* Check that labels texts are empty */
    if (   !sciisTextEmpty(titleId)
        || !sciisTextEmpty(xLabelId)
        || !sciisTextEmpty(yLabelId)
        || !sciisTextEmpty(zLabelId))
    {
      return TRUE;
    }

    /* apparently no need to display the axes */
    return FALSE;
  }
}
/*---------------------------------------------------------------------------------*/
/*
 * Utility function which transforms back positions (logarithmic exponents) to non-log coordinates
 * Required to be able to display correct coordinate values in the console.
 */
static void computeCoordinatesFromLogExponents(double* coordinates, int numCoordinates)
{
    int i;
    double loginit = 1.0;

    for (i = 0; i < coordinates[0]; i++)
    {
      loginit *= 10.0;
    }

    for (i = 0; i < numCoordinates; i++)
    {
      coordinates[i] = loginit;
      loginit *= 10.0;
    }
}
/*---------------------------------------------------------------------------------*/
/*
 * Utility function which generates label strings
 * Its code is equivalent to that of the renderer module's TicksComputers' getTicksPosition
 * methods. It is temporarily located there, in order to be able to initialize labels strings,
 * and should reside within the Java Model.
 */
static void printLabels(char** stringVector, double* ticksLocations, int numTicks, BOOL logFlag)
{
    char labelBuffer[LABEL_BUFFER_LENGTH];
    char format[5];
    char* logPrefix = "10e";
    int i;
    int prefixLength;

    ChoixFormatE(format, ticksLocations[0], ticksLocations[numTicks-1],
        (ticksLocations[numTicks-1] - ticksLocations[0]) / (numTicks-1));

    if (logFlag == FALSE)
    {
        for (i = 0; i < numTicks; i++)
        {
            sprintf(labelBuffer, format, ticksLocations[i]);
            stringVector[i] = strdup(labelBuffer);
        }
    }
    else
    {
        prefixLength = strlen(logPrefix);

        for (i = 0; i < numTicks; i++)
        {
            strcpy(labelBuffer, logPrefix);

            sprintf(labelBuffer+prefixLength, format, ticksLocations[i]);
            stringVector[i] = strdup(labelBuffer);
        }
    }

}
/*---------------------------------------------------------------------------------*/
#undef round
