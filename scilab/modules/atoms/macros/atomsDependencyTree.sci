// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// internal function

function tree_out = atomsDependencyTree(name,version,tree_in)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if (rhs < 2) | (rhs > 3) then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDependencyTree",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDependencyTree",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDependencyTree",2));
	end
	
	if (rhs==3) & (type(tree_in) <> 17) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDependencyTree",3));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name) <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDependencyTree",1));
	end
	
	if size(version) <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDependencyTree",2));
	end
	
	// Define tree_in if not defined as input argument
	// =========================================================================
	
	if rhs<3 then
		tree_in = struct();
	end
	
	// Get dependencies of this toolbox
	// =========================================================================
	
	this_package_details = atomsToolboxDetails(name,version);
	
	// Check if the wanted version is compatible with the current scilab version.
	// =========================================================================
	
	this_package_details = atomsToolboxDetails(name,version);
	
	if ~ atomsIsCompatible(this_package_details("ScilabVersion")) then
		tree_out = %F;
		return tree_out;
	end
	
	tree_out(name) = version;
	
	// Now, loop on dependencies
	// =========================================================================
	
	if isfield(this_package_details,"Depends") & (this_package_details("Depends") ~= "") then
		dependencies = this_package_details("Depends");
	else
		dependencies = [];
	end
	
	for i=1:size(dependencies,"*")
		
		this_dependency_success = %F;
		
		// Split dependencies to get 
		//  - direction ("=", ">=", "<=", "~")
		//  - dependence name
		//  - dependence version (optional)
		
		this_dependency_tab     = stripblanks(strsplit(dependencies(i),regexp(stripblanks(dependencies(i)),"/\s/")));
		
		this_dependency_dir     = this_dependency_tab(1);
		this_dependency_name    = this_dependency_tab(2);
		this_dependency_version = this_dependency_tab(3);
		
		// List versions of the dependency we can test
		
		if this_dependency_dir     == "="  then
			this_dependency_list = this_dependency_version;
		
		elseif this_dependency_dir == "~"  then
			this_dependency_list = atomsGetVersions(this_dependency_name);
		
		elseif this_dependency_dir == ">=" then
			this_dependency_list = atomsGetVersions(this_dependency_name,this_dependency_version);
			
		elseif this_dependency_dir == "<=" then
			this_dependency_list = atomsGetVersions(this_dependency_name,"",this_dependency_version);
			
		end
		
		for j=1:size(this_dependency_list,"*")
			
			tree = atomsDependencyTree(this_dependency_name,this_dependency_list(j),tree_out);
			
			// Dependency Tree fails
			
			if (type(tree) == 4) & (~ tree) then
				continue;
			end
			
			// Dependency Tree OK
			
			if type(tree) == 17 then
				tree_out = atomsCatTree(tree_out,tree);
				this_dependency_success = %T;
				break;
			end
			
		end
		
		if ~  this_dependency_success then
			tree_out = %F;
			return;
		end
		
	end
	
endfunction

// =============================================================================
// tree_out = atomsCatTree( tree_in_1 , tree_in_2 )
//
// Date : 19/05/2009
// =============================================================================

function tree_out = atomsCatTree( tree_in_1 , tree_in_2 )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsMergeTree",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree_in_1) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsMergeTree",1));
	end
	
	if type(tree_in_2) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsMergeTree",2));
	end
	
	// tree_in_1 is the master tree :
	// =========================================================================
	
	tree_out  = tree_in_1;
	
	// Loop on tree_in_2 fields
	// =========================================================================
	
	fields = getfield(1,tree_in_2);
	fields(1:2) = [];
	
	for i=1:size(fields,"*")
		if ~ isfield(tree_out,fields(i)) then
			tree_out(fields(i)) = tree_in_2(fields(i));
		end
	end
	
endfunction
