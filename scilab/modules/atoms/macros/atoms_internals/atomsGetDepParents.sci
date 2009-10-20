// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Give the list of packages that use this package (identified by its name and version)

function packages = atomsGetDepParents(name,version,section)
	
	rhs      = argn(2);
	packages = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetDepParents",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepParents",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepParents",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsGetDepParents",1));
	end
	
	if size(version,"*")<>1  then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsGetDepParents",2));
	end
	
	// All user management
	// =========================================================================
	
	if rhs <= 1 then
		section = "all"; 
		
	else
		// Process the 2nd input argument : section
		// Allusers can be a boolean or equal to "user" or "allusers"
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsGetDepParents",3));
		end
		
		if and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetDepParents",3));
		end
	end
	
	// Load the installed_deps struct
	// =========================================================================
	[ child_deps,parent_deps ] = atomsLoadInstalleddeps(section);
	
	// If name - version is not a field of the struct, the job is done
	// =========================================================================
	
	if ~ isfield(parent_deps,name+" - "+version) then
		return;
	end
	
	// Return the matrix associated with the wanted package (name - version)
	// =========================================================================
	
	packages_list = parent_deps(name+" - "+version);
	
	for i=1:size(packages_list,"*")
		this_package_name    = part(packages_list(i),1:regexp(packages_list(i),"/\s-\s/","o")-1);
		this_package_version = part(packages_list(i),length(this_package_name)+4:length(packages_list(i)));
		packages = [ packages ; this_package_name this_package_version ];
	end
	
endfunction
