/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file dec.hxx
 ** Define the Declaration mother class.
 */

#ifndef AST_DEC_HXX
#define AST_DEC_HXX

#include "ast.hxx"
#include "exp.hxx"

namespace ast
{

    /** \brief Abstract an Declaration node. */
    class Dec : public Exp
    {

        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct a Declaration node.
        ** \param location scanner position informations */
        Dec (const Location& location) 
            : Exp (location)
        {
        }
        /** \} */

        //virtual Dec* clone()
        //{
        //    Location* newloc = const_cast<Location*>(&location_get())->clone();
        //    return new Dec(location_get());
        //}
    };

} // namespace ast

#endif // !AST_DEC_HXX
