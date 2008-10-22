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
 ** \file cellexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef AST_CELLEXP_HXX
# define AST_CELLEXP_HXX

# include "mathexp.hxx"
# include "celllineexp.hxx"

namespace ast
{

  /** \brief Abstract an Array List of Expression node.
   **
   ** \b Example: foo(a), foo(b) */
  class CellExp : public MathExp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Array List of Expression node.
     ** \param location scanner position informations
     ** \param body EXP LIST intruction
     */
    CellExp (const Location& location,
	    std::list<CellLineExp *>& lines) :
      MathExp (location),
      _lines (&lines)
    {
    }

    virtual ~CellExp ()
    {
      delete _lines;
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
    const std::list<CellLineExp *>&	lines_get() const
    {
      return *_lines;
    }

    std::list<CellLineExp *>&	lines_get()
    {
      return *_lines;
    }
    /** \} */


  protected:
    std::list<CellLineExp *>* _lines;
  };

} // namespace ast

#endif // !AST_CELLEXP_HXX
