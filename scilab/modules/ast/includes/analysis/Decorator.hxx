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

#ifndef __DECORATOR_HXX__
#define __DECORATOR_HXX__

#include "Call.hxx"
#include "Result.hxx"

#include <iostream>

namespace analysis
{

    struct Decorator
    {
        Result res;
	Call * call;
	bool cloneData;
	bool deleteData;
	bool hasRefCount;

        Decorator() : res(), call(nullptr), cloneData(false), deleteData(false), hasRefCount(false) { }

	~Decorator()
	    {
		delete call;
	    }

	inline Call * getCall() const
	    {
		return call;
	    }

	inline Call & setCall(Call && call)
	    {
		this->call = new Call(std::move(call));
		return *this->call;
	    }

	inline Call & setCall(Call * call)
	    {
		this->call = call;
		return *this->call;
	    }

        friend std::wostream & operator<<(std::wostream & out, const Decorator & deco)
            {
		out << deco.res;
		if (deco.call)
		{
		    out << L", " << (*deco.call);
		}
		out << L", Cl:" << (deco.cloneData ? L"T" : L"F")
		    << L", Del:" << (deco.deleteData ? L"T" : L"F")
		    << L", RefC:" << (deco.hasRefCount ? L"T" : L"F");

                return out;
            }
    };

} // namespace analysis

#endif // __DECORATOR_HXX__
