// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Installation of a toolbox

function result = atomsInstall(packages,allusers)
	
	result = %F;
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion('scilab')) + ".");
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsInstall",1,2))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstall",1));
	end
	
	packages = stripblanks(packages);
	
	// Install for all users or just for me ?
	// =========================================================================
	
	if rhs == 1 then
		// By default, install for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstall",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// We are installing new package, so we have a net connection
	// => update the package list
	// =========================================================================
	
	atomsGetTOOLBOXES(%T);
	
	// Loop on packages and get the following informations :
	// - package name
	// - package version
	// - status
	//  + Update
	//  + New Installation
	//  + The most recent version is already installed
	// =========================================================================
	
	for i=1:size(packages,"*")
		
		package = packages(i);
		
		if size(regexp(package,"/\s/") ,"*" ) > 1 then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: it must contain at most one space.\n"),"atomsInstall",1));
		end
		
		if size(regexp(package,"/\s/") ,"*" ) == 0 then
			// install the most recent version of the package
			name    = package;
			version = "0";
		else
			// A version is specified
			space   = regexp(package,"/\s/");
			name    = part(package,[1:space-1]);
			version = part(package,[space+1:length(package)]);
		end
		
		package_names(i)    = name;
		package_versions(i) = version;
		
		// Ok, The syntax is correct, Now check if it's a valid package
		
		if ~ atomsIsPackage(package_names(i),package_versions(i)) then
			if package_versions(i) == "0" then
				package_full_name = package_names(i);
			else
				package_full_name = package_names(i)+" - "+package_versions(i);
			end
			error(msprintf(gettext("%s: The package %s is not available.\n"),"atomsInstall",1,package_full_name));
		end
		
		// Get the package version is not specified, get the most recent version
		if package_versions(i) == "0" then
			package_versions(i) = atomsGetMRVersion(package_names(i));
		end
		
		// Check if the package is already installed
		if atomsIsInstalled(package_names(i),package_versions(i),%T) then
			error(msprintf(gettext("%s: The package %s is already installed.\n"),"atomsInstall",package_full_name));
		end
		
		// Check if the wanted version is compatible with the current scilab version.
		this_package_details = atomsToolboxDetails(package_names(i),package_versions(i));
		if ~ atomsIsCompatible(this_package_details("ScilabVersion")) then
			error(msprintf(gettext("%s: The package %s is not compatible with the current scilab version.\n"),"atomsInstall",package_full_name));
		end
		
		
		
		
		
		
		
		
		
		
		
		
		// Register the successfully installed package
		atomsInstallRegister(package_names(i),package_versions(i));
	end
	
	
	

// 	global conflictingList
// 	global conflictLocal
// 	global nomconflictLocal
// 	global conflictVersion
// 
// 	rhs = argn(2)
// 	if (rhs == 2 | rhs == 1 | rhs == 3) then
// 	// We check if the arguments exist
// 	if rhs == 1
// 		checkVersionScilab = %t
// 	end
// 	if rhs <= 2
// 		version = ""
// 	end
// 	// We remove the special characters
// 	name = atomsNormalizeName(name)
// 	// We go to the dedicated directory
// 	rep = atomsToolboxDirectory()
// 	cd (rep)
// 	// Required to empty the global variables before any use of atomsCheckConflict
// 	clearglobal conflictingList
// 	clearglobal conflictLocal
// 	clearglobal nomconflictLocal
// 	clearglobal conflictVersion
// 	// Retrieve of the list of toolboxes to install
// 	listTool = atomsCheckConflict(name, version, checkVersionScilab)
// 	// If there is an empty line, it is because a dependency is missing
// 	if find(listTool == "") <> []
// 		if conflictLocal == 1
// 		atomsDisplayMessage(sprintf(_("The Toolbox version %s locally present doesn''t match. Update it.\n"),nomconflictLocal))
// 		elseif listTool <> ""
// 		atomsDisplayMessage(_("Impossible to install all the dependencies.\n"))
// 		end
// 		result =  %f
// 		return result
// 	elseif conflictVersion == 1
// 		atomsDisplayMessage(sprintf(_("Conflit of version for %s"),nomconflictLocal))
// 		result =  %f
// 		return result
// 	end
// 	// Retrieve of the toolboxes list which are already in local
// 	cd (rep)
// 	listLocal = ls()
// 	// Treatment of the toolboxes to install
// 	[m, n] = size(listTool)
// 	for i=1:m
// 		[name, version] = atomsSeparateVersionDep(listTool(i))
// 		// If it is locally present, we pass to the following one
// 		if find(listLocal == name) <> []
// 		continue
// 		else
// 		atomsDisplayMessage(_("Is going to be install: ") + name)
// 		if ~atomsDlInstall(name, version)
// 			disp(_("Error during the installation"))
// 			result = %f
// 			return result
// 		end
// 		end
// 	end
// 	result = %t
// 	return result



endfunction
