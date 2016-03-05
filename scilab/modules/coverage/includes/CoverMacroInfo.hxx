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

#ifndef __COVER_MACRO_INFO_HXX__
#define __COVER_MACRO_INFO_HXX__

#include <iostream>
#include <string>
#include <cinttypes>

namespace coverage
{

struct CoverMacroInfo
{
    const std::wstring macroModule;
    const std::wstring macroFilePath;
    const uint64_t instrsCount;
    const uint64_t branchesCount;
    const uint64_t pathsCount;

    CoverMacroInfo(const std::wstring & _macroModule, const std::wstring & _macroFilePath, const uint64_t _instrsCount, const uint64_t _branchesCount, const uint64_t _pathsCount) : macroModule(_macroModule), macroFilePath(_macroFilePath), instrsCount(_instrsCount), branchesCount(_branchesCount), pathsCount(_pathsCount) { }

    friend std::wostream & operator<<(std::wostream & out, const CoverMacroInfo & cmi)
    {
        out << L"instrs: " << cmi.instrsCount << L" branches: " << cmi.branchesCount << L" paths: " << cmi.pathsCount;
        return out;
    }

    void toBin(std::fstream & out) const;
    static CoverMacroInfo fromBin(std::fstream & in);
};

} // namespace coverage

#endif // __COVER_MACRO_INFO_HXX__
