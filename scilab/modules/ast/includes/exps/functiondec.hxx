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
** \file functiondec.hxx
** Define the Function Declaration class.
*/

#ifndef AST_FUNCTIONDEC_HXX
#define AST_FUNCTIONDEC_HXX

#include <list>

#include "macro.hxx"

#include "context.hxx"
#include "dec.hxx"
#include "arraylistvar.hxx"
#include "seqexp.hxx"

namespace ast
{
/*
** \brief Abstract a Function Declaration node.
**
** \b Example: function foo(s : string) : int = (print(s); 2097)
*/
class FunctionDec : public Dec
{
    // \name Ctor & dtor.
public:
    /*
    ** \brief Construct a Function Declaration node.
    ** \param location scanner position informations
    ** \param name of function
    ** \param list of params
    ** \param list of returns
    ** \param body
    */
    FunctionDec (const Location& location,
                 symbol::Symbol name,
                 Exp& args,
                 Exp& returns,
                 SeqExp& body)
        : Dec (location),
          _name (name),
          _stack(nullptr),
          macro(nullptr)
    {
        args.setParent(this);
        returns.setParent(this);
        body.setParent(this);
        _exps.push_back(&args);
        _exps.push_back(&returns);
        _exps.push_back(body.getAs<Exp>());

        body.setReturnable();
    }

    virtual ~FunctionDec ()
    {
        //body will be deleted by types::Macro
        //so replace by NULL to avoir delete in ~Exp()
        //_exps[2] = NULL;
        if (macro)
        {
            macro->DecreaseRef();
            macro->killMe();
        }
    }

    virtual FunctionDec* clone()
    {
        FunctionDec* cloned = new FunctionDec(getLocation(), getSymbol(), *getArgs().clone(), *getReturns().clone(), *getBody().clone()->getAs<SeqExp>());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return Exp::equal(e) && _name == static_cast<const FunctionDec &>(e)._name;
    }

    // \name Visitors entry point.
public:
    // \brief Accept a const visitor
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    // \brief Accept a non-const visitor
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }


    // \name Accessors.
public:
    const symbol::Symbol & getSymbol(void) const
    {
        return _name;
    }

    const Exp& getBody(void) const
    {
        return *_exps[2];
    }

    Exp& getBody (void)
    {
        return *_exps[2];
    }

    inline const ArrayListVar & getArgs() const
    {
        return *static_cast<const ArrayListVar *>(_exps[0]);
    }

    inline ArrayListVar & getArgs()
    {
        return *static_cast<ArrayListVar *>(_exps[0]);
    }

    const ArrayListVar & getReturns() const
    {
        return *static_cast<const ArrayListVar *>(_exps[1]);
    }

    ArrayListVar & getReturns()
    {
        return *static_cast<ArrayListVar *>(_exps[1]);
    }

    void setBody(Exp *body)
    {
        _exps[2] = body;
    }

    symbol::Variable* getStack()
    {
        if (_stack == NULL)
        {
            _stack = symbol::Context::getInstance()->getOrCreate(_name);
        }

        return _stack;
    }

    virtual ExpType getType() const
    {
        return FUNCTIONDEC;
    }
    inline bool isFunctionDec() const
    {
        return true;
    }

    inline const types::Macro * getMacro() const
    {
        return macro;
    }

    inline types::Macro * getMacro()
    {
        return macro;
    }

    inline void setMacro(types::Macro * _macro)
    {
        macro = _macro;
        if (macro)
        {
            macro->IncreaseRef();
        }
    }

protected:
    symbol::Symbol _name;
    symbol::Variable * _stack;
    types::Macro * macro;
};

} // namespace ast

#endif // !AST_FUNCTIONDEC_HXX
