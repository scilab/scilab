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
 ** \file ast/var.hxx
 ** Define the Variable mother class.
 */

#ifndef AST_VAR_HXX
# define AST_VAR_HXX

# include <list>

# include "exp.hxx"

namespace ast
{
  class Var : public Exp
  {

    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct a Declaration node.
     ** \param location scanner position informations */
    Var (const Location& location) :
      Exp (location)
    {
    }

    virtual ~Var()
    {
    }
    /** \} */

  };

  /** \brief Define a shorthand for list of Exp* manipulation. */
  typedef std::list<Var *> vars_t;

} // namespace ast

#endif // !AST_VAR_HXX
