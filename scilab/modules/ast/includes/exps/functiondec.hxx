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
** \file functiondec.hxx
** Define the Function Declaration class.
*/

#ifndef AST_FUNCTIONDEC_HXX
#define AST_FUNCTIONDEC_HXX

#include <list>

#include "dec.hxx"
#include "symbol.hxx"
#include "exp.hxx"
#include "arraylistvar.hxx"

using namespace std;

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
                 symbol::Symbol& name,
                 Exp& args,
                 Exp& returns,
                 Exp& body)
        : Dec (location),
          _name (name),
          _stack(NULL)
    {
        args.setParent(this);
        returns.setParent(this);
        body.setParent(this);
        _exps.push_back(&args);
        _exps.push_back(&returns);
        _exps.push_back(&body);
    }

    virtual ~FunctionDec ()
    {
        //body will be deleted by types::Macro
        //so replace by NULL to avoir delete in ~Exp()
        _exps[2] = NULL;
    }

    virtual FunctionDec* clone()
    {
        FunctionDec* cloned = new FunctionDec(getLocation(), *new symbol::Symbol(getSymbol().getName()), *getArgs().clone(), *getReturns().clone(), *getBody().clone());
        cloned->setVerbose(isVerbose());
        return cloned;
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
    symbol::Symbol& getSymbol(void) const
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

    const Exp& getArgs() const
    {
        return *_exps[0];
    }

    Exp& getArgs()
    {
        return *_exps[0];
    }

    const Exp& getReturns() const
    {
        return *_exps[1];
    }

    Exp& getReturns()
    {
        return *_exps[1];
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

    virtual ExpType getType()
    {
        return FUNCTIONDEC;
    }
    inline bool isFunctionDec() const
    {
        return true;
    }
protected:
    symbol::Symbol& _name;
    symbol::Variable* _stack;
};

} // namespace ast

#endif // !AST_FUNCTIONDEC_HXX
