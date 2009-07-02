// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the full packages of the TOOLBOXES present in the differents repositories

function packages = atomsGetTOOLBOXES(update)
	
	// Initialize
	packages = struct();
	
	// Operating system detection + archi
	// =========================================================================
	
	// Operating system
	
	if ~MSDOS then
		OSNAME = unix_g('uname');
		MACOSX = (strcmpi(OSNAME,"darwin") == 0);
		LINUX  = (strcmpi(OSNAME,"linux") == 0);
	else
		MACOSX = %F;
		LINUX  = %F;
	end
	
	if MSDOS then
		OSNAME = "windows";
	elseif LINUX then
		OSNAME = "linux";
	elseif MACOSX then
		OSNAME = "macosx";
	end
	
	// Architecture
	[dynamic_info,static_info] = getdebuginfo();
	arch_info  = static_info(grep(static_info,"/^Compiler Architecture:/","r"))
	
	if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
		ARCH = "64";
	else
		ARCH = "32";
	end
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	if (rhs == 1) & (type(update) <> 4) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	// packages file path definition
	// =========================================================================
	
	packages_path      = pathconvert(SCIHOME+"/.atoms/packages",%F);
	packages_path_info = fileinfo(packages_path);
	
	// If necessary, rebuild the struct
	// =========================================================================
	
	if (packages_path_info == []) ..
		| (getdate("s") - packages_path_info(7) > 3600) ..
		| (rhs == 1 & update) then
		
		// loop on available repositories
		// =====================================================================
		
		repositories = atomsRepositories();
		repositories = repositories(:,1);
		
		if ~isdir(pathconvert(TMPDIR+"/atoms")) then
			mkdir(pathconvert(TMPDIR+"/atoms"));
		end
		
		for i=1:size(repositories,"*")
			
			// Building url & file_out
			// ----------------------------------------
			url            = repositories(i)+"/TOOLBOXES/"+ARCH+"/"+OSNAME;
			file_out       = pathconvert(TMPDIR+"/atoms/TOOLBOXES",%f);
			
			// Remove the existing file
			// ----------------------------------------
			if( fileinfo(file_out) <> [] ) then
				mdelete(file_out);
			end
			
			// Launch the download
			// ----------------------------------------
			atomsDownload(url,file_out);
			
			// Read the download description file
			// ----------------------------------------
			this_description = atomsReadDESCRIPTION(file_out);
			
			// Add information about the repository
			// ----------------------------------------
			this_description = atomsAddRepositoryInfo(this_description,repositories(i));
			
			// concatenate the description with the 
			// global struct
			// ----------------------------------------
			packages = atomsCatDESCRIPTION( packages , this_description );
			
			// file_out is no more needed
			// ----------------------------------------
			mdelete(file_out);
			
		end
		
		// Get DESCRIPTIONs from binary files
		// =====================================================================
		
		DESCRIPTION_files = listfiles( ..
			[ pathconvert(SCI+"/.atoms") + "DESCRIPTION_*.bin" ; .. 
			  pathconvert(SCIHOME+"/atoms") + "DESCRIPTION_*.bin" ] );
			
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
		// Save the "packages" variable in a file
		// ---------------------------------------------------------------------
		if ~isdir(pathconvert(SCIHOME+"/.atoms")) then
			mkdir(pathconvert(SCIHOME+"/.atoms"));
		end
		
		save(packages_path,packages)
		
	// Just load from file
	// =========================================================================
	
	else
		load(packages_path,"packages");
	end
	
endfunction

// =============================================================================
// Name       : atomsAddRepositoryInfo
// Author     : Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright  : (C) 2009 - DIGITEO
//
// => Add the field "repository" in each version of each package
//
// =============================================================================

function tree_out = atomsAddRepositoryInfo( tree_in , repository )
	
	tree_out = tree_in;
	
	package_names      = getfield(1,tree_in);
	package_names(1:2) = [];
	
	for i=1:size(package_names,"*")
		
		package_versions_struct = tree_out(package_names(i));
		package_versions        = getfield(1,package_versions_struct);
		package_versions(1:2)   = [];
		
		for j=1:size(package_versions,"*")
			this_version_struct                          = package_versions_struct(package_versions(j));
			this_version_struct("repository")            = repository;
			package_versions_struct(package_versions(j)) = this_version_struct;
		end
		
		tree_out(package_names(i)) = package_versions_struct;
	end
	
endfunction
