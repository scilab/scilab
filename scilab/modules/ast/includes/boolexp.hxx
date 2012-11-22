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
#include "bool.hxx"

using namespace types;

namespace ast
{
/** \brief Abstract an Boolean Expression node.
**
** \b Example:  true */
class BoolExp : public ConstExp
{
public:
    BoolExp (const Location& location, bool value)
        : ConstExp (location), _value (value), _bigBool (NULL)
    {
    }
    /** \brief Destroy an Boolean Expression node.
    **
    ** Delete size et init (exp) (see constructor). */
    virtual ~BoolExp ()
    {
        if (_bigBool)
        {
            _bigBool->DecreaseRef();
            if (_bigBool->isDeletable())
            {
                delete _bigBool;
            }
        }
    }
    /** \} */

    virtual BoolExp* clone()
    {
        Location* newloc = const_cast<Location*>(&location_get())->clone();
        BoolExp* cloned = new BoolExp(*newloc, value_get());
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
    /** \brief Return the value */
    bool value_get() const
    {
        return _value;
    }

    /** \brief Get the big bool */
    types::Bool* getBigBool() const
    {
        return _bigBool;
    }

    /** \brief Set the big bool */
    void setBigBool(types::Bool *pB)
    {
        if (_bigBool && _bigBool->isRef())
        {
            _bigBool->DecreaseRef();
            if (_bigBool->isDeletable())
            {
                delete _bigBool;
            }
        }
        _bigBool = pB;
        _bigBool->IncreaseRef();
    }
    /** \} */
protected:
    bool            _value;
    types::Bool*    _bigBool;

};

} // namespace ast
#endif
