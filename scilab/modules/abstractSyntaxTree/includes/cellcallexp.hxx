/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file cellcallexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef AST_CELLCALLEXP_HXX
#define AST_CELLCALLEXP_HXX

#include "callexp.hxx"

namespace ast
{

  /** \brief Abstract an Array List of Expression node.
   **
   ** \b Example: foo(a), foo(b) */
  class CellCallExp : public CallExp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /*
    ** \brief Construct a CellCall Expression node.
    ** \param location scanner position informations
    ** \param name of the cell
    ** \param list of the arguments
    */
    CellCallExp (const Location& location,
        Exp& name,
        std::list<Exp *>& args) :
    CallExp(location, name, args)
    {
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

  };

} // namespace ast

#endif // !AST_CELLCALLEXP_HXX
