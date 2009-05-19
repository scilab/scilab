// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the sorted version list of the package entered as input argument

function versions = atomsGetVersions(name,min_version,max_version)
	
	// Initialize
	versions = [];
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs < 1 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetVersions",1,3));
	end
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",1));
	end
	
	if (rhs>1) & (type(min_version)<>10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",2));
	end
	
	if (rhs>2) & (type(max_version)<>10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",3));
	end
	
	// Default values
	// =========================================================================
	
	if (rhs<2) | ( (rhs>=2) & (min_version=="") ) then
		min_version = "0";
	end
	
	if (rhs<3) | ( (rhs>=3) & (max_version=="") ) then
		max_version = "9999999";
	end
	
	// Get all package description
	// =========================================================================
	allpackages = atomsGetTOOLBOXES();
	
	// Check if it's a Atoms package
	// =========================================================================
	if ~ isfield(allpackages,name) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be a atoms package name.\n"),"atomsGetVersions",1));
	end
	
	// Get versions
	// =========================================================================
	package_versions          = allpackages(name);
	package_versions_tab      = getfield(1,package_versions);
	package_versions_tab(1:2) = [];
	
	// Delete out of bounds versions
	// =========================================================================
	package_versions_tab(find((atomsVersionCompare(package_versions_tab,max_version) > 0)|(atomsVersionCompare(package_versions_tab,min_version) < 0))) = [];
	
	// Sort the version matrix
	// =========================================================================
	versions = atomsVersionSort(package_versions_tab,"DESC");
	
endfunction
