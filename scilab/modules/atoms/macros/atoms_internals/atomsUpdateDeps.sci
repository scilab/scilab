// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Update the dependencies with the installation of this new package

function packages_out = atomsUpdateDeps(name,version,allusers)
	
	rhs          = argn(2);
	packages_out = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsUpdateDeps",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsUpdateDeps",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsUpdateDeps",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsUpdateDeps",1));
	end
	
	if size(version,"*")<>1  then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsUpdateDeps",2));
	end
	
	// All user management
	// =========================================================================
	
	if rhs == 2 then
		allusers = %T;
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsUpdateDeps",3));
		end
	end
	
	// The package designed by "name - version" must be installed
	// =========================================================================
	
	if ~ atomsIsInstalled(name,version) then
		error(msprintf(gettext("%s: %s (%s) isn''t installed.\n"),"atomsUpdateDeps",name,version));
	end
	
	// If alluser, process the 2 list (allusers and user)
	// =========================================================================
	
	allusers_mat = [%F];
	
	if allusers then
		// all packages
		allusers_mat = [ allusers_mat ; %T ];
	end
	
	for i=1:size(allusers_mat,"*")
		
		// Load the struct
		
		child_deps    = atomsLoadInstalleddeps(allusers_mat(i));
		packages      = getfield(1,child_deps);
		packages(1:2) = [];
		
		// Loop on packages
		
		for j=1:size(packages,"*")
			
			this_package_childs =  child_deps(packages(j));
			
			packages_concerned  = [ packages(j) ; this_package_childs ];
			concerned_names     = [];
			concerned_versions  = [];
			
			for k=1:size(packages_concerned,"*")
				concerned_names(k)    =  part(packages_concerned(k),1:regexp(packages_concerned(k),"/\s-\s/")-1);
				concerned_versions(k) =  part(packages_concerned(k),regexp(packages_concerned(k),"/\s-\s/")+3:length(packages_concerned(k)));
			end
			
			// Premier tri : on ne garde que les packages dont le package "<name>"
			// est un enfant direct 
			
			concerned_names_filt    = concerned_names(    atomsIsDirectChild(concerned_names,concerned_versions,name));
			concerned_versions_filt = concerned_versions( atomsIsDirectChild(concerned_names,concerned_versions,name));
			
			// if "name" is not the direct child of packages(j) or of one of the child
			// of packages(j) :
			// Job is done
			if isempty(concerned_names_filt) then
				continue;
			end
			
			// Second tri : On regarde si tous les packages qui ont le package "<name>"
			// comme enfant direct accepte la version "<version>" de ce package
			// If it's not the case : Job is done
			if ~ and( atomsPackageIsCompatible(concerned_names_filt,concerned_versions_filt,name,version) ) then
				continue;
			end
			
			// Now, we are sure "name - version" is compatible with  packages(j)
			// and all its child, check if this version is superior than the one
			// used by packages(j);
			
			// Get the current version used by packages(j)
			current_version = concerned_versions( find(concerned_names == name) );
			
			// Comparison
			if atomsVersionCompare(current_version,version) >= 0 then
				continue;
			end
			
			// Ok : the <version> of <name> can now be used by packages(j)
			this_package_childs( grep(this_package_childs,"/^"+name+"\s-\s/","r") ) = name+" - "+version;
			
			// ... and finally : update the tree
			child_deps(packages(j)) = this_package_childs;
			
			// Fill the output matrix
			packages_out = [ packages_out ; concerned_names(1) concerned_versions(1) ];
			
		end
		
		// Save the struct
		atomsSaveInstalleddeps(child_deps,allusers_mat(i));
	end
	
endfunction
