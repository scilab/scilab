/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
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
/* file: XsetXgetParameters.h                                             */
/* desc : list all the parameters which might be used in xset and xget    */
/*------------------------------------------------------------------------*/

#ifndef _XSET_XGET_PARAMETERS_H_
#define _XSET_XGET_PARAMETERS_H_

#define NUMSETFONC 35

static char * KeyTab_[NUMSETFONC + 1] = {
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
  "pixmap",
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
