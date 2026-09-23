/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Implements document and range formatting. Only the whitespace at the
// beginning of lines is rewritten, from the layout collected by
// FormattingVisitor; every other character of the source is kept as the user
// wrote it. A document that does not parse is not formatted, since its
// structure is unknown past the error.

#include <algorithm>

#include "lspEndpoint.hxx"
#include "lspProtocolUtils.hxx"
#include "lsp_dap_tools.hxx"
#include "UTF8.hxx"
#include "localization.hxx"
#include "formattingVisitor.hxx"
#include "exp.hxx"

constexpr int RequestFailed = -32803;

namespace
{

size_t leading_whitespace_length(const std::string& line)
{
    size_t length = line.find_first_not_of(" \t");
    return length == std::string::npos ? line.size() : length;
}

int visual_width(const std::string& line, size_t length, int tabSize)
{
    int width = 0;
    for (size_t i = 0; i < length; ++i)
    {
        width = line[i] == '\t' ? (width / tabSize + 1) * tabSize : width + 1;
    }
    return width;
}

std::string indentation(int width, int tabSize, bool insertSpaces)
{
    width = std::max(width, 0);
    if (insertSpaces)
    {
        return std::string(width, ' ');
    }
    return std::string(width / tabSize, '\t') + std::string(width % tabSize, ' ');
}

// Returns false, with a message, when the document cannot be formatted.
bool formatting_edits(const std::vector<std::string>& lines, int tabSize, bool insertSpaces,
    int firstLine, int lastLine, nlohmann::json& edits, std::string& message)
{
    ast::Exp* tree = nullptr;
    std::string parseError;
    parseDocumentWithRecoveryWithoutRuntime(lines, &tree, &parseError);
    if (!parseError.empty() || tree == nullptr)
    {
        delete tree;
        message = _("Correct the syntax errors before formatting.");
        return false;
    }

    const std::vector<LineLayout> layout = analyzeLayout(*tree, lines);
    delete tree;

    std::vector<int> oldWidths(lines.size());
    std::vector<int> newWidths(lines.size());
    edits = nlohmann::json::array();
    for (size_t line = 0; line < lines.size(); ++line)
    {
        size_t length = leading_whitespace_length(lines[line]);
        oldWidths[line] = visual_width(lines[line], length, tabSize);
        newWidths[line] = oldWidths[line];
        if (layout[line].depth >= 0)
        {
            newWidths[line] = layout[line].depth * tabSize;
        }
        else if (layout[line].continuationOf >= 0)
        {
            int anchor = layout[line].continuationOf;
            newWidths[line] = std::max(0,
                oldWidths[line] + newWidths[anchor] - oldWidths[anchor]);
        }
        else
        {
            continue;
        }

        if (static_cast<int>(line) < firstLine || static_cast<int>(line) > lastLine
                || length == lines[line].size())
        {
            // Out of the requested range, or a blank line: nothing to write.
            continue;
        }

        std::string replacement = indentation(newWidths[line], tabSize, insertSpaces);
        if (lines[line].compare(0, length, replacement) == 0 && length == replacement.size())
        {
            continue;
        }

        edits.push_back({
            { "range", {
                { "start", { { "line", line }, { "character", 0 } } },
                { "end", { { "line", line }, { "character", length } } }
            }},
            { "newText", replacement }
        });
    }
    return true;
}

}

void LspEndpoint::handleFormatting(const nlohmann::json& json_msg)
{
    nlohmann::json resp = response(json_msg);
    const nlohmann::json& params = json_msg["params"];
    std::string uri = params["textDocument"]["uri"];
    const std::vector<std::string>* found = findDocument(uri);
    if (found == nullptr)
    {
        resp["error"] = {
            { "code", InvalidParams },
            { "message", "File not found" }
        };
        sendMessage(resp.dump());
        return;
    }

    int tabSize = 4;
    bool insertSpaces = true;
    if (params.contains("options") && params["options"].is_object())
    {
        tabSize = params["options"].value("tabSize", tabSize);
        insertSpaces = params["options"].value("insertSpaces", insertSpaces);
    }
    tabSize = std::max(tabSize, 1);

    int firstLine = 0;
    int lastLine = static_cast<int>(found->size()) - 1;
    if (params.contains("range") && params["range"].is_object())
    {
        firstLine = params["range"]["start"]["line"];
        lastLine = params["range"]["end"]["line"];
        // A selection ending at the start of a line does not include it.
        if (lastLine > firstLine && params["range"]["end"]["character"] == 0)
        {
            --lastLine;
        }
    }

    nlohmann::json edits;
    std::string message;
    if (!formatting_edits(*found, tabSize, insertSpaces, firstLine, lastLine, edits, message))
    {
        resp["error"] = {
            { "code", RequestFailed },
            { "message", message }
        };
        sendMessage(resp.dump());
        return;
    }

    resp["result"] = edits;
    sendMessage(resp.dump());
}
