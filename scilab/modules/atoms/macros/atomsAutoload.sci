// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Load the toolboxes that are marked "autoload"

function result = atomsAutoload()
	
	result = %F;
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion('scilab')) + ".");
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs > 0 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsAutoload",0))
	end
	
	// Get the list of packages to load
	// =========================================================================
	
	packages = atomsGetAutoload();
	
	// Get the list of lib
	// =========================================================================
	libs_before = librarieslist();
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		loader_file = pathconvert(packages(i,4)) + "loader.sce";
		
		if fileinfo(loader_file)==[] then
			error(msprintf(gettext("%s: The file ''%s'' doesn''t exist or is not read accessible.\n"),"atomsAutoload",loader_file));
		end
		
		exec( loader_file );
	end
	
	// Get the list of lib
	// =========================================================================
	libs_after = librarieslist();
	
	// Loop on libs_after
	// =========================================================================
	for i=1:size(libs_after,"*")
		
		if find(libs_after(i) == libs_before) == [] then
			resume_cmd = msprintf("%s = resume(%s);",libs_after(i),libs_after(i));
			execstr(resume_cmd,"errcatch");
		end
	end
	
endfunction
