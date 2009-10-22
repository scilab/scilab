// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function res = atomsGetInstalledPath(packages,section)
	
	rhs           = argn(2);
	res           = [];
	installed     = []; // Column vector that contain user repositories
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledPath",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetInstalledPath",1));
	end
	
	if size(packages(1,:),"*") <> 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 string matrix expected.\n"),"atomsGetInstalledPath",1));
	end
	
	packages = stripblanks(packages);
	
	// Allusers/user management
	// =========================================================================
	
	if rhs < 2 then
		section = "all";
	
	else
		
		// Process the 2nd input argument : section
		// Allusers can be equal to "user" or "allusers"
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or a single string expected.\n"),"atomsGetInstalledPath",2));
		end
		
		if (type(section) == 10) & and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledPath",2));
		end
		
	end
	
	// Get the list of installed packages
	// =========================================================================
	installedpackages = atomsGetInstalled(section);
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		// Filter on names
		this_packages = installedpackages( find(installedpackages(:,1) == packages(i,1))     ,:);
		
		if this_packages == [] then
			res(i) = "";
			continue;
		end
		
		// Filter on versions
		this_packages = this_packages( find(this_packages(:,2) == packages(i,2)) ,:);
		
		if this_packages == [] then
			res(i) = "";
			continue;
		end
		
		res(i) = this_packages(1,4);
	end
	
endfunction
