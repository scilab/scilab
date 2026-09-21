/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __INLINEHELP_HXX__
#define __INLINEHELP_HXX__

extern "C"
{
#include "dynlib_core.h"
}

#include <string>
#include <vector>

#include "struct.hxx"

CORE_IMPEXP int inlineHelp(const std::wstring& key, std::wstring& content, bool* isScilab = nullptr, bool withLinks = false);
CORE_IMPEXP int generate_inline_links(const std::wstring& lang, const std::wstring& path);
CORE_IMPEXP std::vector<std::wstring> listModuleXmlForLanguage(const std::string& lang);
CORE_IMPEXP void clearInlineHelpLinks();
CORE_IMPEXP int loadToolboxHelp(const std::wstring& path);

CORE_IMPEXP bool loadStyleSheet(const std::string& sciPath);
CORE_IMPEXP std::string buildAbsolutePath(const std::string& sciPath, std::string stored, bool* isScilab = nullptr);

void ensureLibxmlInitialized();

#endif /* !__INLINEHELP_HXX__ */
