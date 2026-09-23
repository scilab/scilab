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

#include "formattingVisitor.hxx"
#include "dummyvisitor.hxx"
#include "argumentsexp.hxx"
#include "caseexp.hxx"
#include "classdec.hxx"
#include "enumdec.hxx"
#include "forexp.hxx"
#include "functiondec.hxx"
#include "ifexp.hxx"
#include "methodsdec.hxx"
#include "propertiesdec.hxx"
#include "selectexp.hxx"
#include "seqexp.hxx"
#include "trycatchexp.hxx"
#include "whileexp.hxx"

namespace
{

bool starts_with_word(const std::string& text, size_t start, const char* word)
{
    size_t length = std::char_traits<char>::length(word);
    if (text.compare(start, length, word) != 0)
    {
        return false;
    }
    if (start + length >= text.size())
    {
        return true;
    }
    char next = text[start + length];
    return !(std::isalnum(static_cast<unsigned char>(next)) || next == '_'
        || next == '%' || next == '#' || next == '!' || next == '$' || next == '?');
}

// The tree keeps no location for else, catch, otherwise, nor for the final
// end: these lines are recognized by their text. Keep this list in sync with
// the grammar keywords that split or close a block.
bool starts_with_block_keyword(const std::string& text)
{
    static const char* keywords[] = {
        "elseif", "else", "catch", "case", "otherwise", "endfunction", "end"
    };
    size_t start = text.find_first_not_of(" \t");
    if (start == std::string::npos)
    {
        return false;
    }
    for (const char* keyword : keywords)
    {
        if (starts_with_word(text, start, keyword))
        {
            return true;
        }
    }
    return false;
}

/*
 * Collects the layout of each line from the statements of the tree.
 *
 * A line that starts a statement takes the depth of the block holding the
 * statement. The following lines of a statement spanning several lines
 * (continuation "...", matrix rows) continue its first line. Block keywords
 * (else, case, end, ...) take the depth of their block. Expressions are not
 * visited: only statements and blocks give the layout.
 */
class FormattingVisitor : public ast::DummyVisitor
{
public:
    explicit FormattingVisitor(const std::vector<std::string>& lines)
        : m_lines(lines), m_layout(lines.size())
    {
    }

    std::vector<LineLayout> collect(const ast::Exp& tree)
    {
        // The root is the statement list of the document, not a statement.
        if (tree.isSeqExp())
        {
            tree.accept(*this);
        }
        else
        {
            statement(tree);
        }
        layoutComments();
        return m_layout;
    }

protected:
    void visit(const ast::SeqExp& e) override
    {
        for (const ast::Exp* exp : e.getExps())
        {
            if (exp != nullptr)
            {
                statement(*exp);
            }
        }
    }

    void visit(const ast::IfExp& e) override
    {
        m_block = true;
        body(e.getThen());
        if (e.hasElse())
        {
            const ast::Exp* elseIf = elseIfOf(e.getElse());
            if (elseIf != nullptr)
            {
                // "elseif" continues this "if" at the same depth and shares
                // its "end".
                statement(*elseIf);
            }
            else
            {
                body(e.getElse());
            }
        }
    }

    void visit(const ast::ForExp& e) override
    {
        m_block = true;
        body(e.getBody());
    }

    void visit(const ast::WhileExp& e) override
    {
        m_block = true;
        body(e.getBody());
    }

    void visit(const ast::TryCatchExp& e) override
    {
        m_block = true;
        body(e.getTry());
        body(e.getCatch());
    }

    void visit(const ast::SelectExp& e) override
    {
        // SciNotes style: "case" and "otherwise" at the depth of "select".
        m_block = true;
        for (const ast::Exp* c : e.getCases())
        {
            if (c != nullptr)
            {
                statement(*c);
            }
        }
        if (e.getDefaultCase() != nullptr)
        {
            body(*e.getDefaultCase());
        }
    }

    void visit(const ast::CaseExp& e) override
    {
        m_block = true;
        if (e.getBody() != nullptr)
        {
            body(*e.getBody());
        }
    }

    void visit(const ast::FunctionDec& e) override
    {
        // A lambda is an expression inside a statement, not a block.
        if (e.isLambda())
        {
            return;
        }
        m_block = true;
        body(e.getBody());
    }

    void visit(const ast::ArgumentsExp& e) override
    {
        m_block = true;
        members(e.getExps());
    }

    void visit(const ast::ClassDec& e) override
    {
        m_block = true;
        members(e.getEnumeration());
        members(e.getProperties());
        members(e.getMethods());
    }

    void visit(const ast::PropertiesDec& e) override
    {
        m_block = true;
        members(e.getProperties());
    }

    void visit(const ast::MethodsDec& e) override
    {
        m_block = true;
        members(e.getMethods());
    }

    void visit(const ast::EnumDec& e) override
    {
        m_block = true;
        members(e.getEnumeration());
    }

private:
    void statement(const ast::Exp& e)
    {
        const Location& location = e.getLocation();
        int first = location.first_line - 1;
        if (first < 0 || first >= static_cast<int>(m_lines.size()))
        {
            return;
        }
        int last = std::min(std::max(location.last_line - 1, first),
            static_cast<int>(m_lines.size()) - 1);

        if (startsLine(first, location.first_column) && m_layout[first].depth < 0)
        {
            m_layout[first].depth = m_depth;
            m_layout[first].continuationOf = -1;
        }

        // Only a block visit sets m_block; the flag belongs to this statement.
        bool enclosingBlock = m_block;
        m_block = false;
        e.accept(*this);
        bool isBlock = m_block;
        m_block = enclosingBlock;

        for (int line = first + 1; line <= last; ++line)
        {
            if (m_layout[line].depth >= 0 || m_layout[line].continuationOf >= 0)
            {
                continue;
            }
            if (!isBlock)
            {
                m_layout[line].continuationOf = first;
            }
            else if (starts_with_block_keyword(m_lines[line]))
            {
                // Nested blocks have placed their own keywords; the remaining
                // ones (else, catch, otherwise, end, ...) belong to this block.
                m_layout[line].depth = m_depth;
            }
        }
    }

    void body(const ast::Exp& e)
    {
        ++m_depth;
        if (e.isSeqExp())
        {
            e.accept(*this);
        }
        else
        {
            statement(e);
        }
        --m_depth;
    }

    void members(const ast::exps_t& exps)
    {
        ++m_depth;
        for (const ast::Exp* exp : exps)
        {
            if (exp != nullptr)
            {
                statement(*exp);
            }
        }
        --m_depth;
    }

    const ast::Exp* elseIfOf(const ast::Exp& elseBody) const
    {
        const ast::Exp* candidate = &elseBody;
        if (elseBody.isSeqExp() && elseBody.getExps().size() == 1)
        {
            candidate = elseBody.getExps().front();
        }
        if (candidate == nullptr || !candidate->isIfExp())
        {
            return nullptr;
        }

        int line = candidate->getLocation().first_line - 1;
        if (line < 0 || line >= static_cast<int>(m_lines.size()))
        {
            return nullptr;
        }
        const std::string& text = m_lines[line];
        size_t start = text.find_first_not_of(" \t");
        return start != std::string::npos && starts_with_word(text, start, "elseif")
            ? candidate : nullptr;
    }

    bool startsLine(int line, int column) const
    {
        size_t start = m_lines[line].find_first_not_of(" \t");
        return start != std::string::npos && static_cast<int>(start) + 1 == column;
    }

    /*
     * Line comments are not always statements of the tree, and block comments
     * never are: give such a comment line the depth of the next laid out line,
     * one deeper when that line closes or splits a block (the comment ends
     * the body above it). The closing line of a block comment follows its
     * opening line; the lines inside are left untouched.
     */
    void layoutComments()
    {
        int nextDepth = 0;
        bool nextIsKeyword = false;
        std::vector<int> commentDepths(m_lines.size(), -1);
        for (int line = static_cast<int>(m_lines.size()) - 1; line >= 0; --line)
        {
            if (m_layout[line].depth >= 0)
            {
                nextDepth = m_layout[line].depth;
                nextIsKeyword = starts_with_block_keyword(m_lines[line]);
            }
            else if (m_layout[line].continuationOf < 0)
            {
                commentDepths[line] = nextIsKeyword ? nextDepth + 1 : nextDepth;
            }
        }

        bool inBlockComment = false;
        int blockCommentDepth = -1;
        for (size_t line = 0; line < m_lines.size(); ++line)
        {
            const std::string& text = m_lines[line];
            size_t start = text.find_first_not_of(" \t");
            if (inBlockComment)
            {
                inBlockComment = text.find("*/") == std::string::npos;
                // The closing "*/" alone at the start of a line is aligned
                // with the opening "/*".
                if (!inBlockComment && start != std::string::npos
                        && text.compare(start, 2, "*/") == 0)
                {
                    m_layout[line].depth = blockCommentDepth;
                }
                continue;
            }
            if (start == std::string::npos || commentDepths[line] < 0)
            {
                continue;
            }
            if (text.compare(start, 2, "/*") == 0)
            {
                m_layout[line].depth = commentDepths[line];
                blockCommentDepth = commentDepths[line];
                inBlockComment = text.find("*/", start + 2) == std::string::npos;
            }
            else if (text.compare(start, 2, "//") == 0)
            {
                m_layout[line].depth = commentDepths[line];
            }
        }
    }

    const std::vector<std::string>& m_lines;
    std::vector<LineLayout> m_layout;
    int m_depth = 0;
    bool m_block = false;
};

}

std::vector<LineLayout> analyzeLayout(const ast::Exp& tree, const std::vector<std::string>& lines)
{
    FormattingVisitor visitor(lines);
    return visitor.collect(tree);
}
