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

#ifndef AST_CALLEXP_HXX
#define AST_CALLEXP_HXX

#include <list>
#include "exp.hxx"

namespace ast
{
/*
** \brief Abstract a Call Expression node.
**
** \b Example: foo(2097)
*/
class CallExp : public Exp
{
    // \brief Ctor & dtor.
public:
    /*
    ** \brief Construct a Call Expression node.
    ** \param location scanner position informations
    ** \param name of the function
    ** \param list of the arguments
    */
    CallExp (const Location& location,
             Exp& name,
             exps_t& args) :
        Exp (location)
    {
        _exps.push_back(&name);
        name.setParent(this);

        for (exps_t::const_iterator it = args.begin(), itEnd = args.end(); it != itEnd; ++it)
        {
            (*it)->setParent(this);
            _exps.push_back(*it);
        }
    }

    virtual CallExp* clone()
    {
        exps_t args;
        for (exps_t::const_iterator it = ++(_exps.begin()); it != _exps.end() ; ++it)
        {
            args.push_back((*it)->clone());
        }

        CallExp* cloned = new CallExp(getLocation(), *getName().clone(), args);
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    // \brief Visitors entry point.
public:
    // \brief Accept a const visitor
    virtual void	accept(Visitor& v)
    {
        v.visit (*this);
    }
    // \brief Accept a non-const visitor
    virtual void	accept(ConstVisitor& v) const
    {
        v.visit (*this);
    }


    // \brief Accessors.
public:
    const Exp& getName() const
    {
        return *_exps[0];
    }

    Exp& getName()
    {
        return *_exps[0];
    }

    void setName(Exp *name)
    {
        _exps[0] = name;
    }

    const exps_t& getArgs() const
    {
        exps_t* args = new exps_t;
        for (exps_t::const_iterator it = ++(_exps.begin()), itEnd = _exps.end(); it != itEnd ; ++it)
        {
            args->push_back(*it);
        }
        return *args;
    }

    exps_t& getArgs()
    {
        exps_t* args = new exps_t;
        for (exps_t::const_iterator it = ++(_exps.begin()), itEnd = _exps.end(); it != itEnd ; ++it)
        {
            args->push_back(*it);
        }
        return *args;
    }

    virtual ExpType getType()
    {
        return CALLEXP;
    }
    inline bool isCallExp() const
    {
        return true;
    }

protected:
};

} // namespace ast

#endif // !AST_CALLEXP_HXX
