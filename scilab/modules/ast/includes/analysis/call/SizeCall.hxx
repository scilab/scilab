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
