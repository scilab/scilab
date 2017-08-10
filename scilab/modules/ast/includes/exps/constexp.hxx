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

#ifndef AST_CONSTEXP_HXX
#define AST_CONSTEXP_HXX

#include "exp.hxx"
#include "internal.hxx"

namespace ast
{
/** \brief Abstract an Constant Expression node.
**
** \b Example:  true or 2 or 1,23*/
class ConstExp : public Exp
{
public:
    ConstExp (const Location& location)
        : Exp (location), constant(NULL)
    {
    }

    virtual ~ConstExp ()
    {
        if (constant)
        {
            constant->DecreaseRef();
            constant->killMe();
        }
    }

    inline bool isConstExp() const
    {
        return true;
    }

    types::InternalType* getConstant() const
    {
        return constant;
    }

    types::InternalType* getConstant()
    {
        return constant;
    }

    void setConstant(types::InternalType* _const)
    {
        if (constant)
        {
            constant->DecreaseRef();
            constant->killMe();
        }

        constant = _const;
        if (_const)
        {
            constant->IncreaseRef();
        }
    }

    virtual bool equal(const Exp & e) const
    {
        // TODO : check for the equality of the contents of constant
        return e.getType() == CONSTEXP && *constant == *static_cast<const ConstExp &>(e).constant;
    }

    virtual ExpType getType() const
    {
        return CONSTEXP;
    }

protected :
    types::InternalType* constant;
};
} // namespace ast
#endif
