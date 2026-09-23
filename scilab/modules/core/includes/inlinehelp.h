/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __INLINEHELP_H__
#define __INLINEHELP_H__

#include <wchar.h>
#include "dynlib_core.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Retrieve the inline help content associated with a help key.
 * C-callable wrapper around the C++ inlineHelp() function.
 */
CORE_IMPEXP int getInlineHelp(const wchar_t* key, wchar_t** content, BOOL* isScilab);

/**
 * Load (and cache) the XSL stylesheet used to transform help pages into text.
 * C-callable wrapper around the C++ loadStyleSheet() function.
 */
CORE_IMPEXP BOOL loadInlineHelpStyleSheet(const char* sheetPath);

#ifdef __cplusplus
}
#endif

#endif /* !__INLINEHELP_H__ */
