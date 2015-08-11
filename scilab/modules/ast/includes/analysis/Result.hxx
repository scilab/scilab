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
    FnName fnname;
    ConstantValue constant;
    SymbolicRange range;
    SymbolicDimension maxIndex;

public:

    Result() : type(), tempId(-1) { }
    Result(const TIType & _type, const int _tempId = -1) : type(_type), tempId(_tempId) { }
    Result(TIType && _type, const int _tempId = -1) : type(_type), tempId(_tempId) { }

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
        out << L"Result {" << res.type
	    << L", temp id:" << res.tempId
	    << L", constant:" << res.constant
	    << L", isAnInt:" << (res.isAnInt() ? L"T" : L"F") << L"}";
        return out;
    }
};

} // namespace analysis

#endif // __RESULT_HXX__
