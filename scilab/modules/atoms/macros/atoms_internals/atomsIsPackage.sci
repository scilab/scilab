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

function result = atomsIsPackage(packages)
	
	// Initialize
	// =========================================================================
	result = [];
	
	// Check input parameters
	// =========================================================================
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsPackage",1));
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsPackage",1));
	end
	
	if size(packages(1,:),"*") > 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsIsPackage",1));
	end
	
	// If packages is mx1 matrix, add a 2nd column with empty versions
	// =========================================================================
	if size(packages(1,:),"*") == 1 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	end
	
	// Get all package description
	// =========================================================================
	allpackages = atomsGetTOOLBOXES();
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		result(i) = %F,
		name      = packages(i,1);
		version   = packages(i,2);
		
		// 1st case : just test the name, not the version
		if isempty(version) & isfield(allpackages,name) then
			result(i) = %T;
		
		// 2nd case : Check a specific version
		elseif isfield(allpackages,name) then
			result(i) = isfield(allpackages(name),version);
		end
		
	end
	
endfunction
