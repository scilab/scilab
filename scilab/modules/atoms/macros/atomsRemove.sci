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
	
	result = [];
	
	// Save the current path
	// =========================================================================
	initialpath = pwd();
	
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
	
	// Verbose Mode ?
	// =========================================================================
	if strcmpi(atomsGetConfig("Verbose"),"True") == 0 then
		VERBOSE = %T;
	else
		VERBOSE = %F;
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
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsRemove",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			chdir(initialpath);
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRemove",2,pathconvert(SCI+"/.atoms")));
		end
	end 
	
	// Build the list of package to Uninstall
	// =========================================================================
	remove_package_list = atomsRemoveList(packages,allusers);
	
	
	// Loop on remList to print if a package has to be remove
	// or not
	// =========================================================================
	if VERBOSE 
		for i=1:size(remove_package_list(:,1),"*")
			if remove_package_list(i,1) == "-" then
				mprintf("\t%s (%s) will be removed\n",remove_package_list(i,3),remove_package_list(i,4));
			elseif (remove_package_list(i,1) == "~") & (remove_package_list(i,1) == "B") then
				mprintf("\t%s (%s) cannot be removed and will be broken\n",remove_package_list(i,3),remove_package_list(i,4));
			end
		end
	end
	
	// Now we have the list of package that have to be uninstalled
	// =========================================================================
	
	for i=1:size(remove_package_list(:,1),"*")
		
		// If the package must be keeped, the job is done
		if remove_package_list(i,1) <> "-" then
			continue;
		end
		
		this_package_name      = remove_package_list(i,3);
		this_package_version   = remove_package_list(i,4);
		this_package_insdet    = atomsGetInstalledDetails(this_package_name,this_package_version);
		this_package_directory = this_package_insdet(4);
		
		if VERBOSE then
			mprintf( "\tRemoving %s (%s) ... " , this_package_name , this_package_version );
		end
		
		// Check if this_package_directory start with SCI or SCIHOME
		
		if (grep(this_package_directory,pathconvert(SCI)) == []) & ..
			(grep(this_package_directory,pathconvert(SCIHOME)) == []) then
			
			chdir(initialpath);
			error(msprintf(gettext("%s: The directory of this package (%s-%s) is located neither in SCI nor in SCIHOME. For security reason, ATOMS refuses to delete this directory.\n"),"atomsRemove",this_package_name,this_package_version));
		end
		
		uninstall_status = rmdir(this_package_directory,"s");
		
		if uninstall_status<>1 then
			chdir(initialpath);
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
				chdir(initialpath);
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
		
		// Fill the result matrix
		// =====================================================================
		result = [ result ; this_package_insdet ];
		
		// Sucess message if needed
		// =====================================================================
		if VERBOSE then
			mprintf(" success\n");
		end
	end
	
	// Go to the initial location
	chdir(initialpath);
	
endfunction
