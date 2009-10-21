// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// update of a toolbox

function result = atomsUpdate(name,section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	result = [];
	
	// Save the initial path
	// =========================================================================
	initialpath = pwd();
	
	// Verbose Mode ?
	// =========================================================================
	if strcmpi(atomsGetConfig("Verbose"),"True") == 0 then
		ATOMSVERBOSE = %T;
	else
		ATOMSVERBOSE = %F;
	end
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsUpdate",0,2))
	end
	
	if (rhs>0) & (~ isempty(name) ) & (type(name)<>10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsUpdate",1));
	end
	
	// Install for all users or just for me ?
	// =========================================================================
	
	if rhs < 2 then
		// By default, install for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			section = "all";
		else
			section = "user";
		end
	
	else
		// Just check if it's a boolean
		if type(section) <> 10 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsInstall",2));
		end
		
		// Check if we have the write access
		if (section=="allusers") & ~ atomsAUWriteAccess() then
			chdir(initialpath);
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,atomsPath("system","allusers")));
		end
	end
	
	// Check if all specified toolboxes are effectively installed
	// =========================================================================
	
	if rhs>0 & ( ~ isempty(name)) then
		
		// Remove leading and trealing whitespaces
		name = stripblanks(name);
		
		for i=1:size(name,"*")
			if ~ atomsIsInstalled(name(i)) then
				chdir(initialpath);
				error(msprintf(gettext("%s: ''%s'' isn''t installed.\n"),"atomsUpdate",name(i)));
			end
		end
	end
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion('scilab')) + ".");
	
	// If name isn't defined or empty, get the full list of installed packages
	// =========================================================================
	
	if (rhs==0) | isempty(name) then
		
		name               = [];
		package_installed  = atomsGetInstalled(section);
		package_installed  = package_installed( find( package_installed(:,5) == "I") , : );
		
		for i=1:size(package_installed(:,1),"*")
			if find( name == package_installed(i,1) ) == [] then
				name = [ name ;  package_installed(i,1) ];
			end
		end
	end
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		this_package_versions    = atomsGetInstalledVers(name(i),section);
		this_package_MRV_ins     = this_package_versions(1);   // Most Recent Version Installed
		this_package_MRV_ava     = atomsGetMRVersion(name(i)); // Most Recent Version Available
		this_package_ins_details = atomsGetInstalledDetails([name(i) this_package_MRV_ins],section);
		this_package_ins_section = this_package_ins_details(3);
		
		if (this_package_MRV_ava == -1) | ..
				( atomsVersionCompare(this_package_MRV_ins,this_package_MRV_ava) == 0 ) then
			// The installed version is already the Most Recent Version Available
			atomsDisp(msprintf("\t%s (%s) : The most recent version is already installed\n\n",name(i),this_package_MRV_ins));
		else
			// Install the new toolbox
			this_result = atomsInstall([name(i) this_package_MRV_ava],this_package_ins_section);
			
			// Fill the output argument
			result = [ result ; this_result ];
		end
		
		// Now check if it's dependencies are up-to-date
		dependencies = atomsInstallList([name(i) this_package_MRV_ins],this_package_ins_section);
		
		for j=1:size(dependencies(:,1),"*")
			
			if ~atomsIsInstalled([dependencies(j,3) dependencies(j,4)],this_package_ins_section) then
				// Install the new toolbox
				this_result = atomsInstall([dependencies(j,3) dependencies(j,4)],this_package_ins_section);
				
				// Fill the output argument
				result = [ result ; this_result ];
			end
			
		end
		
	end
	
	// Go to the initial location
	// =========================================================================
	chdir(initialpath);
	
endfunction
