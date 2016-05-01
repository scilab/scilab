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

        delete &args;
    }

    virtual CallExp* clone()
    {
        exps_t* args = new exps_t;
        for (exps_t::const_iterator it = ++(_exps.begin()); it != _exps.end() ; ++it)
        {
            args->push_back((*it)->clone());
        }

        CallExp* cloned = new CallExp(getLocation(), *getName().clone(), *args);
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

    inline exps_t getArgs() const
    {
        return exps_t(std::next(_exps.begin()), _exps.end());
    }

    inline exps_t getArgs()
    {
        return exps_t(std::next(_exps.begin()), _exps.end());
    }

    Location getFirstLocation() const
    {
        return _exps[0]->getLocation();
    }

    Location getFirstLocation()
    {
        return _exps[0]->getLocation();
    }

    void addArg(ast::Exp* exp)
    {
        _exps.push_back(exp);
    }

    virtual ExpType getType() const
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
