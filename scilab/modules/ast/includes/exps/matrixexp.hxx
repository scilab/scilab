/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    }

    virtual ~MatrixExp ()
    {
    }

    virtual MatrixExp* clone()
    {
        exps_t lines;
        for (exps_t::const_iterator it = getLines().begin(), itEnd = getLines().end(); it != itEnd; ++it)
        {
            lines.push_back((*it)->clone());
        }

        MatrixExp* cloned = new MatrixExp(getLocation(), lines);
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


    virtual ExpType getType()
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
