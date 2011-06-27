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
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            FieldExp* cloned = new FieldExp(*newloc, *head_get()->clone(), *tail_get()->clone());
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
        /** \brief Return the name of the field (read only). */
        const Exp* head_get() const
        {
            return _head;
        }

        /** \brief Return the initial value of the field (read only). */
        const Exp* tail_get() const
        {
            return _tail;
        }

        /** \brief Return the initial value of the field (read and write). */
        Exp* head_get()
        {
            return _head;
        }

        /** \brief Return the initial value of the field (read and write). */
        Exp* tail_get()
        {
            return _tail;
        }
        /** \} */

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
