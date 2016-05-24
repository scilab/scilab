/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010-2012 - Scilab Enterprises - Bruno JOFRET
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

/*======================================================================================*/
/* Copyright INRIA 2006                                                                 */
/* Desc   : Various functions to manipulate SubWindow objects                           */
/*======================================================================================*/

#ifndef __SCI_AXES_H__
#define __SCI_AXES_H__

#include "dynlib_graphics.h"
#include "BOOL.h"

/*-----------------------------------------------------------------------------*/

/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
GRAPHICS_IMPEXP BOOL checkRedrawing(void);

/**
 * Enumeration used to specify the title place relative to parent subwindow
 * Matches the MVC Legend object's LegendLocation enum.
 */
typedef enum
{
    /** */
    SCI_LEGEND_POSITION_UNSPECIFIED = -1,
    /** */
    SCI_LEGEND_IN_UPPER_RIGHT = 0,
    /** */
    SCI_LEGEND_IN_UPPER_LEFT = 1,
    /** */
    SCI_LEGEND_IN_LOWER_RIGHT = 2,
    /** */
    SCI_LEGEND_IN_LOWER_LEFT = 3,
    /** */
    SCI_LEGEND_OUT_UPPER_RIGHT = 4,
    /** */
    SCI_LEGEND_OUT_UPPER_LEFT = 5,
    /** */
    SCI_LEGEND_OUT_LOWER_RIGHT = 6,
    /** */
    SCI_LEGEND_OUT_LOWER_LEFT = 7,
    /** */
    SCI_LEGEND_UPPER_CAPTION = 8,
    /** */
    SCI_LEGEND_LOWER_CAPTION = 9,
    /** */
    SCI_LEGEND_BY_COORDINATES = 10
}  /** */
sciLegendPlace;

/**
 * Convert property name into a sciLegendPlace
 */
GRAPHICS_IMPEXP sciLegendPlace propertyNameToLegendPlace(const char * string);

/*
 * Converts a boolean log flag to the character format
 */
GRAPHICS_IMPEXP char getTextLogFlag(int logFlag);

/*
 * Converts a character log flag to the equivalent boolean
 */
GRAPHICS_IMPEXP int getBooleanLogFlag(char logFlag);

/*-----------------------------------------------------------------------------*/
#endif /*__SCI_AXES_H_*/
