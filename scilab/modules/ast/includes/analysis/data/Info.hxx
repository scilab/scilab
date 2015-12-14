/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __INFO_HXX__
#define __INFO_HXX__

#include <iostream>

#include "TIType.hxx"
#include "Data.hxx"
#include "gvn/GVN.hxx"
#include "gvn/SymbolicRange.hxx"
#include "ConstantValue.hxx"

namespace ast
{
class Exp;
}

namespace analysis
{

struct Info
{
    enum Local
    {
        INFO_TRUE, INFO_FALSE, INFO_UNKNOWN
    };

    bool R;
    bool W;
    bool O;
    bool isint;
    Local local;
    bool cleared;
    bool exists;
    TIType type;
    Data * data;
    ast::Exp * exp;
    ConstantValue constant;
    SymbolicRange range;
    SymbolicDimension maxIndex;

    Info(Data * _data = nullptr) : R(false), W(false), O(false), isint(false), local(Local::INFO_TRUE), cleared(false), exists(true), data(_data), exp(nullptr) { }
    Info(const Info & i) : R(i.R), W(i.W), O(i.O), isint(i.isint), local(i.local), cleared(i.cleared), exists(i.exists), constant(i.constant), range(i.range), maxIndex(i.maxIndex), type(i.type), data(i.data ? new Data(*i.data) : nullptr), exp(i.exp) { }

    void merge(const Info & info, bool & isSameData);
    void addData(const bool known, const symbol::Symbol & sym);
    void addData(Data * _data, const symbol::Symbol & sym);
    SymbolicRange & getRange();
    const SymbolicRange & getRange() const;
    SymbolicDimension & getMaxIndex();
    const SymbolicDimension & getMaxIndex() const;
    SymbolicRange & setRange(SymbolicRange & _range);
    ConstantValue & getConstant();
    const ConstantValue & getConstant() const;
    ConstantValue & setConstant(ConstantValue & val);
    bool isknown() const;
    bool isAnInt() const;
    static const symbol::Symbol & getRightSym(ast::Exp * exp);
    const TIType & getType() const;
    friend std::wostream & operator<<(std::wostream & out, const Info & info);
};

} // namespace analysis

#endif // __INFO_HXX__
