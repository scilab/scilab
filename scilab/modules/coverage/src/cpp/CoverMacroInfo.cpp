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
