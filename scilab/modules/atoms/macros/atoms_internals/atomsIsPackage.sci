// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return true if the package is a valid package or not

function result = atomsIsPackage(package_names,package_versions)
	
	// Initialize
	// =========================================================================
	result = [];
	
	// Check input parameters
	// =========================================================================
	rhs  = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsIsPackage",1,2));
	end
	
	if type(package_names) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsPackage",1));
	end
	
	if (rhs == 2) & type(package_versions) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsPackage",1));
	end
	
	if (rhs == 2) & or( size(package_names) <> size(package_versions) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsIsPackage",1,2));
	end
	
	// Get all package description
	// =========================================================================
	allpackages = atomsGetTOOLBOXES();
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(package_names,"*")
		
		result(i) = %F;
		
		if rhs == 1 then
			version = "";
		else
			version = package_versions(i);
		end
		
		// 1st case : just test the name, not the version
		if isempty(version) & isfield(allpackages,package_names(i)) then
			result(i) = %T;
		
		// 2nd case : Check a specific version
		elseif isfield(allpackages,package_names(i)) then
			this_package = allpackages(package_names(i));
			if isfield(this_package,version) then
				result(i) = %T;
			end
		end
		
	end
	
endfunction
