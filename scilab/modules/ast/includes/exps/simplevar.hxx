/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */


#ifndef AST_SIMPLEVAR_HXX
#define AST_SIMPLEVAR_HXX

#include "var.hxx"
#include "context.hxx"

namespace ast
{
/** \brief Abstract a Simple Variable node.
**
** \b Example: i */
class SimpleVar : public Var
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Simple Variable node.
    ** \param location scanner position informations
    ** \param name the name of the variable
    */
    SimpleVar (const Location& location,
               const symbol::Symbol& name)
        : Var (location),
          _name (name),
          _stack(NULL)
    {
    }
    /** \brief Destroy a Field Variable node.
    **
    ** Delete name, see constructor. */
    virtual ~SimpleVar ()
    {
    }

    virtual SimpleVar* clone()
    {
        SimpleVar* cloned = new SimpleVar(getLocation(), getSymbol());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == SIMPLEVAR && _name == static_cast<const SimpleVar &>(e)._name;
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
    /** \brief Return the Variable's name. */
    const symbol::Symbol & getSymbol() const
    {
        return _name;
    }

    symbol::Symbol & getSymbol()
    {
        return _name;
    }

    symbol::Variable* getStack()
    {
        if (_stack == NULL)
        {
            _stack = symbol::Context::getInstance()->getOrCreate(_name);
        }

        return _stack;
    }
    /** \} */


    bool operator== (const SimpleVar &rhs) const
    {
        return _name == rhs.getSymbol();
    }


    virtual ExpType getType() const
    {
        return SIMPLEVAR;
    }
    inline bool isSimpleVar() const
    {
        return true;
    }
protected:
    /** \brief Variable's name */
    symbol::Symbol _name;
    symbol::Variable* _stack;
};

} // namespace ast

#endif // !AST_SIMPLEVAR_HXX
