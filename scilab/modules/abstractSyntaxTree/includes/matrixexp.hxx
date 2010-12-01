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
            std::list<MatrixLineExp *>& lines) 
            : MathExp (location),
            _lines (&lines)
        {
        }

        virtual ~MatrixExp ()
        {
            std::list<MatrixLineExp *>::const_iterator i;
            for(i = _lines->begin() ; i != _lines->end() ; i++)
            {
                delete *i;
            }
            delete _lines;
        }

        virtual MatrixExp* clone()
        {
            std::list<MatrixLineExp *>* lines = new std::list<MatrixLineExp *>;
            std::list<MatrixLineExp *>::const_iterator it;
            for(it = lines_get().begin() ; it != lines_get().end() ; it++)
            {
                lines->push_back((*it)->clone());
            }

            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return  new MatrixExp(*newloc, *lines);
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
        const std::list<MatrixLineExp *>&	lines_get() const
        {
            return *_lines;
        }

        std::list<MatrixLineExp *>&	lines_get()
        {
            return *_lines;
        }
        /** \} */


    protected:
        std::list<MatrixLineExp *>* _lines;
    };

} // namespace ast

#endif // !AST_MATRIXEXP_HXX
