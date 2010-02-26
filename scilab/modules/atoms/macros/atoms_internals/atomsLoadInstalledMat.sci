// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function matrix_out = atomsLoadInstalledMat(section)
	
	rhs = argn(2);
	
	// Init the output argument
	// =========================================================================
	matrix_out = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	// Check input argument
	// =========================================================================
	
	if type(section) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	if size(section,"*")<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single-string expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	if and(section<>["user","allusers","all"]) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	// Define the path of the file that will record the change according to
	// the "section" value
	// =========================================================================
	installed_file = atomsPath("system",section) + "installed.bin";
	
	// Loop on installed files
	// =========================================================================
	
	for i=1:size(installed_file,"*")
		
		// Does the file exist,
		// If yes, load the tree
		// If not, the job is done for this file
		// ---------------------------------------------------------------------
		
		if fileinfo(installed_file(i)) <> [] then
			load(installed_file(i),"installed_mat");
		else
			continue;
		end
		
		matrix_out = [ matrix_out ; installed_mat ];
	end
	
endfunction
