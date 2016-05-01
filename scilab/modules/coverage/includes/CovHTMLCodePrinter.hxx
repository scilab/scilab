/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __COV_HTML_CODE_PRINTER_HXX__
#define __COV_HTML_CODE_PRINTER_HXX__

#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CodePrinter.hxx"
#include "CoverResult.hxx"
#include "URLEncoder.hxx"
#include "MacroLoc.hxx"

namespace coverage
{


class CovHTMLCodePrinter : public CodePrinter
{

    std::wostringstream & out;
    unsigned int indentLevel;
    unsigned int counter;
    unsigned lineCount;
    bool isNewLine;
    const ast::Exp * current;
    const ast::Exp * last;
    std::map<MacroLoc, CoverResult> & results;
    std::unordered_set<std::wstring> locals;
    std::stack<std::pair<MacroLoc, CoverResult *>> fnStack;
    unsigned int fnId;

public:

    CovHTMLCodePrinter(std::wostringstream & _out, std::map<MacroLoc, CoverResult> & _results) : out(_out), indentLevel(0), counter(0), lineCount(0), isNewLine(true), current(nullptr), last(nullptr), results(_results), fnId(0)
    {
    }

    void handleDefault(const std::wstring & seq) override;
    void handleOperator(const std::wstring & seq) override;
    void handleOpenClose(const std::wstring & seq) override;
    void handleFunctionKwds(const std::wstring & seq) override;
    void handleStructureKwds(const std::wstring & seq) override;
    void handleControlKwds(const std::wstring & seq) override;
    void handleConstants(const std::wstring & seq) override;
    void handleCommands(const std::wstring & seq) override;
    void handleMacros(const std::wstring & seq) override;
    void handleFunctionName(const std::wstring & seq) override;
    void handleFunctionNameDec(const std::wstring & seq) override;
    void handleName(const std::wstring & seq) override;
    void handleInOutArgsDec(const std::wstring & seq) override;
    void handleInOutArgs(const std::wstring & seq) override;
    void handleNumber(const std::wstring & seq) override;
    void handleSpecial(const std::wstring & seq) override;
    void handleString(const std::wstring & seq) override;
    void handleNothing(const std::wstring & seq) override;
    void handleField(const std::wstring & seq) override;
    void handleComment(const std::wstring & seq) override;
    void handleNewLine() override;
    void handleExpStart(const ast::Exp * e) override;
    void handleExpEnd(const ast::Exp * e) override;

    inline std::size_t getIndentSize() const
    {
        return indentLevel * 4;
    }

    inline void incIndent()
    {
        ++indentLevel;
    }

    inline void decIndent()
    {
        --indentLevel;
    }

    inline unsigned int getLineCharCount() const
    {
        return counter;
    }

    inline void close()
    {
        if (lineCount)
        {
            out << L"</pre></td><td></td><td></td><td></td>\n</tr>\n";
            out.flush();
        }
    }

    static std::wstring replaceByEntities(const std::wstring & seq);
    static void getDivPercent(std::wostringstream & out, const unsigned int percent);
    static std::wstring getOrderButton(const unsigned int tableid, const unsigned int id, const unsigned int col, const bool enabled);
    static void getFunctionStats(std::wostringstream & out, const MacroLoc & ml, const CoverResult & result);

private:

    void addNewLineHeader();

    inline const std::wstring & getCurrentFunctionName() const
    {
        return fnStack.top().first.name;
    }

    inline const MacroLoc & getCurrentMacro() const
    {
        return fnStack.top().first;
    }

    inline CoverResult & getCurrentResult() const
    {
        return *fnStack.top().second;
    }

    inline bool isInsideKnownFunction() const
    {
        return !fnStack.empty() && fnStack.top().second != nullptr;
    }

    inline void count(const std::wstring & seq)
    {
        counter += seq.length();
    }

    inline static void pushEntity(std::vector<wchar_t> & buf, const wchar_t * wstr, const unsigned char len)
    {
        for (unsigned int i = 0; i < len; ++i)
        {
            buf.push_back(wstr[i]);
        }
    }

};

} // namespace coverage


#endif // __COV_HTML_CODE_PRINTER_HXX__
