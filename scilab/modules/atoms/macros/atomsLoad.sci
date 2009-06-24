// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Load one or several toolboxes

function result = atomsLoad(name,version)
	
	// Init the output argument
	// =========================================================================
	result = [];
	
	// Check number of input arguments
	// =========================================================================
	rhs = argn(2);
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsLoad",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsLoad",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsLoad",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsLoad",1,2));
	end
	
	// Check if the packages to load are installed
	// =========================================================================
	
	if or( ~ atomsIsInstalled(name,version) ) then
		for i=1:size(name,"*")
			if ~atomsIsInstalled(name(i),version(i)) then
				error(msprintf(gettext("%s: the package ''%s - %s'' is not installed.\n"),"atomsLoad",name(i),version(i)));
			end
		end
	end
	
	// Get path of the toolboxes
	// =========================================================================
	path = atomsGetInstalledPath(name,version);
	
	// Create the TMPDIR/atoms directory
	// =========================================================================
	if ~ isdir(TMPDIR+"/atoms") then
		status = mkdir( TMPDIR+"/atoms" );
		if status <> 1 then
			error(msprintf( ..
				gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"), ..
				"atomsLoad", ..
				TMPDIR+"/atoms"));
		end
	end
	
	// Define the path of the loaded file
	// =========================================================================
	loaded_file = pathconvert(TMPDIR+"/atoms/loaded",%F);
	
	// Does the loaded file exist, if yes load it
	// =========================================================================
	nbAdd = 0;
	if fileinfo(loaded_file) <> [] then
		loaded = mgetl(loaded_file);
	else
		loaded = [];
	end
	
	// Libraries to resume
	// =========================================================================
	libs_resume = [];
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(name,"*")
		
		// Get the list of lib
		// =====================================================================
		libs_before = librarieslist();
		
		// Exec the loader
		// =====================================================================
		
		loader_file = pathconvert(path(i)) + "loader.sce";
		
		if fileinfo(loader_file)==[] then
			error(msprintf(gettext("%s: The file ''%s'' doesn''t exist or is not read accessible.\n"),"atomsLoad",loader_file));
		end
		
		exec( loader_file );
		
		// Get the list of libraries (macros)
		// =====================================================================
		libs_after = librarieslist();
		
		// Loop on libs_after
		// =====================================================================
		for j=1:size(libs_after,"*")
			
			if find(libs_after(j) == libs_before) == [] then
				libs_resume = [ libs_resume ; libs_after(j) ];
			end
		end
		
		// Fill the output argument
		// =====================================================================
		result = [ result ; name(i) version(i) path(i) ];
		
		// fill the loaded matrix
		// =====================================================================
		if find(loaded == name(i)+" - "+version(i)) == [] then
			nbAdd  = nbAdd + 1;
			loaded = [ loaded ; name(i)+" - "+version(i) ];
		end
		
	end
	
	// Apply changes
	// =========================================================================
	if nbAdd > 0 then
		mputl(loaded,loaded_file);
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
