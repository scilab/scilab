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
#include "math_graphics.h"
#include "Format.h"
#include "HandleManagement.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "math.h" /* fabs, floor, log10, pow */
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

//#include "../../../tclsci/includes/GedManagement.h"

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
  double* bounds = NULL;
  int iLogFlag = 0;
  int *piLogFlag = &iLogFlag;

  getGraphicObjectProperty(pSubWinUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&piZoomEnabled);

  if (iZoomEnabled)
  {
      getGraphicObjectProperty(pSubWinUID, __GO_ZOOM_BOX__, jni_double_vector, (void **)&bounds);
  }
  else
  {
      getGraphicObjectProperty(pSubWinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&bounds);
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

  getGraphicObjectProperty(pSubWinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);

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

  getGraphicObjectProperty(pSubWinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);

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

  getGraphicObjectProperty(pSubWinUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);

  if (iLogFlag == 1)
  {
    if ( sciGetLogExponent( *zmin, *zmax, zmin, zmax ) != 0 )
    {
      sciprint(_("Warning: Can't use Log on Z-axis zmin is negative.\n"));
    }
  }

}
