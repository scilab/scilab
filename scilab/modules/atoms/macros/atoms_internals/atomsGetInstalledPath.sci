// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function res = atomsGetInstalledPath(name,version,allusers)
	
	rhs           = argn(2);
	res           = [];
	installed     = []; // Column vector that contain user repositories
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledPath",1,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetInstalledPath",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetInstalledPath",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or(size(name)<>size(version)) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsGetInstalledPath",1,2));
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
		
		// Filter on names
		this_packages = packages( find(packages(:,1) == name(i))     ,:);
		
		if this_packages == [] then
			res(i) = "";
			continue;
		end
		
		// Filter on versions
		this_packages = this_packages( find(this_packages(:,2) == version(i)) ,:);
		
		if this_packages == [] then
			res(i) = "";
			continue;
		end
		
		res(i) = this_packages(4);
	end
	
	// Reshape the matrix
	// =========================================================================
	res = matrix(res,size(name) );
	
endfunction
