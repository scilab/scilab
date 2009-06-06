// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add toolboxes to the list of installed packages
// This function has an impact on the following files :
//  -> ATOMSDIR/installed
//  -> ATOMSDIR/installed_deps

function nbAdd = atomsInstallRegister(name,version,status,allusers)
	
	rhs            = argn(2);
	nbAdd          = 0;
	installed      = []; // Column vector that contain installed packages
	installed_deps = []; // Column vector that contain installed dependencies
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 3 | rhs > 4 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsInstallRegister",3,4));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",2));
	end
	
	if type(status) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",3));
	end
	
	// status is a letter
	// =========================================================================
	
	if ~ and((status == "A") | (status == "I"))  then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Letters ''A'' or ''I'' expected.\n"),"atomsInstallRegister",3));
	end
	
	// name,version and status must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallRegister",1,2));
	end
	
	if or( size(name) <> size(status) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallRegister",1,3));
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 3 then
		// By default, The toolbox is installed for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstallRegister",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstallRegister",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	
	if allusers then
		atoms_directory = pathconvert(SCI+"/.atoms");
	else
		atoms_directory = pathconvert(SCIHOME+"/atoms");
	end
	
	// Does the atoms_directory exist, if not create it
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		mkdir(atoms_directory);
	end
	
	// Does the SCIHOME/atoms/installed exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"installed") <> [] then
		installed = mgetl(atoms_directory+"installed");
	end
	
	// Does the SCIHOME/atoms/installed_deps exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"installed_deps") <> [] then
		installed_deps = mgetl(atoms_directory+"installed_deps");
	end
	
	// Loop on each URL specified as input argument
	// =========================================================================
	
	for i=1:size(name,"*")
		
		// Add the URL only if it doesn't already exist
		if grep( installed ,"/^[AI]\s-\s"+name(i)+"\s-\s"+version(i)+"$/","r") == [] then
			
			nbAdd = nbAdd + 1;
			
			// installed file
			// =================================================================
			installed = [ installed ; status(i)+" - "+name(i)+" - "+version(i) ];
			
			// installed_deps file
			// =================================================================
			
			// Parent package
			// --------------
			this_package_str = ["["+name(i)+" - "+version(i)+"]"];
			
			// Get the depencency tree & and child package
			// -------------------------------------------
			tree              = atomsDependencyTree(name(i),version(i));
			dep_packages      = getfield(1,tree);
			dep_packages(1:2) = [];
			dep_packages(find(dep_packages == name(i))) = [];
			
			// loop on childs
			// -------------------------------------------
			for j=1:size(dep_packages,"*")
				this_dep_package_details = tree(dep_packages(j));
				this_package_str = [this_package_str;"    "+this_dep_package_details("Toolbox")+" - "+this_dep_package_details("Version")];
			end
			
			// Finalize the current packages string
			// ------------------------------------
			this_package_str  = [this_package_str;""];
			
			// Cat the current packages deps with all others
			// ---------------------------------------------
			installed_deps    = [installed_deps;this_package_str];
			
		end
		
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		
		mputl(installed     , atoms_directory+"installed");
		mputl(installed_deps, atoms_directory+"installed_deps");
		
	end
	
endfunction
