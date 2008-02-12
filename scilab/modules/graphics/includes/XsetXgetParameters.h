/*------------------------------------------------------------------------*/
/* file: XsetXgetParameters.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
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
