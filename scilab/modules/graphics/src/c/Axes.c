/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "AxesModel.h"
#include "CurrentSubwin.h"

#include "deleteGraphicObject.h"
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing(void)
{
    return isAxesRedrawing(getCurrentSubWin());
}
/*--------------------------------------------------------------------------------*/
sciLegendPlace propertyNameToLegendPlace(const char * string)
{
    if (strcmp(string, "in_upper_right") == 0)
    {
        return SCI_LEGEND_IN_UPPER_RIGHT;
    }
    else if (strcmp(string, "in_upper_left") == 0)
    {
        return SCI_LEGEND_IN_UPPER_LEFT;
    }
    else if (strcmp(string, "in_lower_right") == 0)
    {
        return SCI_LEGEND_IN_LOWER_RIGHT;
    }
    else if (strcmp(string, "in_lower_left") == 0)
    {
        return SCI_LEGEND_IN_LOWER_LEFT;
    }
    else if (strcmp(string, "out_upper_right") == 0)
    {
        return SCI_LEGEND_OUT_UPPER_RIGHT;
    }
    else if (strcmp(string, "out_upper_left") == 0)
    {
        return SCI_LEGEND_OUT_UPPER_LEFT;
    }
    else if (strcmp(string, "out_lower_right") == 0)
    {
        return SCI_LEGEND_OUT_LOWER_RIGHT;
    }
    else if (strcmp(string, "out_lower_left") == 0)
    {
        return SCI_LEGEND_OUT_LOWER_LEFT;
    }
    else if (strcmp(string, "upper_caption") == 0)
    {
        return SCI_LEGEND_UPPER_CAPTION;
    }
    else if (strcmp(string, "lower_caption") == 0)
    {
        return SCI_LEGEND_LOWER_CAPTION;
    }
    else if (strcmp(string, "by_coordinates") == 0)
    {
        return SCI_LEGEND_BY_COORDINATES;
    }
    else
    {
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

