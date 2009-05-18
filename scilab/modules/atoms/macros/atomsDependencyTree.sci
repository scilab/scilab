// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Installation of a toolbox

function result = atomsDependencyTree(name,version)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDependencyTree",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDependencyTree",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDependencyTree",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name) <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDependencyTree",1));
	end
	
	if size(version) <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDependencyTree",2));
	end
	
	// Check if the wanted version is compatible with the current scilab version.
	// =========================================================================
	
	this_package_details = atomsToolboxDetails(name,version);
	
endfunction
