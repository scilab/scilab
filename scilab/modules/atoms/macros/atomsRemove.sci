// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Remove a toolbox

function result = atomsRemove(packages,allusers)
	
	result = %F;
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion('scilab')) + ".");
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsRemove",1,2))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRemove",1));
	end
	
	packages = stripblanks(packages);
	
	// Operating system detection
	// =========================================================================
	
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
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 1 then
		// By default, uninstall the package (if we have write access
		// of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsRemove",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRemove",2,pathconvert(SCI+"/.atoms")));
		end
	end 
	
	// Loop on packages and to build the list of package to uninstall
	// =========================================================================
	
	packagesToUninstall_W = []; // Wanted packages
	packagesToUninstall_D = []; // Dependency packages
	packagesToUninstall_B = []; // Broken packages
	
	for i=1:size(packages,"*")
		
		package = packages(i);
		
		if size(regexp(package,"/\s/") ,"*" ) > 1 then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: it must contain at most one space.\n"),"atomsInstall",1));
		end
		
		if size(regexp(package,"/\s/") ,"*" ) == 0 then
			// Just the toolbox name is specified
			package_names(i)    = package;
			package_versions(i) = "";
		else
			// A version is specified
			space               = regexp(package,"/\s/");
			package_names(i)    = part(package,[1:space-1]);
			package_versions(i) = part(package,[space+1:length(package)]);
		end
		
		// Ok, The syntax is correct, Now check if the package is really installed
		if ~ atomsIsInstalled(package_names(i),package_versions(i)) then
			continue;
		end
		
		// get the list of the versions of this package to uninstall
		
		if isempty(package_versions(i)) then
			// uninstall all version of this toolbox
			this_package_versions = atomsGetInstalledVers(package_names(i));
		else
			// Just uninstall the specified version
			this_package_versions = package_versions(i);
		end
		
		// Loop on this version list
		
		for j=1:size(this_package_versions,"*")
			
			packagesToUninstall_W = [ packagesToUninstall_W ; atomsGetInstalledDetails( package_names(i),this_package_versions(j)) ];
			
			// Establish the dependency list to uninstall
			// =================================================================
			
			// Get the dependencies of this package
			this_package_child_deps = atomsGetDepChilds(package_names(i),this_package_versions(j));
			
			// Loop on dependency childs
			for k=1:size(this_package_child_deps(:,1),"*")
				
				// If this dependence is automatically installed, add it to the
				// list of package to uninstall
				
				if atomsGetInstalledStatus( this_package_child_deps(k,1) , this_package_child_deps(k,2) ) == "A" then
					packagesToUninstall_D = [ packagesToUninstall_D ; atomsGetInstalledDetails(this_package_child_deps(k,1),this_package_child_deps(k,2)) ];
				end
				
			end
			
			// Establish the broken list to uninstall
			// =================================================================
			
			// Get the dependencies of this package
			this_package_parent_deps = atomsGetDepParents(package_names(i),this_package_versions(j));
			
			// Loop on dependency parents
			for k=1:size(this_package_parent_deps(:,1),"*")
				
				// If this dependence is automatically installed, add it to the
				// list of package to uninstall
				packagesToUninstall_B = [ packagesToUninstall_B ; atomsGetInstalledDetails(this_package_deps(k,1) , this_package_deps(k,2)) ];
				
			end
			
		end
		
	end
	
	// Loop on packages to filter the list of package to uninstall
	// =========================================================================
	
	// All wanted packages must be uninstalled
	packagesToUninstall = packagesToUninstall_W;
	
	// For the moment, All dependency packages must be uninstalled
	packagesToUninstall = [ packagesToUninstall ; packagesToUninstall_D ];
	
	// Now we have the list of package that have to be uninstalled
	// =========================================================================
	
	for i=1:size(packagesToUninstall(:,1),"*")
		
		this_package_name      = packagesToUninstall(i,1);
		this_package_version   = packagesToUninstall(i,2);
		this_package_directory = packagesToUninstall(i,4);
		
		// Check if this_package_directory start with SCI or SCIHOME
		
		if (grep(this_package_directory,pathconvert(SCI)) == []) & ..
			(grep(this_package_directory,pathconvert(SCIHOME)) == []) then
			
			error(msprintf(gettext("%s: The directory of this package (%s-%s) is located neither in SCI nor in SCIHOME. For security reason, ATOMS refuse to delete this directory.\n"),"atomsRemove",packagesToUninstall(1,1),packagesToUninstall(1,2)));
		end
		
		uninstall_status = rmdir(this_package_directory,"s");
		
		if uninstall_status<>1 then
			error(msprintf( ..
				gettext("%s: The directory of this package (%s-%s) cannot been deleted, please check if you have write access on this directory : %s.\n"),..
				"atomsRemove", ..
				this_package_name, ..
				this_package_version, ..
				this_package_directory));
		end
		
		// Check if the parent directory (directory name == toolbox name ) is empty 
		// If yes, detete it
		// =====================================================================
		this_package_root_dir = part(this_package_directory,1:length(pathconvert(this_package_directory)) - length(this_package_version) - 1 );
		
		if isdir(this_package_root_dir) & listfiles(this_package_root_dir)==[] then
			stat = rmdir(this_package_root_dir);
			if stat<>1 then
				error(msprintf( ..
					gettext("%s: The root directory of this package (%s-%s) cannot been deleted, please check if you have write access on this directory : %s.\n"),..
					"atomsRemove", ..
					this_package_name, ..
					this_package_version, ..
					this_package_root_dir));
			end
		end
		
		// Remove this toolbox from the list of installed packages
		// =====================================================================
		atomsInstallUnregister(this_package_name,this_package_version,allusers);
		
		// Remove this toolbox from the list of autoloaded packages
		// =====================================================================
		atomsDelAutoload(this_package_name,this_package_version);
		
	end
	
	result = %T;
	
endfunction
