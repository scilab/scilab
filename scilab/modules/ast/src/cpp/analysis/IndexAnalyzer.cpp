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

namespace analysis
{

bool AnalysisVisitor::analyzeIndices(TIType & type, ast::CallExp & ce)
{
    const ast::exps_t args = ce.getArgs();
    const unsigned int size = args.size();

    if (size >= 3)
    {
        // Not handle yet...
        // TODO
        return false;
    }

    if (size == 0)
    {
        Result & res = ce.getDecorator().setResult(type);
        setResult(res);
        return true;
    }

    SymbolicDimension first, second;
    bool safe, ret;

    argIndices.emplace(static_cast<ast::SimpleVar &>(ce.getName()), size, 1);
    if (size == 1)
    {
        // when there is one argument, a(?) is equivalent to A(?,1)
        // where A = matrix(a, r_a * c_a, 1)

        SymbolicDimension rows(type.rows);
        second = SymbolicDimension(getGVN(), 1);
        if (type.cols != 1)
        {
            rows *= type.cols;
        }
        ret = getDimension(rows, *args.front(), safe, first);
    }
    else
    {
        bool _safe;
        ret = getDimension(type.rows, *args.front(), _safe, first);
        if (ret)
        {
            argIndices.top().getIndex() = 2;
            ret = getDimension(type.cols, *args.back(), safe, second);
            safe = safe && _safe;
        }
        else
        {
            safe = _safe;
        }
    }
    argIndices.pop();

    if (ret)
    {
        TIType typ(getGVN(), type.type, first, second);
        Result & _res = ce.getDecorator().setResult(typ);
        setResult(_res);
        ce.getDecorator().safe = safe;
    }

    return ret;
}


bool AnalysisVisitor::getDimension(SymbolicDimension & dim, ast::Exp & arg, bool & safe, SymbolicDimension & out)
{
    switch (arg.getType())
    {
        case ast::Exp::COLONVAR :
        {
            out = dim;
            safe = true;
            arg.getDecorator().setDollarInfo(argIndices.top());
            return true;
        }
        case ast::Exp::DOLLARVAR : // a($)
        {
            out = SymbolicDimension(getGVN(), 1.);
            safe = true;
            arg.getDecorator().setDollarInfo(argIndices.top());
            return true;
        }
        case ast::Exp::DOUBLEEXP : // a(12) or a([1 2])
        {
            ast::DoubleExp & de = static_cast<ast::DoubleExp &>(arg);
            if (types::InternalType * const pIT = de.getConstant())
            {
                if (pIT->isDouble())
                {
                    types::Double * const pDbl = static_cast<types::Double *>(pIT);
                    if (pDbl->isEmpty())
                    {
                        out = SymbolicDimension(getGVN(), 0.);
                        safe = true;
                        return true;
                    }

                    const double * real = pDbl->getReal();
                    const int size = pDbl->getSize();
                    int64_t max;
                    if (tools::asInteger(real[0], max))
                    {
                        int64_t min = max;
                        if (!pDbl->isComplex())
                        {
                            for (int i = 0; i < size; ++i)
                            {
                                int64_t _real;
                                if (tools::asInteger(real[i], _real))
                                {
                                    if (_real < min)
                                    {
                                        min = _real;
                                    }
                                    else if (_real > max)
                                    {
                                        max = _real;
                                    }
                                }
                                else
                                {
                                    return false;
                                }
                            }

                            out = SymbolicDimension(getGVN(), size);
                            safe = (min >= 1) && getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue(max));
                            return true;
                        }
                        else
                        {
                            const double * imag = pDbl->getImg();
                            int i;
                            for (i = 0; i < size; ++i)
                            {
                                if (imag[i])
                                {
                                    break;
                                }
                                int64_t _real;
                                if (tools::asInteger(real[i], _real))
                                {
                                    if (_real < min)
                                    {
                                        min = _real;
                                    }
                                    else if (_real > max)
                                    {
                                        max = _real;
                                    }
                                }
                            }

                            if (i == size)
                            {
                                out = SymbolicDimension(getGVN(), size);
                                safe = (min >= 1) && getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue(max));
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (pIT->isImplicitList())
                {
                    types::ImplicitList * const pIL = static_cast<types::ImplicitList *>(pIT);
                    double start, step, end;
                    if (AnalysisVisitor::asDouble(pIL->getStart(), start) && AnalysisVisitor::asDouble(pIL->getStep(), step) && AnalysisVisitor::asDouble(pIL->getEnd(), end))
                    {
                        double single;
                        const int type = ForList64::checkList(start, end, step, single);

                        switch (type)
                        {
                            case 0 :
                            {
                                out = SymbolicDimension(getGVN(), 0.);
                                safe = true;
                                return true;
                            }
                            case 1 :
                            {
                                out = SymbolicDimension(getGVN(), 1.);
                                safe = false;
                                return true;
                            }
                            case 2 :
                            {
                                const uint64_t N = ForList64::size(start, end, step);
                                uint64_t max, min;
                                if (step > 0)
                                {
                                    min = start;
                                    max = (uint64_t)(start + (N - 1) * step);
                                }
                                else
                                {
                                    max = start;
                                    min = (uint64_t)(start + (N - 1) * step);
                                }

                                out = SymbolicDimension(getGVN(), N);
                                safe = (min >= 1) && getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue((int64_t)max));
                                return true;
                            }
                        }
                    }
                }
            }
            else
            {
                out = SymbolicDimension(getGVN(), 1.);
                safe = (de.getValue() >= 1) && getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue(de.getValue()));
                return true;
            }
            return false;
        }
        case ast::Exp::BOOLEXP : // a(a > 1) => a([%f %t %t]) => a([2 3])
        {
            ast::BoolExp & be = static_cast<ast::BoolExp &>(arg);
            if (types::InternalType * const pIT = be.getConstant())
            {
                if (pIT->isBool())
                {
                    types::Bool * const pBool = static_cast<types::Bool *>(pIT);
                    const int size = pBool->getSize();
                    const int * data = pBool->get();
                    int64_t max = -1;
                    int64_t count = 0;
                    for (int i = 0; i < size; ++i)
                    {
                        if (data[i])
                        {
                            ++count;
                            max = i;
                        }
                    }

                    out = SymbolicDimension(getGVN(), count);
                    safe = getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue(max));
                    return true;
                }
            }
            else
            {
                if (be.getValue())
                {
                    out = SymbolicDimension(getGVN(), int64_t(1));
                }
                else
                {
                    out = SymbolicDimension(getGVN(), int64_t(0));
                }
                safe = true;
                return true;
            }
            return false;
        }
        case ast::Exp::LISTEXP :
        {
            ast::ListExp & le = static_cast<ast::ListExp &>(arg);
            SymbolicList sl;
            if (SymbolicList::get(*this, le, sl))
            {
                if (sl.isSymbolic())
                {
                    sl.evalDollar(getGVN(), dim.getValue());
                }
                TIType typ;
                if (sl.getType(getGVN(), typ))
                {
                    out = SymbolicDimension(getGVN(), typ.cols.getValue());
                    safe = false;//getCM().check(ConstraintManager::GREATER, dim.getValue(), getGVN().getValue(max));
                    return true;
                }
            }
            return false;
        }
        default :
        {
            arg.accept(*this);
            Result & _res = getResult();
            SymbolicRange & range = _res.getRange();
            if (range.isValid())
            {
                //std::wcerr << *range.getStart()->poly << ":" << *range.getEnd()->poly << ",," << *dim.getValue()->poly << std::endl;
                safe = getCM().check(ConstraintManager::VALID_RANGE, range.getStart(), range.getEnd(), getGVN().getValue(int64_t(1)), dim.getValue());
                out = _res.getType().rows * _res.getType().cols;

                return true;
            }

            if (GVN::Value * const v = _res.getConstant().getGVNValue())
            {
                GVN::Value * w = v;
                if (GVN::Value * const dollar = getGVN().getExistingValue(symbol::Symbol(L"$")))
                {
                    if (GVN::Value * const x = SymbolicList::evalDollar(getGVN(), v, dollar, dim.getValue()))
                    {
                        w = x;
                    }
                }
                bool b = getCM().check(ConstraintManager::GREATER, dim.getValue(), w);
                if (b)
                {
                    safe = getCM().check(ConstraintManager::STRICT_POSITIVE, w);
                }
                else
                {
                    safe = false;
                }
                out = SymbolicDimension(getGVN(), 1);
                return true;
            }

            // To use with find
            // e.g. a(find(a > 0)): find(a > 0) return a matrix where the max index is rc(a) so the extraction is safe
            if (_res.getType().ismatrix() && _res.getType().type != TIType::BOOLEAN)
            {
                out = _res.getType().rows * _res.getType().cols;
                SymbolicDimension & maxIndex = _res.getMaxIndex();
                if (maxIndex.isValid())
                {
                    safe = getCM().check(ConstraintManager::GREATER, dim.getValue(), maxIndex.getValue());
                }
                else
                {
                    safe = false;
                }
                return true;
            }
            return false;
        }
    }
}


}
