/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

/**
 ** \file vardec.hxx
 ** Define the Variable Declaration class.
 */

#ifndef AST_VARDEC_HXX
#define AST_VARDEC_HXX

#include <assert.h>

#include "dec.hxx"
#include "context.hxx"
#include "ForList.hxx"

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
            symbol::Symbol name, Exp& init)
        : Dec (location),
          _name (name),
          _stack(NULL)
    {
        init.setParent(this);
        _exps.push_back(&init);
    }

    /** \brief Destroy a Variable Declaration node.
    **
    ** Delete type_name if it exist, init and name (see constructor). */
    virtual ~VarDec()
    {
    }
    /** \} */

    virtual VarDec* clone()
    {
        VarDec* cloned = new VarDec(getLocation(), getSymbol(), *getInit().clone());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return Exp::equal(e) && _name == static_cast<const VarDec &>(e)._name;
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
    symbol::Symbol getSymbol (void) const
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

    /** \brief Return the initial expression value (read only). */
    const Exp& getInit (void) const
    {
        return *_exps[0];
    }
    /** \brief Return the initial expression value (read and write). */
    Exp& getInit (void)
    {
        return *_exps[0];
    }

    analysis::ForList64 getListInfo() const
    {
        return list_info;
    }

    analysis::ForList64 & getListInfo()
    {
        return list_info;
    }

    void setListInfo(analysis::ForList64 & _list_info)
    {
        list_info = _list_info;
    }

    void setListInfo(analysis::ForList64 && _list_info)
    {
        list_info = _list_info;
    }

    virtual ExpType getType() const
    {
        return VARDEC;
    }
    inline bool isVarDec() const
    {
        return true;
    }
protected:
    analysis::ForList64 list_info;
    /** \brief Name of the declared variable. */
    symbol::Symbol _name;
    symbol::Variable* _stack;
};

} // namespace ast

#endif // !AST_VARDEC_HXX

