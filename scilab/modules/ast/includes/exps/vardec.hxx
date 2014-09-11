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
 ** \file vardec.hxx
 ** Define the Variable Declaration class.
 */

#ifndef AST_VARDEC_HXX
#define AST_VARDEC_HXX

#include <assert.h>

#include "dec.hxx"
#include "context.hxx"

namespace ast
{

/** \brief Abstract a Variable Declaration node.
**
** \b Example: i = 2097 */
class VarDec : public Dec
{
public:

    /** \name Ctor & dtor.
    ** \{ */
public:

    /** \brief Construct a Variable Declaration node.
    ** \param location scanner position informations
    ** \param name name of the declared variable
    ** \param init the initial value (expression) assigned to the variable
    **
    ** \b Example: var i = 2097
    ** \li "i" is the name of the declared variable
    ** \li "2097" is the initial value of the variable
    */
    VarDec (const Location& location,
            symbol::Symbol& name, Exp& init)
        : Dec (location),
          _name (name),
          _init (&init),
          _stack(NULL)
    {
    }

    /** \brief Destroy a Variable Declaration node.
    **
    ** Delete type_name if it exist, init and name (see constructor). */
    virtual ~VarDec()
    {
        delete &_name;
        delete _init;
    }
    /** \} */

    virtual VarDec* clone()
    {
        VarDec* cloned = new VarDec(location_get(), *new symbol::Symbol(name_get().name_get()), *init_get().clone());
        cloned->set_verbose(is_verbose());
        return cloned;
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
    /** \brief Return the variable name (read only). */
    symbol::Symbol& name_get (void) const
    {
        return _name;
    }

    symbol::Variable* stack_get()
    {
        if (_stack == NULL)
        {
            _stack = symbol::Context::getInstance()->getOrCreate(_name);
        }

        return _stack;
    }

    /** \brief Return the initial expression value (read only). */
    const Exp& init_get (void) const
    {
        return *_init;
    }
    /** \brief Return the initial expression value (read and write). */
    Exp& init_get (void)
    {
        return *_init;
    }

protected:
    /** \brief Name of the declared variable. */
    symbol::Symbol& _name;
    /** \brief The initial value (expression) assigned to the variable. */
    Exp* _init;

    symbol::Variable* _stack;
};

} // namespace ast

#endif // !AST_VARDEC_HXX
