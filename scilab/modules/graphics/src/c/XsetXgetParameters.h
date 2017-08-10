/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: XsetXgetParameters.h                                             */
/* desc : list all the parameters which might be used in xset and xget    */
/*------------------------------------------------------------------------*/

#ifndef _XSET_XGET_PARAMETERS_H_
#define _XSET_XGET_PARAMETERS_H_

#define NUMSETFONC 35

static char * KeyTab_[NUMSETFONC + 1] =
{
    "alufunction",
    "auto clear",
    "background",
    "clipoff",
    "clipping",
    "clipgrf",
    "color",
    "colormap",
    "dashes",
    "default",
    "figure",
    "font",
    "font size",
    "foreground",
    "fpf",
    "hidden3d",
    "lastpattern",
    "line mode",
    "line style",
    "mark",
    "mark size",
    "old_style",
    "pattern",
    "thickness",
    "use color",
    "viewport",
    "wdim",
    "white",
    "window",
    "wpdim",
    "wpos",
    "wresize",
    "wshow",
    "wwpc",
    " ", /* added */
};

#endif /* _XSET_XGET_PARAMETERS_H_ */
