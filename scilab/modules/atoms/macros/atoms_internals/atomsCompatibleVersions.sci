// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the sorted list of the versions compatible with this version of Scilab

function versions = atomsCompatibleVersions(name)
	
	// Initialize
	versions = [];
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 1  then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsCompatibleVersions",1));
	end
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsCompatibleVersions",1));
	end
	
	// Get all package description
	// =========================================================================
	allpackages = atomsGetTOOLBOXES();
	
	// Check if it's a Atoms package
	// =========================================================================
	if ~ isfield(allpackages,name) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be a atoms package name.\n"),"atomsCompatibleVersions",1));
	end
	
	// Get versions
	// =========================================================================
	package_versions          = allpackages(name);
	package_versions_tab      = getfield(1,package_versions);
	package_versions_tab(1:2) = [];
	
	// Loop on package_versions_tab
	// =========================================================================
	
	for i=1:size(package_versions_tab,"*")
		this_version = package_versions(package_versions_tab(i));
		versions = [ versions ; package_versions_tab(i) ];
	end
	
	// Sort the version matrix
	// =========================================================================
	versions = atomsVersionSort(versions,"DESC");
	
endfunction
