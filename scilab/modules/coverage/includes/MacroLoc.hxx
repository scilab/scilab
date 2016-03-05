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
