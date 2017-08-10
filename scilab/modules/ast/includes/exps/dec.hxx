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
};

} // namespace ast

#endif // !AST_DEC_HXX
