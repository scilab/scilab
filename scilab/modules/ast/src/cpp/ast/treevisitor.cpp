/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "treevisitor.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "token.hxx"

extern "C"
{
#include "os_string.h"
#include "os_wcstok.h"
}

namespace ast
{
void TreeVisitor::visit(const SeqExp  &e)
{
    types::List* lst = new types::List();
    lst->append(getEOL());

    int last_line = -1;
    for (auto it : e.getExps())
    {
        int first = it->getLocation().first_line;
        while (last_line != -1 && last_line < first)
        {
            lst->append(getEOL());
            ++last_line;
        }

        last_line = it->getLocation().last_line;

        it->accept(*this);
        if (it->isAssignExp() ||
                it->isCommentExp() ||
                it->isForExp() ||
                it->isWhileExp() ||
                it->isTryCatchExp() ||
                it->isSelectExp() ||
                it->isFunctionDec() ||
                it->isIfExp())
        {
            types::InternalType* tmp = getList();
            lst->append(tmp);
            tmp->killMe();
        }
        else
        {
            //add ans = before expression
            types::TList* tl = new types::TList();
            types::String* s = new types::String(1, 4);
            s->set(0, L"equal");
            s->set(1, L"expression");
            s->set(2, L"lhs");
            s->set(3, L"endsymbol");
            //header
            tl->append(s);
            s->killMe();
            //expression
            types::InternalType* tmp = getList();
            tl->append(tmp);
            tmp->killMe();
            //lhs
            types::List* lhs = new types::List();
            tmp = createVar(L"ans");
            lhs->append(tmp);
            tmp->killMe();
            tl->append(lhs);
            lhs->killMe();
            //endsymbol
            tl->append(getVerbose(*it));
            lst->append(tl);
            tl->killMe();
        }
    }

    lst->append(getEOL());
    l = lst;
}

void TreeVisitor::visit(const CommentExp  &e)
{
    types::TList* tl = new types::TList();
    types::String* s = new types::String(1, 2);
    s->set(0, L"comment");
    s->set(1, L"text");
    tl->append(s);
    tl->append(new types::String(e.getComment().c_str()));
    l = tl;
}

void TreeVisitor::visit(const SimpleVar &e)
{
    l = createVar(e.getSymbol().getName());
}

void TreeVisitor::visit(const DoubleExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const StringExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const BoolExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const DollarVar &/*e*/)
{
    l = createVar(L"$");
}

void TreeVisitor::visit(const ColonVar &/*e*/)
{
    l = createVar(L":");
}

void TreeVisitor::visit(const MatrixExp &e)
{
    exps_t lines = e.getLines();

    if (lines.size() == 0)
    {
        l = createConst(types::Double::Empty());
        return;
    }

    if (lines.size() == 1)
    {
        lines.front()->accept(*this);
        return;
    }

    types::List* sub = createOperation();
    types::List* ope = new types::List();

    int idx = 0;
    for (auto it : lines)
    {
        it->accept(*this);

        if (idx >= 2)
        {
            sub->append(ope);
            ope->killMe();
            sub->append(new types::String(L"cc"));

            //create a new operation
            //put previous stage in lhs and
            //result in rhs
            types::List* subcolcatOperation = createOperation();
            types::List* subcolcatOperands = new types::List();
            subcolcatOperands->append(sub);
            sub->killMe();
            //add EOL
            //subcolcatOperands->append(getEOL());
            types::InternalType* tmp = getList();
            subcolcatOperands->append(tmp);
            tmp->killMe();

            ope = subcolcatOperands;
            sub = subcolcatOperation;
        }
        else
        {
            types::InternalType* tmp = getList();
            ope->append(tmp);
            tmp->killMe();
        }

        ++idx;
    }
    sub->append(ope);
    ope->killMe();
    sub->append(new types::String(L"cc"));
    l = sub;
}

void TreeVisitor::visit(const MatrixLineExp &e)
{
    exps_t columns = e.getColumns();
    if (columns.size() == 1)
    {
        columns.front()->accept(*this);
        return;
    }

    types::List* sub = createOperation();
    types::List* ope = new types::List();

    int idx = 0;
    for (auto it : columns)
    {
        it->accept(*this);

        if (idx >= 2)
        {
            sub->append(ope);
            ope->killMe();
            sub->append(new types::String(L"rc"));
            //create a new operation
            //put previous stage in lhs and
            //result in rhs
            types::List* lst = createOperation();
            types::List* l2 = new types::List();
            l2->append(sub);
            sub->killMe();
            types::InternalType* tmp = getList();
            l2->append(tmp);
            tmp->killMe();

            //lst->append(tmp);
            ope = l2;
            sub = lst;
        }
        else
        {
            types::InternalType* tmp = getList();
            ope->append(tmp);
            tmp->killMe();
        }

        ++idx;
    }

    sub->append(ope);
    ope->killMe();
    sub->append(new types::String(L"rc"));
    l = sub;
}

void TreeVisitor::visit(const OpExp &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();

    if (e.getOper() != OpExp::unaryMinus)
    {
        e.getLeft().accept(*this);
        types::InternalType* tmp = getList();
        sub->append(tmp);
        tmp->killMe();
    }
    e.getRight().accept(*this);
    types::InternalType* tmp = getList();
    sub->append(tmp);
    tmp->killMe();
    ope->append(sub);
    sub->killMe();

    switch (e.getOper())
    {
        // Arithmetics.
        case OpExp::plus:
            tmp = new types::String(SCI_PLUS);
            break;
        case OpExp::unaryMinus:
        case OpExp::minus:
            tmp = new types::String(SCI_MINUS);
            break;
        case OpExp::times:
            tmp = new types::String(SCI_TIMES);
            break;
        case OpExp::rdivide:
            tmp = new types::String(SCI_RDIVIDE);
            break;
        case OpExp::ldivide:
            tmp = new types::String(SCI_LDIVIDE);
            break;
        case OpExp::power:
            tmp = new types::String(SCI_POWER);
            break;
        // Element wise.
        case OpExp::dottimes:
            tmp = new types::String(SCI_DOTTIMES);
            break;
        case OpExp::dotrdivide:
            tmp = new types::String(SCI_DOTRDIVIDE);
            break;
        case OpExp::dotldivide:
            tmp = new types::String(SCI_DOTLDIVIDE);
            break;
        case OpExp::dotpower:
            tmp = new types::String(SCI_DOTPOWER);
            break;
        // Kroneckers
        case OpExp::krontimes:
            tmp = new types::String(SCI_KRONTIMES);
            break;
        case OpExp::kronrdivide:
            tmp = new types::String(SCI_KRONRDIVIDE);
            break;
        case OpExp::kronldivide:
            tmp = new types::String(SCI_KRONLDIVIDE);
            break;
        // Control
        case OpExp::controltimes:
            tmp = new types::String(SCI_CONTROLTIMES);
            break;
        case OpExp::controlrdivide:
            tmp = new types::String(SCI_CONTROLRDIVIDE);
            break;
        case OpExp::controlldivide:
            tmp = new types::String(SCI_CONTROLLDIVIDE);
            break;
        // Comparisons
        case OpExp::eq:
            tmp = new types::String(SCI_EQ);
            break;
        case OpExp::ne:
            tmp = new types::String(SCI_NE);
            break;
        case OpExp::lt:
            tmp = new types::String(SCI_LT);
            break;
        case OpExp::le:
            tmp = new types::String(SCI_LE);
            break;
        case OpExp::gt:
            tmp = new types::String(SCI_GT);
            break;
        case OpExp::ge:
            tmp = new types::String(SCI_GE);
            break;
        default:
            tmp = new types::String(L"BAD OPERATOR");
            break;
    }

    ope->append(tmp);
    l = ope;
}

void TreeVisitor::visit(const LogicalOpExp &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();
    types::List* tmplst = nullptr;

    if (e.getOper() != OpExp::unaryMinus)
    {
        e.getLeft().accept(*this);
        tmplst = getList();
        sub->append(tmplst);
        tmplst->killMe();
    }
    e.getRight().accept(*this);
    tmplst = getList();
    sub->append(tmplst);
    tmplst->killMe();
    ope->append(sub);
    sub->killMe();

    types::InternalType* tmp = nullptr;
    switch (e.getOper())
    {
        case LogicalOpExp::logicalAnd:
            tmp = new types::String(SCI_AND);
            break;
        case LogicalOpExp::logicalOr:
            tmp = new types::String(SCI_OR);
            break;
        case LogicalOpExp::logicalShortCutAnd:
            tmp = new types::String(SCI_ANDAND);
            break;
        case LogicalOpExp::logicalShortCutOr:
            tmp = new types::String(SCI_OROR);
            break;
        default:
            tmp = new types::String(L"BAD LOGICAL OPERATOR");
            break;
    }

    ope->append(tmp);
    l = ope;
}

void TreeVisitor::visit(const IfExp  &e)
{
    types::TList* tl = new types::TList();
    bool hasElse = e.hasElse() && e.getElse().isCommentExp() == false;
    //header
    types::String* s = new types::String(1, 5);
    s->set(0, L"ifthenelse");
    s->set(1, L"expression");
    s->set(2, L"then");
    s->set(3, L"elseifs"); //always empty
    s->set(4, L"else");
    tl->append(s);

    //expression -> condition
    e.getTest().accept(*this);
    types::InternalType* tmp = getList();
    tl->append(tmp);
    tmp->killMe();

    //then
    e.getThen().accept(*this);
    tmp = getList();
    tl->append(tmp);
    tmp->killMe();

    //elseif
    tmp = new types::List();
    tl->append(tmp);
    tmp->killMe();

    //else
    if (hasElse)
    {
        e.getElse().accept(*this);
        tmp = getList();
        tl->append(tmp);
        tmp->killMe();
    }
    else
    {
        tmp = new types::List();
        tl->append(tmp);
        tmp->killMe();
    }
    l = tl;
}

void TreeVisitor::visit(const ListExp  &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();

    e.getStart().accept(*this);
    types::InternalType* tmp = getList();
    sub->append(tmp);
    tmp->killMe();

    if (e.hasExplicitStep())
    {
        e.getStep().accept(*this);
        tmp = getList();
        sub->append(tmp);
        tmp->killMe();
    }

    e.getEnd().accept(*this);
    tmp = getList();
    sub->append(tmp);
    tmp->killMe();

    ope->append(sub);
    sub->killMe();
    tmp = new types::String(L":");
    ope->append(tmp);
    tmp->killMe();
    l = ope;
}

void TreeVisitor::visit(const AssignExp &e)
{
    types::List* assign = createAssign();

    //expression : what to assign
    e.getRightExp().accept(*this);
    types::List* tmp = getList();
    assign->append(tmp);

    double* dlhs = NULL;
    if (e.getRightExp().isCallExp())
    {
        types::TList* tl = getList()->getAs<types::TList>();
        types::Double* lhsnb = tl->get(tl->getSize() - 1)->getAs<types::Double>();
        dlhs = lhsnb->get();
    }

    tmp->killMe();

    //lhs : who to assign
    Exp& left = e.getLeftExp();
    if (left.isSimpleVar())
    {
        left.accept(*this);
        types::List* lhs = new types::List();
        tmp = getList();
        lhs->append(tmp);
        tmp->killMe();
        assign->append(lhs);
        lhs->killMe();
        if (dlhs)
        {
            dlhs[0] = 1;//lhs = 1
        }
    }

    if (left.isCellCallExp())
    {
        //not yet manage
    }

    if (left.isCallExp())
    {
        CallExp* call = left.getAs<CallExp>();
        types::List* operation = createOperation();
        types::List* lhs = new types::List();
        //varname
        call->getName().accept(*this);
        tmp = getList();
        lhs->append(tmp);
        tmp->killMe();

        //indexes
        ast::exps_t args = call->getArgs();
        for (auto arg : args)
        {
            arg->accept(*this);
            tmp = getList();
            lhs->append(tmp);
            tmp->killMe();
        }

        if (dlhs)
        {
            dlhs[0] = 1;//lhs = 1
        }
        operation->append(lhs);
        lhs->killMe();

        //operator
        operation->append(new types::String(L"ins"));
        types::List* lst = new types::List();
        lst->append(operation);
        operation->killMe();
        assign->append(lst);
        lst->killMe();
    }

    if (left.isAssignListExp())
    {
        AssignListExp* lst = left.getAs<AssignListExp>();
        types::List* lhs = new types::List();
        for (auto exp : lst->getExps())
        {
            exp->accept(*this);
            tmp = getList();
            lhs->append(tmp);
            tmp->killMe();
        }
        if (dlhs)
        {
            dlhs[0] = static_cast<double>(lst->getExps().size());
        }

        assign->append(lhs);
        lhs->killMe();
    }

    if (left.isFieldExp())
    {
        FieldExp* field = left.getAs<FieldExp>();
        types::List* ins = createOperation();
        types::List* lhs = new types::List();

        field->getHead()->accept(*this);
        tmp = getList();
        lhs->append(tmp);
        tmp->killMe();

        if (field->getTail()->isSimpleVar())
        {
            SimpleVar* pVar = field->getTail()->getAs<SimpleVar>();
            tmp = createConst(new types::String(pVar->getSymbol().getName().c_str()));
            lhs->append(tmp);
            tmp->killMe();
        }
        else
        {
            //never occur ?
            field->accept(*this);
            tmp = getList();
            lhs->append(tmp);
            tmp->killMe();
        }

        ins->append(lhs);
        lhs->killMe();
        //operator
        ins->append(new types::String(L"ins"));
        tmp = new types::List();
        tmp->append(ins);
        ins->killMe();
        assign->append(tmp);
        tmp->killMe();
    }

    //verbose ?
    assign->append(getVerbose(e));
    l = assign;
}

void TreeVisitor::visit(const CallExp &e)
{
    if (e.getName().isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e.getName());

        types::TList* call = new types::TList();

        //header
        types::String* varstr = new types::String(1, 4);
        varstr->set(0, L"funcall");
        varstr->set(1, L"rhs");
        varstr->set(2, L"name");
        varstr->set(3, L"lhsnb");
        call->append(varstr);

        //rhs
        types::List* rhs = new types::List();
        ast::exps_t args = e.getArgs();
        for (auto arg : args)
        {
            arg->accept(*this);
            types::List* tmp = getList();
            rhs->append(tmp);
            tmp->killMe();
        }

        call->append(rhs);
        rhs->killMe();

        //name
        const std::wstring & name = var.getSymbol().getName();
        call->append(new types::String(name.c_str()));

        //lhsnb
        //use default value 1
        //parent exp like assign can adapt it.
        call->append(new types::Double(1));

        l = call;
    }
    else
    {
        //not yet managed
    }
}

void TreeVisitor::visit(const ForExp &e)
{
    types::TList* f = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"for");
    varstr->set(1, L"expression");
    varstr->set(2, L"statements");
    f->append(varstr);

    //expression
    //create a AssignExp to call visitor it
    VarDec* vardec = e.getVardec().getAs<VarDec>();
    SimpleVar* var = new SimpleVar(vardec->getLocation(), vardec->getSymbol());
    Exp* init = vardec->getInit().clone();
    AssignExp* assign = new AssignExp(vardec->getLocation(), *var, *init);
    assign->setVerbose(true);

    assign->accept(*this);
    types::List* tmp = getList();
    f->append(tmp);
    tmp->killMe();
    delete assign;

    //statements
    e.getBody().accept(*this);
    tmp = getList();
    f->append(tmp);
    tmp->killMe();
    l = f;
}

void TreeVisitor::visit(const FieldExp &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getHead()->accept(*this);
    types::List* tmp = getList();
    ope->append(tmp);
    tmp->killMe();

    //arguments
    if (e.getTail()->isSimpleVar())
    {
        const SimpleVar* pVar = e.getTail()->getAs<SimpleVar>();
        tmp = createConst(new types::String(pVar->getSymbol().getName().c_str()));
        ope->append(tmp);
        tmp->killMe();
    }
    else
    {
        tmp = getList();
        ope->append(tmp);
        tmp->killMe();
    }

    //operator
    ext->append(ope);
    ope->killMe();
    ext->append(new types::String(L"ext"));
    l = ext;
}

void TreeVisitor::visit(const TryCatchExp &e)
{
    types::TList* tc = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"trycatch");
    varstr->set(1, L"trystat");
    varstr->set(2, L"catchstat");
    tc->append(varstr);

    //trystat
    e.getTry().accept(*this);
    types::List* tmp = getList();
    tc->append(tmp);
    tmp->killMe();

    //catchstat
    e.getCatch().accept(*this);
    tmp = getList();
    tc->append(tmp);
    tmp->killMe();

    l = tc;
}

void TreeVisitor::visit(const WhileExp &e)
{
    types::TList* w = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"while");
    varstr->set(1, L"expression");
    varstr->set(2, L"statements");
    w->append(varstr);

    //expression
    e.getTest().accept(*this);
    types::List* tmp = getList();
    w->append(tmp);
    tmp->killMe();

    //statements
    e.getBody().accept(*this);
    tmp = getList();
    w->append(tmp);
    tmp->killMe();
    l = w;
}

void TreeVisitor::visit(const ContinueExp &/*e*/)
{
    l = createVar(L"continue");
}

void TreeVisitor::visit(const BreakExp &/*e*/)
{
    l = createVar(L"break");
}

void TreeVisitor::visit(const ReturnExp &e)
{
    if (e.isGlobal())
    {
        l = createVar(L"resume");
    }
    else
    {
        types::List* ext = createOperation();
        types::List* ope = new types::List();

        //function or varname
        ope->append(new types::String(L"return"));

        //arguments
        for (auto arg : e.getExp().getAs<ArrayListExp>()->getExps())
        {
            arg->accept(*this);
            types::List* tmp = getList();
            ope->append(tmp);
            tmp->killMe();
        }

        //operator
        ext->append(ope);
        ope->killMe();
        ext->append(new types::String(L"ext"));
        l = ext;
    }
}

void TreeVisitor::visit(const SelectExp &e)
{
    types::TList* select = new types::TList();

    //header
    types::String* varstr = new types::String(1, 4);
    varstr->set(0, L"selectcase");
    varstr->set(1, L"expression");
    varstr->set(2, L"cases");
    varstr->set(3, L"else");
    select->append(varstr);

    //expression
    types::List* cond = new types::List();
    e.getSelect()->accept(*this);
    types::List* tmp = getList();
    cond->append(tmp);
    tmp->killMe();
    cond->append(getEOL());
    select->append(cond);
    cond->killMe();

    //cases
    types::List* lcases = new types::List();
    ast::exps_t cases = e.getCases();
    for (auto c : cases)
    {
        c->accept(*this);
        tmp = getList();
        lcases->append(tmp);
        tmp->killMe();
    }

    select->append(lcases);
    lcases->killMe();

    //default
    if (e.hasDefault())
    {
        e.getDefaultCase()->accept(*this);
        tmp = getList();
        select->append(tmp);
        tmp->killMe();
    }
    else
    {
        select->append(new types::List());

    }
    l = select;
}

void TreeVisitor::visit(const CaseExp &e)
{
    types::TList* c = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"case");
    varstr->set(1, L"expression");
    varstr->set(2, L"then");
    c->append(varstr);

    //expression
    e.getTest()->accept(*this);
    types::List* tmp = getList();
    c->append(tmp);
    tmp->killMe();

    //then
    e.getBody()->accept(*this);
    tmp = getList();
    c->append(tmp);
    tmp->killMe();

    l = c;
}

void TreeVisitor::visit(const ArrayListExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    ope->append(new types::String(L""));

    //arguments
    for (auto arg : e.getExps())
    {
        arg->accept(*this);
        types::List* tmp = getList();
        ope->append(tmp);
        tmp->killMe();
    }

    //operator
    ext->append(ope);
    ope->killMe();
    ext->append(new types::String(L"ext"));
    l = ext;
}

void TreeVisitor::visit(const NotExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getExp().accept(*this);
    types::List* tmp = getList();
    ope->append(tmp);
    tmp->killMe();

    //operator
    ext->append(ope);
    ope->killMe();
    ext->append(new types::String(L"~"));
    l = ext;
}

void TreeVisitor::visit(const TransposeExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getExp().accept(*this);
    types::List* tmp = getList();
    ope->append(tmp);
    tmp->killMe();

    //operator
    ext->append(ope);
    ope->killMe();
    ext->append(new types::String(L"'"));
    l = ext;
}

void TreeVisitor::visit(const FunctionDec  &e)
{
    wchar_t* pwstState;
    std::wostringstream wostr;
    PrintVisitor pv(wostr, false, false);

    types::TList* function = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"inline");
    varstr->set(1, L"prototype");
    varstr->set(2, L"definition");
    function->append(varstr);
    size_t returnSize = e.getReturns().getOriginal()->getAs<ArrayListVar>()->getVars().size();
    // First ask if there are some return values.
    if (returnSize > 1)
    {
        wostr << SCI_OPEN_RETURNS;
    }

    e.getReturns().getOriginal()->accept(pv);

    if (returnSize > 1)
    {
        wostr << SCI_CLOSE_RETURNS;
    }

    if (returnSize > 0)
    {
        wostr << L" " << SCI_ASSIGN << L" ";
    }

    // Then get the function name
    wostr << e.getSymbol().getName();

    // Then get function args
    wostr << SCI_OPEN_ARGS;
    e.getArgs().getOriginal()->accept(pv);
    wostr << SCI_CLOSE_ARGS << std::endl;
    wchar_t* pwstFun = os_wcsdup(wostr.str().data());
    function->append(new types::String(os_wcstok(pwstFun, L"\n", &pwstState)));
    FREE(pwstFun);

    // Now print function body
    std::wostringstream wostrBody;
    PrintVisitor pvBody(wostrBody, false, true);
    std::vector<wchar_t*> allTokens;
    e.getBody().getOriginal()->accept(pvBody);
    wchar_t* pwstBody = os_wcsdup(wostrBody.str().data());
    wchar_t* pwstToken = os_wcstok(pwstBody, L"\n", &pwstState);
    while (pwstToken != NULL)
    {
        allTokens.push_back(pwstToken);
        pwstToken = os_wcstok(NULL, L"\n", &pwstState);
    }

    if (allTokens.size() > 0)
    {
        types::String* stringMatrix = new types::String(static_cast<int>(allTokens.size()), 1);
        stringMatrix->set(allTokens.data());
        function->append(stringMatrix);
    }
    else
    {
        function->append(types::Double::Empty());
    }
    FREE(pwstBody);

    l = function;
}

types::InternalType* TreeVisitor::getEOL()
{
    if (eol)
    {
        return eol;
    }

    eol = new types::List();
    eol->append(new types::String(L"EOL"));
    return eol;
}

types::List* TreeVisitor::createVar(const std::wstring& str)
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 2);
    varstr->set(0, L"variable");
    varstr->set(1, L"name");
    var->append(varstr);
    var->append(new types::String(str.c_str()));
    return var;
}

types::List* TreeVisitor::createConst(types::InternalType* pIT)
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 2);
    varstr->set(0, L"cste");
    varstr->set(1, L"value");
    var->append(varstr);
    var->append(pIT);
    return var;
}

types::List* TreeVisitor::createOperation()
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"operation");
    varstr->set(1, L"operands");
    varstr->set(2, L"operator");
    var->append(varstr);
    return var;
}

types::List* TreeVisitor::createAssign()
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 4);
    varstr->set(0, L"equal");
    varstr->set(1, L"expression");
    varstr->set(2, L"lhs");
    varstr->set(3, L"endsymbol");
    var->append(varstr);
    return var;
}

types::InternalType* TreeVisitor::getVerbose(const Exp& e)
{
    if (e.isVerbose())
    {
        return new types::String(L"");
    }
    else
    {
        return new types::String(L";");
    }
}
}
