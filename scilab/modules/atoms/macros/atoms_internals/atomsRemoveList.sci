// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return a matrix that list the changes caused by the uninstallation of one or
// more packages

// -->A = atomsRemoveList('toolbox_2',%F)
//  A  =
//  
// !-  U  toolbox_2  1.5  !
// !                      !
// !-  U  toolbox_2  1.3  !
// !                      !
// !-  B  toolbox_3  1.6  !
// !                      !
// !-  B  toolbox_5  1.0  !
// !                      !
// !-  B  toolbox_4  1.0  !

// -  : The package will be removed
// ~  : The package will be keeped

// U  : The package is intentionnaly removed
// P  : The package is a parent of one package
// C  : The package is a child of one package
// B  : The package will be broken (It's a parent but cannot be uninstall)

function remList = atomsRemoveList(packages,allusers)
	
	remList = [];
	
	// Save the initial path
	// =========================================================================
	initialpath = pwd();
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsRemoveList",2))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRemoveList",1));
	end
	
	if type(allusers) <> 4 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsRemoveList",2));
	end
	
	// Loop on packages and to build the list of package to uninstall
	// =========================================================================
	
	packages = stripblanks(packages);
	
	// First step : List the packages wanted by the user
	
	for i=1:size(packages,"*")
		
		package = packages(i);
		
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
		
		// get the list of the versions of this package to uninstall
		
		if isempty(package_versions(i)) then
			// uninstall all version of this toolbox (if we have the right, of
			// course)
			this_package_versions = atomsGetInstalledVers(package_names(i),allusers);
		else
			// Just uninstall the specified version
			this_package_versions = package_versions(i);
		end
		
		for j=1:size(this_package_versions,"*")
			remList = [ remList ; "-" "U" package_names(i) this_package_versions(j) ];
		end
		
	end
	
	// Second Step : List the packages that depends of the uninstalled packages
	// =========================================================================
	
	packages = remList;
	for i=1:size(packages(:,1),"*")
		
		this_package_name    = packages(i,3);
		this_package_version = packages(i,4);
		
		// Get the parents of this toolbox
		// (inevitably removed, unless we have not the right)
		// ----------------------------------------------------
		
		this_package_parents = atomsGetDepParents(this_package_name,this_package_version);
		
		for j=1:size(this_package_parents(:,1),"*")
			
			this_parent_name    = this_package_parents(j,1);
			this_parent_version = this_package_parents(j,2);
			
			// Check if we have the right to remove this package
			// If not, tag it as Broken (for later)
			if ~allusers then
				details = atomsGetInstalledDetails(this_parent_name,this_parent_version);
				if details(3) == "allusers" then
					remList = [ remList ; "~" "B" this_parent_name this_parent_version ]; // B stands for "Broken"
					continue
				end
			end
			
			// Add this parent to the list
			if find(remList(:,3)+" - "+remList(:,4) == this_parent_name+" - "+this_parent_version) == [] then
				remList = [ remList ; "-" "P" this_parent_name this_parent_version ];  // P stands for "Parent"
			end
			
		end
		
		// Get the childs of this toolbox
		// ----------------------------------------------
		
		this_package_childs = atomsGetDepChilds(this_package_name,this_package_version);
		
		for j=1:size(this_package_childs(:,1),"*")
			
			this_child_name    = this_package_childs(j,1);
			this_child_version = this_package_childs(j,2);
			
			// Check if we have the right to remove this package
			// If not, Do not add it to the list
			if ~allusers then
				details = atomsGetInstalledDetails(this_child_name,this_child_version);
				if details(3) == "allusers" then
					continue
				end
			end
			
			if find(remList(:,3)+" - "+remList(:,4) == this_child_name+" - "+this_child_version) == [] then
				remList = [ remList ; "-" "C" this_child_name this_child_version ]; // C stands for "Child" 
			end
			
		end
		
	end
	
	// Third Step : Loop on childs check if we uninstall it or not
	// =========================================================================
	
	packages = remList(find(remList(:,2)=="C"),:);
	
	for i=1:size(remList(:,1),"*")
		
		// This is not a Child package :
		// => continue
		// ----------------------------------------------
		
		if remList(i,2) <> "C" then
			continue;
		end
		
		this_package_name    = remList(i,3);
		this_package_version = remList(i,4);
		
		// The package has been intentionnaly installed :
		// => Do not install it
		// ----------------------------------------------
		
		if atomsGetInstalledStatus(this_package_name,this_package_version) == "I" then
			remList(i,1) = "~";
		end
		
		// Get the parents of this toolbox
		// ----------------------------------------------
		
		this_package_parents = atomsGetDepParents(this_package_name,this_package_version);
		
		for j=1:size(this_package_parents(:,1),"*")
			
			if find(remList(:,3)+" - "+remList(:,4) == this_package_parents(j,1)+" - "+this_package_parents(j,2)) == [] then
				// One of the parent is not the remove list
				// => do not install it
				remList(i,1) = "~";
				continue;
			end
		end
		
	end
	
endfunction
