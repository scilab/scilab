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

    MacrovarVisitor* clone()
    {
        return new MacrovarVisitor();
    }

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

    std::list<std::string>& getIn();
    std::list<std::string>& getOut();
    std::list<std::string>& getExternal();
    std::list<std::string>& getCalled();
    std::list<std::string>& getLocal();
    void add(std::list<std::string>& lst);
    void add();

    bool isAlreadyIn(std::list<std::string>& lst);
    bool isAlreadyUsed();

    void addIn(const std::string& st)
    {
        m_in.push_back(st);
    }

    void addOut(const std::string& st)
    {
        m_out.push_back(st);
    }

    std::list<std::string> m_in; //inout variables
    std::list<std::string> m_out; //output functions
    std::list<std::string> m_external; //external variables
    std::list<std::string> m_called; //called functions
    std::list<std::string> m_local; // local variables

    std::string m_current;
    bool isAssignExpLeftExp;
};
}

#endif /* !__MACROVARVISITOR_HXX__ */
