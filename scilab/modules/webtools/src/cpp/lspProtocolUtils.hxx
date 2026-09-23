/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Declares protocol and source-location utilities shared by LSP handlers.

#ifndef __LSP_PROTOCOL_UTILS_HXX__
#define __LSP_PROTOCOL_UTILS_HXX__

#include <string>
#include "nlohmann_json.hxx"

struct DocumentRange;

constexpr int InvalidParams = -32602;
constexpr int ServerCancelled = -32802;
nlohmann::json response(const nlohmann::json& query);
std::string getWebtoolsEtcFilePath(const std::string& filename);
bool is_single_quote_string_start(const std::string& line, size_t column);
nlohmann::json document_range(const DocumentRange& range);

#endif /* !__LSP_PROTOCOL_UTILS_HXX__ */
