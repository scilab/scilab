/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CovHTMLCodePrinter.hxx"
#include "cover_tools.hxx"
#include "CoverModule.hxx"

namespace coverage
{

    void CovHTMLCodePrinter::handleDefault(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabdefault\'>"
            << replaceByEntities(seq)
            << "</span>";
    }

    void CovHTMLCodePrinter::handleOperator(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilaboperator\'>"
            << replaceByEntities(seq)
            << "</span>";
    }

    void CovHTMLCodePrinter::handleOpenClose(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabopenclose\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleFunctionKwds(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        if (seq == "function" && isInsideKnownFunction())
        {
            const std::string did = "d" + std::to_string(fnId);
            const std::string fid = "f" + std::to_string(fnId++);

            out << "<a class=\'linkStats\' onmouseover=\"show(\'" << did << "\',\'" << fid << "\')\" onmouseout=\"hide(\'" << did << "\')\">"
                << "<div id=\'" << did << "\' class=\'functionStats\'>";

            getFunctionStats(out, getCurrentMacro(), getCurrentResult());

            out << "</div>"
                << "<span id=\'" << fid << "' class=\'scilabfkeyword\'>function</span></a>";
        }
        else
        {
            out << "<span class=\'scilabfkeyword\'>"
                << seq
                << "</span>";
        }
    }

    void CovHTMLCodePrinter::handleStructureKwds(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabskeyword\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleControlKwds(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabckeyword\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleConstants(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabconstants\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleCommands(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabcommand\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleMacros(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabmacro\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleFunctionName(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabfunctionid\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleFunctionNameDec(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabfunctionid\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleName(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        if (locals.find(seq) != locals.end())
        {
            out << "<span class=\'scilabinputoutputargs\'>"
                << seq
                << "</span>";
        }
        else
        {
            out << "<span class=\'scilabid\'>"
                << seq
                << "</span>";
        }
    }

    void CovHTMLCodePrinter::handleInOutArgsDec(const std::string& seq)
    {
        locals.emplace(seq);
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabinputoutputargs\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleInOutArgs(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabinputoutputargs\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleNumber(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabnumber\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleSpecial(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabspecial\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleString(const std::string& seq)
    {
        addNewLineHeader();
        out << "<span class=\'scilabstring\'>";
        for (const auto c : seq)
        {
            if (c == '\'')
            {
                out << "&#0039;&#0039;";
                counter += 2;
            }
            else if (c == '\"')
            {
                out << "&#0034;&#0034;";
                counter += 2;
            }
            else
            {
                if (c == '<')
                {
                    out << "&#0060;";
                }
                else if (c == '>')
                {
                    out << "&#0062;";
                }
                else if (c == '&')
                {
                    out << "&#0038;";
                }
                else
                {
                    out << c;
                }
                ++counter;
            }
        }
        out << "</span>";
    }

    void CovHTMLCodePrinter::handleNothing(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << replaceByEntities(seq);
    }

    void CovHTMLCodePrinter::handleField(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabfield\'>"
            << seq
            << "</span>";
    }

    void CovHTMLCodePrinter::handleComment(const std::string& seq)
    {
        addNewLineHeader();
        count(seq);
        out << "<span class=\'scilabcomment\'>"
            << replaceByEntities(seq)
            << "</span>";
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
                    out << "</pre></td><td></td>\n";
                }
                else
                {
		    if (getCurrentResult().getNanoTime())
		    {
			if (last->isFunctionDec())
			{
			    out << "</pre></td><td style=\'background-color: rgba(83, 232, 37, 1);\' class=\'time\'>" << getCurrentResult().getStringTime() << "</td>\n";
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
			    const std::string color = "rgba(" + std::to_string(R) + "," + std::to_string(G) + "," + std::to_string(B) + ",1);";
			    const uint64_t time = getCurrentResult().getNanoTime(last);
			    if (time)
			    {
				out << "</pre></td><td style=\'background-color: " << color << "\' class=\'time\'>" << getCurrentResult().getStringTime(time) << "</td>\n";
			    }
			    else
			    {
				out << "</pre></td><td></td>\n";
			    }
			}
		    }
		    else
		    {
			out << "</pre></td><td></td>\n";
		    }
		}

                if (last->isIfExp())
                {
                    const std::vector<uint64_t> stats = getCurrentResult().getBranchesStats(last);
                    if (!stats.empty())
                    {
                        const uint64_t total = stats[0] + stats[1];
                        double thenbr = (double)stats[0];
                        double elsebr = (double)stats[1];
                        if (total)
                        {
                            thenbr = std::round(100 * thenbr / (double)total);
                            elsebr = 100 - thenbr;
                        }
                        const std::string thencls = stats[0] == 0 ? "null_stats" : "stats";
                        const std::string elsecls = stats[1] == 0 ? "null_stats" : "stats";

                        out << "<td class=\'" << thencls << "\'>" << thenbr << "%</td><td class=\'" << elsecls << "\'>" << elsebr << "%</td>\n</tr>\n";
                        printed = true;
                    }
                }
                else if (last->isForExp() || last->isWhileExp())
                {
                    uint64_t count;
                    if (getCurrentResult().getLoopStats(last, count))
                    {
                        std::string loopcls = count == 0 ? "null_stats" : "stats";
                        out << "<td class=\'" << loopcls << "\' colspan=\'2\'>&#xD7;" << count << "</td>\n</tr>\n";
                        printed = true;
                    }
                }

                if (!printed)
                {
                    out << "<td></td><td></td></tr>\n";
                    printed = true;
                }
            }
            if (!printed)
            {
                out << "</pre></td><td></td><td></td><td></td></tr>\n";
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
            const std::string& name = static_cast<const ast::FunctionDec *>(e)->getSymbol().getName();
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
            out << "<tr class=\'";
            if (current && isInsideKnownFunction())
            {
                if (current->isCommentExp())
                {
                    out << "none";
                }
                else if (current->isFunctionDec())
                {
                    out << (getCurrentResult().getCounter() ? "cover" : "uncover");
                }
		else if (current->isCaseExp())
                {
		    out << (getCurrentResult().isCovered(static_cast<const ast::CaseExp *>(current)->getTest()) ? "cover" : "uncover");
		}
                else
                {
                    out << (getCurrentResult().isCovered(current) ? "cover" : "uncover");
                }
            }
            else
            {
                out << "none";
            }
            ++lineCount;
            out <<  "\' id=\'L" << lineCount << "\'>\n"
                << "<td class=\'num\'><a href=\'#L" << lineCount << "\'>" << lineCount << "</a></td>\n"
                << "<td class=\'src\'><pre>" << std::string(indent, ' ');

            counter = (unsigned int)indent;
            isNewLine = false;
        }
        current = nullptr;
    }

    void CovHTMLCodePrinter::getFunctionStats(std::ostringstream & out, const MacroLoc & ml, const CoverResult & result)
    {
        const CoverMacroInfo & info = result.getInfo();
        const std::size_t instrs = info.instrsCount;
        const unsigned int percentInstrs = result.getCovInstrsPercent();
        const std::size_t branches = info.branchesCount;
        const unsigned int percentBranches = result.getCovBranchesPercent();

        out << "<table class=\'functionInfo\'>"
            << "<tr><td colspan=\'5\'>Macro <span class=\'scilabfunctionid\'>" << ml.name << "</span>:</td></tr>"
            << "<tr><td>&nbsp;&mdash;&nbsp;called:</td><td>" << result.getCounter() << " time" << tools::getPlural(result.getCounter()) << "</td><td>&nbsp;&mdash;&nbsp;spent time:</td><td>" << result.getStringTime() << "</td></tr>"
            << "<tr><td>&nbsp;&mdash;&nbsp;instrs:</td><td>" << instrs << "</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentInstrs << "%</td><td>";

        getDivPercent(out, percentInstrs);

        out << "</td></tr>"
            << "<tr><td>&nbsp;&mdash;&nbsp;branches:</td><td>" << branches << "</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentBranches << "%</td><td>";

        getDivPercent(out, percentBranches);

        out << "</td></tr>"
            << "</table>";
    }

    void CovHTMLCodePrinter::getDivPercent(std::ostringstream & out, const unsigned int percent)
    {
        if (percent == 0)
        {
            out << "<div class=\'fullPercent\'><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << "%;\'></div></div>";
        }
        else if (percent == 100)
        {
            out << "<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << "%;\'></div></div>";
        }
        else
        {
            out << "<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << "%;\'></div><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << "%;\'></div></div>";
        }
    }

    std::string CovHTMLCodePrinter::getOrderButton(const unsigned int tableid, const unsigned int id, const unsigned int col, const bool enabled)
    {
        std::ostringstream wos;
        const std::string str = enabled ?  "buttonOrderEnable" : "buttonOrderDisable";
        wos << "<span class=\'groupButton\'>"
            << "<a id=\'but_1_" << id << "\' class=\'" << str << "\' onclick=\"order(\'table" << tableid << "\'," << col << ",true,\'but_1_" << id << "\')\">&#x25B4;</a>"
            << "<a id=\'but_2_" << id << "\' class=\'buttonOrderDisable\' onclick=\"order(\'table" << tableid << "\'," << col << ",false,\'but_2_" << id << "\')\">&#x25BE;</a>"
            << "</span>";

        return wos.str();
    }

    std::string CovHTMLCodePrinter::replaceByEntities(const std::string& seq)
    {
        std::vector<char> buf;
        // if seq contains 100 chars and there are 8 special chars
        // then it will contain after the loop 148 chars.
        // So we should avoid the buf resizment.
        buf.reserve((size_t)(1.5 * seq.length()));
        for (auto c : seq)
        {
            if (c == '<')
            {
                pushEntity(buf, "&#0060;", 7);
            }
            else if (c == '>')
            {
                pushEntity(buf, "&#0062;", 7);
            }
            else if (c == '\'')
            {
                pushEntity(buf, "&#0039;", 7);
            }
            else if (c == '\"')
            {
                pushEntity(buf, "&#0034;", 7);
            }
            else if (c == '&')
            {
                pushEntity(buf, "&#0038;", 7);
            }
            else
            {
                buf.push_back(c);
            }
        }

        return std::string(buf.begin(), buf.end());
    }
}
