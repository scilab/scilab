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
** \file Methoddec.hxx
** Define the Method Declaration class.
*/

#ifndef AST_METHODDEC_HXX
#define AST_METHODDEC_HXX

#include <list>

#include "slotdec.hxx"
#include "exp.hxx"
#include "arraylistvar.hxx"

#include "symbol.hxx"

namespace ast
{

  /*
  ** \brief Abstract a Method Declaration node.
  ** \b Example: TODO
  */
  class MethodDec : public SlotDec
  {
    // \name Ctor & dtor.
  public:
    /*
    ** \brief Construct a Method Declaration node.
    ** \param location scanner position informations
    ** \param name of Method
    ** \param attrs slots attributes
    ** \param args method arguments
    ** \param returns method returns variables
    ** \param body method code
    */
    MethodDec (const Location& location,
		 symbol::Symbol& name,
                 ArrayListVar& attrs,
		 ArrayListVar& args,
		 ArrayListVar& returns,
		 Exp& body) :
      SlotDec (location, name, attrs),
      _args (args),
      _returns (returns),
      _body (body)
    {
    }

    virtual ~MethodDec ()
    {
      delete &_args;
      delete &_returns;
    }


    // \name Visitors entry point.
  public:
    // \brief Accept a non-const visitor
    virtual void accept (Visitor& v)
    {
      v.visit (*this);
    }
    // \brief Accept a const visitor
    virtual void accept (ConstVisitor& v) const
    {
      v.visit (*this);
    }


    // \name Accessors.
  public:
    const Exp& body_get (void) const
    {
      return _body;
    }

    const ArrayListVar& args_get () const
    {
      return _args;
    }

    const ArrayListVar& returns_get () const
    {
      return _returns;
    }

  protected:
    ArrayListVar&	_args;
    ArrayListVar&	_returns;
    Exp&		_body;
  };

} // namespace ast

#endif // !AST_METHODDEC_HXX
