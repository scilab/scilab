/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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
 ** \file ast/var.hxx
 ** Define the Variable mother class.
 */

#ifndef AST_VAR_HXX
#define AST_VAR_HXX

#include <list>

#include "exp.hxx"

namespace ast
{
class Var : public Exp
{

public:
    /** \brief Construct a Declaration node.
    ** \param location scanner position informations */
    Var (const Location& location)
        : Exp (location)
    {
    }

    virtual ~Var()
    {
    }
};

/** \brief Define a shorthand for list of Exp* manipulation. */
typedef std::list<Var *> vars_t;

} // namespace ast

#endif // !AST_VAR_HXX
