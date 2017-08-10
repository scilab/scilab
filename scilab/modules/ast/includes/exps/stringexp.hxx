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

#ifndef AST_STRINGEXP_HXX
#define AST_STRINGEXP_HXX

#include "constexp.hxx"

namespace ast
{
/** \brief Abstract an string Expression node.
**
** \b Example: string*/
class StringExp : public ConstExp
{
public:
    StringExp (const Location& location, const std::wstring& value)
        : ConstExp (location),
          _value (value)
    {
    }

    StringExp (const Location& location, types::InternalType* value)
        : ConstExp (location),
          _value (L"")
    {
        setConstant(value);
    }

    /** \brief Destroy an string Exp node.
    **
    ** Delete value (see constructor). */
    virtual ~StringExp ()
    {
    }
    /** \} */

    virtual StringExp* clone()
    {
        StringExp* cloned = new StringExp(getLocation(), getValue());
        cloned->setVerbose(isVerbose());
        cloned->setConstant(getConstant());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == STRINGEXP && _value == static_cast<const StringExp &>(e)._value;
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

public:
    /** \brief Return the name of the type name (read only). */
    const std::wstring& getValue() const
    {
        return _value;
    }
    /** \} */

    virtual ExpType getType() const
    {
        return STRINGEXP;
    }
    inline bool isStringExp() const
    {
        return true;
    }

protected:
    const std::wstring _value;
};

} // namespace ast

#endif // !AST_STRINGEXP_HXX
