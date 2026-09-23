/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __DOCUMENT_ANALYSIS_VISITOR_HXX__
#define __DOCUMENT_ANALYSIS_VISITOR_HXX__

#include <string>
#include <vector>

// Neutral document model collected from the AST for editor features.
// The visitor records syntax roles, hierarchical symbols, and structural regions;
// LSP-specific adapters then expose this shared analysis to Monaco and VS Code.

namespace ast
{
class Exp;
}

enum class SyntaxRole
{
    FunctionKeyword,
    OperatorSpecial,
    Property,
    Callable,
    Macro,
    Parameter,
    Variable,
    Constant,
    String
};

enum class DocumentArgumentRole
{
    None,
    Input,
    Output,
    InputOutput
};

enum class DocumentSymbolRole
{
    Function,
    Class,
    Method,
    Constructor,
    Property,
    Enumeration
};

enum class DocumentRegionRole
{
    Function,
    Class,
    Arguments,
    Conditional,
    Loop,
    Selection,
    TryCatch
};

struct DocumentRange
{
    int firstLine;
    int firstColumn;
    int lastLine;
    int lastColumn;
};

struct SyntaxTokenHint
{
    int line;
    int start;
    int length;
    SyntaxRole role;
    bool definition;
    std::string runtimeExpression;
    bool runtimeSafe;
};

struct DocumentSymbolEntry
{
    std::string name;
    DocumentSymbolRole role;
    DocumentRange range;
    DocumentRange selectionRange;
    std::vector<DocumentSymbolEntry> children;
};

struct DocumentRegionEntry
{
    DocumentRegionRole role;
    DocumentRange range;
};

struct DocumentCallEntry
{
    std::string callable;
    DocumentRange range;
    std::vector<DocumentRange> arguments;
    bool runtimeSafe;
    bool resolvedAsVariable;
};

struct DocumentCallableEntry
{
    std::string name;
    DocumentSymbolRole role;
    DocumentRange range;
    DocumentRange selectionRange;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

struct DocumentIdentifierEntry
{
    std::string name;
    std::string runtimeExpression;
    DocumentRange range;
    SyntaxRole role;
    bool hasRole;
    bool runtimeSafe;
    bool local;
    bool documentSymbol;
    bool definition;
    DocumentArgumentRole argumentRole;
};

struct DocumentAnalysis
{
    std::vector<SyntaxTokenHint> syntaxTokens;
    std::vector<DocumentSymbolEntry> symbols;
    std::vector<DocumentRegionEntry> regions;
    std::vector<DocumentCallEntry> calls;
    std::vector<DocumentCallableEntry> callables;
    std::vector<DocumentIdentifierEntry> identifiers;
};

struct SemanticTokenEntry
{
    int line;
    int start;
    int length;
    int type;
    int modifiers;
    int priority;
};

void collectSyntaxSemanticTokens(ast::Exp* tree,
        const std::vector<std::string>& lines,
        std::vector<SemanticTokenEntry>& tokens,
        int keywordTokenType,
        int operatorSpecialTokenType,
        int propertyTokenType,
        int functionTokenType,
        int macroTokenType,
        int parameterTokenType,
        int variableTokenType,
        int stringTokenType,
        int definitionModifier);

DocumentAnalysis analyzeDocument(ast::Exp* tree, const std::vector<std::string>& lines);

#endif /* !__DOCUMENT_ANALYSIS_VISITOR_HXX__ */
