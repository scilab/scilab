// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Output arguments :

//   insList  : . List  of the changes caused by the installation of one or
//                more packages
//              . matrix ( n x 4 ) 
//              . mandatory
//              . example :
//                 !~  U  toolbox_4  1.0  !
//                 !                      !
//                 !~     toolbox_2  1.3  !
//                 !                      !
//                 !~     toolbox_1  1.9  !
//                 !                      !
//                 !~  U  toolbox_5  1.0  !

//   tree_out : . Dependency tree of the package (returned by atomsDepTreeFlat)
//              . struct
//              . mandatory
//              . Example :
//                   tree_out  = 
//                   toolbox_5 - 1.0: [1x1 struct]
//                   toolbox_4 - 1.0: [1x1 struct]
//                   toolbox_2 - 1.3: [1x1 struct]
//                   toolbox_1 - 1.9: [1x1 struct]

function [insList,depTree] = atomsInstallList(packages)
	
	insList = [];
	depTree = struct();
	
	// Save the initial path
	// =========================================================================
	initialpath = pwd();
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion("scilab")) + ".");
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsInstallList",1))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallList",1));
	end
	
	packages = stripblanks(packages);
	
	// Loop on packages and to build the dependency tree
	// =========================================================================
	
	for i=1:size(packages,"*")
		
		package = packages(i);
		
		if size(regexp(package,"/\s/") ,"*" ) > 1 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong value for input argument #%d: it must contain at most one space.\n"),"atomsInstallList",1));
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
			chdir(initialpath);
			error(msprintf(gettext("%s: The package %s is not available.\n"),"atomsInstallList",package_full_name));
		end
		
		// Build the depencency tree
		[tree,version_out]  = atomsDepTreeFlat(package_names(i),package_versions(i));
		
		if (type(tree) == 4) & (~ tree) then
			chdir(initialpath);
			error(msprintf(gettext("%s: The dependency tree cannot be resolved.\n"),"atomsInstallList",1));
		end
		
		// Update the  package_versions(i) with the version returned by
		// atomsDepTreeFlat
		package_versions(i) = version_out;
		
		// Concatenate the tree with the existing one
		depTree = atomsCatTree( depTree , tree );
	end
	
	// Add a field to detect later if it's the toolbox is automaticaly installed
	// or if it's a user choice
	// =========================================================================
	
	for i=1:size(package_names,"*")
		this_package_details                                = depTree(package_names(i)+" - "+package_versions(i));
		this_package_details("user_choice")                 = %T;
		depTree(package_names(i)+" - "+package_versions(i)) = this_package_details;
	end
	
	// Now we have the list of package that have to be installed
	// We have to check if
	//  - each package is already installed
	//  - If yes, Is it the most recent version
	// =========================================================================
	
	mandatory_packages      = getfield(1,depTree);
	mandatory_packages(1:2) = [];
	
	for i=1:size(mandatory_packages,"*")
		this_package_details = depTree(mandatory_packages(i));
		
		this_package_name    = this_package_details("Toolbox");
		this_package_version = this_package_details("Version");
		
		if isfield(this_package_details,"user_choice") & this_package_details("user_choice") then
			this_package_user_choice = "U"; // stand for User Choice
		else
			this_package_user_choice = ""; // stand for User Choice
		end
		
		to_install = %F;
		
		if atomsIsInstalled(this_package_name) then
			vers = atomsGetInstalledVers(this_package_name);
			if find( vers == this_package_version ) == [] then
				to_install = %T;
			end
		else
			to_install = %T;
		end
		
		if to_install then
			insList = [ insList ; "+" this_package_user_choice this_package_name this_package_version ];
		else
			insList = [ insList ; "~" this_package_user_choice this_package_name this_package_version ];
		end
		
	end
	
	// Go to the initial location
	// =========================================================================
	chdir(initialpath);
	
endfunction
