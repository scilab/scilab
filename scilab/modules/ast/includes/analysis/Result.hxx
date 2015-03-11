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
#include "TIType.hxx"
#include "tools.hxx"

namespace analysis
{
    class Result
    {

    public:

        enum FnName { ZEROS, ONES, RAND, DUNNO };

    private:

	bool visited;
        TIType type;
	tools::IntType inttype;
        int tempId;
        FnName fnname;
	double value;
	bool hasValue;
	GVN::Value * gvnValue;
	
    public:

        Result() : visited(false), type(), inttype(tools::NOTANINT), tempId(-1), hasValue(false), gvnValue(nullptr) { }
        Result(const TIType & _type, const int _tempId = -1) : visited(true), type(_type), inttype(tools::NOTANINT), tempId(_tempId), hasValue(false), gvnValue(nullptr) { }
        Result(TIType && _type, const int _tempId = -1) : visited(true), type(_type), inttype(tools::NOTANINT), tempId(_tempId), hasValue(false), gvnValue(nullptr) { }

        inline bool istemp() const
            {
                return tempId >= 0;
            }

        inline void setFnName(FnName _fnname)
            {
		visited = true;
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

	inline tools::IntType getIntType() const
	    {
		return inttype;
	    }

	inline void setIntType(tools::IntType it)
	    {
		visited = true;
		inttype = it;
	    }

	inline void setValue(const double _value)
	    {
		setValue(_value, tools::getIntType(_value));
	    }

	inline void setValue(const double _value, const tools::IntType _inttype)
	    {
		value = _value;
		inttype = _inttype;
		hasValue = true;
		visited = true;
	    }

	inline void setGVNValue(GVN::Value * _value)
	    {
		gvnValue = _value;
	    }

	inline GVN::Value * getGVNValue()
	    {
		return gvnValue;
	    }

	inline bool getValue(double & _value)
	    {
		if (hasValue)
		{
		    _value = value;
		    return true;
		}
		return false;
	    }

	inline bool hasBeenVisited() const
	    {
		return visited;
	    }

        friend std::wostream & operator<<(std::wostream & out, const Result & res)
            {
                out << L"Result {" << res.type << L", temp id:" << res.tempId << L", int:" << res.inttype << L"}";
                return out;
            }
    };

} // namespace analysis

#endif // __RESULT_HXX__
