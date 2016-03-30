/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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

#include "macrovarvisitor.hxx"
#include "context.hxx"

namespace ast
{

void MacrovarVisitor::visit (const MatrixLineExp &e)
{
    exps_t columns = e.getColumns();
    for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
    {
        (*it)->accept (*this);
        add();
    }
}

void MacrovarVisitor::visit (const SimpleVar &e)
{
    m_current = e.getSymbol().getName();
}

void MacrovarVisitor::visit(const OpExp &e)
{
    e.getLeft().accept(*this);
    add();
    e.getRight().accept(*this);
    add();
}

void MacrovarVisitor::visit(const CallExp &e)
{
    e.getName().accept (*this);
    if (isAssignExpLeftExp)
    {
        add(m_local);
    }
    else
    {
        add();
    }

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->getOriginal()->accept(*this);
        add();
    }
}

void MacrovarVisitor::visit (const ForExp  &e)
{
    e.getVardec().accept(*this);
    add(m_local);
    e.getBody().accept (*this);
}

void MacrovarVisitor::visit (const FieldExp &e)
{
    e.getHead()->accept(*this);
    add(m_local);
}

void MacrovarVisitor::visit(const AssignExp &e)
{
    isAssignExpLeftExp = true;
    e.getLeftExp().getOriginal()->accept (*this);
    isAssignExpLeftExp = false;
    if (e.getLeftExp().isSimpleVar())
    {
        add(m_local);
    }
    else
    {
        add();
    }

    e.getRightExp().getOriginal()->accept (*this);
    add();
}

void MacrovarVisitor::visit (const AssignListExp  &e)
{
    isAssignExpLeftExp = true;
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept (*this);
        if ((*it)->isSimpleVar())
        {
            add(m_local);
        }
        else
        {
            add();
        }
    }
    isAssignExpLeftExp = false;
}

void MacrovarVisitor::visit (const VarDec  &e)
{
    m_current = e.getSymbol().getName();
    add(m_local);
    e.getInit().getOriginal()->accept(*this);
    add();
}

void MacrovarVisitor::visit (const FunctionDec  &e)
{
    // Function declared locally but never called
    // If called then the CallExp visitor does the job
    m_current = e.getSymbol().getName();
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
    types::InternalType* pVar = symbol::Context::getInstance()->get(symbol::Symbol(m_current));
    if (pVar)
    {
        if (pVar->isCallable())
        {
            //symbol already exists and callable
            add(m_called);
            return;
        }
    }

    add(m_external);
}

bool MacrovarVisitor::isAlreadyIn(std::list<std::wstring>& lst)
{
    for (auto l : lst)
    {
        if (l == m_current)
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
