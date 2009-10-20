// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Load one or several toolboxes

function atomsSystemInit()
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check if we have the write access
	// =========================================================================
	if atomsAUWriteAccess() then
		section = "all";
	else
		section = "user";
	end
	
	// Toremove process
	// =========================================================================
	atomsToremoveProcess(section);
	
endfunction
