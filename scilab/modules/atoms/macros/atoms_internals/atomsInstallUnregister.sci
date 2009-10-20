// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Delete an atoms package from the list of available package
// This function has an impact on the following files :
//  -> ATOMSDIR/installed
//  -> ATOMSDIR/installed_deps

function atomsInstallUnregister(name,version,section)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsInstallUnregister",3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",2));
	end
	
	if type(section) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsInstallUnregister",2));
	end
	
	if and(section<>["user","allusers"]) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstallUnregister",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallUnregister",1,2));
	end
	
	// Process installed
	// =========================================================================
	installed_before = atomsLoadInstalledStruct(section);
	installed_after  = atomsRmfields(installed_before,name+" - "+version);
	atomsSaveInstalled(installed_after,section);
	
	// Process installed dependencies
	// =========================================================================
	installed_deps_before = atomsLoadInstalleddeps(section);
	installed_deps_after  = atomsRmfields(installed_deps_before,name+" - "+version);
	atomsSaveInstalleddeps(installed_deps_after,section);
	
endfunction
