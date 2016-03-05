/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
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

#ifndef AST_IFEXP_HXX
#define AST_IFEXP_HXX

#include <assert.h>
#include "controlexp.hxx"

namespace ast
{
/*
** \brief Abstract an If Expression node.
**
** \b Example: if (2097 = 0) then disp("fail") else disp("OK")
*/
class IfExp : public ControlExp
{
public:
    /*
    ** \brief Construct an If Expression node.
    ** \param location scanner position informations
    ** \param test chose the instruction to execute
    ** \param then instruction if test is true
    ** \param else instruction if test is false
    */
    IfExp(const Location& location,
          Exp& test, SeqExp& t, SeqExp& e)
        : ControlExp (location),
          _hasElse (true)
    {
        test.setParent(this);
        t.setParent(this);
        e.setParent(this);
        _exps.push_back(&test);
        _exps.push_back(t.getAs<Exp>());
        _exps.push_back(e.getAs<Exp>());
    }

    IfExp(const Location& location,
          Exp& test, Exp& t)
        : ControlExp (location),
          _hasElse (false)
    {
        test.setParent(this);
        t.setParent(this);

        _exps.push_back(&test);
        _exps.push_back(t.getAs<Exp>());
        _exps.push_back(new ast::CommentExp(location, new std::wstring(L"No else !!")));
        _exps[2]->setParent(this);
    }

    virtual ~IfExp()
    {
    }

    virtual IfExp* clone()
    {
        IfExp* cloned = new IfExp(getLocation(), *getTest().clone(), *getThen().clone()->getAs<SeqExp>(), *getElse().clone()->getAs<SeqExp>());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    // \brief Visitors entry point.
public:
    virtual void	accept(Visitor& v)
    {
        v.visit (*this);
    }
    virtual void	accept(ConstVisitor& v) const
    {
        v.visit (*this);
    }

    // \brief Accessors.
public:
    // \brief Return the select condition of the loop (read only).
    const Exp&	getTest() const
    {
        return *_exps[0];
    }
    // \brief Return the select condition of the loop (read and write).
    Exp& getTest()
    {
        return *_exps[0];
    }

    // \brief Return the intructions if test is true (read only).
    const Exp&	getThen() const
    {
        return *_exps[1];
    }
    // \brief Return the instructions if test is true (read and write).
    Exp& getThen()
    {
        return *_exps[1];
    }

    // \brief Return the instruction if test is false (read only).
    const Exp& getElse() const
    {
        return *_exps[2];
    }
    // \brief Return the instruction if test is false (read and write).
    Exp& getElse()
    {
        return *_exps[2];
    }

    // \brief Return if there is an else body
    bool hasElse()
    {
        return _hasElse;
    }
    // \brief Return if there is an else body
    bool hasElse() const
    {
        return _hasElse;
    }

    virtual ExpType getType() const
    {
        return IFEXP;
    }
    inline bool isIfExp() const
    {
        return true;
    }
protected:
    // \brief "has a value" qualifier.
    bool _hasElse;
};

} // namespace ast

#endif // !AST_IFEXP_HXX
