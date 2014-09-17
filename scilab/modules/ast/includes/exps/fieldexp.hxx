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
        : Exp (location),
          _head (&head),
          _tail (&tail)
    {
    }

    /** \brief Destroy a Field Expression node.
    **
    ** Delete name and init (see constructor). */
    virtual ~FieldExp()
    {
        delete _head;
        delete _tail;
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
        return _head;
    }

    /** \brief Return the initial value of the field (read only). */
    const Exp* getTail() const
    {
        return _tail;
    }

    /** \brief Return the initial value of the field (read and write). */
    Exp* getHead()
    {
        return _head;
    }

    /** \brief Return the initial value of the field (read and write). */
    Exp* getTail()
    {
        return _tail;
    }
    /** \} */

    virtual ExpType getType()
    {
        return FIELDEXP;
    }
    inline bool isFieldExp() const
    {
        return true;
    }
protected:
    /** \brief Name of the field. */
    Exp* _head;
    /** \brief Initial value of the field. */
    Exp* _tail;
};

/** \brief Define shortand type for Field Expression list. */
typedef std::list<FieldExp *> fieldexps_t;

} // namespace ast

#endif // !AST_FIELDEXP_HXX
