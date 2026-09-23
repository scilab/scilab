/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __DAP_INTERFACE_H__
#define __DAP_INTERFACE_H__

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "dynlib_webtools.h"

#ifdef __cplusplus
extern "C"
{
#endif

WEBTOOLS_IMPEXP void dapStartTcp(int port, bool spawned);
WEBTOOLS_IMPEXP void dapShutdown(void);
WEBTOOLS_IMPEXP void dapStartWs(int port, const char* token, bool spawned);

#ifdef __cplusplus
}
#endif

#endif /* !__DAP_INTERFACE_H__ */
