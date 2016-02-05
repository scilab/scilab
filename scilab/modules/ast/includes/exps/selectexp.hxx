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

/**
 ** \file selectexp.hxx
 ** Define Select Expression class
 */

#ifndef __AST_SELECT_EXP_HXX__
#define __AST_SELECT_EXP_HXX__

#include "controlexp.hxx"
#include "exp.hxx"
#include "caseexp.hxx"
#include "seqexp.hxx"

namespace ast
{
class SelectExp : public ControlExp
{
public :
    SelectExp(const Location& location,
              Exp& select,
              exps_t& cases,
              Exp& defaultCase)
        : ControlExp (location),
          _hasDefault(true)
    {
        select.setParent(this);
        _exps.push_back(&select);

        for (auto it : cases)
        {
            it->setParent(this);
            _exps.push_back(it);
        }

        delete &cases;
        defaultCase.setParent(this);
        _exps.push_back(&defaultCase);
    }

    SelectExp(const Location& location,
              Exp& select,
              exps_t& cases)
        : ControlExp (location),
          _hasDefault(false)
    {
        select.setParent(this);
        _exps.push_back(&select);

        for (exps_t::const_iterator it = cases.begin(), itEnd = cases.end(); it != itEnd ; ++it)
        {
            (*it)->setParent(this);
            _exps.push_back(*it);
        }

        delete &cases;
    }

    ~SelectExp()
    {
    }

    virtual SelectExp* clone()
    {
        exps_t * cases = cloneCases();
        SelectExp* cloned = nullptr;
        if (_hasDefault)
        {
            cloned = new SelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone());
        }
        else
        {
            cloned = new SelectExp(getLocation(), *getSelect()->clone(), *cases);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    exps_t * cloneCases()
    {
        exps_t* cases = new exps_t;
        exps_t::const_iterator it = ++(_exps.begin());
        exps_t::const_iterator itEnd = _exps.end();
        if (_hasDefault)
        {
            --itEnd;
        }

        for (; it != itEnd ; ++it)
        {
            cases->push_back((*it)->clone());
        }

        return cases;
    }

    virtual bool equal(const Exp & e) const
    {
        return Exp::equal(e) && hasDefault() == static_cast<const SelectExp &>(e).hasDefault();
    }

public :
    Exp* getSelect() const
    {
        return _exps[0];
    }

    inline exps_t getCases() const
    {
        return ast::exps_t(std::next(_exps.begin()), _hasDefault ? std::prev(_exps.end()) : _exps.end());
    }

    inline exps_t getCases()
    {
        return ast::exps_t(std::next(_exps.begin()), _hasDefault ? std::prev(_exps.end()) : _exps.end());
    }

    Exp* getDefaultCase() const
    {
        if (_hasDefault)
        {
            return _exps.back();
        }
        else
        {
            return NULL;
        }
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

    virtual ExpType getType() const
    {
        return SELECTEXP;
    }

    inline bool isSelectExp() const
    {
        return true;
    }

    bool hasDefault()
    {
        return _hasDefault;
    }

    bool hasDefault() const
    {
        return _hasDefault;
    }

    bool _hasDefault;
};

}


#endif /* !__AST_SELECT_EXP_HXX__ */
