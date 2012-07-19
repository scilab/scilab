/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
 *    This file contains all structures definitions used for New Graphics mode.
 --------------------------------------------------------------------------*/
#ifndef __SCI_OBJECT_STRUCTURE__
#define __SCI_OBJECT_STRUCTURE__

#define NUMCOLORS_SCI 32

/** @name Text Alignment
 * Text alignement enum.
 */
typedef enum {
    ALIGN_NONE = 0,
    ALIGN_LEFT = 1,
    ALIGN_CENTER = 2,
    ALIGN_RIGHT = 3
}
sciTextAlignment;

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

typedef enum
{
    SCI_FAC3D = 0,
    SCI_PLOT3D = 1,
    SCI_CONTOUR,
    SCI_PARAM3D,
    SCI_PARAM3D1
}
sciTypeOf3D;

#endif /*__SCI_OBJECT_STRUCTURE__ */
