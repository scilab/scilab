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

#include "DollarInfo.hxx"
#include "simplevar.hxx"

namespace analysis
{

std::wostream & operator<<(std::wostream & out, const DollarInfo & di)
{
    out << L"$ in " << di.var.getSymbol() << L" at index " << di.index;
    return out;
}
}
