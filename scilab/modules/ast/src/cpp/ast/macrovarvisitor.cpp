/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "macrovarvisitor.hxx"
#include "context.hxx"

using namespace std;
using namespace ast;
using namespace symbol;
namespace ast
{
void MacrovarVisitor::visit (const SimpleVar &e)
{
    m_current = e.name_get().name_get();
}

void MacrovarVisitor::visit(const CallExp &e)
{
    e.name_get().accept (*this);
    add();

    std::list<Exp *>::const_iterator i;
    for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
    {
        (*i)->accept (*this);
        add();
    }
}

void MacrovarVisitor::visit(const AssignExp &e)
{
    e.left_exp_get().accept (*this);
    add();
    e.right_exp_get().accept (*this);
    add();
}

void MacrovarVisitor::visit (const VarDec  &e)
{
    e.init_get().accept(*this);
    add(m_local);
}

std::list<std::wstring>& MacrovarVisitor::getIn()
{
    return m_in;
}

std::list<std::wstring>& MacrovarVisitor::getOut()
{
    return m_out;
}

std::list<std::wstring>& MacrovarVisitor::getExternal()
{
    return m_external;
}

std::list<std::wstring>& MacrovarVisitor::getCalled()
{
    return m_called;
}

std::list<std::wstring>& MacrovarVisitor::getLocal()
{
    return m_local;
}

void MacrovarVisitor::add(std::list<std::wstring>& lst)
{
    if (m_current == L"")
    {
        return;
    }

    if (isAlreadyUsed() == false)
    {
        lst.push_back(m_current);
    }

    m_current = L"";
}

void MacrovarVisitor::add()
{
    InternalType* pVar = Context::getInstance()->get(Symbol(m_current));

    if (pVar)
    {
        if (pVar->isCallable())
        {
            //symbol already exists and callable
            add(m_called);
            return;
        }

        //symbol already exists and not callable.
        //it is a external var
        add(m_external);
        return;
    }

    add(m_local);
}

bool MacrovarVisitor::isAlreadyIn(std::list<std::wstring>& lst)
{
    std::list<std::wstring>::iterator it;
    for (it = lst.begin(); it != lst.end() ; it++)
    {
        if (*it == m_current)
        {
            return true;
        }
    }

    return false;
}

bool MacrovarVisitor::isAlreadyUsed()
{
    if (isAlreadyIn(m_in))
    {
        return true;
    }

    if (isAlreadyIn(m_out))
    {
        return true;
    }

    if (isAlreadyIn(m_external))
    {
        return true;
    }

    if (isAlreadyIn(m_called))
    {
        return true;
    }

    if (isAlreadyIn(m_local))
    {
        return true;
    }

    return false;
}
}