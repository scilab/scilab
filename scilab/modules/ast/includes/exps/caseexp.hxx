/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __AST_CASE_EXP_HXX__
#define __AST_CASE_EXP_HXX__

#include "exp.hxx"
#include "seqexp.hxx"

namespace ast
{
class CaseExp : public ControlExp
{
public :
    CaseExp(const Location& location,
            Exp& test,
            Exp& body) :
        ControlExp (location)
    {
        test.setParent(this);
        body.setParent(this);
        _exps.push_back(&test);
        _exps.push_back(&body);
    }

    ~CaseExp()
    {
        delete _exps[0];
        delete _exps[1];
    }

    virtual CaseExp* clone()
    {
        CaseExp* cloned = new CaseExp(getLocation(), *getTest()->clone(), *getBody()->clone());
        cloned->setVerbose(isVerbose());
        return cloned;
    }
    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
    /** \} */

public :
    Exp* getTest() const
    {
        return _exps[0];
    }

    Exp* getBody() const
    {
        return _exps[1];
    }

    virtual ExpType getType()
    {
        return CASEEXP;
    }

    inline bool isCaseExp() const
    {
        return true;
    }
};
}

#endif /* !__AST_CASE_EXP_HXX__ */
