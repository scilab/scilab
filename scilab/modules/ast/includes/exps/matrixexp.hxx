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
 ** \file matrixexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef AST_MATRIXEXP_HXX
#define AST_MATRIXEXP_HXX

#include "mathexp.hxx"
#include "matrixlineexp.hxx"

namespace ast
{
/** \brief Abstract an Array List of Expression node.
**
** \b Example: foo(a), foo(b) */
class MatrixExp : public MathExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Array List of Expression node.
    ** \param location scanner position informations
    ** \param body EXP LIST intruction
    */
    MatrixExp (const Location& location,
               exps_t& lines)
        : MathExp (location)
    {
        for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
        {
            (*it)->setParent(this);
            _exps.push_back(*it);
        }

        delete &lines;
    }

    virtual ~MatrixExp ()
    {
    }

    virtual MatrixExp* clone()
    {
        exps_t* newlines = new exps_t;
        exps_t& lines = getLines();

        for (auto line : lines)
        {
            newlines->push_back(line->clone());
        }

        MatrixExp* cloned = new MatrixExp(getLocation(), *newlines);
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
    const exps_t& getLines() const
    {
        return _exps;
    }

    exps_t& getLines()
    {
        return _exps;
    }
    /** \} */


    virtual ExpType getType() const
    {
        return MATRIXEXP;
    }
    inline bool isMatrixExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_MATRIXEXP_HXX
