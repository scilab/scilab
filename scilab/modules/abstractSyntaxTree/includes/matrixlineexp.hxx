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
 ** \file matrixlineexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef __AST_MATRIXLINEEXP_HXX__
# define __AST_MATRIXLINEEXP_HXX__

# include "mathexp.hxx"

namespace ast
{

  /** \brief Abstract an Matrix Line Expression node.
   **
   ** \b Example: a, b, c, d */
  class MatrixLineExp : public MathExp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Matrix Columns node.
     ** \param location scanner position informations
     ** \param columns EXP LIST intruction
     */
    MatrixLineExp (const Location& location,
	    std::list<Exp *>& columns) :
      MathExp (location),
      _columns (&columns)
    {
    }

    virtual ~MatrixLineExp ()
    {
      delete _columns;
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
    const std::list<Exp *>&	columns_get() const
    {
      return *_columns;
    }

    std::list<Exp *>&	columns_get()
    {
      return *_columns;
    }
    /** \} */


  protected:
    std::list<Exp *>* _columns;
  };

} // namespace ast

#endif /* !__AST_MATRIXLINEEXP_HXX__ */


