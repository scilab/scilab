// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Load one or several toolboxes

function result = atomsLoad(packages,section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Init the output argument
	// =========================================================================
	result = [];
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsLoad",1,2))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsLoad",1));
	end
	
	if size(packages(1,:),"*") > 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsLoad",1));
	end
	
	packages = stripblanks(packages);
	
	// Allusers/user management
	// =========================================================================
	
	if rhs < 2 then
		section = "all";
	
	else
		
		// Process the 2nd input argument : allusers
		// Allusers can be a boolean or equal to "user" or "allusers"
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or a single string expected.\n"),"atomsLoad",2));
		end
		
		if and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsLoad",2));
		end
		
	end
	
	// If package is a mx1 matrix, add a 2nd column with empty versions
	// =========================================================================
	
	if size(packages(1,:),"*") == 1 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	end
	
	// If only one input argument, define the version (The Most Recent Version)
	// =========================================================================
	
	
	for i=1:size(packages(:,1),"*")
		
		if isempty(packages(i,2)) then
			
			this_module_versions = atomsGetInstalledVers(packages(i,1),section);
			
			if isempty(this_module_versions) then
				error(msprintf(gettext("%s: No version of the module ''%s'' is installed.\n"),"atomsLoad",packages(i,1)));
			else
				packages(i,2) = this_module_versions(1);
			end
		
		else
			if ~atomsIsInstalled([packages(i,1) packages(i,2)],section) then
				error(msprintf(gettext("%s: the module ''%s - %s'' is not installed.\n"),"atomsLoad",packages(i,1),packages(i,2)));
			end
		
		end
		
	end
	
	// Get path of the toolboxes
	// =========================================================================
	path = atomsGetInstalledPath(packages);
	
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
	
	// Verbose Mode ?
	// =========================================================================
	if strcmpi(atomsGetConfig("Verbose"),"True") == 0 then
		ATOMSVERBOSE = %T;
	else
		ATOMSVERBOSE = %F;
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
	
	// Loop on packages gived by the user
	// =========================================================================
	
	mandatory_packages      = struct();
	mandatory_packages_name = struct();
	mandatory_packages_mat  = [];
	
	for i=1:size(packages(:,1),"*")
		
		this_package_name    = packages(i,1);
		this_package_version = packages(i,2);
		this_package_path    = path(i);
	
		// Check if the user try to load 2 versions of the same toolbox at the
		// same time
		// =====================================================================
		if size( find( this_package_name == packages(:,1) ) > 1 ) then
			this_versions = packages( find( this_package_name == packages(:,1) ) , 2 );
			for j=2:size(this_versions,"*")
				if this_versions(j) <> this_versions(1) then
					mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",this_package_name);
					mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),this_package_name,this_versions(1));
					mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),this_package_name,this_versions(j));
					mprintf("\n");
					error("");
				end
			end
		end
		
		// Check if this toolbox is already loaded
		// =====================================================================
		if atomsIsLoaded(packages(i,:)) then
			atomsDisp(msprintf("\tThe package %s (%s) is already loaded\n\n",this_package_name,this_package_version));
			continue;
		end
		
		// Check if another version of this toolbox is already loaded
		// =====================================================================
		[is_loaded,loaded_version] =  atomsIsLoaded(this_package_name);
		if is_loaded then
			error(msprintf(gettext("%s: Another version of the package %s is already loaded : %s\n"),"atomsLoad",this_package_name,loaded_version));
			continue;
		end
		
		mandatory_packages(this_package_name+" - "+this_package_version) = "asked_by_user";
		mandatory_packages_name(this_package_name) = this_package_version;
		mandatory_packages_mat = [ mandatory_packages_mat ; this_package_name this_package_version this_package_path ];
		
	end
	
	// Fill the list of package to load
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		childs = atomsGetDepChilds(packages(i,:));
		
		for j=1:size( childs(:,1) , "*")
			
			// Check if it is already loaded
			// -------------------------------------------------------
			if atomsIsLoaded(childs(j,:)) then
				continue;
			end
			
			// Check if another version of this package is already loaded
			// -------------------------------------------------------
			[is_loaded,loaded_version] =  atomsIsLoaded(childs(j,1));
			if is_loaded then
				mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",childs(j,1));
				mprintf(gettext("\t - ''%s - %s'' is already loaded\n"),childs(j,1),loaded_version);
				mprintf(gettext("\t - ''%s - %s'' is needed by ''%s - %s''\n"),childs(j,1),childs(j,2),packages(i,1),packages(i,2));
				mprintf("\n");
				error("");
			end
			
			// Check if it is already in the list
			// -------------------------------------------------------
			if isfield( mandatory_packages , childs(j,1)+" - "+childs(j,2) ) then
				continue;
			end
			
			// Check if another version is already in the list
			// -------------------------------------------------------
			if isfield( mandatory_packages_name , childs(j,1) ) then
				
				// if it's not the name version => error
				if mandatory_packages_name(childs(j,1)) <> childs(j,2) then
					
					mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",childs(j,1));
					mprintf(gettext("\t - ''%s - %s'' is needed by ''%s - %s''\n"),childs(j,1),childs(j,2),packages(i,1),packages(i,2));
					
					// The other version of the package is asked by the user
					if mandatory_packages(childs(j,1)+" - "+mandatory_packages_name(childs(j,1))) == "asked_by_user" then
						mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),childs(j,1),mandatory_packages_name(childs(j,1)));
					
					// The other version of the package is a need by another package
					else
						mprintf(gettext("\t - ''%s - %s'' is needed by ''%s''\n"), .. 
							childs(j,1), .. // name
							mandatory_packages_name(childs(j,1)), .. // version
							mandatory_packages(childs(j,1)+" - "+mandatory_packages_name(childs(j,1))) .. // name - version
							);
					end
					
					mprintf("\n");
					error("");
				end
			end
			
			// The child has passed the test, add it to the mandatory
			// packages to load
			// -------------------------------------------------------
			
			mandatory_packages(childs(j,1)+" - "+childs(j,2)) = packages(i,1)+" - "+packages(i,2);
			mandatory_packages_name(childs(j,1)) = childs(j,2);
			mandatory_packages_mat = [ mandatory_packages_mat ; childs(j,1) childs(j,2) atomsGetInstalledPath(childs(j,:),section) ];
			
		end
	end
	
	// Libraries to resume
	// =========================================================================
	libs_resume = [];
	
	if ~ isempty(mandatory_packages_mat) then
		mprintf("\n");
	end
	
	for i=1:size(mandatory_packages_mat(:,1),"*")
		
		this_package_name    = mandatory_packages_mat(i,1);
		this_package_version = mandatory_packages_mat(i,2);
		this_package_path    = mandatory_packages_mat(i,3);
		
		// Get the list of lib
		// =====================================================================
		libs_before = librarieslist();
		
		// Exec the loader
		// =====================================================================
		
		loader_file = pathconvert(this_package_path) + "loader.sce";
		
		if fileinfo(loader_file)==[] then
			error(msprintf(gettext("%s: The file ''%s'' doesn''t exist or is not read accessible.\n"),"atomsLoad",loader_file));
		end
		
		exec( loader_file );
		mprintf("\n");
		
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
		result = [ result ; mandatory_packages_mat(i,:) ];
		
		// fill the loaded matrix
		// =====================================================================
		if find(loaded == this_package_name + " - " + this_package_version) == [] then
			nbAdd  = nbAdd + 1;
			loaded = [ loaded ; this_package_name + " - " + this_package_version ];
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
	execstr(resume_cmd);
	
endfunction
