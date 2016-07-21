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

/**
** \file ast.hxx
** Define the Abstract Syntax Tree mother class.
*/

#ifndef AST_AST_HXX
#define AST_AST_HXX

#include "location.hxx"
#include "visitor.hxx"
#include "Decorator.hxx"

namespace ast
{

/** \brief Abstract an Abstract Syntax Tree node. */
class Ast
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Ast node.
    ** \param location scanner position informations */
    Ast (const Location& location) : decorator(), coverId(0), _location (location), m_dblElapsedTime(0)
    {
        nodeNumber = globalNodeNumber++;
    }


    /** \brief Destroys an Ast node. */
    virtual ~Ast ()
    {
    }
    /** \} */

    virtual Ast* clone() = 0;

    /** \name Accessors.
    ** \{ */
public:
    /** \brief Set scanner position informations.
    ** \param location scanner position informations
    **
    ** Return a reference to this.
    */
    Ast& setLocation(const Location& location)
    {
        _location = location;
        return *this;
    }
    /** \brief Get scanner position informations stored. */
    inline const Location& getLocation() const
    {
        return _location;
    }

    /** \brief Get scanner position informations stored. */
    inline Location& getLocation()
    {
        return _location;
    }
    /** \} */

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (ConstVisitor& v) const = 0;
    /** \brief Accept a non const visitor \a v. */
    virtual void accept (Visitor& v) = 0;
    /** \} */

    void setElapsedtime(double _dblElapsedTime)
    {
        m_dblElapsedTime = _dblElapsedTime;
    }

    size_t getNodeNumber() const
    {
        return nodeNumber;
    }

    void setNodeNumber(size_t _nodeNumber)
    {
        nodeNumber = _nodeNumber;
    }

    const analysis::Decorator & getDecorator() const
    {
        return decorator;
    }

    analysis::Decorator & getDecorator()
    {
        return decorator;
    }

    inline void setCoverId(const uint64_t id)
    {
        coverId = id;
    }

    inline uint64_t getCoverId() const
    {
        return coverId;
    }

    inline uint64_t getCoverId()
    {
        return coverId;
    }

private:
    analysis::Decorator decorator;
    uint64_t coverId;
    /** \brief Construct an Ast by copy. */
    Ast (const Ast&);
    /** \brief Assign an Ast to this. */
    Ast& operator= (const Ast&);

    /** \brief Scanner position informations. */
    Location _location;
    double m_dblElapsedTime;
    size_t nodeNumber;
    static EXTERN_AST size_t globalNodeNumber;
};

} // namespace ast

#endif // !AST_AST_HXX
