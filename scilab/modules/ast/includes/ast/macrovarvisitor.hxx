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

#ifndef __MACROVARVISITOR_HXX__
#define __MACROVARVISITOR_HXX__

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "dummyvisitor.hxx"

/*
** This class defines a dummy visitor which only purpose is to
** followup itself through the AST.
*/

namespace ast
{
class EXTERN_AST MacrovarVisitor : public DummyVisitor
{
public:
    MacrovarVisitor()
    {
        isAssignExpLeftExp = false;
    }
    ~MacrovarVisitor() {}

    void visit (const MatrixLineExp &e);
    void visit (const SimpleVar &e);
    void visit(const OpExp &e);
    void visit(const CallExp &e);
    void visit(const ForExp &e);
    void visit (const VarDec &e);
    void visit (const FunctionDec &e);
    void visit(const FieldExp &e);
    void visit(const AssignExp &e);
    void visit(const AssignListExp &e);

    std::list<std::wstring>& getIn();
    std::list<std::wstring>& getOut();
    std::list<std::wstring>& getExternal();
    std::list<std::wstring>& getCalled();
    std::list<std::wstring>& getLocal();
    void add(std::list<std::wstring>& lst);
    void add();

    bool isAlreadyIn(std::list<std::wstring>& lst);
    bool isAlreadyUsed();

    void addIn(const std::wstring& st)
    {
        m_in.push_back(st);
    }

    void addOut(const std::wstring& st)
    {
        m_out.push_back(st);
    }

    std::list<std::wstring> m_in; //inout variables
    std::list<std::wstring> m_out; //output functions
    std::list<std::wstring> m_external; //external variables
    std::list<std::wstring> m_called; //called functions
    std::list<std::wstring> m_local; // local variables

    std::wstring m_current;
    bool isAssignExpLeftExp;
};
}

#endif /* !__MACROVARVISITOR_HXX__ */
