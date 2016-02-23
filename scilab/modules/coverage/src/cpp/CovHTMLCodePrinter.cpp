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

#include "CovHTMLCodePrinter.hxx"
#include "cover_tools.hxx"
#include "CoverModule.hxx"

namespace coverage
{

void CovHTMLCodePrinter::handleDefault(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabdefault\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleOperator(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilaboperator\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleOpenClose(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabopenclose\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    if (seq == L"function" && isInsideKnownFunction())
    {
        const std::wstring did = L"d" + std::to_wstring(fnId);
        const std::wstring fid = L"f" + std::to_wstring(fnId++);

        out << L"<a class=\'linkStats\' onmouseover=\"show(\'" << did << L"\',\'" << fid << L"\')\" onmouseout=\"hide(\'" << did << L"\')\">"
            << L"<div id=\'" << did << L"\' class=\'functionStats\'>";

        getFunctionStats(out, getCurrentMacro(), getCurrentResult());

        out << L"</div>"
            << L"<span id=\'" << fid << L"' class=\'scilabfkeyword\'>function</span></a>";
    }
    else
    {
        out << L"<span class=\'scilabfkeyword\'>"
            << seq
            << L"</span>";
    }
}

void CovHTMLCodePrinter::handleStructureKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabskeyword\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleControlKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabckeyword\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleConstants(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabconstants\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleCommands(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabcommand\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleMacros(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabmacro\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionName(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfunctionid\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionNameDec(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfunctionid\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleName(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    if (locals.find(seq) != locals.end())
    {
        out << L"<span class=\'scilabinputoutputargs\'>"
            << seq
            << L"</span>";
    }
    else
    {
        out << L"<span class=\'scilabid\'>"
            << seq
            << L"</span>";
    }
}

void CovHTMLCodePrinter::handleInOutArgsDec(const std::wstring & seq)
{
    locals.emplace(seq);
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabinputoutputargs\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleInOutArgs(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabinputoutputargs\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleNumber(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabnumber\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleSpecial(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabspecial\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleString(const std::wstring & seq)
{
    addNewLineHeader();
    out << L"<span class=\'scilabstring\'>";
    for (const auto c : seq)
    {
        if (c == L'\'')
        {
            out << L"&#0039;&#0039;";
            counter += 2;
        }
        else if (c == L'\"')
        {
            out << L"&#0034;&#0034;";
            counter += 2;
        }
        else
        {
            if (c == L'<')
            {
                out << L"&#0060;";
            }
            else if (c == L'>')
            {
                out << L"&#0062;";
            }
            else if (c == L'&')
            {
                out << L"&#0038;";
            }
            else
            {
                out << c;
            }
            ++counter;
        }
    }
    out << L"</span>";
}

void CovHTMLCodePrinter::handleNothing(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << replaceByEntities(seq);
}

void CovHTMLCodePrinter::handleField(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfield\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleComment(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabcomment\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleNewLine()
{
    bool printed = false;
    if (lineCount)
    {
        if (last && isInsideKnownFunction())
        {
            if (last->isCommentExp())
            {
                out << L"</pre></td><td></td>\n";
            }
            else
            {
                if (getCurrentResult().getNanoTime())
                {
                    if (last->isFunctionDec())
                    {
                        out << L"</pre></td><td style=\'background-color: rgba(83, 232, 37, 1);\' class=\'time\'>" << getCurrentResult().getStringTime() << L"</td>\n";
                    }
                    else
                    {
                        unsigned int R = 255 - 83;
                        unsigned int G = 255 - 232;
                        unsigned int B = 255 - 37;
                        const double ratio = getCurrentResult().getTimeRatio(last);
                        R = 255 - (unsigned int)std::round((double)R * ratio);
                        G = 255 - (unsigned int)std::round((double)G * ratio);
                        B = 255 - (unsigned int)std::round((double)B * ratio);
                        const std::wstring color = L"rgba(" + std::to_wstring(R) + L"," + std::to_wstring(G) + L"," + std::to_wstring(B) + L",1);";
                        const uint64_t time = getCurrentResult().getNanoTime(last);
                        if (time)
                        {
                            out << L"</pre></td><td style=\'background-color: " << color << L"\' class=\'time\'>" << getCurrentResult().getStringTime(time) << L"</td>\n";
                        }
                        else
                        {
                            out << L"</pre></td><td></td>\n";
                        }
                    }
                }
                else
                {
                    out << L"</pre></td><td></td>\n";
                }
            }

            if (last->isIfExp())
            {
                const std::vector<uint64_t> stats = getCurrentResult().getBranchesStats(last);
                if (!stats.empty())
                {
                    const uint64_t total = stats[0] + stats[1];
                    double thenbr = stats[0];
                    double elsebr = stats[1];
                    if (total)
                    {
                        thenbr = std::round(100 * thenbr / (double)total);
                        elsebr = 100 - thenbr;
                    }
                    const std::wstring thencls = stats[0] == 0 ? L"null_stats" : L"stats";
                    const std::wstring elsecls = stats[1] == 0 ? L"null_stats" : L"stats";

                    out << L"<td class=\'" << thencls << L"\'>" << thenbr << L"%</td><td class=\'" << elsecls << L"\'>" << elsebr << L"%</td>\n</tr>\n";
                    printed = true;
                }
            }
            else if (last->isForExp() || last->isWhileExp())
            {
                uint64_t count;
                if (getCurrentResult().getLoopStats(last, count))
                {
                    std::wstring loopcls = count == 0 ? L"null_stats" : L"stats";
                    out << L"<td class=\'" << loopcls << L"\' colspan=\'2\'>&#xD7;" << count << L"</td>\n</tr>\n";
                    printed = true;
                }
            }

            if (!printed)
            {
                out << L"<td></td><td></td></tr>\n";
                printed = true;
            }
        }
        if (!printed)
        {
            out << L"</pre></td><td></td><td></td><td></td></tr>\n";
        }
        out.flush();
    }

    last = nullptr;
    isNewLine = true;
}

void CovHTMLCodePrinter::handleExpStart(const ast::Exp * e)
{
    current = e;
    if (!last && !e->isSeqExp())
    {
        last = e;
    }
    if (e->isFunctionDec())
    {
        const std::wstring & name = static_cast<const ast::FunctionDec *>(e)->getSymbol().getName();
        const Location & loc = static_cast<const ast::FunctionDec *>(e)->getBody().getLocation();
        MacroLoc ml(name, loc);
        auto i = results.find(ml);
        if (i != results.end())
        {
            fnStack.emplace(ml, &i->second);
        }
        else
        {
            fnStack.emplace(ml, nullptr);
        }
    }
}

void CovHTMLCodePrinter::handleExpEnd(const ast::Exp * e)
{
    if (e->isFunctionDec())
    {
        fnStack.pop();
    }
}

void CovHTMLCodePrinter::addNewLineHeader()
{
    if (isNewLine)
    {
        const std::size_t indent = getIndentSize();
        out << L"<tr class=\'";
        if (current && isInsideKnownFunction())
        {
            if (current->isCommentExp())
            {
                out << L"none";
            }
            else if (current->isFunctionDec())
            {
                out << (getCurrentResult().getCounter() ? L"cover" : L"uncover");
            }
            else if (current->isCaseExp())
            {
                out << (getCurrentResult().isCovered(static_cast<const ast::CaseExp *>(current)->getTest()) ? L"cover" : L"uncover");
            }
            else
            {
                out << (getCurrentResult().isCovered(current) ? L"cover" : L"uncover");
            }
        }
        else
        {
            out << L"none";
        }
        ++lineCount;
        out <<  L"\' id=\'L" << lineCount << L"\'>\n"
            << L"<td class=\'num\'><a href=\'#L" << lineCount << L"\'>" << lineCount << L"</a></td>\n"
            << L"<td class=\'src\'><pre>" << std::wstring(indent, L' ');

        counter = indent;
        isNewLine = false;
    }
    current = nullptr;
}

void CovHTMLCodePrinter::getFunctionStats(std::wostringstream & out, const MacroLoc & ml, const CoverResult & result)
{
    const CoverMacroInfo & info = result.getInfo();
    const std::size_t instrs = info.instrsCount;
    const unsigned int percentInstrs = result.getCovInstrsPercent();
    const std::size_t branches = info.branchesCount;
    const unsigned int percentBranches = result.getCovBranchesPercent();

    out << L"<table class=\'functionInfo\'>"
        << L"<tr><td colspan=\'5\'>Macro <span class=\'scilabfunctionid\'>" << ml.name << L"</span>:</td></tr>"
        << L"<tr><td>&nbsp;&mdash;&nbsp;called:</td><td>" << result.getCounter() << L" time" << tools::getPlural(result.getCounter()) << L"</td><td>&nbsp;&mdash;&nbsp;spent time:</td><td>" << result.getStringTime() << L"</td></tr>"
        << L"<tr><td>&nbsp;&mdash;&nbsp;instrs:</td><td>" << instrs << L"</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentInstrs << L"%</td><td>";

    getDivPercent(out, percentInstrs);

    out << L"</td></tr>"
        << L"<tr><td>&nbsp;&mdash;&nbsp;branches:</td><td>" << branches << L"</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentBranches << L"%</td><td>";

    getDivPercent(out, percentBranches);

    out << L"</td></tr>"
        << L"</table>";
}

void CovHTMLCodePrinter::getDivPercent(std::wostringstream & out, const unsigned int percent)
{
    if (percent == 0)
    {
        out << L"<div class=\'fullPercent\'><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << L"%;\'></div></div>";
    }
    else if (percent == 100)
    {
        out << L"<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << L"%;\'></div></div>";
    }
    else
    {
        out << L"<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << L"%;\'></div><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << L"%;\'></div></div>";
    }
}

std::wstring CovHTMLCodePrinter::getOrderButton(const unsigned int tableid, const unsigned int id, const unsigned int col, const bool enabled)
{
    std::wostringstream wos;
    const std::wstring str = enabled ?  L"buttonOrderEnable" : L"buttonOrderDisable";
    wos << L"<span class=\'groupButton\'>"
        << L"<a id=\'but_1_" << id << L"\' class=\'" << str << L"\' onclick=\"order(\'table" << tableid << L"\'," << col << L",true,\'but_1_" << id << L"\')\">&#x25B4;</a>"
        << L"<a id=\'but_2_" << id << L"\' class=\'buttonOrderDisable\' onclick=\"order(\'table" << tableid << "\'," << col << L",false,\'but_2_" << id << L"\')\">&#x25BE;</a>"
        << L"</span>";

    return wos.str();
}

std::wstring CovHTMLCodePrinter::replaceByEntities(const std::wstring & seq)
{
    std::vector<wchar_t> buf;
    // if seq contains 100 chars and there are 8 special chars
    // then it will contain after the loop 148 chars.
    // So we should avoid the buf resizment.
    buf.reserve(1.5 * seq.length());
    for (auto c : seq)
    {
        if (c == L'<')
        {
            pushEntity(buf, L"&#0060;", 7);
        }
        else if (c == L'>')
        {
            pushEntity(buf, L"&#0062;", 7);
        }
        else if (c == L'\'')
        {
            pushEntity(buf, L"&#0039;", 7);
        }
        else if (c == L'\"')
        {
            pushEntity(buf, L"&#0034;", 7);
        }
        else if (c == L'&')
        {
            pushEntity(buf, L"&#0038;", 7);
        }
        else
        {
            buf.push_back(c);
        }
    }

    return std::wstring(buf.begin(), buf.end());
}
}
