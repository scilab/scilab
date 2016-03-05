/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include "AnalysisVisitor.hxx"
#include "analyzers/OperAnalyzer.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"

namespace analysis
{
bool OperAnalyzer::analyze(AnalysisVisitor & visitor, ast::Exp & e)
{
    ast::OpExp & oe = static_cast<ast::OpExp &>(e);
    const ast::OpExp::Oper oper = oe.getOper();
    if (oper == ast::OpExp::plus || oper == ast::OpExp::minus || oper == ast::OpExp::times)
    {
        if (ast::MemfillExp * mfe = analyzeMemfill(visitor, oe))
        {
            mfe->setVerbose(e.isVerbose());
            e.replace(mfe);

            return true;
        }
    }

    /*if (ast::ExtendedOpExp * eoe = analyzeMemfill(visitor, oe))
      {
      eoe->setVerbose(e.isVerbose());
      e.replace(eoe);

      return true;
      }*/

    return false;
}

/*ast::ExtendedOpExp * OperAnalyzer::analyzeTransposedArgs(ast::OpExp & oe)
  {
  ast::Exp & L = oe.getLeft();
  ast::Exp & R = oe.getRight();
  ast::ExtendedOpExp::OP Lop, Rop;
  ast::Exp * Le = &L;
  ast::Exp * Re = &R;

  if (L.isTransposeExp())
  {
  ast::TransposeExp & te = static_cast<ast::TransposeExp &>(L);
  if (te.getConjugate() == ast::TransposeExp::_Conjugate_)
  {
  Lop = ast::ExtendedOpExp::ADJOINT;
  }
  else
  {
  Lop = ast::ExtendedOpExp::TRANSP;
  }
  Le = &te.getExp();
  }

  if (R.isTransposeExp())
  {
  ast::TransposeExp & te = static_cast<ast::TransposeExp &>(R);
  if (te.getConjugate() == ast::TransposeExp::_Conjugate_)
  {
  Rop = ast::ExtendedOpExp::ADJOINT;
  }
  else
  {
  Rop = ast::ExtendedOpExp::TRANSP;
  }
  Re = &te.getExp();
  }

  if (Lop != ast::ExtendedOpExp::NONE || Rop != ast::ExtendedOpExp::NONE)
  {
  return new ast::ExtendedOpExp(oe.getLocation(), *Le, Lop, oe.getOper(), *Re, Rop);
  }

  return nullptr;
  }*/

ast::MemfillExp * OperAnalyzer::analyzeMemfill(AnalysisVisitor & visitor, ast::OpExp & oe)
{
    const ast::OpExp::Oper oper = oe.getOper();
    ast::Exp & L = oe.getLeft();
    ast::Exp & R = oe.getRight();

    ast::Exp * constant = nullptr;
    ast::MemfillExp * me = nullptr;
    ast::exps_t args;
    double value;
    bool callAtLeft;

    // We try to match something like A +* ones(...) or A +* zeros(...)
    if (L.isCallExp())
    {
        ast::CallExp & ce = static_cast<ast::CallExp &>(L);
        if (ce.getName().isSimpleVar())
        {
            const std::wstring & name = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            if (name == L"ones")
            {
                value = 1;
                args = ce.getArgs();
                constant = &R;
                callAtLeft = true;
            }
            else if (name == L"zeros")
            {
                value = 0;
                args = ce.getArgs();
                constant = &R;
                callAtLeft = true;
            }
        }
    }

    if (!constant)
    {
        if (R.isCallExp())
        {
            ast::CallExp & ce = static_cast<ast::CallExp &>(R);
            if (ce.getName().isSimpleVar())
            {
                const std::wstring & name = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol().getName();
                if (name == L"ones")
                {
                    value = 1;
                    args = ce.getArgs();
                    constant = &L;
                    callAtLeft = false;
                }
                else if (name == L"zeros")
                {
                    value = 0;
                    args = ce.getArgs();
                    constant = &L;
                    callAtLeft = false;
                }
            }
        }
    }

    if (constant && (oper == ast::OpExp::plus || oper == ast::OpExp::minus || oper == ast::OpExp::times))
    {
        Result & res = constant->getDecorator().getResult();
        if (res.getType().ismatrix() && res.getType().isscalar())
        {
            TIType ty(visitor.getGVN(), TIType::DOUBLE);
            ast::exps_t cloneArgs;
            cloneArgs.reserve(args.size());
            for (auto arg : args)
            {
                ast::Exp * cl = arg->clone();
                cl->getDecorator().setResult(arg->getDecorator().getResult());
                cloneArgs.push_back(cl);
            }

            switch (oper)
            {
                case ast::OpExp::plus :
                {
                    // plus is commutative so callAtLeft is ignored
                    // we have something like x + ones(...) => it is a fill with x+1
                    const Location & loc = oe.getLocation();
                    ast::Exp * valExp;
                    double x;
                    if (res.getConstant().getDblValue(x))
                    {
                        valExp = new ast::DoubleExp(loc, new types::Double(x + value));
                        valExp->getDecorator().setResult(Result(ty));
                    }
                    else
                    {
                        ast::Exp * cl = constant->clone();
                        cl->getDecorator().setResult(constant->getDecorator().getResult());
                        valExp = new ast::DoubleExp(loc, new types::Double(value));
                        valExp->getDecorator().setResult(Result(ty));
                        valExp = new ast::OpExp(loc, *cl, ast::OpExp::plus, *valExp);
                        valExp->getDecorator().setResult(Result(Checkers::check_____add____(visitor.getGVN(), cl->getDecorator().getResult().getType(), ty)));
                    }
                    me = new ast::MemfillExp(loc, *valExp, cloneArgs);
                    break;
                }
                case ast::OpExp::minus :
                {
                    // we have something like x - ones(...) => it is a fill with x-1
                    const Location & loc = oe.getLocation();
                    ast::Exp * valExp;
                    double x;
                    if (res.getConstant().getDblValue(x))
                    {
                        valExp = new ast::DoubleExp(loc, new types::Double(callAtLeft ? value - x : x - value));
                        valExp->getDecorator().setResult(Result(ty));
                    }
                    else
                    {
                        ast::Exp * cl = constant->clone();
                        cl->getDecorator().setResult(constant->getDecorator().getResult());
                        valExp = new ast::DoubleExp(loc, new types::Double(value));
                        valExp->getDecorator().setResult(Result(ty));
                        if (callAtLeft)
                        {
                            valExp = new ast::OpExp(loc, *valExp, ast::OpExp::minus, *cl);
                            valExp->getDecorator().setResult(Result(Checkers::check_____sub____(visitor.getGVN(), ty, cl->getDecorator().getResult().getType())));
                        }
                        else
                        {
                            valExp = new ast::OpExp(loc, *cl, ast::OpExp::minus, *valExp);
                            valExp->getDecorator().setResult(Result(Checkers::check_____sub____(visitor.getGVN(), cl->getDecorator().getResult().getType(), ty)));
                        }
                    }
                    me = new ast::MemfillExp(loc, *valExp, cloneArgs);
                    break;
                }
                case ast::OpExp::times :
                {
                    // times is commutative so callAtLeft is ignored
                    // we have something like x * ones(...) => it is a fill with x
                    const Location & loc = oe.getLocation();
                    ast::Exp * valExp;
                    double x = 0;
                    if (value == 0 || res.getConstant().getDblValue(x))
                    {
                        valExp = new ast::DoubleExp(loc, new types::Double(x * value));
                        valExp->getDecorator().setResult(Result(ty));
                    }
                    else
                    {
                        valExp = constant->clone();
                        valExp->getDecorator().setResult(constant->getDecorator().getResult());
                    }
                    me = new ast::MemfillExp(loc, *valExp, cloneArgs);
                    break;
                }
            }

            if (me)
            {
                if (callAtLeft)
                {
                    ast::CallExp & ce = static_cast<ast::CallExp &>(L);
                    me->getDecorator().setResult(ce.getDecorator().getResult());
                }
                else
                {
                    ast::CallExp & ce = static_cast<ast::CallExp &>(R);
                    me->getDecorator().setResult(ce.getDecorator().getResult());
                }
            }
        }
    }

    return me;
}
}
