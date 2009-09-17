/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Simon LIPP
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
** \file propertydec.hxx
** Define the Property Declaration property.
*/

#ifndef AST_PROPERTYDEC_HXX
#define AST_PROPERTYDEC_HXX

#include "slotdec.hxx"
#include "arraylistvar.hxx"

#include "symbol.hxx"

namespace ast
{

  /*
  ** \brief Abstract a Property Declaration node.
  ** \b Example: TODO
  */
  class PropertyDec : public SlotDec
  {
    // \name Ctor & dtor.
  public:
    /*
    ** \brief Construct a Property Declaration node.
    ** \param location scanner position informations
    ** \param name of mehod
    ** \param attrs slots attributes
    ** \param default_value expression evalued to the default value of the property
    */
    PropertyDec (const Location& location,
		 symbol::Symbol& name,
		 ArrayListVar& attrs,
                 Exp* default_value) :
      SlotDec (location, name, attrs),
      _default_value (default_value)
    {
    }

    virtual ~PropertyDec ()
    {
      if (_default_value) {
        delete _default_value;
      }
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
    const Exp* default_get (void) const
    {
      return _default_value;
    }

  protected:
    Exp* _default_value;   
  };

} // namespace ast

#endif // !AST_PROPERTYDEC_HXX
