// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the most recent version of each package specified as first input argument

function versions = atomsGetMRVersion(packages)
	
	// Initialize
	versions = [];
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetMRVersion",1));
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetMRVersion",1));
	end
	
	// Get all package description
	// =========================================================================
	
	allpackages = atomsGetTOOLBOXES();
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages,"*")
		
		versions(i) = "-1";
		
		if ~ isfield(allpackages,packages(i)) then
			continue;
		end
		
		package_versions          = allpackages(packages(i));
		package_versions_tab      = getfield(1,package_versions);
		package_versions_tab(1:2) = [];
		package_versions_tab      = atomsVersionSort( package_versions_tab , "DESC" );
		versions(i)               = package_versions_tab(1);
		
	end
	
endfunction
