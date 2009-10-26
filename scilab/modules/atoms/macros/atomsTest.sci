// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Output argument :

//   path : . If the toolbox is loaded, path is the path of the loaded toolbox
//            otherwise, path = ""
//          . Matrix of string (n x 1)
//          . mandatory

function path = atomsTest(name,version)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs         = argn(2);
	lhs         = argn(1);
	res         = [];
	path        = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsTest",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsTest",1));
	end
	
	if rhs>1 &  (~isempty(version)) & type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsTest",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if rhs>1 & version<>[] & or(size(name)<>size(version)) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsTest",1,2));
	end
	
	// Check if the module is installed
	// =========================================================================
	
	if rhs < 2 then
		if ~ and(atomsIsInstalled(name)) then
			for i=1:size(name,"*")
				if ~ atomsIsInstalled(name(i)) then
					error(msprintf(gettext("%s: No version of the module ''%s'' is installed.\n"),"atomsTest",name(i)));
				end
			end
		end
	else
		if ~ and(atomsIsInstalled([name version])) then
			for i=1:size(name,"*")
				if ~ atomsIsInstalled([name(i) version(i)]) then
					error(msprintf(gettext("%s: The version ''%s'' of The module ''%s'' is not installed.\n"),"atomsTest",version(i),name(i)));
				end
			end
		end
	end
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		this_module_name = name(i);
		
		if rhs>2 then
			this_module_version  = version(i);
		else
			this_module_versions = atomsGetInstalledVers(this_module_name);
			this_module_version  = this_module_versions(1);
		end
		
		test_run(atomsGetInstalledPath(this_module_name,this_module_version));
		
	end
	
endfunction
