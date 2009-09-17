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
** \file classdec.hxx
** Define the Class Declaration class.
*/

#ifndef AST_CLASSDEC_HXX
#define AST_CLASSDEC_HXX

#include "dec.hxx"
#include "slotdec.hxx"

#include "symbol.hxx"

namespace ast
{

  /*
  ** \brief Abstract a Class Declaration node.
  ** \b Example: TODO
  */
  class ClassDec : public Dec
  {
    // \name Ctor & dtor.
  public:
    /*
    ** \brief Construct a Class Declaration node.
    ** \param location scanner position informations
    ** \param name of the class
    ** \param super name of the super class
    ** \param slots list of slots
    */
    ClassDec (const Location& location,
		 symbol::Symbol* name,
                 symbol::Symbol* super,
                 slots_t& slots) :
      Dec (location),
      _name (name),
      _super (super),
      _slots (slots)
    {
    }

    virtual ~ClassDec ()
    {
      slots_t::iterator it;
      for (it = _slots.begin(); it != _slots.end(); ++it) {
        delete *it;
      }
      if (_name) {
        delete _name;
      }
      if (_super) {
        delete _super;
      }
      delete &_slots;
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
    const symbol::Symbol* name_get (void) const
    {
      return _name;
    }

    const symbol::Symbol* super_get (void) const
    {
      return _super;
    }
    
    const slots_t& slots_get (void) const
    {
      return _slots;
    }

  protected:
    symbol::Symbol*	_name;
    symbol::Symbol*	_super;
    slots_t&		_slots;
  };

} // namespace ast

#endif // !AST_CLASSDEC_HXX
