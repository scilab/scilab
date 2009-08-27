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
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check input parameters
	// =========================================================================
	rhs = argn(2);
	if rhs > 0 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsAutoload",0))
	end
	
	// Get the list of packages to load
	// =========================================================================
	packages = atomsGetAutoload();
	
	// Libraries to resume
	// =========================================================================
	libs_resume = [];
	
	// Get the list of lib [before]
	// =====================================================================
	libs_before = librarieslist();
	
	// Load the wanted packages
	// =========================================================================
	result = atomsLoad(packages(:,1),packages(:,2));
	
	// Get the list of lib [after]
	// =====================================================================
	libs_after = librarieslist();
	
	// Loop on libs_after
	// =====================================================================
	for i=1:size(libs_after,"*")
		
		if find(libs_after(i) == libs_before) == [] then
			libs_resume = [ libs_resume ; libs_after(i) ];
		end
	end
	
	// If libs_resume is empty, the job is done
	// =========================================================================
	if isempty(libs_resume) then
		return;
	end
	
	// Build the resume cmd
	// =========================================================================
	
	resume_cmd = "[";
	
	for i=1:size(libs_resume,"*")
		resume_cmd = resume_cmd + libs_resume(i);
		if i<size(libs_resume,"*") then
			resume_cmd = resume_cmd + ",";
		else
			resume_cmd = resume_cmd + "] = resume(";
		end
	end
	
	for i=1:size(libs_resume,"*")
		resume_cmd = resume_cmd + libs_resume(i);
		if i<size(libs_resume,"*") then
			resume_cmd = resume_cmd + ",";
		else
			resume_cmd = resume_cmd + ");";
		end
	end
	
	// Exec the resume cmd
	// =========================================================================
	execstr(resume_cmd,"errcatch");
	
endfunction
