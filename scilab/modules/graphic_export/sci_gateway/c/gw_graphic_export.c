/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "BOOL.h"
#include "gw_graphic_export.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
static BOOL loadedDepVectorialExport = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
  {NULL, "xs2bmp"}, //sci_xs2bmp
  {NULL, "xs2gif"}, //sci_xs2gif
  {NULL, "xs2jpg"}, //sci_xs2jpg
  {NULL, "xs2png"}, //sci_xs2png
  {NULL, "xs2ppm"}, //sci_xs2ppm
  {NULL, "xs2eps"}, //sci_xs2eps
  {NULL, "xs2pdf"}, //sci_xs2pdf
  {NULL, "xs2svg"}, //sci_xs2svg
  {NULL, "xs2ps"}   //sci_xs2ps
};
/*--------------------------------------------------------------------------*/
int gw_graphic_export(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
