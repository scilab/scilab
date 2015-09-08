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
    Ast (const Location& location) : decorator(), _location (location)
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

private:
    analysis::Decorator decorator;
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
