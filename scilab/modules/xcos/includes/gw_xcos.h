
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2010 - Allan CORNET <allan.cornet@scilab.org>
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

#ifndef __GW_XCOS__
#define __GW_XCOS__
/*--------------------------------------------------------------------------*/
#include "dynlib_xcos.h"
/*--------------------------------------------------------------------------*/
XCOS_IMPEXP int sci_xcosNotify(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_warnBlockByUID(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_closeXcosFromScilab(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosDiagramOpen(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosDiagramClose(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalLoad(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalCategoryAdd(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalDelete(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalMove(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalEnable(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalDisable(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosConfigureXmlFile(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosAddToolsMenu(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosUpdateBlock(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_loadXcos(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosSimulationStarted(char *fname, void *pvApiCtx);
XCOS_IMPEXP int sci_xcosPalGet(char *fname, void *pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_XCOS__ */
/*--------------------------------------------------------------------------*/
