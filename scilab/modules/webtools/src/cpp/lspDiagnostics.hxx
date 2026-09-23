/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Exposes diagnostic notifications to the document synchronization handlers.

#ifndef __LSP_DIAGNOSTICS_HXX__
#define __LSP_DIAGNOSTICS_HXX__

#include <string>
#include <vector>
#include "nlohmann_json.hxx"
#include "documentAnalysisVisitor.hxx"

class RuntimeSnapshot;

nlohmann::json syntax_diagnostics_notification(const std::string& uri, const std::vector<std::string>& lines);
nlohmann::json syntax_diagnostics_notification(const std::string& uri,
    const std::vector<std::string>& lines, const DocumentAnalysis& analysis,
    const std::string& parseError, const RuntimeSnapshot* snapshot = nullptr);

#endif /* !__LSP_DIAGNOSTICS_HXX__ */
