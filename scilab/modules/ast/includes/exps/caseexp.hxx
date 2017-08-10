/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
            SeqExp& body) :
        ControlExp (location)
    {
        test.setParent(this);
        body.setParent(this);
        _exps.push_back(&test);
        _exps.push_back(body.getAs<Exp>());
    }

    ~CaseExp()
    {
    }

    virtual CaseExp* clone()
    {
        CaseExp* cloned = new CaseExp(getLocation(), *getTest()->clone(), *getBody()->clone()->getAs<SeqExp>());
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

    virtual ExpType getType() const
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
