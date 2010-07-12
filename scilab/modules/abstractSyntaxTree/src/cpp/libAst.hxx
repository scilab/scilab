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
 ** \file libast.hxx
 ** \brief Declare functions and variables exported by ast module.
 */

#ifndef AST_LIBAST_HXX
# define AST_LIBAST_HXX

# include <iostream>
# include "all.hxx"

namespace ast
{
  /** \brief Global root node of abstract syntax tree. */
  extern Exp* the_program;

  /** \brief Print an abstract syntax tree.
   ** \param tree abstract syntax tree's root node
   ** \param ostr output stream  */
  void print (const Exp &tree, std::wostream &ostr);

  /** \brief Intercept output stream redirection. */
  inline std::wostream& operator<< (std::wostream& ostr, const Exp &tree)
  {
    print (tree, ostr);
    return ostr;
  }

} // namespace ast

#endif // ! AST_LIBAST_HXX
