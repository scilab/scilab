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
 ** \file forexp.hxx
 ** Define the For Expression class.
 */

#ifndef AST_FOREXP_HXX
# define AST_FOREXP_HXX

# include "controlexp.hxx"
# include "vardec.hxx"

namespace ast
{

  /** \brief Abstract a For Expression node.
   **
   ** \b Example: for i = 0:2097 do print("WipeOut") */
  class ForExp : public ControlExp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct a For Expression node.
     ** \param location scanner position informations
     ** \param vardec implicit variable declaration
     ** \param body instructions executed in the for loop
     **
     ** \b Example: for i = 0:2097 do print("WipeOut")
     ** \li "i = 0:2097" is the variable declaration
     ** \li "print("WipeOut")" is the body
     */
    ForExp (const Location& location,
	    VarDec& vardec, Exp& body):
      ControlExp (location),
      _vardec (&vardec),
      _body (&body)
    {
    }

    /** \brief Destroy a For Expression node.
     **
     ** Delete vardec, hi and body (see constructor). */
    virtual ~ForExp ()
    {
      //delete  _vardec;
      //delete  _body;
    }
    /** \} */


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
    /** \brief Return the implicit variable declaration (read only) */
    const VarDec& vardec_get () const
    {
      return *_vardec;
    }
    /** \brief Return the implicit variable declaration (read and write) */
    VarDec& vardec_get ()
    {
      return *_vardec;
    }

    /** \brief Return the body of the loop (read only) */
    const Exp& body_get () const
    {
      return *_body;
    }
    /** \brief Return the body of the loop (read and write) */
    Exp& body_get ()
    {
      return *_body;
    }
    /** \} */


  protected:
    /** \brief Implicit variable declaration. */
    VarDec* _vardec;
    /** \brief Instructions executed in the loop. */
    Exp* _body;
  };

} // namespace ast

#endif // !AST_FOREXP_HXX
