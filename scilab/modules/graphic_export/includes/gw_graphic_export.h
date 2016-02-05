/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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

#ifndef __GW_GRAPHIC_EXPORT__
#define __GW_GRAPHIC_EXPORT__
/*--------------------------------------------------------------------------*/
#include "dynlib_graphic_export.h"
/*--------------------------------------------------------------------------*/
GRAPHIC_EXPORT_IMPEXP int gw_graphic_export(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/

GRAPHIC_EXPORT_IMPEXP int sci_xs2bmp(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2eps(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2gif(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2jpg(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2pdf(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2png(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2ppm(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2svg(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2ps(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_driver(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xinit(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xend(char* fname, void* pvApiCtx);
GRAPHIC_EXPORT_IMPEXP int sci_xs2emf(char* fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_GRAPHIC_EXPORT__ */
/*--------------------------------------------------------------------------*/
