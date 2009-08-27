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

function [res,version_out] = atomsIsLoaded(name,version)
	
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
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsIsLoaded",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsLoaded",1));
	end
	
	if rhs>1 &  (~isempty(version)) & type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsLoaded",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if rhs>1 & version<>[] & or(size(name)<>size(version)) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsIsLoaded",1,2));
	end
	
	// Value of version if not precised
	// =========================================================================
	
	if rhs < 2 then
		version = [];
	end
	
	// Get the list of installed packages
	// =========================================================================
	packages = atomsGetLoaded();
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		if isempty(version) then
			// Just check the name
			res(i) = or(packages(:,1) == name(i));
			
		else
			// Filter on names
			packages_version = packages( find(packages(:,1) == name(i)) , 2 );
			
			// Check if the wanted version is present$
			res(i) = or(packages_version == version(i) );
		end
		
		if lhs>1 then
			if res(i) then
				version_out(i) = packages( find(packages(:,1) == name(i)) , 2 )
			else
				version_out(i) = "";
			end
		end
		
	end
	
	// Reshape the matrix [res]
	// =========================================================================
	res = matrix(res,size(name) );
	
	// Reshape the matrix [version_out]
	// =========================================================================
	if lhs > 1
		version_out = matrix(version_out,size(name) );
	end
	
endfunction
