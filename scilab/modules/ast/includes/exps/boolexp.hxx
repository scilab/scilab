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
