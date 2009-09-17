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
** \file slotdec.hxx
** Define the Slot Declaration class.
*/

#ifndef AST_SLOTDEC_HXX
#define AST_SLOTDEC_HXX

#include <list>

#include "dec.hxx"
#include "arraylistvar.hxx"

#include "symbol.hxx"

namespace ast
{

  /*
  ** \brief Abstract a Class Declaration node.
  ** \b Example: TODO
  */
  class SlotDec : public Dec
  {
    // \name Ctor & dtor.
  public:
    /*
    ** \brief Construct a Slot Declaration node.
    ** \param location scanner position informations
    ** \param name of the slot
    ** \param attrs list of the attributes of the slot (should all be SimpleVar)
    */
    SlotDec (const Location& location,
		 symbol::Symbol& name,
                 ArrayListVar& attrs) :
      Dec (location),
      _name (name),
      _attrs (attrs)
    {
    }

    virtual ~SlotDec ()
    {
      delete &_name;
      delete &_attrs;
    }

    // \name Accessors.
  public:
    const symbol::Symbol& name_get (void) const
    {
      return _name;
    }

    const ArrayListVar& attributes_get (void) const
    {
      return _attrs;
    }

  protected:
    symbol::Symbol&	_name;
    ArrayListVar&	_attrs;
  };

  /** \brief Define a shorthand for list of Slot* manipulation. */
  typedef std::list<SlotDec *> slots_t;

} // namespace ast

#endif // !AST_SLOTDEC_HXX
