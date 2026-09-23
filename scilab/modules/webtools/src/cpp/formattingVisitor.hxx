/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __FORMATTING_VISITOR_HXX__
#define __FORMATTING_VISITOR_HXX__

#include <string>
#include <vector>

// Neutral layout of a document collected from the AST for formatting.
// The visitor records the block depth of each line; LSP adapters turn this
// layout into text edits for Monaco and VS Code.

namespace ast
{
class Exp;
}

struct LineLayout
{
    // Block depth of a line that starts a statement or a block keyword, or -1.
    int depth = -1;
    // 0-based first line of the statement this line continues, or -1.
    int continuationOf = -1;
};

// One entry per source line. A line with neither a depth nor a continuation
// (blank line, inside of a block comment, ...) must be left untouched.
std::vector<LineLayout> analyzeLayout(const ast::Exp& tree, const std::vector<std::string>& lines);

#endif /* !__FORMATTING_VISITOR_HXX__ */
