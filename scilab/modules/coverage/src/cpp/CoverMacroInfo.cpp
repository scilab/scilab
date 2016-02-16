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

#include "CoverMacroInfo.hxx"
#include "CoverModule.hxx"

namespace coverage
{

void CoverMacroInfo::toBin(std::fstream & out) const
{
    CoverModule::write(out, macroModule);
    CoverModule::write(out, macroFilePath);
    CoverModule::write(out, instrsCount);
    CoverModule::write(out, branchesCount);
    CoverModule::write(out, pathsCount);
    out.flush();
}

CoverMacroInfo CoverMacroInfo::fromBin(std::fstream & in)
{
    const std::wstring mod = CoverModule::readWstring(in);
    const std::wstring fp = CoverModule::readWstring(in);
    const std::size_t ic = CoverModule::readUint64_t(in);
    const std::size_t bc = CoverModule::readUint64_t(in);
    const std::size_t pc = CoverModule::readUint64_t(in);

    return CoverMacroInfo(mod, fp, ic, bc, pc);
}
}
