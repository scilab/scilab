/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

#include "prettyprintvisitor.hxx"

#ifdef _MSC_VER

#include "Windows.h"

static WORD OutputReverse(WORD c)
{

    const WORD foreground_mask = FOREGROUND_BLUE | FOREGROUND_GREEN |
                                 FOREGROUND_RED | FOREGROUND_INTENSITY;
    const WORD background_mask = BACKGROUND_BLUE | BACKGROUND_GREEN |
                                 BACKGROUND_RED | BACKGROUND_INTENSITY;
    const WORD color_mask = foreground_mask | background_mask;

    WORD new_attributes =
        ((c & FOREGROUND_BLUE) ? BACKGROUND_BLUE : 0) |
        ((c & FOREGROUND_GREEN) ? BACKGROUND_GREEN : 0) |
        ((c & FOREGROUND_RED) ? BACKGROUND_RED : 0) |
        ((c & FOREGROUND_INTENSITY) ? BACKGROUND_INTENSITY : 0) |
        ((c & BACKGROUND_BLUE) ? FOREGROUND_BLUE : 0) |
        ((c & BACKGROUND_GREEN) ? FOREGROUND_GREEN : 0) |
        ((c & BACKGROUND_RED) ? FOREGROUND_RED : 0) |
        ((c & BACKGROUND_INTENSITY) ? FOREGROUND_INTENSITY : 0) |
        0;
    return ((c & ~color_mask) | (new_attributes & color_mask));
}
#else
const std::wstring clNORMAL = L"\033[0m";
const std::wstring clBOLD = L"\033[1m";
const std::wstring clRED = L"\033[91m";
const std::wstring clGREEN = L"\033[92m";
const std::wstring clYELLOW = L"\033[93m";
const std::wstring clBLUE = L"\033[94m";
const std::wstring clMAGENTA = L"\033[95m";
const std::wstring clCYAN = L"\033[96m";
const std::wstring clWHITE = L"\033[97m";
#endif

namespace ast
{
static int level = -1;

#ifdef _MSC_VER
std::wostream& operator<<(std::wostream& os, const TermColor& c)
{
    if (PrettyPrintVisitor::colored == false)
    {
        return os;
    }

    HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
    WORD color = 0;

    switch (c)
    {
        default:
        case NORMAL:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
            break;
        case BOLD:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case RED:
            color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case GREEN:
            color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case YELLOW:
            color = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case BLUE:
            color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case MAGENTA:
            color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case CYAN:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case WHITE:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case RESET:
        {
            color = 0;
            if (ConfigVariable::getScilabMode() == SCILAB_NW)
            {
                color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
            }
            else
            {
                color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
            }

            SetConsoleTextAttribute(h, color);
            return os;
        }
    }

    if (ConfigVariable::getScilabMode() == SCILAB_NW)
    {
        color = OutputReverse(color);
        color |= BACKGROUND_INTENSITY;
    }

    SetConsoleTextAttribute(h, color);

    return os;
}

#else
std::wostream& operator<<(std::wostream& os, const TermColor& c)
{
    if (PrettyPrintVisitor::colored == false)
    {
        return os;
    }

    switch (c)
    {
        case NORMAL:
        case RESET:
            os << clNORMAL;
            break;
        case BOLD:
            os << clBOLD;
            break;
        case RED:
            os << clRED;
            break;
        case GREEN:
            os << clGREEN;
            break;
        case YELLOW:
            os << clYELLOW;
            break;
        case BLUE:
            os << clBLUE;
            break;
        case MAGENTA:
            os << clMAGENTA;
            break;
        case CYAN:
            os << clCYAN;
            break;
        case WHITE:
            os << clWHITE;
            break;
    }
    return os;
}

#endif

bool PrettyPrintVisitor::colored = false;

void PrettyPrintVisitor::START_NODE(const ast::Ast & e)
{
    *ostr << NORMAL << L"(" << e.getNodeNumber() << L") ";
    ++level;
}

void PrettyPrintVisitor::END_NODE(void)
{
    --level;
    *ostr << RESET;
}

void PrettyPrintVisitor::print(const TermColor& c, const std::wstring & str)
{
    for (int i = 0 ; i < level; ++i)
    {
        *ostr << L"  ";
    }
    if (level > 0)
    {
        *ostr << L"     ";
    }
    *ostr << str << std::endl;
}

void PrettyPrintVisitor::print(const Location & loc)
{
    *ostr << L"@(" << YELLOW << loc.first_line << L"." << BLUE << loc.first_column << NORMAL << L" -> "
          << YELLOW << loc.last_line << L"." << BLUE << loc.last_column << NORMAL << L")";
}

void PrettyPrintVisitor::print(const TermColor& cpre, const std::wstring & pre, const Location & loc, const TermColor& cpost, const std::wstring & post, const TermColor& cdeco, const std::wstring & deco)
{
    for (int i = 0 ; i < level; ++i)
    {
        *ostr << L"  ";
    }
    if (level > 0)
    {
        *ostr << L"|_./ ";
    }

    *ostr << cpre << pre << NORMAL << L' ';
    print(loc);

    if (!post.empty())
    {
        *ostr << L" : " << cpost << post << NORMAL;
    }
    if (!deco.empty())
    {
        *ostr << L' ' << cdeco << deco << NORMAL;
    }
    *ostr << std::endl;
}

void PrettyPrintVisitor::print(const TermColor& c, const std::wstring & str, const Exp & e)
{
    std::wstring expType;
    expType = e.getTypeString();

    if (printDecoration)
    {
        std::wostringstream wos;
        wos << L"Deco(" << e.getDecorator() << L")";
        print(BOLD, expType, e.getLocation(), c, str, NORMAL, wos.str());
    }
    else
    {
        print(BOLD, expType, e.getLocation(), c, str, NORMAL, L"");
    }
}

void PrettyPrintVisitor::print(const Exp & e)
{
    print(NORMAL, L"", e);
}

void PrettyPrintVisitor::visit(const MatrixExp & e)
{
    START_NODE(e);
    print(e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const MatrixLineExp & e)
{
    START_NODE(e);
    print(e);
    exps_t columns = e.getColumns();
    for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const CellExp & e)
{
    START_NODE(e);
    print(e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const StringExp & e)
{
    START_NODE(e);
    std::wostringstream stream;
    if (e.getConstant())
    {
        printInternalType<types::String>(stream, e.getConstant());
    }
    else
    {
        stream << e.getValue();
    }

    print(RED, stream.str(), e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const CommentExp & e)
{
    START_NODE(e);
    print(GREEN, e.getComment(), e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const DoubleExp & e)
{
    START_NODE(e);
    std::wostringstream stream;
    types::InternalType * pIT = e.getConstant();
    if (pIT)
    {
        if (pIT->isImplicitList())
        {
            types::ImplicitList * pIL = static_cast<types::ImplicitList *>(pIT);
            stream << static_cast<types::Double *>(pIL->getStart())->get(0) << L":"
                   << static_cast<types::Double *>(pIL->getStep())->get(0) << L":"
                   << static_cast<types::Double *>(pIL->getEnd())->get(0);
        }
        else
        {
            printInternalType<types::Double>(stream, pIT);
        }
    }
    else
    {
        stream << e.getValue();
    }

    print(RED, stream.str(), e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const BoolExp & e)
{
    START_NODE(e);
    std::wostringstream stream;
    if (e.getConstant())
    {
        printInternalType<types::Bool>(stream, e.getConstant());
    }
    else
    {
        stream << e.getValue();
    }

    print(RED, stream.str(), e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const NilExp & e)
{
    START_NODE(e);
    print(e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const SimpleVar & e)
{
    START_NODE(e);
    std::wstring str;
    str = e.getSymbol().getName();
    if (printDecoration)
    {
        std::wstring ty;
        analysis::TIType type = e.getDecorator().getResult().getType();
        if (type.type != analysis::TIType::UNKNOWN)
        {
            if (type.isscalar())
            {
                ty = L" (" + analysis::TIType::toString(type.type) + L")";
            }
            else
            {
                ty = L" (" + analysis::TIType::toString(type.type) + L"*)";
            }
        }
        str += ty;
    }
    print(RED, str, e);

    END_NODE();
}

void PrettyPrintVisitor::visit(const ColonVar & e)
{
    START_NODE(e);
    print(e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const DollarVar & e)
{
    START_NODE(e);
    print(e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const ArrayListVar & e)
{
    START_NODE(e);
    print(e);
    exps_t vars = e.getVars();
    for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const FieldExp & e)
{
    START_NODE(e);
    print(e);
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const OpExp & e)
{
    START_NODE(e);
    print(RED, e.getString(), e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const LogicalOpExp & e)
{
    START_NODE(e);
    print(e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const AssignExp & e)
{
    START_NODE(e);
    print(e);
    e.getLeftExp().accept(*this);
    e.getRightExp().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const CellCallExp & e)
{
    START_NODE(e);
    print(e);
    e.getName().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void PrettyPrintVisitor::visit(const CallExp & e)
{
    START_NODE(e);
    print(e);
    e.getName().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void PrettyPrintVisitor::visit(const IfExp & e)
{
    START_NODE(e);
    print(e);
    e.getTest().accept(*this);
    e.getThen().accept(*this);
    if (e.hasElse())
    {
        e.getElse ().accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const TryCatchExp & e)
{
    START_NODE(e);
    print(e);
    e.getTry().accept(*this);
    e.getCatch().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const WhileExp & e)
{
    START_NODE(e);
    print(e);
    e.getTest().accept(*this);
    e.getBody().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const ForExp & e)
{
    START_NODE(e);
    print(e);
    e.getVardec().accept(*this);
    e.getBody().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const ContinueExp & e)
{
    START_NODE(e);
    print(e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const BreakExp & e)
{
    START_NODE(e);
    print(e);
    END_NODE();
}

void PrettyPrintVisitor::visit(const ReturnExp & e)
{
    START_NODE(e);
    print(e);
    if (!e.isGlobal())
    {
        e.getExp().accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const SelectExp & e)
{
    START_NODE(e);
    print(e);
    e.getSelect()->accept(*this);

    exps_t cases = e.getCases();
    for (auto exp : cases)
    {
        exp->accept(*this);
    }

    if (e.getDefaultCase() != NULL)
    {
        e.getDefaultCase()->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const CaseExp & e)
{
    START_NODE(e);
    print(e);
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const SeqExp & e)
{
    START_NODE(e);
    print(e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const ArrayListExp & e)
{
    START_NODE(e);
    print(e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const AssignListExp & e)
{
    START_NODE(e);
    print(e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void PrettyPrintVisitor::visit(const NotExp & e)
{
    START_NODE(e);
    print(e);
    e.getExp().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const TransposeExp & e)
{
    START_NODE(e);
    print(e);
    e.getExp().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const VarDec & e)
{
    std::wstring sym, name;
    sym = L"Symbol";
    name = e.getSymbol().getName();

    START_NODE(e);
    print(e);
    START_NODE(e);
    print(BOLD, sym, e.getLocation(), RED, name, NORMAL, L"");
    END_NODE();
    e.getInit().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const FunctionDec & e)
{
    START_NODE(e);
    print(e);
    // FIXME

    // First ask if there are some return values.
    //visit(e.returns_get());

    // Then get the function name
    //visit(e.getName());

    // Then get function args
    //visit(e.args_get());

    // Now debug function body
    e.getBody().accept(*this);

    END_NODE();
}

void PrettyPrintVisitor::visit(const ListExp &e)
{
    START_NODE(e);
    print(e);
    e.getStart().accept(*this);
    e.getStep().accept(*this);
    e.getEnd().accept(*this);
    END_NODE();
}

void PrettyPrintVisitor::visit(const OptimizedExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrettyPrintVisitor::visit(const MemfillExp &e)
{
    START_NODE(e);
    print(e);
    e.getValue().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void PrettyPrintVisitor::visit(const DAXPYExp &e)
{
    START_NODE(e);
    print(e);
    e.getA().accept(*this);
    e.getX().accept(*this);
    e.getY().accept(*this);
    END_NODE();

    //e.getOriginal()->accept(*this);
}

void PrettyPrintVisitor::visit(const IntSelectExp & e)
{
    e.getOriginal()->accept(*this);
}

void PrettyPrintVisitor::visit(const StringSelectExp & e)
{
    e.getOriginal()->accept(*this);
}

}
