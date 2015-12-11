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

#ifndef __MACRO_LOC_HXX__
#define __MACRO_LOC_HXX__

#include <string>

namespace coverage
{

    struct MacroLoc
    {
	const std::wstring name;
	const Location loc;
	
	MacroLoc(const std::wstring & _name, const Location & _loc) : name(_name), loc(_loc) { }
	
	bool operator<(const MacroLoc & R) const
	    {
		return (name < R.name) || ((name == R.name) && loc < R.loc);
	    }
    };


} // namespace coverage

#endif // __MACRO_LOC_HXX__
