// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Output arguments :

//   res  :  .%T if the toolbox is loaded in this scilab session
//           . Matrix of boolean ( n x 1 ) 
//           . mandatory

//   version_out : . If the toolbox is loaded, version_out is the version of the
//                   loaded toolbox
//                   otherwise, version_out = ""
//                 . Matrix of string (n x 1)
//                 . mandatory

function [res,version_out] = atomsIsLoaded(packages)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs         = argn(2);
	lhs         = argn(1);
	res         = [];
	version_out = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsLoaded",1));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsLoaded",1));
	end
	
	if size(packages(1,:),"*") > 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsIsLoaded",1));
	end
	
	// If packages is mx1 matrix, add a 2nd column with empty versions
	// =========================================================================
	
	if size(packages(1,:),"*") == 1 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	end
	
	// Get the list of installed packages
	// =========================================================================
	loadedpackages = atomsGetLoaded();
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		name    = packages(i,1);
		version = packages(i,2);
		
		if isempty(version) then
			// Just check the name
			res(i) = or(loadedpackages(:,1) == name);
			
		else
			// Filter on names
			packages_version = loadedpackages( find(loadedpackages(:,1) == name) , 2 );
			
			// Check if the wanted version is present
			res(i) = or(packages_version == version);
		end
		
		if lhs>1 then
			if res(i) then
				version_out(i) = loadedpackages( find(loadedpackages(:,1) == name) , 2 )
			else
				version_out(i) = "";
			end
		end
		
	end
	
endfunction
