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
 ** \file fieldexp.hxx
 ** Define the Field Expression class.
 */

#ifndef AST_FIELDEXP_HXX
#define AST_FIELDEXP_HXX

#include <list>

#include "exp.hxx"

namespace ast
{

/** \brief Abstract a Field Expression node.
**
** \b Example: var.field(n)
**            |___________| */
class FieldExp : public Exp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Field Expression node.
    ** \param location scanner position informations
    ** \param name field identifier
    ** \param init initial value assigned to the field
    **
    ** \b Example: field = 2097
    ** \li "field" is the name of the field
    ** \li "2097" is the initial value of the field */
    FieldExp (const Location& location,
              Exp& head, Exp& tail)
        : Exp (location)
    {
        head.setParent(this);
        tail.setParent(this);
        _exps.push_back(&head);
        _exps.push_back(&tail);
    }

    /** \brief Destroy a Field Expression node.
    **
    ** Delete name and init (see constructor). */
    virtual ~FieldExp()
    {
    }
    /** \}*/

    virtual FieldExp* clone()
    {
        FieldExp* cloned = new FieldExp(getLocation(), *getHead()->clone(), *getTail()->clone());
        cloned->setVerbose(isVerbose());
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
    /** \brief Return the name of the field (read only). */
    const Exp* getHead() const
    {
        return _exps[0];
    }

    /** \brief Return the initial value of the field (read only). */
    const Exp* getTail() const
    {
        return _exps[1];
    }

    /** \brief Return the initial value of the field (read and write). */
    Exp* getHead()
    {
        return _exps[0];
    }

    /** \brief Return the initial value of the field (read and write). */
    Exp* getTail()
    {
        return _exps[1];
    }
    /** \} */

    virtual ExpType getType() const
    {
        return FIELDEXP;
    }

    inline bool isFieldExp() const
    {
        return true;
    }
protected:
};

/** \brief Define shortand type for Field Expression list. */
typedef std::list<FieldExp *> fieldexps_t;

} // namespace ast

#endif // !AST_FIELDEXP_HXX
