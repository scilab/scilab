/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __RESULT_HXX__
#define __RESULT_HXX__

#include <iostream>

#include "gvn/GVN.hxx"
#include "gvn/SymbolicDimension.hxx"
#include "gvn/SymbolicRange.hxx"
#include "TIType.hxx"
#include "tools.hxx"
#include "ConstantValue.hxx"

namespace analysis
{
class Result
{

public:

    enum FnName { ZEROS, ONES, RAND, DUNNO };

private:

    TIType type;
    int tempId;
    uint64_t functionId;
    FnName fnname;
    ConstantValue constant;
    SymbolicRange range;
    SymbolicDimension maxIndex;

public:

    Result() : type(), tempId(-1), functionId(0), fnname(DUNNO), constant(), range(), maxIndex() { }
    Result(const TIType & _type, const int _tempId = -1, const uint64_t _functionId = 0) : type(_type), tempId(_tempId), functionId(_functionId), fnname(DUNNO), constant(), range(), maxIndex() { }
    Result(TIType && _type, const int _tempId = -1, const uint64_t _functionId = 0) : type(_type), tempId(_tempId), functionId(_functionId), fnname(DUNNO), constant(), range(), maxIndex() { }

    inline bool istemp() const
    {
        return tempId >= 0;
    }

    inline void setFnName(FnName _fnname)
    {
        fnname = _fnname;
    }

    inline FnName getFnName() const
    {
        return fnname;
    }

    inline const TIType & getType() const
    {
        return type;
    }

    inline TIType & getType()
    {
        return type;
    }

    inline int getTempId() const
    {
        return tempId;
    }

    inline bool isTemp() const
    {
        return tempId != -1;
    }

    inline void setFunctionId(const uint64_t id)
    {
        functionId = id;
    }

    inline uint64_t getFunctionId() const
    {
        return functionId;
    }

    inline bool hasGVNValue() const
    {
        return constant.getGVNValue() != nullptr;
    }

    inline ConstantValue & getConstant()
    {
        return constant;
    }

    inline const ConstantValue & getConstant() const
    {
        return constant;
    }

    inline ConstantValue & setConstant(ConstantValue & val)
    {
        constant = val;
        return constant;
    }

    inline SymbolicRange & getRange()
    {
        return range;
    }

    inline const SymbolicRange & getRange() const
    {
        return range;
    }

    inline SymbolicRange & setRange(SymbolicRange & _range)
    {
        range = _range;
        return range;
    }

    inline SymbolicRange & setRange(SymbolicRange && _range)
    {
        range = _range;
        return range;
    }

    inline bool isAnInt() const
    {
        return hasGVNValue() || getRange().isValid();
    }

    inline SymbolicDimension & getMaxIndex()
    {
        return maxIndex;
    }

    inline const SymbolicDimension & getMaxIndex() const
    {
        return maxIndex;
    }

    inline SymbolicDimension & setMaxIndex(SymbolicDimension & _maxIndex)
    {
        maxIndex = _maxIndex;
        return maxIndex;
    }

    inline SymbolicDimension & setMaxIndex(SymbolicDimension && _maxIndex)
    {
        maxIndex = _maxIndex;
        return maxIndex;
    }

    friend std::wostream & operator<<(std::wostream & out, const Result & res)
    {
        out << L"Result {" << res.type;
        if (res.tempId != -1)
        {
            out << L", temp id:" << res.tempId;
        }
        if (res.functionId)
        {
            out << L", function id:" << res.functionId;
        }
        if (res.constant.isKnown())
        {
            out << L", constant:" << res.constant;
        }
        if (res.isAnInt())
        {
            out << L", isAnInt: T";
        }
        out << L'}';

        return out;
    }
};

} // namespace analysis

#endif // __RESULT_HXX__
