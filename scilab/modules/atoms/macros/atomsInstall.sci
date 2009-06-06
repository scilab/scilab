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
	
	// Loop on packages and to build the dependency tree
	// =========================================================================
	
	dependency_tree = struct();
	
	for i=1:size(packages,"*")
		
		package = packages(i);
		
		if size(regexp(package,"/\s/") ,"*" ) > 1 then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: it must contain at most one space.\n"),"atomsInstall",1));
		end
		
		if size(regexp(package,"/\s/") ,"*" ) == 0 then
			// install the most recent version of the package
			package_names(i)    = package;
			package_versions(i) = "";
		else
			// A version is specified
			space               = regexp(package,"/\s/");
			package_names(i)    = part(package,[1:space-1]);
			package_versions(i) = part(package,[space+1:length(package)]);
		end
		
		// Ok, The syntax is correct, Now check if it's a valid package
		if ~ atomsIsPackage(package_names(i),package_versions(i)) then
			if isempty(package_versions(i)) then
				package_full_name = package_names(i);
			else
				package_full_name = package_names(i)+" - "+package_versions(i);
			end
			error(msprintf(gettext("%s: The package %s is not available.\n"),"atomsInstall",1,package_full_name));
		end
		
		// Build the depencency tree
		tree = atomsDependencyTree(package_names(i),package_versions(i));
		
		if (type(dependency_tree) == 4) & (~ dependency_tree) then
			error(msprintf(gettext("%s: The dependency tree cannot be resolved.\n"),"atomsInstall",1));
		end
		
		// Concatenate the tree with the existing one
		dependency_tree = atomsCatTree( dependency_tree , tree );
		
	end
	
	// Now we have the list of package that have to be installed
	// We have to check if
	//  - each package is already installed
	//  - If yes, Is it the most recent version
	// =========================================================================
	
	mandatory_packages      = getfield(1,dependency_tree);
	mandatory_packages(1:2) = [];
	
	for i=1:size(mandatory_packages,"*")
		
		this_package_details = dependency_tree(mandatory_packages(i));
		
		this_package_name    = this_package_details("Toolbox");
		this_package_version = this_package_details("Version");
		
		this_package_details("to_install") = %F; 
		
		if atomsIsInstalled(this_package_name) then
			vers = atomsGetInstalledVers(mandatory_packages(i));
			if atomsVersionCompare( vers(1) , this_package_version ) < 0 then
				this_package_details("to_install") = %T;
			end
		else
			this_package_details("to_install") = %T;
		end
		
		dependency_tree(mandatory_packages(i)) = this_package_details;
		
	end
	
	// Now really install the packages
	// =========================================================================
	
	for i=1:size(mandatory_packages,"*")
		
		if ~ this_package_details("to_install") then
			continue;
		end
		
		this_package_details = dependency_tree(mandatory_packages(i));
		this_package_name    = this_package_details("Toolbox");
		this_package_version = this_package_details("Version");
		
		// Define the path of the directory where will be installed this toolbox
		// =====================================================================
		
		if allusers then
			atoms_directory = pathconvert(SCI+"/contrib/"+this_package_name);
		else
			atoms_directory = pathconvert(SCIHOME+"/atoms/"+this_package_name);
		end
		
		// Create the parent directory of this toolbox if it's not already exist
		// =====================================================================
		
		if ~ isdir( atoms_directory ) then
			status = mkdir( atoms_directory );
			if status <> 1 then
				error(msprintf( ..
					gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"),..
					atoms_directory));
			end
		end
		
		// Define the path of the downloaded file
		// =====================================================================
		
		fileout = pathconvert(atoms_directory+this_package_details(OSNAME+"Name"),%F);
		filein  = this_package_details(OSNAME+"Url");
		
		// Get the package
		// =====================================================================
		
		[rep,stat,err] = unix_g("wget "+filein + " -O " + fileout)
		
		if stat ~= 0 then
			disp("wget "+ filein + " -O " + fileout);
			disp(err);
		end
		
		// Check the md5sum
		// =====================================================================
		
		filemd5 = getmd5(fileout);
		
		if filemd5 <> this_package_details(OSNAME+"Md5") then
			error(msprintf(gettext("%s: The downloaded binary file (%s) doesn''t check the MD5SUM.\n"),"atomsInstall",fileout));
		end
		
		// unarchive it
		// =====================================================================
		
		chdir( atoms_directory );
		
		// get the list of directories before unarchive
		dir_list_before = atomsListDir();
		
		if regexp(fileout,"/\.tar\.gz$/","o") <> [] then
			
			[rep,stat,err] = unix_g("tar xzf "+ fileout + " -C "+ atoms_directory );
			
			if stat ~= 0 then
				disp("tar xzf "+ fileout + " -C "+ atoms_directory);
				disp(err);
			end
			
		elseif regexp(fileout,"/\.zip$/","o") <> [] then
			
			[rep,stat,err] = unix_g("unzip "+ fileout);
			
			if stat ~= 0 then
				disp("unzip "+ fileout);
				disp(err);
			end
			
		end
		
		// get the list of directories after unarchive
		dir_list_after = atomsListDir();
		
		// Get the name of the created directory
		// =====================================================================
		
		unarchive_dir = "";
		
		for i=1:size(dir_list_after,"*")
			if find(dir_list_after(i) == dir_list_before) == [] then
				unarchive_dir = dir_list_after(i);
				break;
			end
		end
		
		// Rename the created directory
		// =====================================================================
		
		if MSDOS then
			rename_cmd = "rename";
		else
			rename_cmd = "mv";
		end
		
		rename_cmd = rename_cmd+" """+unarchive_dir+""" """+this_package_version+"""";
		
		[rep,stat]=unix_g(rename_cmd)
		
		if stat <> 0 then
			disp(rename_cmd);
			error(msprintf(gettext("%s: Error while creating the directory ''%s''.\n"),"atomsInstall",pathconvert(atoms_directory+this_package_version)));
		end
		
		// Register the successfully installed package
		// =====================================================================
		
		if find(package_names == this_package_name) == [] then
			// Automaticaly installed
			this_package_status = "A";
		else
			// Volontary Installed
			this_package_status = "I";
		end
		
		atomsInstallRegister(this_package_name,this_package_version,this_package_status,allusers);
		
	end
	
	result = dependency_tree;
	
endfunction


// =============================================================================
// Just get the list of the directories present in the current directory
// =============================================================================

function result = atomsListDir()
	
	result = [];
	
	items  = listfiles();
	
	// Loop on items
	for i=1:size(items,"*")
		if isdir(items(i)) then
			result = [ result ; items(i) ];
		end
	end
	
endfunction
