/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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
 ** \file matrixlineexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef __AST_MATRIXLINEEXP_HXX__
#define __AST_MATRIXLINEEXP_HXX__

#include "mathexp.hxx"

namespace ast
{

/** \brief Abstract an Matrix Line Expression node.
**
** \b Example: a, b, c, d */
class MatrixLineExp : public MathExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Matrix Columns node.
    ** \param location scanner position informations
    ** \param columns EXP LIST intruction
    */
    MatrixLineExp (const Location& location,
                   exps_t& columns)
        : MathExp (location)
    {
        for (auto exp : columns)
        {
            exp->setParent(this);
            _exps.push_back(exp);
        }

        delete &columns;
    }

    virtual ~MatrixLineExp ()
    {
    }

    virtual MatrixLineExp* clone()
    {
        exps_t* columns = new exps_t;
        for (exps_t::const_iterator it = _exps.begin(), itEnd = _exps.end(); it != itEnd; ++it)
        {
            columns->push_back((*it)->clone());
        }

        MatrixLineExp* cloned = new MatrixLineExp(getLocation(), *columns);
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


    /** \name Accessors.
    ** \{ */
public:
    const exps_t&	getColumns() const
    {
        return _exps;
    }

    exps_t& getColumns()
    {
        return _exps;
    }
    /** \} */


    virtual ExpType getType() const
    {
        return MATRIXLINEEXP;
    }
    inline bool isMatrixLineExp() const
    {
        return true;
    }
};

} // namespace ast

#endif /* !__AST_MATRIXLINEEXP_HXX__ */


