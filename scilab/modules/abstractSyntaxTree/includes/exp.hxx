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
 ** \file exp.hxx
 ** Define the Expression mother class.
 */

#ifndef AST_EXP_HXX
# define AST_EXP_HXX

# include <list>

# include "ast.hxx"

namespace ast
{

  /** \brief Abstract an Expression node. */
  class Exp : public Ast
  {

    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Expression node.
     ** \param location scanner position informations */
    Exp (const Location& location) :
      Ast (location)
    {
    }
    /** \brief Destroys an Expression node. */
    virtual ~Exp ()
    {
    }
    /** \} */

  };

  /** \brief Define a shorthand for list of Exp* manipulation. */
  typedef std::list<Exp *> exps_t;

} // namespace ast

#endif // !AST_EXP_HXX
