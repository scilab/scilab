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
//  -> ATOMSDIR/installed.txt, ATOMSDIR/installed.bin
//  -> ATOMSDIR/installed_deps.txt, ATOMSDIR/installed_deps.bin

function nbAdd = atomsInstallRegister(name,version,status,allusers)
	
	rhs            = argn(2);
	nbAdd          = 0;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 4 then
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
	
	if type(allusers) <> 4 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstallRegister",4));
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
	
	// load installed packages (a struct)
	// =========================================================================
	installed = atomsLoadInstalledStruct(allusers);
	
	// Load the installed_deps (a struct)
	// =========================================================================
	installed_deps = atomsLoadInstalleddeps(allusers);
	
	// Loop on each URL specified as input argument
	// =========================================================================
	for i=1:size(name,"*")
		
		if isfield(installed,name(i)+" - "+version(i)) then
			// This package is already registered
			continue;
		else
			// We have at least one change
			nbAdd = nbAdd + 1;
		end
		
		// installed struct
		// ---------------------------------------------------------------------
		
		// Build the matrix
		
		if allusers then
			this_package_path    = pathconvert(SCI+"/contrib/"+name(i)+"/"+version(i),%F);
			this_package_uservar = "allusers";
		else
			this_package_path    = pathconvert(SCIHOME+"/atoms/"+name(i)+"/"+version(i),%F);
			this_package_uservar = "user";
		end
		
		// Add this package to the struct
		installed(name(i)+" - "+version(i)) = [ .. 
			name(i)              ; .. // name
			version(i)           ; .. // version
			this_package_path    ; .. // path
			this_package_uservar ; .. // allusers / user
			status(i)            ];   // I / A
		
		// installed_deps file
		// Get the depencency tree & and child package
		// ---------------------------------------------------------------------
		
		childs_tree = atomsDepTreeFlat(name(i),version(i));
		childs_mat  = getfield(1,childs_tree);
		childs_mat(1:2) = [];
		childs_mat( find(childs_mat == name(i)+" - "+version(i)) ) = [];
		installed_deps(name(i)+" - "+version(i)) = childs_mat';
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		atomsSaveInstalled(installed,allusers);
		atomsSaveInstalleddeps(installed_deps,allusers);
	end
	
endfunction
