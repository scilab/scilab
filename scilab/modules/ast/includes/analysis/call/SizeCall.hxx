/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SIZE_CALL_HXX__
#define __SIZE_CALL_HXX__

#include "Call.hxx"

namespace analysis
{
    class SizeCall : public Call
    {

    public:

	enum Kind { R, C, RC, R_C, ONE, BOTH, DUNNO };

    private:
	
	Kind kind;
	
    public:

        SizeCall(Kind _kind) : Call(L"size"), kind(_kind) { }

	inline Kind getKind() const
	    {
		return kind;
	    }
    };

} // namespace analysis

#endif // __SIZE_CALL_HXX__
