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


#ifndef AST_BOOLEXP_HXX
#define AST_BOOLEXP_HXX

#include "constexp.hxx"

namespace ast
{
/** \brief Abstract an Boolean Expression node.
**
** \b Example:  true */
class BoolExp : public ConstExp
{
public:
    BoolExp (const Location& location, bool value)
        : ConstExp (location), _value (value)
    {
    }

    BoolExp (const Location& location, types::InternalType* value)
        : ConstExp (location),
          _value (false)
    {
        setConstant(value);
    }
    /** \brief Destroy an Boolean Expression node.
    **
    ** Delete size et init (exp) (see constructor). */
    virtual ~BoolExp ()
    {
    }
    /** \} */

    virtual BoolExp* clone()
    {
        BoolExp* cloned = new BoolExp(getLocation(), getValue());
        cloned->setVerbose(isVerbose());
        cloned->setConstant(getConstant());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == BOOLEXP && _value == static_cast<const BoolExp &>(e)._value;
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
    /** \brief Return the value */
    bool getValue() const
    {
        return _value;
    }

    virtual ExpType getType() const
    {
        return BOOLEXP;
    }
    inline bool isBoolExp() const
    {
        return true;
    }

protected:
    bool _value;

};

} // namespace ast
#endif
