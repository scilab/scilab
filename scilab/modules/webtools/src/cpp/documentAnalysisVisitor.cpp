/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <unordered_map>

#include "documentAnalysisVisitor.hxx"

#include "dummyvisitor.hxx"
#include "dollarvar.hxx"
#include "fieldexp.hxx"
#include "functiondec.hxx"
#include "classdec.hxx"
#include "methodsdec.hxx"
#include "propertiesdec.hxx"
#include "argumentdec.hxx"
#include "argumentsexp.hxx"
#include "assignexp.hxx"
#include "enumdec.hxx"
#include "callexp.hxx"
#include "ifexp.hxx"
#include "whileexp.hxx"
#include "forexp.hxx"
#include "selectexp.hxx"
#include "trycatchexp.hxx"
#include "simplevar.hxx"
#include "boolexp.hxx"
#include "stringexp.hxx"
#include "UTF8.hxx"

namespace
{
static const size_t npos = std::string::npos;
static const int AST_TOKEN_PRIORITY = 0;

// First AST pass collecting macro and class-method declarations so the main
// analysis can classify calls even when their declarations appear later.
class DeclarationCollector : public ast::DummyVisitor
{
public:
    DeclarationCollector(std::set<std::wstring>& macros, std::set<std::wstring>& methods)
        : m_macros(macros), m_methods(methods)
    {
    }

protected:
    void visit(const ast::FunctionDec& function) override
    {
        if (!function.isLambda())
        {
            m_macros.insert(function.getSymbol().getName());
            if (m_classDepth > 0)
            {
                m_methods.insert(function.getSymbol().getName());
            }
        }
        function.getArgs().accept(*this);
        if (!function.isLambda())
        {
            function.getReturns().accept(*this);
        }
        function.getBody().accept(*this);
    }

    void visit(const ast::ClassDec& declaration) override
    {
        m_macros.insert(declaration.getSymbol().getName());
        ++m_classDepth;
        ast::DummyVisitor::visit(declaration);
        --m_classDepth;
    }

    void visit(const ast::MethodsDec& declaration) override
    {
        for (ast::Exp* method : declaration.getMethods())
        {
            method->accept(*this);
        }
    }

private:
    std::set<std::wstring>& m_macros;
    std::set<std::wstring>& m_methods;
    int m_classDepth = 0;
};

// Build a client-neutral document analysis from the AST. The collected
// syntax hints, symbols, and regions are reused by semantic tokens, document
// symbols, folding, and future editor-oriented LSP features.
class DocumentAnalysisVisitor : public ast::DummyVisitor
{
public:
    DocumentAnalysisVisitor(const std::vector<std::string>& lines, DocumentAnalysis& analysis)
        : m_lines(lines),
          m_analysis(analysis)
    {
    }

    void collect(ast::Exp* tree)
    {
        if (tree == nullptr)
        {
            return;
        }

        m_scopes.emplace_back();
        m_argumentScopes.emplace_back();
        DeclarationCollector declarations(m_macroNames, m_methodNames);
        tree->accept(declarations);
        tree->accept(*this);
        m_argumentScopes.pop_back();
        m_scopes.pop_back();
    }

protected:
    void visit(const ast::FunctionDec& function) override
    {
        m_scopes.emplace_back();
        m_argumentScopes.emplace_back();
        registerFunctionVariables(function.getArgs(), DocumentArgumentRole::Input);

        if (!function.isLambda())
        {
            registerFunctionVariables(function.getReturns(), DocumentArgumentRole::Output);
        }

        collectFunctionVariables(function.getArgs());
        if (!function.isLambda())
        {
            collectFunctionVariables(function.getReturns());
            DocumentSymbolRole callableRole = DocumentSymbolRole::Function;
            if (!m_classNames.empty())
            {
                callableRole = function.getSymbol().getName() == m_classNames.back()
                    ? DocumentSymbolRole::Constructor : DocumentSymbolRole::Method;
            }
            addCallable(function, callableRole);

            const Location& loc = function.getLocation();
            addKeywordToken(loc.first_line - 1, "function", SyntaxRole::FunctionKeyword, true);
            addFunctionNameToken(function);
            addFunctionEndToken(function);
            addRegion(DocumentRegionRole::Function, loc);
            if (!m_suppressSymbols && m_functionDepth == 0)
            {
                addSymbol(function, DocumentSymbolRole::Function, m_analysis.symbols);
            }
        }

        ++m_functionDepth;
        function.getArgs().accept(*this);
        if (!function.isLambda())
        {
            function.getReturns().accept(*this);
        }
        function.getBody().accept(*this);
        --m_functionDepth;
        m_argumentScopes.pop_back();
        m_scopes.pop_back();
    }

    void visit(const ast::SimpleVar& var) override
    {
        ExpressionState state;
        state.terminal = &var;
        state.runtimeExpression = scilab::UTF8::toUTF8(var.getSymbol().getName());

        if (var.getSymbol().getName() == L"end")
        {
            const Location& loc = var.getLocation();
            int line = loc.first_line - 1;
            if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
            {
                m_expressionStates[&var] = state;
                return;
            }

            size_t column = findTokenNear(m_lines[line], "end", locationColumn(loc), true);
            if (column != npos && isIndexEndContext(m_lines[line], column))
            {
                addToken(line, column, 3, SyntaxRole::OperatorSpecial, false);
            }
            m_expressionStates[&var] = state;
            return;
        }

        SyntaxRole role = SyntaxRole::Variable;
        DocumentArgumentRole argumentRole = DocumentArgumentRole::None;
        bool hasRole = false;
        bool local = false;
        bool documentSymbol = false;
        if (findCurrentScopeRole(var.getSymbol().getName(), role, argumentRole))
        {
            hasRole = true;
            local = role == SyntaxRole::Parameter || m_scopes.size() > 1;
            state.tokenIndex = addSimpleVarToken(var, role, false);
        }
        else if (m_macroNames.find(var.getSymbol().getName()) != m_macroNames.end())
        {
            hasRole = true;
            role = SyntaxRole::Macro;
            documentSymbol = true;
            state.tokenIndex = addSimpleVarToken(var, SyntaxRole::Macro, false);
        }
        state.identifierIndex = addIdentifier(var, state.runtimeExpression,
            hasRole, role, false, local, documentSymbol, false, argumentRole);
        m_expressionStates[&var] = state;
    }

    void visit(const ast::BoolExp& value) override
    {
        const Location& loc = value.getLocation();
        int line = loc.first_line - 1;
        if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return;
        }

        size_t column = locationColumn(loc);
        if (column + 2 <= m_lines[line].size())
        {
            addToken(line, column, 2, SyntaxRole::Constant, false);
        }
    }

    void visit(const ast::StringExp& value) override
    {
        const Location& loc = value.getLocation();
        int line = loc.first_line - 1;
        if (line < 0 || loc.last_line != loc.first_line
                || static_cast<size_t>(line) >= m_lines.size())
        {
            return;
        }

        size_t column = locationColumn(loc);
        // last_column is 1-based and points past the end of the location.
        size_t end = static_cast<size_t>(std::max(0, loc.last_column - 1));
        end = std::min(end, m_lines[line].size());
        if (column < end)
        {
            addToken(line, column, end - column, SyntaxRole::String, false);
        }
    }

    void visit(const ast::ArgumentsExp& arguments) override
    {
        addRegion(DocumentRegionRole::Arguments, arguments.getLocation());
        // DummyVisitor intentionally skips ArgumentsExp. Walk its declarations
        // so function parameters keep their AST role inside the arguments block.
        for (ast::Exp* declaration : arguments.getExps())
        {
            declaration->accept(*this);
        }
    }

    void visit(const ast::CallExp& call) override
    {
        ast::DummyVisitor::visit(call);

        ExpressionState state = expressionState(&call.getName());
        state.called = true;
        if (state.field && state.tokenIndex != npos && state.terminal != nullptr)
        {
            SyntaxTokenHint& token = m_analysis.syntaxTokens[state.tokenIndex];
            token.role = m_methodNames.find(state.terminal->getSymbol().getName()) != m_methodNames.end()
                ? SyntaxRole::Macro : SyntaxRole::Property;
        }
        if (state.field && state.identifierIndex != npos)
        {
            DocumentIdentifierEntry& identifier = m_analysis.identifiers[state.identifierIndex];
            identifier.role = m_methodNames.find(state.terminal->getSymbol().getName()) != m_methodNames.end()
                ? SyntaxRole::Macro : SyntaxRole::Property;
            identifier.hasRole = true;
        }
        state.containsCall = true;
        m_expressionStates[&call] = state;

        if (!state.runtimeExpression.empty())
        {
            DocumentCallEntry entry;
            entry.callable = state.runtimeExpression;
            entry.range = convertRange(call.getLocation());
            for (ast::Exp* argument : call.getArgs())
            {
                entry.arguments.push_back(convertRange(argument->getLocation()));
            }
            entry.runtimeSafe = state.runtimeSafe;
            entry.resolvedAsVariable = false;
            if (state.identifierIndex != npos)
            {
                const DocumentIdentifierEntry& identifier =
                    m_analysis.identifiers[state.identifierIndex];
                entry.resolvedAsVariable = identifier.hasRole
                    && (identifier.role == SyntaxRole::Variable
                        || identifier.role == SyntaxRole::Parameter);
            }
            m_analysis.calls.push_back(entry);
        }
    }

    void visit(const ast::AssignExp& assignment) override
    {
        bool declared = false;
        if (assignment.getLeftExp().isSimpleVar() && !m_scopes.empty())
        {
            const ast::SimpleVar& variable = *assignment.getLeftExp().getAs<ast::SimpleVar>();
            std::map<std::wstring, SyntaxRole>& scope = m_scopes.back();
            if (scope.find(variable.getSymbol().getName()) == scope.end())
            {
                scope[variable.getSymbol().getName()] = SyntaxRole::Variable;
                addSimpleVarToken(variable, SyntaxRole::Variable, true);
                addIdentifier(variable, scilab::UTF8::toUTF8(variable.getSymbol().getName()),
                    true, SyntaxRole::Variable, false, m_scopes.size() > 1, false, true);
                declared = true;
            }
        }
        if (!declared)
        {
            assignment.getLeftExp().accept(*this);
        }
        assignment.getRightExp().accept(*this);
    }

    void visit(const ast::FieldExp& field) override
    {
        ast::DummyVisitor::visit(field);

        ExpressionState head = expressionState(field.getHead());
        ExpressionState tail = expressionState(field.getTail());
        if (tail.terminal == nullptr)
        {
            m_expressionStates[&field] = {};
            return;
        }

        ExpressionState state = tail;
        state.field = true;
        state.runtimeExpression = head.runtimeExpression.empty() || tail.runtimeExpression.empty()
            ? std::string() : head.runtimeExpression + "." + tail.runtimeExpression;
        bool runtimeSafe = !head.containsCall && (!tail.containsCall || tail.called);
        state.runtimeSafe = runtimeSafe;
        SyntaxRole role = m_methodNames.find(tail.terminal->getSymbol().getName()) != m_methodNames.end()
            ? SyntaxRole::Macro : SyntaxRole::Property;
        if (state.tokenIndex == npos)
        {
            state.tokenIndex = addSimpleVarToken(*tail.terminal, role, false,
                state.runtimeExpression, runtimeSafe);
        }
        else
        {
            SyntaxTokenHint& token = m_analysis.syntaxTokens[state.tokenIndex];
            token.role = role;
            token.runtimeExpression = state.runtimeExpression;
            token.runtimeSafe = runtimeSafe;
        }
        if (state.identifierIndex != npos)
        {
            DocumentIdentifierEntry& identifier = m_analysis.identifiers[state.identifierIndex];
            identifier.runtimeExpression = state.runtimeExpression;
            identifier.role = role;
            identifier.hasRole = true;
            identifier.runtimeSafe = runtimeSafe;
            identifier.documentSymbol = role == SyntaxRole::Macro;
        }
        state.containsCall = head.containsCall || tail.containsCall;
        m_expressionStates[&field] = state;
    }

    void visit(const ast::DollarVar& var) override
    {
        const Location& loc = var.getLocation();
        int line = loc.first_line - 1;
        if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return;
        }

        size_t column = findTokenNear(m_lines[line], "$", locationColumn(loc), false);
        if (column != npos)
        {
            addToken(line, column, 1, SyntaxRole::OperatorSpecial, false);
        }
    }

    void visit(const ast::ClassDec& declaration) override
    {
        addDeclarationNameToken(declaration.getSymbol().getName(),
            declaration.getLocation(), SyntaxRole::Macro);
        DocumentSymbolEntry symbol;
        symbol.name = scilab::UTF8::toUTF8(declaration.getSymbol().getName());
        symbol.role = DocumentSymbolRole::Class;
        symbol.range = convertRange(declaration.getLocation());
        symbol.selectionRange = declarationNameRange(declaration.getSymbol().getName(),
            declaration.getLocation());
        addClassMembers(declaration, symbol);
        if (!m_suppressSymbols)
        {
            m_analysis.symbols.push_back(symbol);
        }
        addRegion(DocumentRegionRole::Class, declaration.getLocation());
        bool suppressSymbols = m_suppressSymbols;
        m_suppressSymbols = true;
        m_classNames.push_back(declaration.getSymbol().getName());
        ast::DummyVisitor::visit(declaration);
        m_classNames.pop_back();
        m_suppressSymbols = suppressSymbols;
    }

    void visit(const ast::MethodsDec& declaration) override
    {
        addRegion(DocumentRegionRole::Class, declaration.getLocation());
        for (ast::Exp* method : declaration.getMethods())
        {
            method->accept(*this);
        }
    }

    void visit(const ast::PropertiesDec& declaration) override
    {
        addRegion(DocumentRegionRole::Class, declaration.getLocation());
        ast::DummyVisitor::visit(declaration);
    }

    void visit(const ast::EnumDec& declaration) override
    {
        addRegion(DocumentRegionRole::Class, declaration.getLocation());
        ast::DummyVisitor::visit(declaration);
    }

    void visit(const ast::IfExp& expression) override
    {
        addRegion(DocumentRegionRole::Conditional, expression.getLocation());
        ast::DummyVisitor::visit(expression);
    }

    void visit(const ast::WhileExp& expression) override
    {
        addRegion(DocumentRegionRole::Loop, expression.getLocation());
        ast::DummyVisitor::visit(expression);
    }

    void visit(const ast::ForExp& expression) override
    {
        addRegion(DocumentRegionRole::Loop, expression.getLocation());
        ast::DummyVisitor::visit(expression);
    }

    void visit(const ast::SelectExp& expression) override
    {
        addRegion(DocumentRegionRole::Selection, expression.getLocation());
        ast::DummyVisitor::visit(expression);
    }

    void visit(const ast::TryCatchExp& expression) override
    {
        addRegion(DocumentRegionRole::TryCatch, expression.getLocation());
        ast::DummyVisitor::visit(expression);
    }

private:
    struct ExpressionState
    {
        const ast::SimpleVar* terminal = nullptr;
        std::string runtimeExpression;
        size_t tokenIndex = npos;
        size_t identifierIndex = npos;
        bool field = false;
        bool called = false;
        bool containsCall = false;
        bool runtimeSafe = false;
    };

    ExpressionState expressionState(const ast::Exp* expression) const
    {
        auto found = m_expressionStates.find(expression);
        return found == m_expressionStates.end() ? ExpressionState() : found->second;
    }

    size_t addIdentifier(const ast::SimpleVar& var, const std::string& runtimeExpression,
        bool hasRole, SyntaxRole role, bool runtimeSafe, bool local, bool documentSymbol,
        bool definition = false,
        DocumentArgumentRole argumentRole = DocumentArgumentRole::None)
    {
        std::string name = scilab::UTF8::toUTF8(var.getSymbol().getName());
        const Location& location = var.getLocation();
        int line = location.first_line - 1;
        if (name.empty() || line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return npos;
        }

        size_t column = findTokenNear(m_lines[line], name, locationColumn(location), true);
        if (column == npos)
        {
            return npos;
        }

        m_analysis.identifiers.push_back({
            name,
            runtimeExpression,
            { line, static_cast<int>(column), line, static_cast<int>(column + name.size()) },
            role,
            hasRole,
            runtimeSafe,
            local,
            documentSymbol,
            definition,
            argumentRole
        });
        return m_analysis.identifiers.size() - 1;
    }

    DocumentRange declarationNameRange(const std::wstring& wideName,
            const Location& location)
    {
        DocumentRange fallback = convertRange(location);
        std::string name = scilab::UTF8::toUTF8(wideName);
        int line = location.first_line - 1;
        if (name.empty() || line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return fallback;
        }

        size_t column = findTokenNear(m_lines[line], name, locationColumn(location), true);
        if (column == npos)
        {
            return fallback;
        }
        return { line, static_cast<int>(column), line,
            static_cast<int>(column + name.size()) };
    }

    void addDeclarationNameToken(const std::wstring& wideName,
            const Location& location, SyntaxRole role)
    {
        std::string name = scilab::UTF8::toUTF8(wideName);
        int line = location.first_line - 1;
        if (name.empty() || line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return;
        }

        size_t column = findTokenNear(m_lines[line], name, locationColumn(location), true);
        if (column != npos)
        {
            addToken(line, column, name.size(), role, true);
        }
    }

    bool addKeywordToken(int line, const std::string& keyword, SyntaxRole role, bool definition)
    {
        if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return false;
        }

        size_t column = findKeyword(m_lines[line], keyword, 0);
        if (column == std::string::npos)
        {
            return false;
        }

        addToken(line, column, keyword.size(), role, definition);

        return true;
    }

    size_t addSimpleVarToken(const ast::SimpleVar& var, SyntaxRole role, bool definition,
        const std::string& runtimeExpression = std::string(), bool runtimeSafe = false)
    {
        std::string token = scilab::UTF8::toUTF8(var.getSymbol().getName());
        if (token.empty())
        {
            return npos;
        }

        const Location& loc = var.getLocation();
        int line = loc.first_line - 1;
        if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return npos;
        }

        size_t column = findTokenNear(m_lines[line], token, locationColumn(loc), true);
        if (column != npos)
        {
            addToken(line, column, token.size(), role, definition,
                runtimeExpression, runtimeSafe);
            return m_analysis.syntaxTokens.size() - 1;
        }
        return npos;
    }

    void addToken(int line, size_t column, size_t length, SyntaxRole role, bool definition,
        const std::string& runtimeExpression = std::string(), bool runtimeSafe = false)
    {
        if (line < 0 || static_cast<size_t>(line) >= m_lines.size())
        {
            return;
        }

        m_analysis.syntaxTokens.push_back({
            line,
            static_cast<int>(column),
            static_cast<int>(length),
            role,
            definition,
            runtimeExpression,
            runtimeSafe
        });
    }

    bool addFunctionEndToken(const ast::FunctionDec& function)
    {
        const Location& loc = function.getLocation();
        int firstLine = std::max(0, loc.first_line - 1);
        int lastLine = std::min(static_cast<int>(m_lines.size()) - 1, loc.last_line - 1);
        int bodyLastLine = std::max(firstLine, function.getBody().getLocation().last_line - 1);
        // The body location may end on the closing keyword of its last nested
        // block. Search backwards so that the function's own closing keyword
        // wins over an immediately preceding for/select/if "end".
        for (int line = lastLine; line >= bodyLastLine; --line)
        {
            if (addKeywordToken(line, "endfunction", SyntaxRole::FunctionKeyword, true))
            {
                return true;
            }

            if (addKeywordToken(line, "end", SyntaxRole::FunctionKeyword, true))
            {
                return true;
            }
        }

        return false;
    }

    size_t locationColumn(const Location& loc)
    {
        return static_cast<size_t>(std::max(0, loc.first_column - 1));
    }

    size_t findTokenNear(const std::string& line, const std::string& token, size_t approximateColumn, bool requireIdentifierBoundary) const
    {
        size_t bestColumn = npos;
        size_t bestDistance = static_cast<size_t>(-1);
        size_t column = line.find(token);
        while (column != npos)
        {
            if (!requireIdentifierBoundary || isKeywordBoundary(line, column, token.size()))
            {
                size_t distance = tokenDistance(column, token.size(), approximateColumn);
                if (distance < bestDistance)
                {
                    bestColumn = column;
                    bestDistance = distance;
                }
            }

            column = line.find(token, column + 1);
        }

        return bestColumn;
    }

    size_t tokenDistance(size_t column, size_t length, size_t approximateColumn) const
    {
        if (approximateColumn >= column && approximateColumn < column + length)
        {
            return 0;
        }

        if (approximateColumn < column)
        {
            return column - approximateColumn;
        }

        return approximateColumn - (column + length - 1);
    }

    bool isKeywordBoundary(const std::string& line, size_t column, size_t length) const
    {
        bool before = column == 0 || !isIdentifierPart(line[column - 1]);
        size_t end = column + length;
        bool after = end >= line.size() || !isIdentifierPart(line[end]);
        return before && after;
    }

    bool isIndexEndContext(const std::string& line, size_t column)
    {
        size_t previous = previousNonSpace(line, column);
        if (previous == npos)
        {
            return false;
        }

        return line[previous] == ':' || line[previous] == ',' || line[previous] == '(' || line[previous] == '[';
    }

    size_t previousNonSpace(const std::string& line, size_t column)
    {
        size_t previous = column;
        while (previous > 0)
        {
            --previous;
            unsigned char c = static_cast<unsigned char>(line[previous]);
            if (!std::isspace(c))
            {
                return previous;
            }
        }

        return npos;
    }

    size_t findKeyword(const std::string& line, const std::string& keyword, size_t start)
    {
        size_t column = line.find(keyword, start);
        while (column != std::string::npos)
        {
            bool before = column == 0 || !isIdentifierPart(line[column - 1]);
            size_t end = column + keyword.size();
            bool after = end >= line.size() || !isIdentifierPart(line[end]);
            if (before && after)
            {
                return column;
            }

            column = line.find(keyword, column + 1);
        }

        return std::string::npos;
    }

    bool isIdentifierPart(char c) const
    {
        unsigned char uc = static_cast<unsigned char>(c);
        return std::isalnum(uc) || c == '_' || c == '%' || c == '$';
    }

    DocumentRange convertRange(const Location& location)
    {
        return {
            std::max(0, location.first_line - 1),
            std::max(0, location.first_column - 1),
            std::max(0, location.last_line - 1),
            std::max(0, location.last_column)
        };
    }

    void addRegion(DocumentRegionRole role, const Location& location)
    {
        DocumentRange range = convertRange(location);
        if (range.lastLine > range.firstLine)
        {
            m_analysis.regions.push_back({ role, range });
        }
    }

    void addSymbol(const ast::FunctionDec& function, DocumentSymbolRole role,
                   std::vector<DocumentSymbolEntry>& symbols)
    {
        DocumentSymbolEntry symbol;
        symbol.name = scilab::UTF8::toUTF8(function.getSymbol().getName());
        symbol.role = role;
        symbol.range = convertRange(function.getLocation());
        symbol.selectionRange = declarationNameRange(function.getSymbol().getName(),
            function.getLocation());
        symbols.push_back(symbol);
    }

    std::vector<std::string> functionVariableNames(const ast::ArrayListVar& variables)
    {
        std::vector<std::string> names;
        for (ast::Exp* variable : variables.getVars())
        {
            ast::Exp* name = variable;
            if (variable->isArgumentDec())
            {
                name = variable->getAs<ast::ArgumentDec>()->getArgumentName();
            }
            if (name->isSimpleVar())
            {
                names.push_back(scilab::UTF8::toUTF8(
                    name->getAs<ast::SimpleVar>()->getSymbol().getName()));
            }
        }
        return names;
    }

    void addCallable(const ast::FunctionDec& function, DocumentSymbolRole role)
    {
        m_analysis.callables.push_back({
            scilab::UTF8::toUTF8(function.getSymbol().getName()),
            role,
            convertRange(function.getLocation()),
            declarationNameRange(function.getSymbol().getName(), function.getLocation()),
            functionVariableNames(function.getArgs()),
            functionVariableNames(function.getReturns())
        });
    }

    void registerFunctionVariables(const ast::ArrayListVar& variables,
        DocumentArgumentRole argumentRole)
    {
        if (m_scopes.empty() || m_argumentScopes.empty())
        {
            return;
        }

        for (ast::Exp* variable : variables.getVars())
        {
            ast::Exp* name = variable;
            if (variable->isArgumentDec())
            {
                name = variable->getAs<ast::ArgumentDec>()->getArgumentName();
            }

            if (name->isSimpleVar())
            {
                const ast::SimpleVar& simpleName = *name->getAs<ast::SimpleVar>();
                const std::wstring& symbol = simpleName.getSymbol().getName();
                m_scopes.back()[symbol] = SyntaxRole::Parameter;
                DocumentArgumentRole& registeredRole = m_argumentScopes.back()[symbol];
                if (registeredRole != DocumentArgumentRole::None
                        && registeredRole != argumentRole)
                {
                    registeredRole = DocumentArgumentRole::InputOutput;
                }
                else
                {
                    registeredRole = argumentRole;
                }
            }
        }
    }

    void collectFunctionVariables(const ast::ArrayListVar& variables)
    {
        for (ast::Exp* variable : variables.getVars())
        {
            ast::Exp* name = variable;
            if (variable->isArgumentDec())
            {
                name = variable->getAs<ast::ArgumentDec>()->getArgumentName();
            }

            if (name->isSimpleVar())
            {
                const ast::SimpleVar& simpleName = *name->getAs<ast::SimpleVar>();
                DocumentArgumentRole argumentRole = DocumentArgumentRole::None;
                auto found = m_argumentScopes.back().find(simpleName.getSymbol().getName());
                if (found != m_argumentScopes.back().end())
                {
                    argumentRole = found->second;
                }
                addSimpleVarToken(simpleName, SyntaxRole::Parameter, true);
                addIdentifier(simpleName,
                    scilab::UTF8::toUTF8(simpleName.getSymbol().getName()),
                    true, SyntaxRole::Parameter, false, true, false, true, argumentRole);
            }
        }
    }

    bool findCurrentScopeRole(const std::wstring& name, SyntaxRole& role,
        DocumentArgumentRole& argumentRole)
    {
        if (m_scopes.empty())
        {
            return false;
        }

        // A definition found in an enclosing AST scope is not local to this
        // FunctionDec. Only parameters, outputs, and assignments collected in
        // the current scope are local; other names are resolved separately
        // from the active Scilab context.
        auto found = m_scopes.back().find(name);
        if (found == m_scopes.back().end())
        {
            return false;
        }

        role = found->second;
        auto argument = m_argumentScopes.back().find(name);
        if (argument != m_argumentScopes.back().end())
        {
            argumentRole = argument->second;
        }
        return true;
    }

    void addFunctionNameToken(const ast::FunctionDec& function)
    {
        addDeclarationNameToken(function.getSymbol().getName(),
            function.getLocation(), SyntaxRole::Macro);
    }

    void addClassMembers(const ast::ClassDec& declaration, DocumentSymbolEntry& symbol)
    {
        for (ast::Exp* entry : declaration.getMethods())
        {
            if (!entry->isMethodsDec())
            {
                continue;
            }

            for (ast::Exp* method : entry->getAs<ast::MethodsDec>()->getMethods())
            {
                if (method->isFunctionDec())
                {
                    ast::FunctionDec* function = method->getAs<ast::FunctionDec>();
                    DocumentSymbolRole role = function->getSymbol().getName() == declaration.getSymbol().getName()
                        ? DocumentSymbolRole::Constructor : DocumentSymbolRole::Method;
                    addSymbol(*function, role, symbol.children);
                }
            }
        }

        for (ast::Exp* entry : declaration.getProperties())
        {
            if (!entry->isPropertiesDec())
            {
                continue;
            }

            for (ast::Exp* property : entry->getAs<ast::PropertiesDec>()->getProperties())
            {
                if (!property->isArgumentDec())
                {
                    continue;
                }

                ast::Exp* name = property->getAs<ast::ArgumentDec>()->getArgumentName();
                if (!name->isSimpleVar())
                {
                    continue;
                }

                DocumentSymbolEntry child;
                child.name = scilab::UTF8::toUTF8(name->getAs<ast::SimpleVar>()->getSymbol().getName());
                child.role = DocumentSymbolRole::Property;
                child.range = convertRange(name->getLocation());
                child.selectionRange = child.range;
                symbol.children.push_back(child);
            }
        }

        for (ast::Exp* entry : declaration.getEnumeration())
        {
            if (!entry->isEnumDec())
            {
                continue;
            }

            for (ast::Exp* enumeration : entry->getAs<ast::EnumDec>()->getEnumeration())
            {
                ast::SimpleVar* name = nullptr;
                if (enumeration->isSimpleVar())
                {
                    name = enumeration->getAs<ast::SimpleVar>();
                }
                else if (enumeration->isCallExp()
                        && enumeration->getAs<ast::CallExp>()->getName().isSimpleVar())
                {
                    name = enumeration->getAs<ast::CallExp>()->getName().getAs<ast::SimpleVar>();
                }

                if (name == nullptr)
                {
                    continue;
                }

                DocumentSymbolEntry child;
                child.name = scilab::UTF8::toUTF8(name->getSymbol().getName());
                child.role = DocumentSymbolRole::Enumeration;
                child.range = convertRange(name->getLocation());
                child.selectionRange = child.range;
                symbol.children.push_back(child);
            }
        }
    }

    const std::vector<std::string>& m_lines;
    DocumentAnalysis& m_analysis;
    bool m_suppressSymbols = false;
    int m_functionDepth = 0;
    std::vector<std::map<std::wstring, SyntaxRole>> m_scopes;
    std::vector<std::map<std::wstring, DocumentArgumentRole>> m_argumentScopes;
    std::set<std::wstring> m_macroNames;
    std::set<std::wstring> m_methodNames;
    std::vector<std::wstring> m_classNames;
    std::unordered_map<const ast::Exp*, ExpressionState> m_expressionStates;
};
}

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
        int definitionModifier)
{
    DocumentAnalysis analysis = analyzeDocument(tree, lines);
    for (const SyntaxTokenHint& token : analysis.syntaxTokens)
    {
        int type = keywordTokenType;
        switch (token.role)
        {
            case SyntaxRole::FunctionKeyword: type = keywordTokenType; break;
            case SyntaxRole::OperatorSpecial: type = operatorSpecialTokenType; break;
            case SyntaxRole::Property: type = propertyTokenType; break;
            case SyntaxRole::Callable: type = functionTokenType; break;
            case SyntaxRole::Macro: type = macroTokenType; break;
            case SyntaxRole::Parameter: type = parameterTokenType; break;
            case SyntaxRole::Variable: type = variableTokenType; break;
            case SyntaxRole::Constant: type = variableTokenType; break;
            case SyntaxRole::String: type = stringTokenType; break;
        }

        tokens.push_back({
            token.line,
            token.start,
            token.length,
            type,
            token.definition ? definitionModifier : 0,
            AST_TOKEN_PRIORITY
        });
    }
}

DocumentAnalysis analyzeDocument(ast::Exp* tree, const std::vector<std::string>& lines)
{
    DocumentAnalysis analysis;
    DocumentAnalysisVisitor visitor(lines, analysis);
    visitor.collect(tree);
    return analysis;
}
