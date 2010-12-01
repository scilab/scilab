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
 ** \file ast/decs.hxx
 ** Define the Decs class.
 */

#ifndef AST_DECS_HXX
#define AST_DECS_HXX

#include <list>

#include "ast/ast.hxx"

namespace ast
{

    /** \brief Abstract a Declarations node. */
    class Decs : public Ast
    {

        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct a Declarations node.
        ** \param location scanner position informations */
        Decs (const Location& location) 
            : Ast (location)
        {
        }

        virtual Decs* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new Decs(*newloc);
        }
    };
    /** \brief Define shortand type for list of Declarations. */
    typedef std::list<Decs *> decs_t;

} // namespace ast

#endif // !AST_DECS_HXX
