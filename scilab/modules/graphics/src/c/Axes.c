/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
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
#include "Axes.h"
#include "DestroyObjects.h"
#include "InitObjects.h"
#include "SetProperty.h"
#include "axesScale.h"
#include "CurrentSubwin.h"

#include "GetProperty.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "AxesModel.h"
#include "CurrentSubwin.h"

#include "deleteGraphicObject.h"
/*--------------------------------------------------------------------------------*/
static void initSubWinAngles(char * pSubWinUID);
static void initSubWinBounds(char * pSubWinUID);
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position ) */
static void reinitSubWin(char * pSubWinUID)
{
  int visible = 0;
  int firstPlot = 0;
  int axisLocation = 0;
  char *labelUID = NULL;
  int iChildrenCount = 0;
  int *piChildrenCount = &iChildrenCount;
  int i = 0;
  char **pstChildrenUID = NULL;

  /* Deletes the Axes' children */
  getGraphicObjectProperty(pSubWinUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

  if (iChildrenCount != 0)
  {
      getGraphicObjectProperty(pSubWinUID, __GO_CHILDREN__, jni_string_vector, (void **) &pstChildrenUID);

      for (i = 0 ; i < iChildrenCount ; ++i)
      {
          deleteGraphicObject(pstChildrenUID[i]);
      }
  }

  initSubWinBounds(pSubWinUID);

  labelUID = initLabel(pSubWinUID);
  setGraphicObjectProperty(pSubWinUID, __GO_TITLE__, labelUID, jni_string, 1);

  labelUID = initLabel(pSubWinUID);
  setGraphicObjectProperty(pSubWinUID, __GO_X_AXIS_LABEL__, labelUID, jni_string, 1);

  labelUID = initLabel(pSubWinUID);
  setGraphicObjectProperty(pSubWinUID, __GO_Y_AXIS_LABEL__, labelUID, jni_string, 1);

  labelUID = initLabel(pSubWinUID);
  setGraphicObjectProperty(pSubWinUID, __GO_Z_AXIS_LABEL__, labelUID, jni_string, 1);

/* bottom */
  axisLocation = 0;
  setGraphicObjectProperty(pSubWinUID, __GO_X_AXIS_LOCATION__, &axisLocation, jni_int, 1);
  /* left */
  axisLocation = 4;
  setGraphicObjectProperty(pSubWinUID, __GO_Y_AXIS_LOCATION__, &axisLocation, jni_int, 1);

  visible = 1;
  setGraphicObjectProperty(pSubWinUID, __GO_VISIBLE__, &visible, jni_bool, 1);
  firstPlot = 1;
  setGraphicObjectProperty(pSubWinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

  initSubWinAngles(pSubWinUID);
}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
static void initSubWinAngles(char * pSubWinUID)
{
    int iViewType = 0;
    int* piViewType = &iViewType;
    double* rotationAngles = NULL;
    char* axesModelUID = (char*)getAxesModel();

    getGraphicObjectProperty(axesModelUID, __GO_VIEW__, jni_int, (void **) &piViewType);
    setGraphicObjectProperty(pSubWinUID, __GO_VIEW__, &iViewType, jni_int, 1);

    getGraphicObjectProperty(axesModelUID, __GO_ROTATION_ANGLES__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(pSubWinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    getGraphicObjectProperty(axesModelUID, __GO_ROTATION_ANGLES_3D__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(pSubWinUID, __GO_ROTATION_ANGLES_3D__, rotationAngles, jni_double_vector, 2);
}

/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
static void initSubWinBounds(char * pSubWinUID)
{
    double* dataBounds = NULL;
    double* realDataBounds = NULL;
    char* axesModelUID = (char*)getAxesModel();

    getGraphicObjectProperty(axesModelUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);
    setGraphicObjectProperty(pSubWinUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    getGraphicObjectProperty(axesModelUID, __GO_REAL_DATA_BOUNDS__, jni_double_vector, (void **)&realDataBounds);
    setGraphicObjectProperty(pSubWinUID, __GO_REAL_DATA_BOUNDS__, realDataBounds, jni_double_vector, 6);
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void )
{
    int iAutoClear = 0;
    int* piAutoClear = &iAutoClear;

    //  nbCheckRedraw++;
    //  fprintf(stderr, "[DEBUG] checkRedrawing : %d\n", nbCheckRedraw);
    char* pstSubWinID = (char*)getCurrentSubWin();

    getGraphicObjectProperty(pstSubWinID, __GO_AUTO_CLEAR__, jni_bool, (void **)&piAutoClear);

    if (iAutoClear)
    {
        reinitSubWin(pstSubWinID);
        return TRUE;
    }

    return FALSE;
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
		return SCI_LEGEND_POSITION_UNSPECIFIED;
	}
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a boolean log flag to the character format
 */
char getTextLogFlag(int logFlag)
{
    if (logFlag)
    {
        return 'l';
    }
    else
    {
        return 'n';
    }
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a character log flag to the equivalent boolean
 */
int getBooleanLogFlag(char logFlag)
{
    if (logFlag == 'l')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*--------------------------------------------------------------------------------*/

