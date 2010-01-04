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

function res = atomsIsInstalled(packages,section)
	
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
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsIsInstalled",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsInstalled",1));
	end
	
	if size(packages(1,:),"*") > 3 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1, mx2 or mx3 string matrix expected.\n"),"atomsIsInstalled",1));
	end
	
	// Remove leadind & trailing whitespaces
	// =========================================================================
	packages = stripblanks(packages);
	
	// Complete packages matrix with empty columns to have a mx3 matrix
	// =========================================================================
	
	if size(packages(1,:),"*") == 1 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];
	
	elseif size(packages(1,:),"*") == 2 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	
	end
	
	// "all", "user" section or "allusers" section packages ?
	// =========================================================================
	
	if rhs < 2 then
		section = "all"
		
	else
		// Just check if it's a boolean
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsIsInstalled",2));
		end
		
		if and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsIsInstalled",2));
		end
		
	end
	
	// Get the list of installed packages
	// =========================================================================
	installedpackages = atomsGetInstalled(section);
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		name    = packages(i,1);
		version = packages(i,2);
		section = packages(i,3);
		
		if packages(i,3) == "all" then
			section = "";
		else
			section = packages(i,3);
		end
		
		if isempty(version) & isempty(section) then
			// Just check the name
			res(i) = or(installedpackages(:,1) == name);
			
		elseif isempty(version) & ~isempty(section) then
			// Filter on names
			packages_filtered = installedpackages( find(installedpackages(:,1) == name) , 3 );
			
			// Check if the wanted section is present
			res(i) = or(packages_filtered == section);
			
		elseif ~isempty(version) & isempty(section) then
			// Filter on names
			packages_filtered = installedpackages( find(installedpackages(:,1) == name) , 2 );
			
			// Check if the wanted version is present
			res(i) = or(packages_filtered == version);
			
		else
			// Filter on names
			packages_filtered = installedpackages( find(installedpackages(:,1) == name) , [2 3] );
			
			// Filter on version
			packages_filtered = packages_filtered( find(packages_filtered(:,1) == version) , 2 );
			
			// Check if the wanted section is present
			res(i) = or(packages_filtered == section);
		end
		
	end
	
endfunction
