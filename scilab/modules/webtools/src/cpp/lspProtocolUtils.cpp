/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Implements the small protocol and source-location utilities shared by
// multiple LSP feature handlers.

#include <cctype>
#include "lspProtocolUtils.hxx"
#include "documentAnalysisVisitor.hxx"
extern "C"
{
    #include "sci_malloc.h"
    #include "sci_path.h"
}

using json = nlohmann::json;

json response(const json& _query)
{
    json obj = R"({"jsonrpc": "2.0"})"_json;
    obj["id"] = _query.at("id");

    return obj;
}

std::string getWebtoolsEtcFilePath(const std::string& filename)
{
    char* sci = getSCI();
    if (sci == nullptr)
    {
        return filename;
    }

    std::string path = std::string(sci) + "/modules/webtools/etc/" + filename;
    FREE(sci);
    return path;
}

bool is_single_quote_string_start(const std::string& line, size_t column)
{
    if (column == 0)
    {
        return true;
    }

    char previous = line[column - 1];
    unsigned char character = static_cast<unsigned char>(previous);
    bool identifierPart = std::isalnum(character)
        || previous == '_' || previous == '%' || previous == '$';
    return !identifierPart && previous != ']' && previous != ')';
}

json document_range(const DocumentRange& range)
{
    return {
        { "start", {{ "line", range.firstLine }, { "character", range.firstColumn }}},
        { "end", {{ "line", range.lastLine }, { "character", range.lastColumn }}}
    };
}
