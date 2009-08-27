// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Returns TRUE if a package is installed, FALSE otherwise

// Input parameters :
//   name     : matrix of strings
//   version  : matrix of strings
//   allusers : boolean

// Output parameters :
//   res      : matrix of boolean

function res = atomsIsInstalled(name,version,allusers)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs           = argn(2);
	res           = [];
	installed     = []; // Column vector that contain user repositories
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsIsInstalled",1,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsInstalled",1));
	end
	
	if rhs>1 &  (~isempty(version)) & type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsInstalled",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if rhs>1 & version<>[] & or(size(name)<>size(version)) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsIsInstalled",1,2));
	end
	
	// Value of version if not precised
	// =========================================================================
	
	if rhs < 2 then
		version = [];
	end
	
	// allusers management
	// =========================================================================
	
	if rhs < 3 then
		allusers = %T;
	end
	
	// Get the list of installed packages
	// =========================================================================
	packages = atomsGetInstalled(allusers);
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		if isempty(version) then
			// Just check the name
			res(i) = or(packages(:,1) == name(i));
		
		else
			// Filter on names
			packages_version = packages( find(packages(:,1) == name(i)) , 2 );
			
			// Check if the wnated version is present$
			res(i) = or(packages_version == version(i) );
		end
		
	end
	
	// Reshape the matrix
	// =========================================================================
	res = matrix(res,size(name) );
	
endfunction
